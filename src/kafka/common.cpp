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
const std::string k = "";

const std::string KafkaGlobalConfig::kGroupID = "group.id";
const std::string KafkaGlobalConfig::kEnableAutoCommit = "enable.auto.commit";
const std::string KafkaGlobalConfig::kEnableAutoOffsetStore = "enable.auto.offset.store";

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

// KafkaTopicConfig static variables
const std::string KafkaTopicConfig::kRequestRequiredAcks = "request.required.acks";
const std::string KafkaTopicConfig::kRequestTimeoutMS = "request.timeout.ms";
const std::string KafkaTopicConfig::kMessageTimeoutMS = "message.timeout.ms";
const std::string KafkaTopicConfig::kPartitioner = "partitioner";
const std::string KafkaTopicConfig::kCompressionCodec = "compression.codec";
const std::string KafkaTopicConfig::kCompressionLevel = "compression.level";
const std::string KafkaTopicConfig::kAutoOffsetReset = "auto.offset.reset";

} // namespace ant