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

#ifndef LIBANT_INCLUDE_LIBANT_NET_HTTP_HTTP_CLIENT_POOL_H_
#define LIBANT_INCLUDE_LIBANT_NET_HTTP_HTTP_CLIENT_POOL_H_

#include <chrono>
#include <random>
#include <vector>

#include "base_http_client.h"

namespace ant {

class HttpClientPool {
public:
    enum class SecurityMode {
        http,
        https_verify_none,
        https_verify_peer_cert,
    };

public:
    /**
     * Construct a pool of http_client/https_client objects.
     */
    HttpClientPool(SecurityMode securityMode, boost::asio::io_context& ioCtx, const std::string& svrAddr, uint16_t poolSize, uint16_t maxRetryTimes = 2,
                   std::chrono::milliseconds ioTimeoutMs = std::chrono::milliseconds(5000),
                   std::chrono::milliseconds connectTimeoutMs = std::chrono::milliseconds(2000));

    void Get(uint64_t hint, BaseHttpClient::Callback cb, const std::string& target, const std::unordered_map<std::string, std::string>* params = nullptr,
             const std::vector<std::string>* cookies = nullptr)
    {
        clients_[hint % clients_.size()]->Get(std::move(cb), target, params, cookies);
    }

    void Post(uint64_t hint, BaseHttpClient::Callback cb, const std::string& target, const std::unordered_map<std::string, std::string>* params = nullptr,
              const std::string* contentType = nullptr, const std::string* httpBody = nullptr, const std::vector<std::string>* cookies = nullptr,
              const std::unordered_map<boost::beast::http::field, std::string>* additionalHeaders = nullptr)
    {
        clients_[hint % clients_.size()]->Post(std::move(cb), target, params, contentType, httpBody, cookies, additionalHeaders);
    }

    void Get(BaseHttpClient::Callback cb, const std::string& target, const std::unordered_map<std::string, std::string>* params = nullptr,
             const std::vector<std::string>* cookies = nullptr)
    {
        Get(rand_(), std::move(cb), target, params, cookies);
    }

    void Post(BaseHttpClient::Callback cb, const std::string& target, const std::unordered_map<std::string, std::string>* params = nullptr,
              const std::string* contentType = nullptr, const std::string* httpBody = nullptr, const std::vector<std::string>* cookies = nullptr,
              const std::unordered_map<boost::beast::http::field, std::string>* additionalHeaders = nullptr)
    {
        Post(rand_(), std::move(cb), target, params, contentType, httpBody, cookies, additionalHeaders);
    }

private:
    std::mt19937 rand_{static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count())};
    boost::asio::ssl::context sslCtx_{boost::asio::ssl::context::tlsv12_client};
    std::vector<std::shared_ptr<BaseHttpClient>> clients_;
};

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_HTTP_HTTP_CLIENT_POOL_H_
