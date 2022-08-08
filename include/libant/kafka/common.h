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
