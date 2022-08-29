#include <libant/kafka/common.h>

namespace ant {

// KafkaGlobalConfig static variables
const std::string KafkaGlobalConfig::kBrokerList = "metadata.broker.list";
const std::string KafkaGlobalConfig::kGroupID = "group.id";
const std::string KafkaGlobalConfig::kEnableAutoCommit = "enable.auto.commit";
const std::string KafkaGlobalConfig::kEnableAutoOffsetStore = "enable.auto.offset.store";

// KafkaTopicConfig static variables
const std::string KafkaTopicConfig::kRequestRequiredAcks = "request.required.acks";
const std::string KafkaTopicConfig::kRequestTimeoutMS = "request.timeout.ms";
const std::string KafkaTopicConfig::kMessageTimeoutMS = "message.timeout.ms";
const std::string KafkaTopicConfig::kPartitioner = "partitioner";
const std::string KafkaTopicConfig::kCompressionCodec = "compression.codec";
const std::string KafkaTopicConfig::kCompressionLevel = "compression.level";
const std::string KafkaTopicConfig::kAutoOffsetReset = "auto.offset.reset";

} // namespace ant