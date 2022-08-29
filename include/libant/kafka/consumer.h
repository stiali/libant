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
	 * @param cfg global config
	 */
    KafkaConsumer(KafkaGlobalConfig cfg)
        : cfg_(std::move(cfg))
    {
    }

    ~KafkaConsumer()
    {
        // consumer_->close(); // TODO andy: It sometimes blocks forever! Need it or not?
    }

    /**
     * Initialize the KafkaConsumer object.
     * @return true on success, false on error and ErrorMessage() could be called to get the error message
     */
    bool Init()
    {
        consumer_ = std::unique_ptr<RdKafka::KafkaConsumer>(RdKafka::KafkaConsumer::create(cfg_.conf_.get(), errMsg_));
        if (consumer_) {
            return true;
        }
        return false;
    }

    /**
     * @brief Update the subscription set to \p topics.
     *
     * Any previous subscription will be unassigned and unsubscribed first.
     *
     * The subscription set denotes the desired topics to consume and this
     * set is provided to the partition assignor (one of the elected group
     * members) for all clients which then uses the configured
     * \c partition.assignment.strategy to assign the subscription sets'
     * topics' partitions to the consumers, depending on their subscription.
     *
     * The result of such an assignment is a rebalancing which is either
     * handled automatically in librdkafka or can be overridden by the application
     * by providing a RdKafka::RebalanceCb.
     *
     * The rebalancing passes the assigned partition set to
     * RdKafka::KafkaConsumer::assign() to update what partitions are actually
     * being fetched by the KafkaConsumer.
     *
     * Regex pattern matching automatically performed for topics prefixed
     * with \c \"^\" (e.g. \c \"^myPfx[0-9]_.*\"
     *
     * @returns an error if the provided list of topics is invalid and ErrorMessage() could be called to get the error message.
     */
    KafkaErrCode Subscribe(const std::vector<std::string>& topics)
    {
        auto r = consumer_->subscribe(topics);
        if (r == RdKafka::ERR_NO_ERROR) {
            return RdKafka::ERR_NO_ERROR;
        }
        errMsg_ = RdKafka::err2str(r);
        return r;
    }

    /**
	 * @brief Consume a message or error event. A corresponding callback will be triggered
     * @param timeoutMS: 0 for non-blocking, -1 for block as long as it takes to get a Message
	 * @returns One of:
	 *  - proper message (Message::err() is ERR_NO_ERROR)
	 *  - error event (Message::err() is != ERR_NO_ERROR)
	 *  - timeout due to no message or event in timeoutMS
	 *    (RdKafka::Message::err() is ERR__TIMED_OUT)
	 */
    std::unique_ptr<Message> Consume(int timeoutMS)
    {
        return std::unique_ptr<Message>(consumer_->consume(timeoutMS));
    }

    /**
	 * @brief Commit offset asynchronously
	 */
    void AsyncCommit(const std::unique_ptr<Message>& msg)
    {
        consumer_->commitAsync(msg.get());
    }

    /**
     * Gets the last error message
     * @return last error message
     */
    const std::string& ErrorMessage() const
    {
        return errMsg_;
    }

private:
    KafkaGlobalConfig cfg_;
    std::string errMsg_;
    std::unique_ptr<RdKafka::KafkaConsumer> consumer_;
};

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_KAFKA_CONSUMER_H_
