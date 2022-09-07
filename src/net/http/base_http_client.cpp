#include <boost/algorithm/string.hpp>
#include <boost/beast/version.hpp>
#include <libant/net/http/base_http_client.h>

using namespace std;

namespace ant {

//================================================================================
// Public Methods
//================================================================================

BaseHttpClient::BaseHttpClient(boost::asio::io_context& ioCtx, const std::string& svrAddr, uint16_t maxRetryTimes, std::chrono::milliseconds ioTimeoutMs,
                               std::chrono::milliseconds connectTimeoutMs, const char* defPort)
    : ioTimeout_(ioTimeoutMs)
    , connectTimeout_(connectTimeoutMs)
    , maxRetryTimes_(maxRetryTimes)
    , retriedTimes_(0)
    , connected_(false)
    , resolver_(ioCtx)
{
    vector<string> hostPort;
    boost::split(hostPort, svrAddr, boost::is_any_of(":"));
    host_ = move(hostPort[0]);
    if (hostPort.size() >= 2) {
        port_ = move(hostPort[1]);
    } else {
        port_ = defPort;
    }
}

//================================================================================
// Protected Methods
//================================================================================

void BaseHttpClient::on_handshake(const boost::beast::error_code& ec)
{
    if (!ec) {
        connected_ = true;
        return send_request();
    }

    reset_stream();
    if (ec.value() != boost::beast::errc::operation_canceled && retriedTimes_ < maxRetryTimes_) {
        ++retriedTimes_;
        return resolve();
    }

    response(ec);
}

void BaseHttpClient::on_request_sent(const boost::beast::error_code& ec, size_t)
{
    if (!ec) {
        return read_response();
    }

    connected_ = false;
    reset_stream();
    if (ec.value() != boost::beast::errc::operation_canceled && retriedTimes_ < maxRetryTimes_) {
        ++retriedTimes_;
        return resolve();
    }

    response(ec);
}

void BaseHttpClient::on_response_read(const boost::beast::error_code& ec, size_t)
{
    if (!ec) {
        if (!response_.keep_alive()) {
            connected_ = false;
            buffer_.clear();
            reset_stream();
        }

        if (response_.result() == boost::beast::http::status::ok || retriedTimes_ >= maxRetryTimes_) {
            return response(ec);
        }

        response_.clear();
        response_.body().clear();
        ++retriedTimes_;
        if (connected_) {
            return send_request();
        }
        return resolve();
    }

    connected_ = false;
    buffer_.clear();
    reset_stream();
    if (ec.value() != boost::beast::errc::operation_canceled && retriedTimes_ < maxRetryTimes_) {
        response_.clear();
        response_.body().clear();
        ++retriedTimes_;
        return resolve();
    }

    response(ec);
}

//================================================================================
// Private Methods
//================================================================================

void BaseHttpClient::init_request(boost::beast::http::verb method, const std::string& target, const std::unordered_map<std::string, std::string>* params,
                                  const std::string* contentType, const std::string* httpBody, const std::vector<std::string>* cookies,
                                  const std::unordered_map<std::string, std::string>* additionalHeaders)
{
    request_.clear();
    request_.version(11);
    request_.set(boost::beast::http::field::host, host_);
    request_.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    request_.method(method);

    tmpStr_.clear();
    tmpStr_.append(target);
    if (params) {
        tmpStr_.append("?");
        for (const auto& kv : *params) {
            tmpStr_.append(kv.first).append("=").append(kv.second).append("&");
        }
        tmpStr_.resize(tmpStr_.size() - 1);
    }
    request_.target(tmpStr_);

    if (contentType) {
        request_.set(boost::beast::http::field::content_type, *contentType);
    }

    if (cookies) {
        tmpStr_.clear();
        tmpStr_.reserve(512);
        for (const auto& ck : *cookies) {
            tmpStr_.append(ck).append("; ");
        }
        if (tmpStr_.size() >= 2) {
            tmpStr_.resize(tmpStr_.size() - 2);
        }
        request_.set(boost::beast::http::field::cookie, tmpStr_);
    }

    if (additionalHeaders) {
        for (const auto& kv : *additionalHeaders) {
            request_.set(kv.first, kv.second);
        }
    }

    // 这个必须放在最后
    if (httpBody) {
        request_.body() = *httpBody;
    } else {
        request_.body().clear();
    }
    request_.prepare_payload();
}

void BaseHttpClient::init_request(const request_params* req_params)
{
    request_.clear();
    request_.version(11);
    request_.set(boost::beast::http::field::host, host_);
    request_.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    request_.method(req_params->method);
    request_.target(req_params->target);

    if (req_params->content_type) {
        request_.set(boost::beast::http::field::content_type, *req_params->content_type);
    }

    if (req_params->cookies) {
        request_.set(boost::beast::http::field::cookie, *req_params->cookies);
    }

    if (req_params->additional_headers) {
        for (const auto& kv : *req_params->additional_headers) {
            request_.set(kv.first, kv.second);
        }
    }

    // 这个必须放在最后
    if (req_params->body) {
        request_.body() = move(*req_params->body);
    } else {
        request_.body().clear();
    }
    request_.prepare_payload();
}

void BaseHttpClient::on_resolve(const boost::beast::error_code& ec, boost::asio::ip::tcp::resolver::results_type results)
{
    if (!ec) {
        resolvedResults_ = move(results);
        return connect();
    }

    if (ec.value() != boost::beast::errc::operation_canceled && retriedTimes_ < maxRetryTimes_) {
        ++retriedTimes_;
        return resolve();
    }

    response(ec);
}

void BaseHttpClient::on_connect(const boost::beast::error_code& ec, const boost::asio::ip::tcp::resolver::results_type::endpoint_type&)
{
    if (!ec) {
        return handshake();
    }

    if (ec.value() != boost::beast::errc::operation_canceled && retriedTimes_ < maxRetryTimes_) {
        ++retriedTimes_;
        return resolve();
    }

    response(ec);
}

//================================================================================
// request_params public Methods
//================================================================================

BaseHttpClient::request_params::request_params(Callback cb_func, boost::beast::http::verb http_method, const std::string& http_target,
                                               const std::unordered_map<std::string, std::string>* params, const std::string* http_content_type,
                                               const std::string* http_body, const std::vector<std::string>* http_cookies,
                                               const std::unordered_map<std::string, std::string>* additionalHeaders)
    : cb(std::move(cb_func))
    , method(http_method)
    , target(http_target)
{
    if (params) {
        target.append("?");
        for (const auto& kv : *params) {
            target.append(kv.first).append("=").append(kv.second).append("&");
        }
        target.resize(target.size() - 1);
    }

    if (http_content_type) {
        content_type = new std::string(*http_content_type);
    }

    if (http_body) {
        body = new std::string(*http_body);
    }

    if (http_cookies) {
        auto cks = new std::string();
        cks->reserve(512);
        for (const auto& ck : *http_cookies) {
            cks->append(ck).append("; ");
        }
        if (cks->size() >= 2) {
            cks->resize(cks->size() - 2);
        }
        cookies = cks;
    }

    if (additionalHeaders) {
        additional_headers = new std::unordered_map<std::string, std::string>;
        *additional_headers = *additionalHeaders;
    }
}

} // namespace ant