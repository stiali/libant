#include <libant/net/http/http_client.h>
#include <libant/net/http/https_client.h>
#include <libant/net/http/http_client_pool.h>

namespace ant {

HttpClientPool::HttpClientPool(SecurityMode securityMode, boost::asio::io_context& ioCtx, const std::string& svrAddr, uint16_t poolSize, uint16_t maxRetryTimes,
                               std::chrono::milliseconds ioTimeoutMs, std::chrono::milliseconds connectTimeoutMs)
{
    clients_.reserve(poolSize);
    if (securityMode == SecurityMode::http) {
        for (uint16_t i = 0; i != poolSize; ++i) {
            clients_.emplace_back(std::make_shared<HttpClient>(ioCtx, svrAddr, maxRetryTimes, ioTimeoutMs, connectTimeoutMs));
        }
    } else {
        sslCtx_.set_default_verify_paths();
        sslCtx_.set_verify_mode((securityMode == SecurityMode::https_verify_peer_cert) ? boost::asio::ssl::verify_peer : boost::asio::ssl::verify_none);
        for (uint16_t i = 0; i != poolSize; ++i) {
            clients_.emplace_back(std::make_shared<HttpsClient>(ioCtx, sslCtx_, svrAddr, maxRetryTimes, ioTimeoutMs, connectTimeoutMs));
        }
    }
}

} // namespace ant