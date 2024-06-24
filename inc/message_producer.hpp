#pragma once

#include "queue.hpp"

#define DEFAULT_TX_TIMEOUT 100U
#define DEFAULT_TX_QUEUE_SIZE 10U

/**
 * @brief Template C++ wrapper for message producer objects
 * 
 * @tparam T type of messages in queue
 * @tparam queue_size queue length
 */
template <typename T, size_t queue_size = DEFAULT_TX_QUEUE_SIZE>
class MessageProducer {
public:
    /**
     * Constructs new MessageProducer object
     * @param queue outcoming queue
     */
    explicit MessageProducer(Queue<T, queue_size>* queue = nullptr)
        :   tx_queue_(queue) {
    }

    /**
     * Set outcoming queue
     * @param queue desired queue object
     */
    void setOutcomingQueue(Queue<T, queue_size>* queue) {
        tx_queue_ = queue;
    }

    /**
     * Get outcoming queue
     * @return outcoming queue bject pointer
     */
    Queue<T, queue_size>* getOutcomingQueue() const {
        return tx_queue_;
    }

    /**
     * Send new message to outcoming queue
     * @param message messae to send
     * @param timeout_ms timeout in ms
     * @return TRUE if message was enqueued, otherwise FALSE
     */
    bool produceMessage(const T& message, const uint32_t timeout_ms = DEFAULT_TX_TIMEOUT) {
        bool ret{false};
        if(tx_queue_->enqueueBack(message, timeout_ms)) {
            ret = true;
        }
        return ret;
    }
protected:
    /**
     * @brief Pointer to outcoming queue object
     */
    Queue<T, queue_size>* tx_queue_{};
};
