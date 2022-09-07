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

#ifndef LIBANT_INCLUDE_LIBANT_NET_HTTP_HTTP_CLIENT_H_
#define LIBANT_INCLUDE_LIBANT_NET_HTTP_HTTP_CLIENT_H_

#include <deque>
#include <unordered_map>
#include "base_http_client.h"

namespace ant {

class HttpClient : public BaseHttpClient {
public:
    HttpClient(boost::asio::io_context& ioCtx, const std::string& svrAddr, uint16_t maxRetryTimes = 2,
               std::chrono::milliseconds ioTimeoutMs = std::chrono::milliseconds(5000),
               std::chrono::milliseconds connectTimeoutMs = std::chrono::milliseconds(2000))
        : BaseHttpClient(ioCtx, svrAddr, maxRetryTimes, ioTimeoutMs, connectTimeoutMs)
        , stream_(ioCtx)
    {
    }

private:
    boost::beast::tcp_stream& get_stream() override
    {
        return stream_;
    }

    void reset_stream() override
    {
        stream_.close();
    }

    void handshake() override
    {
        on_handshake(boost::beast::error_code());
    }

    void send_request() override
    {
        // Set a timeout on the operation
        stream_.expires_after(ioTimeout_);
        // Send the HTTP request to the remote host
        boost::beast::http::async_write(stream_, request_, boost::beast::bind_front_handler(&HttpClient::on_request_sent, shared_from_this()));
    }

    void read_response() override
    {
        // Set a timeout on the operation
        stream_.expires_after(ioTimeout_);
        // Receive the HTTP response
        boost::beast::http::async_read(stream_, buffer_, response_, boost::beast::bind_front_handler(&HttpClient::on_response_read, shared_from_this()));
    }

    void close() override
    {
        stream_.close();
    }

private:
    boost::beast::tcp_stream stream_;
};

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_HTTP_HTTP_CLIENT_H_
