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

#ifndef LIBANT_INCLUDE_LIBANT_KAFKA_CONSUMER_H_
#define LIBANT_INCLUDE_LIBANT_KAFKA_CONSUMER_H_

#include <vector>

#include "common.h"

namespace ant {

/**
 * @brief A simple wrapper for RdKafka::KafkaConsumer
 */
class KafkaConsumer {
public:
    using Message = RdKafka::Message;

public:
    /**
	 * @brief Constructor
	 * @param brokers broker addresses separated by ','. eg: "1.1.1.1:1111,2.2.2.2:2222"
	 * @param groupID
	 * @param topics topic list separated by ','
     * @param offsetCb offset commit callback
     * @param evcb event callback
	 */
    KafkaConsumer(const std::string& brokers, const std::string& groupID, const std::string& topics, RdKafka::OffsetCommitCb* offsetCb = nullptr,
                  RdKafka::EventCb* evcb = nullptr);

    ~KafkaConsumer()
    {
        // consumer_->close(); // TODO andy: It sometimes blocks forever! Need it or not?
        delete consumer_;
    }

    /**
	 * @brief Consume a message or error event. A corresponding callback will be triggered
	 * @note use delete to free message
	 * @returns One of:
	 *  - proper message (Message::err() is ERR_NO_ERROR)
	 *  - error event (Message::err() is != ERR_NO_ERROR)
	 *  - timeout due to no message or event in timeoutMS
	 *    (RdKafka::Message::err() is ERR__TIMED_OUT)
	 */
    Message* Consume(int timeoutMS)
    {
        return consumer_->consume(timeoutMS);
    }

    /**
	 * @brief Commit offset asynchronously
	 */
    void AsyncCommit(Message* msg)
    {
        consumer_->commitAsync(msg);
    }

private:
    RdKafka::KafkaConsumer* consumer_;
};

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_KAFKA_CONSUMER_H_
