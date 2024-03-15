#ifndef FREERTOS_UTILS_STATIC_QUEUE_H_
#define FREERTOS_UTILS_STATIC_QUEUE_H_

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "config.h"

/**
 * @brief Template C++ wrapper for static queue operations
 * 
 * @tparam T type of messages in queue
 * @tparam length queue length
 */
template <typename T, size_t length>
class Queue {
public:
    /**
     * @brief Construct a new Static Queue object
     * 
     */
    Queue() : queue_handle_{xQueueCreate(length, sizeof(T))} {
        assert(NULL != queue_handle_);
    }

    /**
     * @brief Add a message at the end of the queue
     *
     * @param msg message object
     * @param timeout max ticks to wait for
     * @return true if the message was enqueued
     * @return false otherwise
     */
    bool enqueueBack(const T &msg, const TickType_t timeout = 0u) {
        return pdTRUE == (IS_IN_ISR() ? xQueueSendToBackFromISR(queue_handle_, &msg, NULL)
                                        : xQueueSendToBack(queue_handle_, &msg, timeout));
    }

    /**
     * @brief Add a message at the begining of the queue
     *
     * @param msg message object
     * @param timeout max ticks to wait for
     * @return true if the message was enqueued
     * @return false otherwise
     */
    bool enqueueFront(const T &msg, const TickType_t timeout = 0u) {
        return pdTRUE == (IS_IN_ISR() ? xQueueSendToFrontFromISR(queue_handle_, &msg, NULL)
                                        : xQueueSendToFront(queue_handle_, &msg, timeout));
    }

    /**
     * @brief Read last message from the queue, leaves the messages in the queue
     *
     * @param out object to read into
     * @param timeout max ticks to wait for
     * @return true if read operation was successfull
     * @return false otherwise
     */
    bool peek(T &out, const TickType_t timeout = 0) {
        return pdTRUE ==
               (IS_IN_ISR() ? xQueuePeekFromISR(queue_handle_, &out) : xQueuePeek(queue_handle_, &out, timeout));
    }

    /**
     * @brief Read last message from the queue, pops the message out from the queue
     *
     * @param out object to read into
     * @param timeout max ticks to wait for
     * @return true if read operation was successfull
     * @return false otherwise
     */
    bool receive(T &out, const TickType_t timeout = 0) {
        return pdTRUE == (IS_IN_ISR() ? xQueueReceiveFromISR(queue_handle_, &out, NULL) : xQueueReceive(queue_handle_, &out, timeout));
    }

    /**
     * @brief Get current number of messages in the queue
     *
     * @return current number of messages in the queue
     */
    size_t size() const {
        return uxQueueMessagesWaiting(queue_handle_);
    }

    /**
     * @brief Get number of available spaces in the queue for new messages
     *
     * @return number of available spaces in the queue for new messages
     */
    size_t available() const {
        return uxQueueSpacesAvailable(queue_handle_);
    }

    /**
     * @brief Destroy the Static Queue object
     * 
     */
    ~Queue() {
        if (queue_handle_) {
            vQueueDelete(queue_handle_);
        }
    }

    QueueHandle_t raw() const {
        return queue_handle_;
    }

private:
    /**
     * @brief Static queue handler
     * 
     */
    QueueHandle_t queue_handle_{NULL};
};

#endif  // FREERTOS_UTILS_STATIC_QUEUE_H_
