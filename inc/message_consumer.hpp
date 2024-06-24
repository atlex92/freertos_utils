#pragma once

#include "queue.hpp"

#define DEFAULT_RX_QUEUE_SIZE 10U
#define DEFAULT_RX_TIMEOUT 0xFFFFFFFFU

/**
 * @brief Template C++ wrapper for message consumer objects
 * 
 * @tparam T type of messages in queue
 * @tparam queue_size queue length
 */
template <typename T, size_t queue_size = DEFAULT_RX_QUEUE_SIZE>
class MessageConsumer {
public:
    /**
     * Constructs new MessageConsumer object
     */
    explicit MessageConsumer() {
    }

    /**
     * Check if there are some messages in queue to be read
     * @return TRUE if there is atleast one message in queue, otherwise FALSE
     */
    bool hasMessages() const {
        return queue_.size() > 0U;
    }

    /**
     * Wait for any message for a specified timeout
     * @param out message object to fill
     * @param timeout_ms timeout in ms
     * @return TRUE if message was received within timeout, otherwise FALSE
     */
    bool consumeMessage(T& out, const uint32_t timeout_ms = DEFAULT_RX_TIMEOUT) {
        bool ret{false};
        if(queue_.receive(out, pdMS_TO_TICKS(timeout_ms))) {
            ret = true;
        }
        return ret;
    }

    /**
     * Get incomming queue object
     * @return pointer to incomming messages queue object
     */
    Queue<T, queue_size>* incommingQueue() {
        return &queue_;
    }

private:
    /**
     * Internal queue object
     */
    Queue<T, queue_size> queue_{};
};