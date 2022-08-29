#include <libant/kafka/common.h>

namespace ant {

// KafkaGlobalConfig static variables
const std::string KafkaGlobalConfig::kBrokerList = "metadata.broker.list";

// Maximum Kafka protocol request message size.
// Due to differing framing overhead between protocol versions the producer is unable to reliably
// enforce a strict max message limit at produce time and may exceed the maximum size by one message
// in protocol ProduceRequests, the broker will enforce the topic's `max.message.bytes` limit (see Apache Kafka documentation)
// Range: 1000 .. 1000000000. Default: 1000000
const std::string kMessageMaxBytes = "message.max.bytes";
// Maximum size for message to be copied to buffer.
// Messages larger than this will be passed by reference (zero-copy) at the expense of larger iovecs.
// Range: 0 .. 1000000000. Default: 65535
const std::string kMessageCopyMaxBytes = "message.copy.max.bytes";
// Maximum Kafka protocol response message size.
// This serves as a safety precaution to avoid memory exhaustion in case of protocol pickups.
// This value must be at least `fetch.max.bytes` + 512 to allow for protocol overhead;
// the value is adjusted automatically unless the configuration property is explicitly set.
// Range: 1000 .. 2147483647. Default: 100000000
const std::string kReceiveMessageMaxBytes = "receive.message.max.bytes";
// Maximum number of in-flight requests per broker connection.
// This is a generic property applied to all broker communication, however it is primarily relevant to produce requests.
// In particular, note that other mechanisms limit the number of outstanding consumer fetch request per broker to one.
// Range: 1 .. 1000000. Default: 1000000
const std::string kMaxInFlightRequestsPerConnection = "max.in.flight.requests.per.connection";
// Period of time in milliseconds at which topic and broker metadata is refreshed in order to
// proactively discover any new brokers, topics, partitions or partition leader changes.
// Use -1 to disable the refresh (not recommended).
// If there are no locally referenced topics (no topic objects created, no messages produced,
// no subscription or no assignment) then only the broker list will be refreshed every interval
// but no more often than every 10s.
// Range: -1 .. 3600000. Default: 300000
const std::string kTopicMetadataRefreshIntervalMS = "topic.metadata.refresh.interval.ms";
// Metadata cache max age. Defaults to topic.metadata.refresh.interval.ms * 3
// Range: 1 .. 86400000. Default: 900000
const std::string kMetadataMaxAgeMS = "metadata.max.age.ms";
// When a topic loses its leader a new metadata request will be enqueued with this initial interval,
// exponentially increasing until the topic metadata has been refreshed.
// This is used to recover quickly from transitioning leader brokers.
// Range: 1 .. 60000. Default: 250
const std::string kTopicMetadataRefreshFastIntervalMS = "topic.metadata.refresh.fast.interval.ms";
// Sparse metadata requests (consumes less network bandwidth)
// Range: true, false. Default: true
const std::string kTopicMetadataRefreshSparse = "topic.metadata.refresh.sparse";
// Apache Kafka topic creation is asynchronous, and it takes some time for a new topic to propagate throughout the cluster to all brokers.
// If a client requests topic metadata after manual topic creation but before the topic has been fully propagated to the broker the client
// is requesting metadata from, the topic will seem to be non-existent and the client will mark the topic as such, failing queued produced
// messages with `ERR__UNKNOWN_TOPIC`. This setting delays marking a topic as non-existent until the configured propagation max time has passed.
// The maximum propagation time is calculated from the time the topic is first referenced in the client, e.g., on Produce().
// Range: 0 .. 3600000. Default: 30000
const std::string kTopicMetadataPropagationMaxMS = "topic.metadata.propagation.max.ms";
// Topic blacklist, a comma-separated list of regular expressions for matching topic names that should be ignored
// in broker metadata information as if the topics did not exist.
const std::string kTopicBlacklist = "topic.blacklist";
// Default timeout for network requests. Producer: ProduceRequests will use the lesser value of `socket.timeout.ms`
// and remaining `message.timeout.ms` for the first message in the batch. Consumer: FetchRequests will use `fetch.wait.max.ms` + `socket.timeout.ms`.
// Admin: Admin requests will use `socket.timeout.ms` or explicitly set `rd_kafka_AdminOptions_set_operation_timeout()` value.
// Range: 10 .. 300000. Default: 60000
const std::string kSocketTimeoutMS = "socket.timeout.ms";
// Broker socket send buffer size. System default is used if 0.
// Range: 0 .. 100000000. Default: 0
const std::string kSocketSendBufferBytes = "socket.send.buffer.bytes";
// Broker socket receive buffer size. System default is used if 0.
// Range: 0 .. 100000000. Default: 0
const std::string kSocketReceiveBufferBytes = "socket.receive.buffer.bytes";
// Enable TCP keepalive (SO_KEEPALIVE) on broker sockets.
// Range: true, false. Default: false
const std::string kSocketKeepaliveEnable = "socket.keepalive.enable";
// Disable the Nagle algorithm (TCP_NODELAY) on broker sockets.
// Range: true, false. Default: false
const std::string kSocketNagleDisable = "socket.nagle.disable";
// Disconnect from broker when this number of send failures (e.g., timed out requests) is reached. Disable with 0.
// WARNING: It is highly recommended leaving this setting at its default value of 1 to avoid the client and broker
// to become desynchronized in case of request timeouts. NOTE: The connection is automatically re-established.
// Range: 0 .. 1000000. Default: 1
const std::string kSocketMaxFails = "socket.max.fails";
// How long to cache the broker address resolving results (milliseconds).
// Range: 0 .. 86400000. Default: 1000
const std::string kBrokerAddressTTL = "broker.address.ttl";
// Allowed broker IP address families: any, v4, v6
// Range: any, v4, v6. Default: any
const std::string kBrokerAddressFamily = "broker.address.family";
// Maximum time allowed for broker connection setup (TCP connection setup as well SSL and SASL handshake).
// If the connection to the broker is not fully functional after this the connection will be closed and retried.
// Range: 1000 .. 2147483647. Default: 30000
const std::string kSocketConnectionSetupTimeoutMS = "socket.connection.setup.timeout.ms";
// Close broker connections after the specified time of inactivity. Disable with 0.
// If this property is left at its default value some heuristics are performed to determine a suitable default value,
// this is currently limited to identifying brokers on Azure (see librdkafka issue #3109 for more info).
// Range: 0 .. 2147483647. Default: 0
const std::string kConnectionsMaxIdleMS = "connections.max.idle.ms";
// The initial time to wait before reconnecting to a broker after the connection has been closed.
// The time is increased exponentially until `reconnect.backoff.max.ms` is reached.
// -25% to +50% jitter is applied to each reconnect backoff. A value of 0 disables the backoff and reconnects immediately.
// Range: 0 .. 3600000. Default: 100
const std::string kReconnectBackoffMS = "reconnect.backoff.ms";
// The maximum time to wait before reconnecting to a broker after the connection has been closed.
// Range: 0 .. 3600000. Default: 10000
const std::string kReconnectBackoffMaxMS = "reconnect.backoff.max.ms";
// librdkafka statistics emit interval. The application also needs to register a statistics callback using `rd_kafka_conf_set_stats_cb()`.
// The granularity is 1000ms. A value of 0 disables statistics.
// Range: 0 .. 86400000. Default: 0
const std::string kStatisticsIntervalMS = "statistics.interval.ms";
// See `rd_kafka_conf_set_events()`
// Range: 0 .. 2147483647. Default: 0
const std::string kEnabledEvents = "enabled_events";
// Logging level (syslog(3) levels)
// Range: 0 .. 7. Default: 6
const std::string kLogLevel = "log_level";
// Disable spontaneous log_cb from internal librdkafka threads, instead enqueue log messages on queue set with `rd_kafka_set_log_queue()`
// and serve log callbacks or events through the standard poll APIs. **NOTE**: Log messages will linger in a temporary queue until the log queue has been set.
// Range: true, false. Default: false
const std::string kLogQueue = "log.queue";
// Print internal thread name in log messages (useful for debugging librdkafka internals)
// Range: true, false. Default: true
const std::string kLogThreadName = "log.thread.name";
// If enabled librdkafka will initialize the PRNG with srand(current_time.milliseconds)
// on the first invocation of rd_kafka_new() (required only if rand_r() is not available on your platform).
// If disabled the application must call srand() prior to calling rd_kafka_new().
// Range: true, false. Default: true
const std::string kEnableRandomSeed = "enable.random.seed";
// Log broker disconnects. It might be useful to turn this off when interacting with 0.9 brokers with an aggressive `connections.max.idle.ms` value.
// Range: true, false. Default: true
const std::string kLogConnectionClose = "log.connection.close";
// Signal that librdkafka will use to quickly terminate on rd_kafka_destroy().
// If this signal is not set then there will be a delay before rd_kafka_wait_destroyed() returns true
// as internal threads are timing out their system calls. If this signal is set however the delay will be minimal.
// The application should mask this signal as an internal signal handler is installed.
// Range: 0 .. 128. Default: 0
const std::string kInternalTerminationSignal = "internal.termination.signal";
// Request broker's supported API versions to adjust functionality to available protocol features.
// If set to false, or the ApiVersionRequest fails, the fallback version `broker.version.fallback` will be used.
// **NOTE**: Depends on broker version >=0.10.0. If the request is not supported by (an older) broker the `broker.version.fallback` fallback is used.
// Range: true, false. Default: true
const std::string kApiVersionRequest = "api.version.request";
// Timeout for broker API version requests.
// Range: 1 .. 300000. Default: 10000
const std::string kApiVersionRequestTimeoutMS = "api.version.request.timeout.ms";
// Dictates how long the `broker.version.fallback` fallback is used in the case the ApiVersionRequest fails.
// **NOTE**: The ApiVersionRequest is only issued when a new connection to the broker is made (such as after an upgrade).
// Range: 0 .. 604800000. Default: 0
const std::string kApiVersionFallbackMS = "api.version.fallback.ms";
// Older broker versions (before 0.10.0) provide no way for a client to query for supported protocol features
// (ApiVersionRequest, see `api.version.request`) making it impossible for the client to know what features it may use.
// As a workaround a user may set this property to the expected broker version and the client will automatically
// adjust its feature set accordingly if the ApiVersionRequest fails (or is disabled).
// The fallback broker version will be used for `api.version.fallback.ms`.
// Valid values are: 0.9.0, 0.8.2, 0.8.1, 0.8.0. Any other value >= 0.10, such as 0.10.2.1, enables ApiVersionRequests.
// Default: 0.10.0
const std::string kBrokerVersionFallback = "broker.version.fallback";
// Protocol used to communicate with brokers.
// Range: plaintext, ssl, sasl_plaintext, sasl_ssl. Default: plaintext
const std::string kSecurityProtocol = "security.protocol";
// A cipher suite is a named combination of authentication, encryption, MAC and key exchange algorithm used to negotiate
// the security settings for a network connection using TLS or SSL network protocol. See manual page for ciphers(1) and SSL_CTX_set_cipher_list(3).
const std::string kSslCipherSuites = "ssl.cipher.suites";
// The supported-curves extension in the TLS ClientHello message specifies the curves (standard/named, or 'explicit' GF(2^k) or GF(p))
// the client is willing to have the server use. See manual page for `SSL_CTX_set1_curves_list(3)`. OpenSSL >= 1.0.2 required.
const std::string kSslCurvesList = "ssl.curves.list";
// The client uses the TLS ClientHello signature_algorithms extension to indicate to the server which signature/hash algorithm pairs may be used
// in digital signatures. See manual page for `SSL_CTX_set1_sigalgs_list(3)`. OpenSSL >= 1.0.2 required.
const std::string kSslSigalgsList = "ssl.sigalgs.list";
// Path to client's private key (PEM) used for authentication.
const std::string kSslKeyLocation = "ssl.key.location";
// Private key passphrase (for use with `ssl.key.location` and `set_ssl_cert()`)
const std::string kSslKeyPassword = "ssl.key.password";
// Client's private key string (PEM format) used for authentication.
const std::string kSslKeyPem = "ssl.key.pem";
// Path to client's public key (PEM) used for authentication.
const std::string kSslCertificateLocation = "ssl.certificate.location";
// Client's public key string (PEM format) used for authentication.
const std::string kSslCertificatePem = "ssl.certificate.pem";
// File or directory path to CA certificate(s) for verifying the broker's key.
// Defaults: On Windows the system's CA certificates are automatically looked up in the Windows Root certificate store.
// On Mac OSX this configuration defaults to `probe`. It is recommended to install openssl using Homebrew, to provide CA certificates.
// On Linux install the distribution's ca-certificates package. If OpenSSL is statically linked
// or `ssl.ca.location` is set to `probe` a list of standard paths will be probed and the first one found will be used as the default CA certificate
// location path. If OpenSSL is dynamically linked the OpenSSL library's default path will be used (see `OPENSSLDIR` in `openssl version -a`).
const std::string kSslCaLocation = "ssl.ca.location";
// CA certificate string (PEM format) for verifying the broker's key.
const std::string kSslCaPem = "ssl.ca.pem";
// Comma-separated list of Windows Certificate stores to load CA certificates from. Certificates will be loaded in the same order as stores are specified.
// If no certificates can be loaded from any of the specified stores an error is logged and the OpenSSL library's default CA location is used instead.
// Store names are typically one or more of: MY, Root, Trust, CA.
// Default: Root
const std::string kSslCaCertificateStores = "ssl.ca.certificate.stores";
// Path to CRL for verifying broker's certificate validity.
const std::string kSslCrlLocation = "ssl.crl.location";
// Path to client's keystore (PKCS#12) used for authentication.
const std::string kSslKeystoreLocation = "ssl.keystore.location";
// Client's keystore (PKCS#12) password.
const std::string kSslKeystorePassword = "ssl.keystore.password";
// Path to OpenSSL engine library. OpenSSL >= 1.1.0 required.
const std::string kSslEngineLocation = "ssl.engine.location";
// OpenSSL engine id is the name used for loading engine.
// Default: dynamic
const std::string kSslEngineID = "ssl.engine.id";
// Enable OpenSSL's builtin broker (server) certificate verification.
// This verification can be extended by the application by implementing a certificate_verify_cb.
// Range: true, false. Default: true
const std::string kEnableSslCertificateVerification = "enable.ssl.certificate.verification";
// Endpoint identification algorithm to validate broker hostname using broker certificate.
// https - Server (broker) hostname verification as specified in RFC2818. none - No endpoint verification. OpenSSL >= 1.0.2 required.
// Range: none, https. Default: none
const std::string kSslEndpointIdentificationAlgorithm = "ssl.endpoint.identification.algorithm";
// SASL mechanism to use for authentication. Supported: GSSAPI, PLAIN, SCRAM-SHA-256, SCRAM-SHA-512, OAUTHBEARER.
// **NOTE**: Despite the name only one mechanism must be configured.
// Default: GSSAPI
const std::string kSaslMechanisms = "sasl.mechanisms";
// Kerberos principal name that Kafka runs as, not including /hostname@REALM
// Default: kafka
const std::string kSaslKerberosServiceName = "sasl.kerberos.service.name";
//const std::string k = "";
//const std::string k = "";
//const std::string k = "";
//const std::string k = "";
//const std::string k = "";
//const std::string k = "";
//const std::string k = "";
//const std::string k = "";

const std::string KafkaGlobalConfig::kGroupID = "group.id";
const std::string KafkaGlobalConfig::kEnableAutoCommit = "enable.auto.commit";
const std::string KafkaGlobalConfig::kEnableAutoOffsetStore = "enable.auto.offset.store";

//sasl.kerberos.principal                  |  *  |                 |   kafkaclient | low        | This client's Kerberos principal name. (Not supported on Windows, will use the logon user's principal). <br>*Type: string*
//sasl.kerberos.kinit.cmd                  |  *  |                 | kinit -R -t "%{sasl.kerberos.keytab}" -k %{sasl.kerberos.principal} \|\| kinit -t "%{sasl.kerberos.keytab}" -k %{sasl.kerberos.principal} | low        | Shell command to refresh or acquire the client's Kerberos ticket. This command is executed on client creation and every sasl.kerberos.min.time.before.relogin (0=disable). %{config.prop.name} is replaced by corresponding config object value. <br>*Type: string*
//sasl.kerberos.keytab                     |  *  |                 |               | low        | Path to Kerberos keytab file. This configuration property is only used as a variable in `sasl.kerberos.kinit.cmd` as ` ... -t "%{sasl.kerberos.keytab}"`. <br>*Type: string*
//sasl.kerberos.min.time.before.relogin    |  *  | 0 .. 86400000   |         60000 | low        | Minimum time in milliseconds between key refresh attempts. Disable automatic key refresh by setting this property to 0. <br>*Type: integer*
//sasl.username                            |  *  |                 |               | high       | SASL username for use with the PLAIN and SASL-SCRAM-.. mechanisms <br>*Type: string*
//sasl.password                            |  *  |                 |               | high       | SASL password for use with the PLAIN and SASL-SCRAM-.. mechanism <br>*Type: string*
//sasl.oauthbearer.config                  |  *  |                 |               | low        | SASL/OAUTHBEARER configuration. The format is implementation-dependent and must be parsed accordingly. The default unsecured token implementation (see https://tools.ietf.org/html/rfc7515#appendix-A.5) recognizes space-separated name=value pairs with valid names including principalClaimName, principal, scopeClaimName, scope, and lifeSeconds. The default value for principalClaimName is "sub", the default value for scopeClaimName is "scope", and the default value for lifeSeconds is 3600. The scope value is CSV format with the default value being no/empty scope. For example: `principalClaimName=azp principal=admin scopeClaimName=roles scope=role1,role2 lifeSeconds=600`. In addition, SASL extensions can be communicated to the broker via `extension_NAME=value`. For example: `principal=admin extension_traceId=123` <br>*Type: string*
//enable.sasl.oauthbearer.unsecure.jwt     |  *  | true, false     |         false | low        | Enable the builtin unsecure JWT OAUTHBEARER token handler if no oauthbearer_refresh_cb has been set. This builtin handler should only be used for development or testing, and not in production. <br>*Type: boolean*
//oauthbearer_token_refresh_cb             |  *  |                 |               | low        | SASL/OAUTHBEARER token refresh callback (set with rd_kafka_conf_set_oauthbearer_token_refresh_cb(), triggered by rd_kafka_poll(), et.al. This callback will be triggered when it is time to refresh the client's OAUTHBEARER token. Also see `rd_kafka_conf_enable_sasl_queue()`. <br>*Type: see dedicated API*
//sasl.oauthbearer.method                  |  *  | default, oidc   |       default | low        | Set to "default" or "oidc" to control which login method to be used. If set to "oidc", the following properties must also be be specified: `sasl.oauthbearer.client.id`, `sasl.oauthbearer.client.secret`, and `sasl.oauthbearer.token.endpoint.url`. <br>*Type: enum value*
//sasl.oauthbearer.client.id               |  *  |                 |               | low        | Public identifier for the application. Must be unique across all clients that the authorization server handles. Only used when `sasl.oauthbearer.method` is set to "oidc". <br>*Type: string*
//sasl.oauthbearer.client.secret           |  *  |                 |               | low        | Client secret only known to the application and the authorization server. This should be a sufficiently random string that is not guessable. Only used when `sasl.oauthbearer.method` is set to "oidc". <br>*Type: string*
//sasl.oauthbearer.scope                   |  *  |                 |               | low        | Client use this to specify the scope of the access request to the broker. Only used when `sasl.oauthbearer.method` is set to "oidc". <br>*Type: string*
//sasl.oauthbearer.extensions              |  *  |                 |               | low        | Allow additional information to be provided to the broker. Comma-separated list of key=value pairs. E.g., "supportFeatureX=true,organizationId=sales-emea".Only used when `sasl.oauthbearer.method` is set to "oidc". <br>*Type: string*
//sasl.oauthbearer.token.endpoint.url      |  *  |                 |               | low        | OAuth/OIDC issuer token endpoint HTTP(S) URI used to retrieve token. Only used when `sasl.oauthbearer.method` is set to "oidc". <br>*Type: string*
//plugin.library.paths                     |  *  |                 |               | low        | List of plugin libraries to load (; separated). The library search path is platform dependent (see dlopen(3) for Unix and LoadLibrary() for Windows). If no filename extension is specified the platform-specific extension (such as .dll or .so) will be appended automatically. <br>*Type: string*
//interceptors                             |  *  |                 |               | low        | Interceptors added through rd_kafka_conf_interceptor_add_..() and any configuration handled by interceptors. <br>*Type: see dedicated API*
//client.rack                              |  *  |                 |               | low        | A rack identifier for this client. This can be any string value which indicates where this client is physically located. It corresponds with the broker config `broker.rack`. <br>*Type: string*

//group.id                                 |  C  |                 |               | high       | Client group id string. All clients sharing the same group.id belong to the same group. <br>*Type: string*
//group.instance.id                        |  C  |                 |               | medium     | Enable static group membership. Static group members are able to leave and rejoin a group within the configured `session.timeout.ms` without prompting a group rebalance. This should be used in combination with a larger `session.timeout.ms` to avoid group rebalances caused by transient unavailability (e.g. process restarts). Requires broker version >= 2.3.0. <br>*Type: string*
//partition.assignment.strategy            |  C  |                 | range,roundrobin | medium     | The name of one or more partition assignment strategies. The elected group leader will use a strategy supported by all members of the group to assign partitions to group members. If there is more than one eligible strategy, preference is determined by the order of this list (strategies earlier in the list have higher priority). Cooperative and non-cooperative (eager) strategies must not be mixed. Available strategies: range, roundrobin, cooperative-sticky. <br>*Type: string*
//session.timeout.ms                       |  C  | 1 .. 3600000    |         45000 | high       | Client group session and failure detection timeout. The consumer sends periodic heartbeats (heartbeat.interval.ms) to indicate its liveness to the broker. If no hearts are received by the broker for a group member within the session timeout, the broker will remove the consumer from the group and trigger a rebalance. The allowed range is configured with the **broker** configuration properties `group.min.session.timeout.ms` and `group.max.session.timeout.ms`. Also see `max.poll.interval.ms`. <br>*Type: integer*
//heartbeat.interval.ms                    |  C  | 1 .. 3600000    |          3000 | low        | Group session keepalive heartbeat interval. <br>*Type: integer*
//group.protocol.type                      |  C  |                 |      consumer | low        | Group protocol type. NOTE: Currently, the only supported group protocol type is `consumer`. <br>*Type: string*
//coordinator.query.interval.ms            |  C  | 1 .. 3600000    |        600000 | low        | How often to query for the current client group coordinator. If the currently assigned coordinator is down the configured query interval will be divided by ten to more quickly recover in case of coordinator reassignment. <br>*Type: integer*
//max.poll.interval.ms                     |  C  | 1 .. 86400000   |        300000 | high       | Maximum allowed time between calls to consume messages (e.g., rd_kafka_consumer_poll()) for high-level consumers. If this interval is exceeded the consumer is considered failed and the group will rebalance in order to reassign the partitions to another consumer group member. Warning: Offset commits may be not possible at this point. Note: It is recommended to set `enable.auto.offset.store=false` for long-time processing applications and then explicitly store offsets (using offsets_store()) *after* message processing, to make sure offsets are not auto-committed prior to processing has finished. The interval is checked two times per second. See KIP-62 for more information. <br>*Type: integer*
//enable.auto.commit                       |  C  | true, false     |          true | high       | Automatically and periodically commit offsets in the background. Note: setting this to false does not prevent the consumer from fetching previously committed start offsets. To circumvent this behaviour set specific start offsets per partition in the call to assign(). <br>*Type: boolean*
//auto.commit.interval.ms                  |  C  | 0 .. 86400000   |          5000 | medium     | The frequency in milliseconds that the consumer offsets are committed (written) to offset storage. (0 = disable). This setting is used by the high-level consumer. <br>*Type: integer*
//enable.auto.offset.store                 |  C  | true, false     |          true | high       | Automatically store offset of last message provided to application. The offset store is an in-memory store of the next offset to (auto-)commit for each partition. <br>*Type: boolean*
//queued.min.messages                      |  C  | 1 .. 10000000   |        100000 | medium     | Minimum number of messages per topic+partition librdkafka tries to maintain in the local consumer queue. <br>*Type: integer*
//queued.max.messages.kbytes               |  C  | 1 .. 2097151    |         65536 | medium     | Maximum number of kilobytes of queued pre-fetched messages in the local consumer queue. If using the high-level consumer this setting applies to the single consumer queue, regardless of the number of partitions. When using the legacy simple consumer or when separate partition queues are used this setting applies per partition. This value may be overshot by fetch.message.max.bytes. This property has higher priority than queued.min.messages. <br>*Type: integer*
//fetch.wait.max.ms                        |  C  | 0 .. 300000     |           500 | low        | Maximum time the broker may wait to fill the Fetch response with fetch.min.bytes of messages. <br>*Type: integer*
//fetch.message.max.bytes                  |  C  | 1 .. 1000000000 |       1048576 | medium     | Initial maximum number of bytes per topic+partition to request when fetching messages from the broker. If the client encounters a message larger than this value it will gradually try to increase it until the entire message can be fetched. <br>*Type: integer*
//max.partition.fetch.bytes                |  C  | 1 .. 1000000000 |       1048576 | medium     | Alias for `fetch.message.max.bytes`: Initial maximum number of bytes per topic+partition to request when fetching messages from the broker. If the client encounters a message larger than this value it will gradually try to increase it until the entire message can be fetched. <br>*Type: integer*
//fetch.max.bytes                          |  C  | 0 .. 2147483135 |      52428800 | medium     | Maximum amount of data the broker shall return for a Fetch request. Messages are fetched in batches by the consumer and if the first message batch in the first non-empty partition of the Fetch request is larger than this value, then the message batch will still be returned to ensure the consumer can make progress. The maximum message batch size accepted by the broker is defined via `message.max.bytes` (broker config) or `max.message.bytes` (broker topic config). `fetch.max.bytes` is automatically adjusted upwards to be at least `message.max.bytes` (consumer config). <br>*Type: integer*
//fetch.min.bytes                          |  C  | 1 .. 100000000  |             1 | low        | Minimum number of bytes the broker responds with. If fetch.wait.max.ms expires the accumulated data will be sent to the client regardless of this setting. <br>*Type: integer*
//fetch.error.backoff.ms                   |  C  | 0 .. 300000     |           500 | medium     | How long to postpone the next fetch request for a topic+partition in case of a fetch error. <br>*Type: integer*
//isolation.level                          |  C  | read_uncommitted, read_committed | read_committed | high       | Controls how to read messages written transactionally: `read_committed` - only return transactional messages which have been committed. `read_uncommitted` - return all messages, even transactional messages which have been aborted. <br>*Type: enum value*
//consume_cb                               |  C  |                 |               | low        | Message consume callback (set with rd_kafka_conf_set_consume_cb()) <br>*Type: see dedicated API*
//rebalance_cb                             |  C  |                 |               | low        | Called after consumer group has been rebalanced (set with rd_kafka_conf_set_rebalance_cb()) <br>*Type: see dedicated API*
//offset_commit_cb                         |  C  |                 |               | low        | Offset commit result propagation callback. (set with rd_kafka_conf_set_offset_commit_cb()) <br>*Type: see dedicated API*
//enable.partition.eof                     |  C  | true, false     |         false | low        | Emit RD_KAFKA_RESP_ERR__PARTITION_EOF event whenever the consumer reaches the end of a partition. <br>*Type: boolean*
//check.crcs                               |  C  | true, false     |         false | medium     | Verify CRC32 of consumed messages, ensuring no on-the-wire or on-disk corruption to the messages occurred. This check comes at slightly increased CPU usage. <br>*Type: boolean*
//allow.auto.create.topics                 |  C  | true, false     |         false | low        | Allow automatic topic creation on the broker when subscribing to or assigning non-existent topics. The broker must also be configured with `auto.create.topics.enable=true` for this configuraiton to take effect. Note: The default value (false) is different from the Java consumer (true). Requires broker version >= 0.11.0.0, for older broker versions only the broker configuration applies. <br>*Type: boolean*

//transactional.id                         |  P  |                 |               | high       | Enables the transactional producer. The transactional.id is used to identify the same transactional producer instance across process restarts. It allows the producer to guarantee that transactions corresponding to earlier instances of the same producer have been finalized prior to starting any new transactions, and that any zombie instances are fenced off. If no transactional.id is provided, then the producer is limited to idempotent delivery (if enable.idempotence is set). Requires broker version >= 0.11.0. <br>*Type: string*
//transaction.timeout.ms                   |  P  | 1000 .. 2147483647 |         60000 | medium     | The maximum amount of time in milliseconds that the transaction coordinator will wait for a transaction status update from the producer before proactively aborting the ongoing transaction. If this value is larger than the `transaction.max.timeout.ms` setting in the broker, the init_transactions() call will fail with ERR_INVALID_TRANSACTION_TIMEOUT. The transaction timeout automatically adjusts `message.timeout.ms` and `socket.timeout.ms`, unless explicitly configured in which case they must not exceed the transaction timeout (`socket.timeout.ms` must be at least 100ms lower than `transaction.timeout.ms`). This is also the default timeout value if no timeout (-1) is supplied to the transactional API methods. <br>*Type: integer*
//enable.idempotence                       |  P  | true, false     |         false | high       | When set to `true`, the producer will ensure that messages are successfully produced exactly once and in the original produce order. The following configuration properties are adjusted automatically (if not modified by the user) when idempotence is enabled: `max.in.flight.requests.per.connection=5` (must be less than or equal to 5), `retries=INT32_MAX` (must be greater than 0), `acks=all`, `queuing.strategy=fifo`. Producer instantation will fail if user-supplied configuration is incompatible. <br>*Type: boolean*
//queue.buffering.max.messages             |  P  | 1 .. 10000000   |        100000 | high       | Maximum number of messages allowed on the producer queue. This queue is shared by all topics and partitions. <br>*Type: integer*
//queue.buffering.max.kbytes               |  P  | 1 .. 2147483647 |       1048576 | high       | Maximum total message size sum allowed on the producer queue. This queue is shared by all topics and partitions. This property has higher priority than queue.buffering.max.messages. <br>*Type: integer*
//queue.buffering.max.ms                   |  P  | 0 .. 900000     |             5 | high       | Delay in milliseconds to wait for messages in the producer queue to accumulate before constructing message batches (MessageSets) to transmit to brokers. A higher value allows larger and more effective (less overhead, improved compression) batches of messages to accumulate at the expense of increased message delivery latency. <br>*Type: float*
//message.send.max.retries                 |  P  | 0 .. 2147483647 |    2147483647 | high       | How many times to retry sending a failing Message. **Note:** retrying may cause reordering unless `enable.idempotence` is set to true. <br>*Type: integer*
//retry.backoff.ms                         |  P  | 1 .. 300000     |           100 | medium     | The backoff time in milliseconds before retrying a protocol request. <br>*Type: integer*
//queue.buffering.backpressure.threshold   |  P  | 1 .. 1000000    |             1 | low        | The threshold of outstanding not yet transmitted broker requests needed to backpressure the producer's message accumulator. If the number of not yet transmitted requests equals or exceeds this number, produce request creation that would have otherwise been triggered (for example, in accordance with linger.ms) will be delayed. A lower number yields larger and more effective batches. A higher value can improve latency when using compression on slow machines. <br>*Type: integer*
//compression.codec                        |  P  | none, gzip, snappy, lz4, zstd |          none | medium     | compression codec to use for compressing message sets. This is the default value for all topics, may be overridden by the topic configuration property `compression.codec`.  <br>*Type: enum value*
//batch.num.messages                       |  P  | 1 .. 1000000    |         10000 | medium     | Maximum number of messages batched in one MessageSet. The total MessageSet size is also limited by batch.size and message.max.bytes. <br>*Type: integer*
//batch.size                               |  P  | 1 .. 2147483647 |       1000000 | medium     | Maximum size (in bytes) of all messages batched in one MessageSet, including protocol framing overhead. This limit is applied after the first message has been added to the batch, regardless of the first message's size, this is to ensure that messages that exceed batch.size are produced. The total MessageSet size is also limited by batch.num.messages and message.max.bytes. <br>*Type: integer*
//delivery.report.only.error               |  P  | true, false     |         false | low        | Only provide delivery reports for failed messages. <br>*Type: boolean*
//dr_msg_cb                                |  P  |                 |               | low        | Delivery report callback (set with rd_kafka_conf_set_dr_msg_cb()) <br>*Type: see dedicated API*
//sticky.partitioning.linger.ms            |  P  | 0 .. 900000     |            10 | low        | Delay in milliseconds to wait to assign new sticky partitions for each topic. By default, set to double the time of linger.ms. To disable sticky behavior, set to 0. This behavior affects messages with the key NULL in all cases, and messages with key lengths of zero when the consistent_random partitioner is in use. These messages would otherwise be assigned randomly. A higher value allows for more effective batching of these messages. <br>*Type: integer*

// KafkaTopicConfig static variables
const std::string KafkaTopicConfig::kRequestRequiredAcks = "request.required.acks";
const std::string KafkaTopicConfig::kRequestTimeoutMS = "request.timeout.ms";
const std::string KafkaTopicConfig::kMessageTimeoutMS = "message.timeout.ms";
const std::string KafkaTopicConfig::kPartitioner = "partitioner";
const std::string KafkaTopicConfig::kCompressionCodec = "compression.codec";
const std::string KafkaTopicConfig::kCompressionLevel = "compression.level";
const std::string KafkaTopicConfig::kAutoOffsetReset = "auto.offset.reset";

} // namespace ant