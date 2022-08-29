#include <memory>
#include <libant/utils/likely.h>
#include <libant/kafka/producer.h>

using namespace std;

namespace ant {

//===========================================================================
// Public methods
//===========================================================================

KafkaErrCode KafkaProducer::Produce(const std::string& topic, const std::string& payload, void* msgOpaque)
{
    auto it = topics_.find(topic);
    if (unlikely(it == topics_.end())) {
        auto newTopic = RdKafka::Topic::create(producer_.get(), topic, nullptr, errMsg_);
        if (unlikely(!newTopic)) {
            return RdKafka::ERR__UNKNOWN_TOPIC;
        }

        auto r = topics_.emplace(topic, unique_ptr<RdKafka::Topic>(newTopic));
        it = r.first;
    }
    // Message is dequeued only after delivery report callback is called.
    // So don't retry immediately after Produce() tells us that the queue is full.
    // Retry periodically after Poll().
    return producer_->produce(it->second.get(), RdKafka::Topic::PARTITION_UA, RdKafka::Producer::RK_MSG_COPY, const_cast<char*>(payload.data()), payload.size(),
                              nullptr, msgOpaque);
}

} // namespace ant