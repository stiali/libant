#ifndef LIBANT_INCLUDE_LIBANT_KAFKA_COMMON_H_
#define LIBANT_INCLUDE_LIBANT_KAFKA_COMMON_H_

#include <stdexcept>
#include <string>

#include <librdkafka/rdkafkacpp.h>

namespace ant {

using KafkaErrCode = RdKafka::ErrorCode;

inline void setConfig(RdKafka::Conf* conf, const std::string& key, const std::string& value)
{
    std::string errstr;
    if (conf->set(key, value, errstr) == RdKafka::Conf::CONF_OK) {
        return;
    }
    throw std::runtime_error("Failed to set '" + key + "' to '" + value + "'! err=" + errstr);
}

inline void setConfig(RdKafka::Conf* conf, const std::string& key, RdKafka::EventCb* cbObj)
{
    std::string errstr;
    if (conf->set(key, cbObj, errstr) == RdKafka::Conf::CONF_OK) {
        return;
    }
    throw std::runtime_error("Failed to set '" + key + "'! err=" + errstr);
}

inline void setConfig(RdKafka::Conf* conf, const std::string& key, RdKafka::OffsetCommitCb* cbObj)
{
    std::string errstr;
    if (conf->set(key, cbObj, errstr) == RdKafka::Conf::CONF_OK) {
        return;
    }
    throw std::runtime_error("Failed to set '" + key + "'! err=" + errstr);
}

inline void setConfig(RdKafka::Conf* conf, const std::string& key, RdKafka::DeliveryReportCb* cbObj)
{
    std::string errstr;
    if (conf->set(key, cbObj, errstr) == RdKafka::Conf::CONF_OK) {
        return;
    }
    throw std::runtime_error("Failed to set '" + key + "'! err=" + errstr);
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_KAFKA_COMMON_H_
