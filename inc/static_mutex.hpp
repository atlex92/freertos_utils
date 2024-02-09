#ifndef FREERTOS_UTILS_MUTEX_HPP_
#define FREERTOS_UTILS_MUTEX_HPP_

#include <assert.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "config.h"

/**
 * @class StaticMutex
 * 
 * @brief C++ wrapper for binary mutex operations
 * 
 */
class StaticMutex {
public:
    /**
     * @brief Construct a new StaticMutex object
     * 
     */
    StaticMutex() : handle_{xSemaphoreCreateRecursiveMutexStatic(&static_internal_data_)} {
        assert(handle_ != nullptr);
    }

    /**
     * @brief Try to lock the mutex with specified timeout
     * 
     * @param delay timeout in ticks
     * @return true if the mutex was locked, otherwise false
     */
    bool tryLock(TickType_t delay) {
        assert(!IS_IN_ISR());
        return (xSemaphoreTakeRecursive(handle_, delay) != pdFALSE);
    }

    /**
     * @brief Lock the mutex without timeout
     * 
     */
    void lock() {
        while (!tryLock(portMAX_DELAY)) {
        }
    }

    /**
     * @brief Try to unlock the mutex
     * 
     * @return true if it was unlocked, otherwise false
     */
    bool tryUnlock() {
        return (xSemaphoreGiveRecursive(handle_) != pdFALSE);
    }

    /**
     * @brief Unlokc the mutex
     * 
     */
    void unlock() {
        assert(tryUnlock());
    }

    /**
     * @brief Get the raw mutex handler @see SemaphoreHandle_t
     * 
     * @return SemaphoreHandle_t raw mutex handler
     */
    SemaphoreHandle_t raw() const {
        return handle_;
    }

private:
    /**
     * @brief raw mutex handler @see SemaphoreHandle_t
     * 
     */
    SemaphoreHandle_t handle_{nullptr};

    /**
     * @brief internal data storage
     * 
     */
    StaticSemaphore_t static_internal_data_{};

    StaticMutex(const StaticMutex &) = delete;
    StaticMutex(StaticMutex &&) = delete;
    StaticMutex &operator=(const StaticMutex &) = delete;
};

#endif // FREERTOS_UTILS_MUTEX_HPP_