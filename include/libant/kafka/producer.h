#ifndef LIBANT_INCLUDE_LIBANT_KAFKA_PRODUCER_H_
#define LIBANT_INCLUDE_LIBANT_KAFKA_PRODUCER_H_

#include <mutex>

#include "common.h"

namespace ant {

/**
 * @brief A simple wrapper for RdKafka::Producer
 */
class KafkaProducer {
public:
    /**
	 * @brief Constructor
	 * @param brokers broker addresses separated by ','. eg: "1.1.1.1:1111,2.2.2.2:2222"
	 * @param topic
     * @param drcb delivery report callback
     * @param evcb event callback
	 */
    KafkaProducer(const std::string& brokers, const std::string& topic, RdKafka::DeliveryReportCb* drcb = nullptr, RdKafka::EventCb* evcb = nullptr);

    ~KafkaProducer()
    {
        producer_->flush(10000);
        delete topic_;
        delete producer_;
    }

    /**
	 * @brief Send a message to a kafka broker asynchronously
	 * @param payload message payload
	 * @param msgOpaque passed to delivery report callback as it is
	 * @return
	 *  - ERR_NO_ERROR           - Success
	 *  - ERR__QUEUE_FULL        - queue.buffering.max.message
	 *  - ERR_MSG_SIZE_TOO_LARGE - messages.max.bytes
	 *  - ERR__UNKNOWN_PARTITION
	 *  - ERR__UNKNOWN_TOPIC
	 */
    KafkaErrCode Produce(const std::string& payload, void* msgOpaque = nullptr)
    {
        // Message is dequeued only after delivery report callback is called.
        // So don't retry immediately after Produce() tells us that the queue is full.
        // Retry periodically after Poll().
        return producer_->produce(topic_, RdKafka::Topic::PARTITION_UA, RdKafka::Producer::RK_MSG_COPY, const_cast<char*>(payload.data()), payload.size(),
                                  nullptr, msgOpaque);
    }

    /**
	* @brief Send a message to a kafka broker synchronously
	* @param payload message payload
	* @return KafkaErrCode as documented at Produce()
	*/
    KafkaErrCode SyncProduce(const std::string& payload);

    /**
	 * @brief Call this method periodically to trigger callbacks.
	 * @param timeoutMS blocking time in milliseconds. 0 for non-blocking, -1 for block as long as it takes to trigger a callback
	 * @returns number of callbacks triggered
	 */
    int Poll(int timeoutMS = 0)
    {
        return producer_->poll(timeoutMS);
    }

private:
    RdKafka::Producer* producer_;
    RdKafka::Topic* topic_;
};

/**
* @brief KafkaProducer object pool, thread-safe。
*/
class KafkaProducerPool {
public:
    /**
	 * @brief Constructor
     * @param maxPooledProducers
     * @param brokers
     * @param topic
     * @param drcb make sure your callback is thread-safe
     * @param evcb make sure your callback is thread-safe
	 * @note User of KafkaProducerPool must make sure that drcb and evcb are thread-safe
	 */
    KafkaProducerPool(size_t maxPooledProducers, const std::string& brokers, const std::string& topic, RdKafka::DeliveryReportCb* drcb = nullptr,
                      RdKafka::EventCb* evcb = nullptr)
        : maxPooledProducers_(maxPooledProducers)
        , brokers_(brokers)
        , topic_(topic)
        , drcb_(drcb)
        , evcb_(evcb)
    {
    }

    ~KafkaProducerPool()
    {
        for (auto cli : pooledProducers_) {
            delete cli;
        }
    }

    KafkaProducerPool(const KafkaProducerPool&) = delete;
    const KafkaProducerPool& operator=(const KafkaProducerPool&) = delete;

    /**
	 * @brief Get a KafkaProducer object. Return it back to the pool by calling Put().
	 */
    KafkaProducer* Get();

    /**
	 * @brief Return a KafkaProducer object to the pool.
	 */
    void Put(KafkaProducer* producer);

private:
    const size_t maxPooledProducers_;
    const std::string brokers_;
    const std::string topic_;
    RdKafka::DeliveryReportCb* const drcb_;
    RdKafka::EventCb* const evcb_;

    std::mutex lock_;
    std::list<KafkaProducer*> pooledProducers_;
};

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_KAFKA_PRODUCER_H_
