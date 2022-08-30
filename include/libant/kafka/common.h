/*
*
* LibAnt - A handy C++ library
* Copyright (C) 2022 Antigloss Huang (https://github.com/antigloss) All rights reserved.
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

#ifndef LIBANT_INCLUDE_LIBANT_KAFKA_COMMON_H_
#define LIBANT_INCLUDE_LIBANT_KAFKA_COMMON_H_

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include <librdkafka/rdkafkacpp.h>

namespace ant {

using KafkaErrCode = RdKafka::ErrorCode;

/**
 * KafkaConfException
 */
class KafkaConfException : public std::invalid_argument {
public:
    KafkaConfException(const std::string& msg)
        : std::invalid_argument(msg)
    {
    }
};

/**
 * KafkaTopicConfig
 */
class KafkaTopicConfig {
public:
    friend class KafkaGlobalConfig;
    friend class KafkaProducer;

    // For Producer

    // This field indicates the number of acknowledgements the leader broker must receive from ISR brokers before responding to the request:
    // 0=Broker does not send any response/ack to client, -1 or all=Broker will block until message is committed by all in sync replicas (ISRs).
    // If there are less than min.insync.replicas (broker configuration) in the ISR set the produce request will fail.
    // Range: -1 .. 1000. Default: -1
    static const std::string kRequestRequiredAcks;
    // The ack timeout of the producer request in milliseconds. This value is only enforced by the broker and relies on request.required.acks being != 0.
    // Range: 1 .. 900000. Default: 30000
    static const std::string kRequestTimeoutMS;
    // Local message timeout. This value is only enforced locally and limits the time a produced message waits for successful delivery.
    // A time of 0 is infinite. This is the maximum time librdkafka may use to deliver a message (including retries).
    // Delivery error occurs when either the retry count or the message timeout are exceeded.
    // The message timeout is automatically adjusted to transaction.timeout.ms if transactional.id is configured.
    // Range: 0 .. 2147483647. Default: 300000
    static const std::string kMessageTimeoutMS;
    // Partitioner: random - random distribution, consistent - CRC32 hash of key (Empty and NULL keys are mapped to single partition),
    //              consistent_random - CRC32 hash of key (Empty and NULL keys are randomly partitioned),
    //              murmur2 - Java Producer compatible Murmur2 hash of key (NULL keys are mapped to single partition),
    //              murmur2_random - Java Producer compatible Murmur2 hash of key (NULL keys are randomly partitioned.
    //                               This is functionally equivalent to the default partitioner in the Java Producer.),
    //              fnv1a - FNV-1a hash of key (NULL keys are mapped to single partition),
    //              fnv1a_random - FNV-1a hash of key (NULL keys are randomly partitioned).
    // Default: consistent_random
    static const std::string kPartitioner;
    // Compression codec to use for compressing message sets. inherit = inherit global compression.codec configuration.
    // Range: none, gzip, snappy, lz4, zstd, inherit. Default: inherit
    static const std::string kCompressionCodec;
    // Compression level parameter for algorithm selected by configuration property compression.codec.
    // Higher values will result in better compression at the cost of more CPU usage.
    // Usable range is algorithm-dependent: [0-9] for gzip; [0-12] for lz4; only 0 for snappy; -1 = codec-dependent default compression level.
    // Default: -1
    static const std::string kCompressionLevel;

    // For Consumer

    // Action to take when there is no initial offset in offset store or the desired offset is out of range:
    //     'smallest','earliest' - automatically reset the offset to the smallest offset,
    //     'largest','latest' - automatically reset the offset to the largest offset,
    //     'error' - trigger an error (ERR__AUTO_OFFSET_RESET) which is retrieved by consuming messages and checking 'message->err'.
    // Default: largest
    static const std::string kAutoOffsetReset;

public:
    /**
     * Sets a topic configuration
     * @param key
     * @param value
     * @param errMessage
     * @return true on success, false on error and errMessage contains the error message.
     */
    bool Set(const std::string& key, const std::string& value, std::string& errMessage)
    {
        return conf_->set(key, value, errMessage) == RdKafka::Conf::ConfResult::CONF_OK;
    }

    /**
     * Sets a topic configuration
     * @param key
     * @param value
     * @throw KafkaConfException on error
     */
    void Set(const std::string& key, const std::string& value)
    {
        std::string err;
        if (conf_->set(key, value, err) == RdKafka::Conf::ConfResult::CONF_OK) {
            return;
        }
        throw KafkaConfException(err);
    }

private:
    std::unique_ptr<RdKafka::Conf> conf_{RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC)};
};

/**
 * KafkaGlobalConfig
 */
class KafkaGlobalConfig {
public:
    friend class KafkaConsumer;
    friend class KafkaProducer;

    // For Both

    // Initial list of brokers as a CSV list of broker host or host:port. The application may also add brokers during runtime.
    static const std::string kBrokerList;
    // Maximum Kafka protocol request message size.
    // Due to differing framing overhead between protocol versions the producer is unable to reliably
    // enforce a strict max message limit at produce time and may exceed the maximum size by one message
    // in protocol ProduceRequests, the broker will enforce the topic's `max.message.bytes` limit (see Apache Kafka documentation)
    // Range: 1000 .. 1000000000. Default: 1000000
    static const std::string kMessageMaxBytes;
    // Maximum size for message to be copied to buffer.
    // Messages larger than this will be passed by reference (zero-copy) at the expense of larger iovecs.
    // Range: 0 .. 1000000000. Default: 65535
    static const std::string kMessageCopyMaxBytes;
    // Maximum Kafka protocol response message size.
    // This serves as a safety precaution to avoid memory exhaustion in case of protocol pickups.
    // This value must be at least `fetch.max.bytes` + 512 to allow for protocol overhead;
    // the value is adjusted automatically unless the configuration property is explicitly set.
    // Range: 1000 .. 2147483647. Default: 100000000
    static const std::string kReceiveMessageMaxBytes;
    // Maximum number of in-flight requests per broker connection.
    // This is a generic property applied to all broker communication, however it is primarily relevant to produce requests.
    // In particular, note that other mechanisms limit the number of outstanding consumer fetch request per broker to one.
    // Range: 1 .. 1000000. Default: 1000000
    static const std::string kMaxInFlightRequestsPerConnection;
    // Period of time in milliseconds at which topic and broker metadata is refreshed in order to
    // proactively discover any new brokers, topics, partitions or partition leader changes.
    // Use -1 to disable the refresh (not recommended).
    // If there are no locally referenced topics (no topic objects created, no messages produced,
    // no subscription or no assignment) then only the broker list will be refreshed every interval
    // but no more often than every 10s.
    // Range: -1 .. 3600000. Default: 300000
    static const std::string kTopicMetadataRefreshIntervalMS;
    // Metadata cache max age. Defaults to topic.metadata.refresh.interval.ms * 3
    // Range: 1 .. 86400000. Default: 900000
    static const std::string kMetadataMaxAgeMS;
    // When a topic loses its leader a new metadata request will be enqueued with this initial interval,
    // exponentially increasing until the topic metadata has been refreshed.
    // This is used to recover quickly from transitioning leader brokers.
    // Range: 1 .. 60000. Default: 250
    static const std::string kTopicMetadataRefreshFastIntervalMS;
    // Sparse metadata requests (consumes less network bandwidth)
    // Range: true, false. Default: true
    static const std::string kTopicMetadataRefreshSparse;
    // Apache Kafka topic creation is asynchronous, and it takes some time for a new topic to propagate throughout the cluster to all brokers.
    // If a client requests topic metadata after manual topic creation but before the topic has been fully propagated to the broker the client
    // is requesting metadata from, the topic will seem to be non-existent and the client will mark the topic as such, failing queued produced
    // messages with `ERR__UNKNOWN_TOPIC`. This setting delays marking a topic as non-existent until the configured propagation max time has passed.
    // The maximum propagation time is calculated from the time the topic is first referenced in the client, e.g., on Produce().
    // Range: 0 .. 3600000. Default: 30000
    static const std::string kTopicMetadataPropagationMaxMS;
    // Topic blacklist, a comma-separated list of regular expressions for matching topic names that should be ignored
    // in broker metadata information as if the topics did not exist.
    static const std::string kTopicBlacklist;
    // Default timeout for network requests. Producer: ProduceRequests will use the lesser value of `socket.timeout.ms`
    // and remaining `message.timeout.ms` for the first message in the batch. Consumer: FetchRequests will use `fetch.wait.max.ms` + `socket.timeout.ms`.
    // Admin: Admin requests will use `socket.timeout.ms` or explicitly set `rd_kafka_AdminOptions_set_operation_timeout()` value.
    // Range: 10 .. 300000. Default: 60000
    static const std::string kSocketTimeoutMS;
    // Broker socket send buffer size. System default is used if 0.
    // Range: 0 .. 100000000. Default: 0
    static const std::string kSocketSendBufferBytes;
    // Broker socket receive buffer size. System default is used if 0.
    // Range: 0 .. 100000000. Default: 0
    static const std::string kSocketReceiveBufferBytes;
    // Enable TCP keepalive (SO_KEEPALIVE) on broker sockets.
    // Range: true, false. Default: false
    static const std::string kSocketKeepaliveEnable;
    // Disable the Nagle algorithm (TCP_NODELAY) on broker sockets.
    // Range: true, false. Default: false
    static const std::string kSocketNagleDisable;
    // Disconnect from broker when this number of send failures (e.g., timed out requests) is reached. Disable with 0.
    // WARNING: It is highly recommended leaving this setting at its default value of 1 to avoid the client and broker
    // to become desynchronized in case of request timeouts. NOTE: The connection is automatically re-established.
    // Range: 0 .. 1000000. Default: 1
    static const std::string kSocketMaxFails;
    // How long to cache the broker address resolving results (milliseconds).
    // Range: 0 .. 86400000. Default: 1000
    static const std::string kBrokerAddressTTL;
    // Allowed broker IP address families: any, v4, v6
    // Range: any, v4, v6. Default: any
    static const std::string kBrokerAddressFamily;
    // Maximum time allowed for broker connection setup (TCP connection setup as well SSL and SASL handshake).
    // If the connection to the broker is not fully functional after this the connection will be closed and retried.
    // Range: 1000 .. 2147483647. Default: 30000
    static const std::string kSocketConnectionSetupTimeoutMS;
    // Close broker connections after the specified time of inactivity. Disable with 0.
    // If this property is left at its default value some heuristics are performed to determine a suitable default value,
    // this is currently limited to identifying brokers on Azure (see librdkafka issue #3109 for more info).
    // Range: 0 .. 2147483647. Default: 0
    static const std::string kConnectionsMaxIdleMS;
    // The initial time to wait before reconnecting to a broker after the connection has been closed.
    // The time is increased exponentially until `reconnect.backoff.max.ms` is reached.
    // -25% to +50% jitter is applied to each reconnect backoff. A value of 0 disables the backoff and reconnects immediately.
    // Range: 0 .. 3600000. Default: 100
    static const std::string kReconnectBackoffMS;
    // The maximum time to wait before reconnecting to a broker after the connection has been closed.
    // Range: 0 .. 3600000. Default: 10000
    static const std::string kReconnectBackoffMaxMS;
    // librdkafka statistics emit interval. The application also needs to register a statistics callback using `rd_kafka_conf_set_stats_cb()`.
    // The granularity is 1000ms. A value of 0 disables statistics.
    // Range: 0 .. 86400000. Default: 0
    static const std::string kStatisticsIntervalMS;
    // See `rd_kafka_conf_set_events()`
    // Range: 0 .. 2147483647. Default: 0
    static const std::string kEnabledEvents;
    // Logging level (syslog(3) levels)
    // Range: 0 .. 7. Default: 6
    static const std::string kLogLevel;
    // Disable spontaneous log_cb from internal librdkafka threads, instead enqueue log messages on queue set with `rd_kafka_set_log_queue()`
    // and serve log callbacks or events through the standard poll APIs. **NOTE**: Log messages will linger in a temporary queue until the log queue has been set.
    // Range: true, false. Default: false
    static const std::string kLogQueue;
    // Print internal thread name in log messages (useful for debugging librdkafka internals)
    // Range: true, false. Default: true
    static const std::string kLogThreadName;
    // If enabled librdkafka will initialize the PRNG with srand(current_time.milliseconds)
    // on the first invocation of rd_kafka_new() (required only if rand_r() is not available on your platform).
    // If disabled the application must call srand() prior to calling rd_kafka_new().
    // Range: true, false. Default: true
    static const std::string kEnableRandomSeed;
    // Log broker disconnects. It might be useful to turn this off when interacting with 0.9 brokers with an aggressive `connections.max.idle.ms` value.
    // Range: true, false. Default: true
    static const std::string kLogConnectionClose;
    // Signal that librdkafka will use to quickly terminate on rd_kafka_destroy().
    // If this signal is not set then there will be a delay before rd_kafka_wait_destroyed() returns true
    // as internal threads are timing out their system calls. If this signal is set however the delay will be minimal.
    // The application should mask this signal as an internal signal handler is installed.
    // Range: 0 .. 128. Default: 0
    static const std::string kInternalTerminationSignal;
    // Request broker's supported API versions to adjust functionality to available protocol features.
    // If set to false, or the ApiVersionRequest fails, the fallback version `broker.version.fallback` will be used.
    // **NOTE**: Depends on broker version >=0.10.0. If the request is not supported by (an older) broker the `broker.version.fallback` fallback is used.
    // Range: true, false. Default: true
    static const std::string kApiVersionRequest;
    // Timeout for broker API version requests.
    // Range: 1 .. 300000. Default: 10000
    static const std::string kApiVersionRequestTimeoutMS;
    // Dictates how long the `broker.version.fallback` fallback is used in the case the ApiVersionRequest fails.
    // **NOTE**: The ApiVersionRequest is only issued when a new connection to the broker is made (such as after an upgrade).
    // Range: 0 .. 604800000. Default: 0
    static const std::string kApiVersionFallbackMS;
    // Older broker versions (before 0.10.0) provide no way for a client to query for supported protocol features
    // (ApiVersionRequest, see `api.version.request`) making it impossible for the client to know what features it may use.
    // As a workaround a user may set this property to the expected broker version and the client will automatically
    // adjust its feature set accordingly if the ApiVersionRequest fails (or is disabled).
    // The fallback broker version will be used for `api.version.fallback.ms`.
    // Valid values are: 0.9.0, 0.8.2, 0.8.1, 0.8.0. Any other value >= 0.10, such as 0.10.2.1, enables ApiVersionRequests.
    // Default: 0.10.0
    static const std::string kBrokerVersionFallback;
    // Protocol used to communicate with brokers.
    // Range: plaintext, ssl, sasl_plaintext, sasl_ssl. Default: plaintext
    static const std::string kSecurityProtocol;
    // A cipher suite is a named combination of authentication, encryption, MAC and key exchange algorithm used to negotiate
    // the security settings for a network connection using TLS or SSL network protocol. See manual page for ciphers(1) and SSL_CTX_set_cipher_list(3).
    static const std::string kSslCipherSuites;
    // The supported-curves extension in the TLS ClientHello message specifies the curves (standard/named, or 'explicit' GF(2^k) or GF(p))
    // the client is willing to have the server use. See manual page for `SSL_CTX_set1_curves_list(3)`. OpenSSL >= 1.0.2 required.
    static const std::string kSslCurvesList;
    // The client uses the TLS ClientHello signature_algorithms extension to indicate to the server which signature/hash algorithm pairs may be used
    // in digital signatures. See manual page for `SSL_CTX_set1_sigalgs_list(3)`. OpenSSL >= 1.0.2 required.
    static const std::string kSslSigalgsList;
    // Path to client's private key (PEM) used for authentication.
    static const std::string kSslKeyLocation;
    // Private key passphrase (for use with `ssl.key.location` and `set_ssl_cert()`)
    static const std::string kSslKeyPassword;
    // Client's private key string (PEM format) used for authentication.
    static const std::string kSslKeyPem;
    // Path to client's public key (PEM) used for authentication.
    static const std::string kSslCertificateLocation;
    // Client's public key string (PEM format) used for authentication.
    static const std::string kSslCertificatePem;
    // File or directory path to CA certificate(s) for verifying the broker's key.
    // Defaults: On Windows the system's CA certificates are automatically looked up in the Windows Root certificate store.
    // On Mac OSX this configuration defaults to `probe`. It is recommended to install openssl using Homebrew, to provide CA certificates.
    // On Linux install the distribution's ca-certificates package. If OpenSSL is statically linked
    // or `ssl.ca.location` is set to `probe` a list of standard paths will be probed and the first one found will be used as the default CA certificate
    // location path. If OpenSSL is dynamically linked the OpenSSL library's default path will be used (see `OPENSSLDIR` in `openssl version -a`).
    static const std::string kSslCaLocation;
    // CA certificate string (PEM format) for verifying the broker's key.
    static const std::string kSslCaPem;
    // Comma-separated list of Windows Certificate stores to load CA certificates from. Certificates will be loaded in the same order as stores are specified.
    // If no certificates can be loaded from any of the specified stores an error is logged and the OpenSSL library's default CA location is used instead.
    // Store names are typically one or more of: MY, Root, Trust, CA.
    // Default: Root
    static const std::string kSslCaCertificateStores;
    // Path to CRL for verifying broker's certificate validity.
    static const std::string kSslCrlLocation;
    // Path to client's keystore (PKCS#12) used for authentication.
    static const std::string kSslKeystoreLocation;
    // Client's keystore (PKCS#12) password.
    static const std::string kSslKeystorePassword;
    // Path to OpenSSL engine library. OpenSSL >= 1.1.0 required.
    static const std::string kSslEngineLocation;
    // OpenSSL engine id is the name used for loading engine.
    // Default: dynamic
    static const std::string kSslEngineID;
    // Enable OpenSSL's builtin broker (server) certificate verification.
    // This verification can be extended by the application by implementing a certificate_verify_cb.
    // Range: true, false. Default: true
    static const std::string kEnableSslCertificateVerification;
    // Endpoint identification algorithm to validate broker hostname using broker certificate.
    // https - Server (broker) hostname verification as specified in RFC2818. none - No endpoint verification. OpenSSL >= 1.0.2 required.
    // Range: none, https. Default: none
    static const std::string kSslEndpointIdentificationAlgorithm;
    // SASL mechanism to use for authentication. Supported: GSSAPI, PLAIN, SCRAM-SHA-256, SCRAM-SHA-512, OAUTHBEARER.
    // **NOTE**: Despite the name only one mechanism must be configured.
    // Default: GSSAPI
    static const std::string kSaslMechanisms;
    // Kerberos principal name that Kafka runs as, not including /hostname@REALM
    // Default: kafka
    static const std::string kSaslKerberosServiceName;
    // This client's Kerberos principal name. (Not supported on Windows, will use the logon user's principal).
    // Default: kafkaclient
    static const std::string kSaslKerberosPrincipal;
    // Shell command to refresh or acquire the client's Kerberos ticket.
    // This command is executed on client creation and every sasl.kerberos.min.time.before.relogin (0=disable).
    // %{config.prop.name} is replaced by corresponding config object value.
    // Default: kinit -R -t "%{sasl.kerberos.keytab}" -k %{sasl.kerberos.principal} \|\| kinit -t "%{sasl.kerberos.keytab}" -k %{sasl.kerberos.principal}
    static const std::string kSaslKerberosKinitCmd;
    // Path to Kerberos keytab file. This configuration property is only used as a variable in `sasl.kerberos.kinit.cmd` as ` ... -t "%{sasl.kerberos.keytab}"`.
    static const std::string kSaslKerberosKeytab;
    // Minimum time in milliseconds between key refresh attempts. Disable automatic key refresh by setting this property to 0.
    // Range: 0 .. 86400000. Default: 60000
    static const std::string kSaslKerberosMinTimeBeforeReLogin;
    // SASL username for use with the PLAIN and SASL-SCRAM-.. mechanisms
    static const std::string kSaslUsername;
    // SASL password for use with the PLAIN and SASL-SCRAM-.. mechanism
    static const std::string kSaslPassword;
    // SASL/OAUTHBEARER configuration. The format is implementation-dependent and must be parsed accordingly.
    // The default unsecured token implementation (see https://tools.ietf.org/html/rfc7515#appendix-A.5) recognizes
    // space-separated name=value pairs with valid names including principalClaimName, principal, scopeClaimName, scope, and lifeSeconds.
    // The default value for principalClaimName is "sub", the default value for scopeClaimName is "scope", and the default value for lifeSeconds is 3600.
    // The scope value is CSV format with the default value being no/empty scope.
    // For example: `principalClaimName=azp principal=admin scopeClaimName=roles scope=role1,role2 lifeSeconds=600`.
    // In addition, SASL extensions can be communicated to the broker via `extension_NAME=value`. For example: `principal=admin extension_traceId=123`
    static const std::string kSaslOAuthBearerConfig;

    // For Consumer

    // Client group id string. All clients sharing the same group.id belong to the same group.
    static const std::string kGroupID;
    // Automatically and periodically commit offsets in the background.
    // Note: setting this to false does not prevent the consumer from fetching previously committed start offsets.
    // To circumvent this behaviour set specific start offsets per partition in the call to assign().
    // Range: true, false. Default: true
    static const std::string kEnableAutoCommit;
    // Automatically store offset of last message provided to application.
    // The offset store is an in-memory store of the next offset to (auto-)commit for each partition.
    // Range: true, false. Default: true
    static const std::string kEnableAutoOffsetStore;

public:
    /**
     * Sets a global configuration
     * @param key
     * @param value
     * @param errMessage
     * @return true on success, false on error and errMessage contains the error message.
     */
    bool Set(const std::string& key, const std::string& value, std::string& errMessage)
    {
        return conf_->set(key, value, errMessage) == RdKafka::Conf::ConfResult::CONF_OK;
    }

    /**
     * Sets a global configuration
     * @param key
     * @param value
     * @throw KafkaConfException on error
     */
    void Set(const std::string& key, const std::string& value)
    {
        std::string err;
        if (conf_->set(key, value, err) == RdKafka::Conf::ConfResult::CONF_OK) {
            return;
        }
        throw KafkaConfException(err);
    }

    /**
     * Sets default topic configuration for automatically subscribed topics
     * @param topicCfg
     * @param errMessage
     * @return true on success, and the underlying memory of topicCfg is freed and should no longer be reused.
     *         false on error, topicCfg is left untouched.
     */
    bool SetDefaultTopicConf(KafkaTopicConfig& topicCfg, std::string& errMessage)
    {
        defTopicConf_.swap(topicCfg.conf_);
        if (conf_->set("default_topic_conf", defTopicConf_.get(), errMessage) == RdKafka::Conf::ConfResult::CONF_OK) {
            return true;
        }
        defTopicConf_.swap(topicCfg.conf_);
        return false;
    }

    /**
     * Sets default topic configuration for automatically subscribed topics
     * @param topicCfg
     * @warning On success, the underlying memory of topicCfg is freed and should no longer be reused.
     *          On error, topicCfg is left untouched.
     */
    void SetDefaultTopicConf(KafkaTopicConfig& topicCfg)
    {
        defTopicConf_.swap(topicCfg.conf_);
        std::string err;
        if (conf_->set("default_topic_conf", defTopicConf_.get(), err) == RdKafka::Conf::ConfResult::CONF_OK) {
            return;
        }
        defTopicConf_.swap(topicCfg.conf_);
        throw KafkaConfException(err);
    }

    /**
     * Sets client's certificates
     * @param certType
     * @param certEnc
     * @param buffer
     * @param size
     * @param errMessage
     * @return true on success, false on error
     */
    bool SetSslCert(RdKafka::CertificateType certType, RdKafka::CertificateEncoding certEnc, const void* buffer, size_t size, std::string& errMessage)
    {
        return conf_->set_ssl_cert(certType, certEnc, buffer, size, errMessage) == RdKafka::Conf::ConfResult::CONF_OK;
    }

    /**
     * Sets client's certificates
     * @param certType
     * @param certEnc
     * @param buffer
     * @param size
     * @throw KafkaConfException on error
     */
    void SetSslCert(RdKafka::CertificateType certType, RdKafka::CertificateEncoding certEnc, const void* buffer, size_t size)
    {
        std::string err;
        if (conf_->set_ssl_cert(certType, certEnc, buffer, size, err) == RdKafka::Conf::ConfResult::CONF_OK) {
            return;
        }
        throw KafkaConfException(err);
    }

    /**
     * Sets Callback to verify the broker certificate chain
     * @param cb
     * @param errMessage
     * @return true on success, false on error and errMessage contains the error message.
     */
    bool SetSslCertificateVerifyCallback(RdKafka::SslCertificateVerifyCb* cb, std::string& errMessage)
    {
        return conf_->set("ssl.certificate.verify_cb", cb, errMessage) == RdKafka::Conf::ConfResult::CONF_OK;
    }

    /**
     * Sets Callback to verify the broker certificate chain
     * @param cb
     * @throw KafkaConfException on error
     */
    void SetSslCertificateVerifyCallback(RdKafka::SslCertificateVerifyCb* cb)
    {
        std::string err;
        if (conf_->set("ssl.certificate.verify_cb", cb, err) == RdKafka::Conf::ConfResult::CONF_OK) {
            return;
        }
        throw KafkaConfException(err);
    }

    /**
     * Sets Delivery Report Callback
     * @param cb
     * @param errMessage
     * @return true on success, false on error and errMessage contains the error message.
     */
    bool SetDeliveryReportCallback(RdKafka::DeliveryReportCb* cb, std::string& errMessage)
    {
        return conf_->set("dr_cb", cb, errMessage) == RdKafka::Conf::ConfResult::CONF_OK;
    }

    /**
     * Sets Delivery Report Callback
     * @param cb
     * @throw KafkaConfException on error
     */
    void SetDeliveryReportCallback(RdKafka::DeliveryReportCb* cb)
    {
        std::string err;
        if (conf_->set("dr_cb", cb, err) == RdKafka::Conf::ConfResult::CONF_OK) {
            return;
        }
        throw KafkaConfException(err);
    }

    /**
     * Sets Event Callback
     * @param cb
     * @param errMessage
     * @return true on success, false on error and errMessage contains the error message.
     */
    bool SetEventCallback(RdKafka::EventCb* cb, std::string& errMessage)
    {
        return conf_->set("event_cb", cb, errMessage) == RdKafka::Conf::ConfResult::CONF_OK;
    }

    /**
     * Sets Event Callback
     * @param cb
     * @throw KafkaConfException on error
     */
    void SetEventCallback(RdKafka::EventCb* cb)
    {
        std::string err;
        if (conf_->set("event_cb", cb, err) == RdKafka::Conf::ConfResult::CONF_OK) {
            return;
        }
        throw KafkaConfException(err);
    }

    /**
     * Sets Offset Commit Callback
     * @param cb
     * @param errMessage
     * @return true on success, false on error and errMessage contains the error message.
     */
    bool SetOffsetCommitCallback(RdKafka::OffsetCommitCb* cb, std::string& errMessage)
    {
        return conf_->set("offset_commit_cb", cb, errMessage) == RdKafka::Conf::ConfResult::CONF_OK;
    }

    /**
     * Sets Offset Commit Callback
     * @param cb
     * @throw KafkaConfException on error
     */
    void SetOffsetCommitCallback(RdKafka::OffsetCommitCb* cb)
    {
        std::string err;
        if (conf_->set("offset_commit_cb", cb, err) == RdKafka::Conf::ConfResult::CONF_OK) {
            return;
        }
        throw KafkaConfException(err);
    }

    /**
     * Sets Socket creation callback to provide race-free CLOEXEC
     * @param cb
     * @param errMessage
     * @return true on success, false on error and errMessage contains the error message.
     */
    bool SetSocketCallback(RdKafka::SocketCb* cb, std::string& errMessage)
    {
        return conf_->set("socket_cb", cb, errMessage) == RdKafka::Conf::ConfResult::CONF_OK;
    }

    /**
     * Sets Socket creation callback to provide race-free CLOEXEC
     * @param cb
     * @throw KafkaConfException on error
     */
    void SetSocketCallback(RdKafka::SocketCb* cb)
    {
        std::string err;
        if (conf_->set("socket_cb", cb, err) == RdKafka::Conf::ConfResult::CONF_OK) {
            return;
        }
        throw KafkaConfException(err);
    }

    /**
     * Sets File open callback to provide race-free CLOEXEC
     * @param cb
     * @param errMessage
     * @return true on success, false on error and errMessage contains the error message.
     */
    bool SetOpenCallback(RdKafka::OpenCb* cb, std::string& errMessage)
    {
        return conf_->set("open_cb", cb, errMessage) == RdKafka::Conf::ConfResult::CONF_OK;
    }

    /**
     * Sets File open callback to provide race-free CLOEXEC
     * @param cb
     * @throw KafkaConfException on error
     */
    void SetOpenCallback(RdKafka::OpenCb* cb)
    {
        std::string err;
        if (conf_->set("open_cb", cb, err) == RdKafka::Conf::ConfResult::CONF_OK) {
            return;
        }
        throw KafkaConfException(err);
    }

private:
    std::unique_ptr<RdKafka::Conf> defTopicConf_;
    std::unique_ptr<RdKafka::Conf> conf_{RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL)};
};

/**
 * KafkaTopicConfig
 */
class KafkaHeaders {
public:
    friend class KafkaProducer;

public:
    /**
     * @brief Adds a Header to the end of the list.
     *
     * Convenience method for adding a std::string as a value for the header.
     *
     * @param key header key/name
     * @param value value string
     *
     * @returns a KafkaErrCode signalling success or failure to add the header.
     */
    virtual KafkaErrCode Add(const std::string& key, const std::string& value)
    {
        return headers_->add(key, value);
    }

private:
    std::unique_ptr<RdKafka::Headers> headers_{RdKafka::Headers::create()};
};

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_KAFKA_COMMON_H_
