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

#ifndef LIBANT_INCLUDE_LIBANT_NET_HTTP_HTTPS_CLIENT_H_
#define LIBANT_INCLUDE_LIBANT_NET_HTTP_HTTPS_CLIENT_H_

#include <deque>
#include <unordered_map>
#include <boost/beast/ssl.hpp>

#include "base_http_client.h"

namespace ant {

class HttpsClient : public BaseHttpClient {
public:
    HttpsClient(boost::asio::io_context& ioCtx, boost::asio::ssl::context& sslCtx, const std::string& svrAddr, uint16_t maxRetryTimes = 2,
                std::chrono::milliseconds ioTimeoutMs = std::chrono::milliseconds(5000),
                std::chrono::milliseconds connectTimeoutMs = std::chrono::milliseconds(2000))
        : BaseHttpClient(ioCtx, svrAddr, maxRetryTimes, ioTimeoutMs, connectTimeoutMs, "443")
        , ioCtx_(ioCtx)
        , sslCtx_(sslCtx)
        , stream_(ioCtx, sslCtx)
    {
    }

private:
    boost::beast::tcp_stream& get_stream() override
    {
        return boost::beast::get_lowest_layer(stream_);
    }

    void reset_stream() override
    {
        stream_.~ssl_stream<boost::beast::tcp_stream>();
        new (&stream_) boost::beast::ssl_stream<boost::beast::tcp_stream>(ioCtx_, sslCtx_);
    }

    void handshake() override
    {
        //LOG_INFO("Handshaking {}:{}/{}", host_, port_, request_.target().to_string());
        // Set SNI Hostname (many hosts need this to handshake successfully)
        if (SSL_set_tlsext_host_name(stream_.native_handle(), host_.c_str())) {
            // Perform the SSL handshake
            return stream_.async_handshake(boost::asio::ssl::stream_base::client,
                                           boost::beast::bind_front_handler(&HttpsClient::on_handshake, shared_from_this()));
        }
        on_handshake(boost::beast::error_code(static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()));
    }

    void send_request() override
    {
        //LOG_INFO("Sending request {}:{}/{}", host_, port_, request_.target().to_string());
        // Set a timeout on the operation
        boost::beast::get_lowest_layer(stream_).expires_after(ioTimeout_);
        // Send the HTTP request to the remote host
        boost::beast::http::async_write(stream_, request_, boost::beast::bind_front_handler(&HttpsClient::on_request_sent, shared_from_this()));
    }

    void read_response() override
    {
        //LOG_INFO("Reading response {}:{}/{}", host_, port_, request_.target().to_string());
        // Set a timeout on the operation
        boost::beast::get_lowest_layer(stream_).expires_after(ioTimeout_);
        // Receive the HTTP response
        boost::beast::http::async_read(stream_, buffer_, response_, boost::beast::bind_front_handler(&HttpsClient::on_response_read, shared_from_this()));
    }

private:
    boost::asio::io_context& ioCtx_;
    boost::asio::ssl::context& sslCtx_;
    boost::beast::ssl_stream<boost::beast::tcp_stream> stream_;
};

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_HTTP_HTTPS_CLIENT_H_
