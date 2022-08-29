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

#ifndef LIBANT_INCLUDE_LIBANT_KAFKA_PRODUCER_H_
#define LIBANT_INCLUDE_LIBANT_KAFKA_PRODUCER_H_

#include "common.h"

namespace ant {

/**
 * @brief A simple wrapper for RdKafka::Producer
 */
class KafkaProducer {
public:
    /**
	 * @brief Constructor
	 * @param cfg global config
	 */
    KafkaProducer(KafkaGlobalConfig cfg)
        : cfg_(std::move(cfg))
    {
    }

    ~KafkaProducer()
    {
        producer_->flush(3000);
    }

    /**
     * Initialize the KafkaProducer object.
     * @return true on success, false on error and ErrorMessage() could be called to get the error message
     */
    bool Init()
    {
        producer_ = std::unique_ptr<RdKafka::Producer>(RdKafka::Producer::create(cfg_.conf_.get(), errMsg_));
        if (producer_) {
            return true;
        }
        return false;
    }

    /**
	 * @brief Send a message to a kafka broker asynchronously
     * @param topic topic name
	 * @param payload message payload
	 * @param msgOpaque passed to delivery report callback as it is
	 * @return
	 *  - ERR_NO_ERROR           - Success
	 *  - ERR__QUEUE_FULL        - queue.buffering.max.message
	 *  - ERR_MSG_SIZE_TOO_LARGE - messages.max.bytes
	 *  - ERR__UNKNOWN_PARTITION
	 *  - ERR__UNKNOWN_TOPIC
     * @warning
     *  Message is dequeued only after delivery report callback is called.
     *  So don't retry immediately after Produce() tells us that the queue is full.
     *  Retry periodically after Poll().
	 */
    KafkaErrCode Produce(const std::string& topic, const std::string& payload, void* msgOpaque = nullptr)
    {
        return producer_->produce(topic, RdKafka::Topic::PARTITION_UA, RdKafka::Producer::RK_MSG_COPY, const_cast<char*>(payload.data()), payload.size(),
                                  nullptr, 0, 0, msgOpaque);
    }

    /**
	 * @brief Send a message to a kafka broker asynchronously
     * @param topic topic name
	 * @param payload message payload
     * @param headers Headers to be sent alongside with the message payload
	 * @param msgOpaque passed to delivery report callback as it is
	 * @return
	 *  - ERR_NO_ERROR           - Success
	 *  - ERR__QUEUE_FULL        - queue.buffering.max.message
	 *  - ERR_MSG_SIZE_TOO_LARGE - messages.max.bytes
	 *  - ERR__UNKNOWN_PARTITION
	 *  - ERR__UNKNOWN_TOPIC
     * @warning
     *  The underlying memory for headers will be freed if the Produce() call succeeds, or left untouched if Produce() fails.
     *
     *  Message is dequeued only after delivery report callback is called.
     *  So don't retry immediately after Produce() tells us that the queue is full.
     *  Retry periodically after Poll().
	 */
    KafkaErrCode Produce(const std::string& topic, const std::string& payload, KafkaHeaders& headers, void* msgOpaque = nullptr)
    {
        auto h = headers.headers_.release();
        auto r = producer_->produce(topic, RdKafka::Topic::PARTITION_UA, RdKafka::Producer::RK_MSG_COPY, const_cast<char*>(payload.data()), payload.size(),
                                    nullptr, 0, 0, h, msgOpaque);
        if (r == RdKafka::ERR_NO_ERROR) {
            return RdKafka::ERR_NO_ERROR;
        }
        headers.headers_.reset(h);
        return r;
    }

    /**
	 * @brief Call this method periodically to trigger callbacks.
	 * @param timeoutMS blocking time in milliseconds. 0 for non-blocking, -1 for block as long as it takes to trigger a callback
	 * @returns number of callbacks triggered
	 */
    int Poll(int timeoutMS = 0)
    {
        return producer_->poll(timeoutMS);
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
    std::unique_ptr<RdKafka::Producer> producer_;
};

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_KAFKA_PRODUCER_H_
