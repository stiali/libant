/*
*
* LibAnt - A handy C++ library
* Copyright (C) 2021 Antigloss Huang (https://github.com/antigloss) All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef LIBANT_INCLUDE_LIBANT_NET_HTTP_BASE_HTTP_CLIENT_H_
#define LIBANT_INCLUDE_LIBANT_NET_HTTP_BASE_HTTP_CLIENT_H_

#include <deque>
#include <unordered_map>
#include <boost/asio/ssl/context.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace ant {

class BaseHttpClient : public std::enable_shared_from_this<BaseHttpClient> {
public:
    using Callback = std::function<void(boost::beast::error_code, boost::beast::http::response<boost::beast::http::string_body>)>;

public:
    BaseHttpClient(boost::asio::io_context& ioCtx, const std::string& svrAddr, uint16_t maxRetryTimes = 2,
                   std::chrono::milliseconds ioTimeoutMs = std::chrono::milliseconds(5000),
                   std::chrono::milliseconds connectTimeoutMs = std::chrono::milliseconds(2000), const char* defPort = "80");

    virtual ~BaseHttpClient()
    {
        Close();

        boost::beast::error_code ec(boost::beast::errc::operation_canceled, boost::beast::generic_category());
        for (auto p : pendingRequests_) {
            if (p->cb) {
                p->cb(ec, response_);
            }
            delete p;
        }
    }

    void Get(Callback cb, const std::string& target, const std::unordered_map<std::string, std::string>* params = nullptr,
             const std::vector<std::string>* cookies = nullptr, const std::unordered_map<std::string, std::string>* additionalHeaders = nullptr)
    {
        request(std::move(cb), boost::beast::http::verb::get, target, params, nullptr, nullptr, cookies, additionalHeaders);
    }

    void Post(Callback cb, const std::string& target, const std::unordered_map<std::string, std::string>* params = nullptr,
              const std::string* contentType = nullptr, const std::string* httpBody = nullptr, const std::vector<std::string>* cookies = nullptr,
              const std::unordered_map<std::string, std::string>* additionalHeaders = nullptr)
    {
        request(std::move(cb), boost::beast::http::verb::post, target, params, contentType, httpBody, cookies, additionalHeaders);
    }

    void Close()
    {
        if (closed_) {
            return;
        }

        closed_ = true;
        close();
        resolver_.cancel();
    }

protected:
    void on_handshake(const boost::beast::error_code& ec);
    void on_request_sent(const boost::beast::error_code& ec, size_t);
    void on_response_read(const boost::beast::error_code& ec, size_t);

private:
    virtual boost::beast::tcp_stream& get_stream() = 0;
    virtual void reset_stream() = 0;
    virtual void handshake() = 0;
    virtual void send_request() = 0;
    virtual void read_response() = 0;
    virtual void close() = 0;

private:
    struct request_params {
        request_params(Callback cb_func)
            : cb(std::move(cb_func))
        {
        }

        request_params(Callback cb_func, boost::beast::http::verb http_method, const std::string& http_target,
                       const std::unordered_map<std::string, std::string>* params, const std::string* http_content_type, const std::string* http_body,
                       const std::vector<std::string>* http_cookies, const std::unordered_map<std::string, std::string>* additionalHeaders);

        ~request_params()
        {
            delete content_type;
            delete body;
            delete cookies;
            delete additional_headers;
        }

        Callback cb;
        boost::beast::http::verb method;
        std::string target;
        const std::string* content_type{nullptr};
        std::string* body{nullptr};
        const std::string* cookies{nullptr};
        std::unordered_map<std::string, std::string>* additional_headers{nullptr};
    };

private:
    void request(Callback cb, boost::beast::http::verb method, const std::string& target, const std::unordered_map<std::string, std::string>* params,
                 const std::string* contentType, const std::string* httpBody, const std::vector<std::string>* cookies,
                 const std::unordered_map<std::string, std::string>* additionalHeaders)
    {
        if (!pendingRequests_.empty()) {
            pendingRequests_.push_back(new request_params(std::move(cb), method, target, params, contentType, httpBody, cookies, additionalHeaders));
            return;
        }

        pendingRequests_.push_back(new request_params(std::move(cb)));
        do_request(method, target, params, contentType, httpBody, cookies, additionalHeaders);
    }

    void response(const boost::beast::error_code& ec)
    {
        auto req = pendingRequests_.front();
        if (req->cb) {
            req->cb(ec, std::move(response_));
        } else {
            response_.clear();
            response_.body().clear();
        }
        pendingRequests_.pop_front();
        delete req;

        if (!pendingRequests_.empty() && !closed_) {
            do_request(pendingRequests_.front());
        }
    }

    void do_request(boost::beast::http::verb method, const std::string& target, const std::unordered_map<std::string, std::string>* params,
                    const std::string* contentType, const std::string* httpBody, const std::vector<std::string>* cookies,
                    const std::unordered_map<std::string, std::string>* additionalHeaders)
    {
        retriedTimes_ = 0;
        init_request(method, target, params, contentType, httpBody, cookies, additionalHeaders);
        if (connected_) {
            return send_request();
        }
        resolve();
    }

    void do_request(const request_params* req_params)
    {
        retriedTimes_ = 0;
        init_request(req_params);
        if (connected_) {
            return send_request();
        }
        resolve();
    }

    void init_request(boost::beast::http::verb method, const std::string& target, const std::unordered_map<std::string, std::string>* params,
                      const std::string* contentType, const std::string* httpBody, const std::vector<std::string>* cookies,
                      const std::unordered_map<std::string, std::string>* additionalHeaders);
    void init_request(const request_params* req_params);

    void resolve()
    {
        // Look up the domain name
        resolver_.async_resolve(host_, port_, boost::beast::bind_front_handler(&BaseHttpClient::on_resolve, shared_from_this()));
    }
    void on_resolve(const boost::beast::error_code& ec, boost::asio::ip::tcp::resolver::results_type results);

    void connect()
    {
        auto& stream = get_stream();
        // Set a timeout on the operation
        stream.expires_after(connectTimeout_);
        // Make the connection on the IP address we get from a lookup
        stream.async_connect(resolvedResults_, boost::beast::bind_front_handler(&BaseHttpClient::on_connect, shared_from_this()));
    }
    void on_connect(const boost::beast::error_code& ec, const boost::asio::ip::tcp::resolver::results_type::endpoint_type&);

protected:
    const std::chrono::milliseconds ioTimeout_;
    std::string host_;
    std::string port_;
    boost::beast::flat_buffer buffer_;
    boost::beast::http::request<boost::beast::http::string_body> request_;
    boost::beast::http::response<boost::beast::http::string_body> response_;

private:
    const std::chrono::milliseconds connectTimeout_;
    const uint16_t maxRetryTimes_;
    uint16_t retriedTimes_;
    bool closed_;
    bool connected_;
    std::string tmpStr_;
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::resolver::results_type resolvedResults_;
    std::deque<request_params*> pendingRequests_;
};

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_HTTP_BASE_HTTP_CLIENT_H_
