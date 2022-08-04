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
