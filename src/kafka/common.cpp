#include <libant/kafka/common.h>

namespace ant {

// KafkaGlobalConfig static variables
// For Both
const std::string KafkaGlobalConfig::kBrokerList = "metadata.broker.list";
const std::string KafkaGlobalConfig::kMessageMaxBytes = "message.max.bytes";
const std::string KafkaGlobalConfig::kMessageCopyMaxBytes = "message.copy.max.bytes";
const std::string KafkaGlobalConfig::kReceiveMessageMaxBytes = "receive.message.max.bytes";
const std::string KafkaGlobalConfig::kMaxInFlightRequestsPerConnection = "max.in.flight.requests.per.connection";
const std::string KafkaGlobalConfig::kTopicMetadataRefreshIntervalMS = "topic.metadata.refresh.interval.ms";
const std::string KafkaGlobalConfig::kMetadataMaxAgeMS = "metadata.max.age.ms";
const std::string KafkaGlobalConfig::kTopicMetadataRefreshFastIntervalMS = "topic.metadata.refresh.fast.interval.ms";
const std::string KafkaGlobalConfig::kTopicMetadataRefreshSparse = "topic.metadata.refresh.sparse";
const std::string KafkaGlobalConfig::kTopicMetadataPropagationMaxMS = "topic.metadata.propagation.max.ms";
const std::string KafkaGlobalConfig::kTopicBlacklist = "topic.blacklist";
const std::string KafkaGlobalConfig::kSocketTimeoutMS = "socket.timeout.ms";
const std::string KafkaGlobalConfig::kSocketSendBufferBytes = "socket.send.buffer.bytes";
const std::string KafkaGlobalConfig::kSocketReceiveBufferBytes = "socket.receive.buffer.bytes";
const std::string KafkaGlobalConfig::kSocketKeepaliveEnable = "socket.keepalive.enable";
const std::string KafkaGlobalConfig::kSocketNagleDisable = "socket.nagle.disable";
const std::string KafkaGlobalConfig::kSocketMaxFails = "socket.max.fails";
const std::string KafkaGlobalConfig::kBrokerAddressTTL = "broker.address.ttl";
const std::string KafkaGlobalConfig::kBrokerAddressFamily = "broker.address.family";
const std::string KafkaGlobalConfig::kSocketConnectionSetupTimeoutMS = "socket.connection.setup.timeout.ms";
const std::string KafkaGlobalConfig::kConnectionsMaxIdleMS = "connections.max.idle.ms";
const std::string KafkaGlobalConfig::kReconnectBackoffMS = "reconnect.backoff.ms";
const std::string KafkaGlobalConfig::kReconnectBackoffMaxMS = "reconnect.backoff.max.ms";
const std::string KafkaGlobalConfig::kStatisticsIntervalMS = "statistics.interval.ms";
const std::string KafkaGlobalConfig::kEnabledEvents = "enabled_events";
const std::string KafkaGlobalConfig::kLogLevel = "log_level";
const std::string KafkaGlobalConfig::kLogQueue = "log.queue";
const std::string KafkaGlobalConfig::kLogThreadName = "log.thread.name";
const std::string KafkaGlobalConfig::kEnableRandomSeed = "enable.random.seed";
const std::string KafkaGlobalConfig::kLogConnectionClose = "log.connection.close";
const std::string KafkaGlobalConfig::kInternalTerminationSignal = "internal.termination.signal";
const std::string KafkaGlobalConfig::kApiVersionRequest = "api.version.request";
const std::string KafkaGlobalConfig::kApiVersionRequestTimeoutMS = "api.version.request.timeout.ms";
const std::string KafkaGlobalConfig::kApiVersionFallbackMS = "api.version.fallback.ms";
const std::string KafkaGlobalConfig::kBrokerVersionFallback = "broker.version.fallback";
const std::string KafkaGlobalConfig::kSecurityProtocol = "security.protocol";
const std::string KafkaGlobalConfig::kSslCipherSuites = "ssl.cipher.suites";
const std::string KafkaGlobalConfig::kSslCurvesList = "ssl.curves.list";
const std::string KafkaGlobalConfig::kSslSigalgsList = "ssl.sigalgs.list";
const std::string KafkaGlobalConfig::kSslKeyLocation = "ssl.key.location";
const std::string KafkaGlobalConfig::kSslKeyPassword = "ssl.key.password";
const std::string KafkaGlobalConfig::kSslKeyPem = "ssl.key.pem";
const std::string KafkaGlobalConfig::kSslCertificateLocation = "ssl.certificate.location";
const std::string KafkaGlobalConfig::kSslCertificatePem = "ssl.certificate.pem";
const std::string KafkaGlobalConfig::kSslCaLocation = "ssl.ca.location";
const std::string KafkaGlobalConfig::kSslCaPem = "ssl.ca.pem";
const std::string KafkaGlobalConfig::kSslCaCertificateStores = "ssl.ca.certificate.stores";
const std::string KafkaGlobalConfig::kSslCrlLocation = "ssl.crl.location";
const std::string KafkaGlobalConfig::kSslKeystoreLocation = "ssl.keystore.location";
const std::string KafkaGlobalConfig::kSslKeystorePassword = "ssl.keystore.password";
const std::string KafkaGlobalConfig::kSslEngineLocation = "ssl.engine.location";
const std::string KafkaGlobalConfig::kSslEngineID = "ssl.engine.id";
const std::string KafkaGlobalConfig::kEnableSslCertificateVerification = "enable.ssl.certificate.verification";
const std::string KafkaGlobalConfig::kSslEndpointIdentificationAlgorithm = "ssl.endpoint.identification.algorithm";
const std::string KafkaGlobalConfig::kSaslMechanisms = "sasl.mechanisms";
const std::string KafkaGlobalConfig::kSaslKerberosServiceName = "sasl.kerberos.service.name";
const std::string KafkaGlobalConfig::kSaslKerberosPrincipal = "sasl.kerberos.principal";
const std::string KafkaGlobalConfig::kSaslKerberosKinitCmd = "sasl.kerberos.kinit.cmd";
const std::string KafkaGlobalConfig::kSaslKerberosKeytab = "sasl.kerberos.keytab";
const std::string KafkaGlobalConfig::kSaslKerberosMinTimeBeforeReLogin = "sasl.kerberos.min.time.before.relogin";
const std::string KafkaGlobalConfig::kSaslUsername = "sasl.username";
const std::string KafkaGlobalConfig::kSaslPassword = "sasl.password";
const std::string KafkaGlobalConfig::kSaslOAuthBearerConfig = "sasl.oauthbearer.config";
const std::string KafkaGlobalConfig::kEnableSaslOAuthBearerUnsecureJwt = "enable.sasl.oauthbearer.unsecure.jwt";
const std::string KafkaGlobalConfig::kSaslOAuthBearerMethod = "sasl.oauthbearer.method";
const std::string KafkaGlobalConfig::kSaslOAuthBearerClientID = "sasl.oauthbearer.client.id";
const std::string KafkaGlobalConfig::kSaslOAuthBearerClientSecret = "sasl.oauthbearer.client.secret";
const std::string KafkaGlobalConfig::kSaslOAuthBearerScope = "sasl.oauthbearer.scope";
const std::string KafkaGlobalConfig::kSaslOAuthBearerExtensions = "sasl.oauthbearer.extensions";
const std::string KafkaGlobalConfig::kSaslOAuthBearerTokenEndpointURL = "sasl.oauthbearer.token.endpoint.url";
const std::string KafkaGlobalConfig::kPluginLibraryPaths = "plugin.library.paths";
const std::string KafkaGlobalConfig::kInterceptors = "interceptors";
const std::string KafkaGlobalConfig::kClientRack = "client.rack";
// For Producer
const std::string KafkaGlobalConfig::kTransactionalID = "transactional.id";
const std::string KafkaGlobalConfig::kTransactionTimeoutMS = "transaction.timeout.ms";
const std::string KafkaGlobalConfig::kEnableIdempotence = "enable.idempotence";
const std::string KafkaGlobalConfig::kQueueBufferingMaxMessages = "queue.buffering.max.messages";
const std::string KafkaGlobalConfig::kQueueBufferingMaxKBytes = "queue.buffering.max.kbytes";
const std::string KafkaGlobalConfig::kQueueBufferingMaxMS = "queue.buffering.max.ms";
const std::string KafkaGlobalConfig::kMessageSendMaxRetries = "message.send.max.retries";
const std::string KafkaGlobalConfig::kRetryBackoffMS = "retry.backoff.ms";
const std::string KafkaGlobalConfig::kQueueBufferingBackpressureThreshold = "queue.buffering.backpressure.threshold";
const std::string KafkaGlobalConfig::kCompressionCodec = "compression.codec";
const std::string KafkaGlobalConfig::kBatchNumMessages = "batch.num.messages";
const std::string KafkaGlobalConfig::kBatchSize = "batch.size";
const std::string KafkaGlobalConfig::kDeliveryReportOnlyError = "delivery.report.only.error";
const std::string KafkaGlobalConfig::kStickyPartitioningLingerMS = "sticky.partitioning.linger.ms";
// For Consumer
const std::string KafkaGlobalConfig::kGroupID = "group.id";
const std::string KafkaGlobalConfig::kGroupInstanceID = "group.instance.id";
const std::string KafkaGlobalConfig::kPartitionAssignmentStrategy = "partition.assignment.strategy";
const std::string KafkaGlobalConfig::kSessionTimeoutMS = "session.timeout.ms";
const std::string KafkaGlobalConfig::kHeartbeatIntervalMS = "heartbeat.interval.ms";
const std::string KafkaGlobalConfig::kGroupProtocolType = "group.protocol.type";
const std::string KafkaGlobalConfig::kCoordinatorQueryIntervalMS = "coordinator.query.interval.ms";
const std::string KafkaGlobalConfig::kMaxPollIntervalMS = "max.poll.interval.ms";
const std::string KafkaGlobalConfig::kEnableAutoCommit = "enable.auto.commit";
const std::string KafkaGlobalConfig::kAutoCommitIntervalMS = "auto.commit.interval.ms";
const std::string KafkaGlobalConfig::kEnableAutoOffsetStore = "enable.auto.offset.store";
const std::string KafkaGlobalConfig::kQueuedMinMessages = "queued.min.messages";
const std::string KafkaGlobalConfig::kQueuedMaxMessagesKBytes = "queued.max.messages.kbytes";
const std::string KafkaGlobalConfig::kFetchWaitMaxMS = "fetch.wait.max.ms";
const std::string KafkaGlobalConfig::kFetchMessageMaxBytes = "fetch.message.max.bytes";
const std::string KafkaGlobalConfig::kFetchMaxBytes = "fetch.max.bytes";
const std::string KafkaGlobalConfig::kFetchMinBytes = "fetch.min.bytes";
const std::string KafkaGlobalConfig::kFetchErrorBackoffMS = "fetch.error.backoff.ms";
const std::string KafkaGlobalConfig::kIsolationLevel = "isolation.level";
const std::string KafkaGlobalConfig::kEnablePartitionEOF = "enable.partition.eof";
const std::string KafkaGlobalConfig::kCheckCrcs = "check.crcs";
const std::string KafkaGlobalConfig::kAllowAutoCreateTopics = "allow.auto.create.topics";

// KafkaTopicConfig static variables
const std::string KafkaTopicConfig::kRequestRequiredAcks = "request.required.acks";
const std::string KafkaTopicConfig::kRequestTimeoutMS = "request.timeout.ms";
const std::string KafkaTopicConfig::kMessageTimeoutMS = "message.timeout.ms";
const std::string KafkaTopicConfig::kPartitioner = "partitioner";
const std::string KafkaTopicConfig::kCompressionCodec = "compression.codec";
const std::string KafkaTopicConfig::kCompressionLevel = "compression.level";
const std::string KafkaTopicConfig::kAutoOffsetReset = "auto.offset.reset";

} // namespace ant