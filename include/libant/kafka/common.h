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

#include <stdexcept>
#include <string>
#include <unordered_map>

#include <librdkafka/rdkafkacpp.h>

namespace ant {

using KafkaErrCode = RdKafka::ErrorCode;

/**
 * KafkaGlobalConfig
 */
class KafkaGlobalConfig {
public:
public:
    /**
     * Sets a global configuration
     * @param key
     * @param value
     */
    void Set(const std::string& key, const std::string& value)
    {
        configs_[key] = value;
    }

private:
    std::unordered_map<std::string, std::string> configs_;
};

/**
 * KafkaTopicConfig
 */
class KafkaTopicConfig {
public:
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
     */
    void Set(const std::string& key, const std::string& value)
    {
        configs_[key] = value;
    }

private:
    std::unordered_map<std::string, std::string> configs_;
};

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
