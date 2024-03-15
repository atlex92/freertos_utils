#ifndef FREERTOS_UTILS_MUTEX_HPP_
#define FREERTOS_UTILS_MUTEX_HPP_

#include <assert.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "config.h"


/**
 * @class Mutex
 * 
 * @brief C++ wrapper for binary mutex operations
 * 
 */
class Mutex {
public:
    /**
     * @brief Construct a new Mutex object
     * 
     */
    explicit Mutex() : handle_{xSemaphoreCreateRecursiveMutex()} {
        assert(handle_);
    }

    ~Mutex() {
    }

    Mutex(const Mutex &) = delete;
    Mutex(Mutex &&) = delete;
    Mutex(Mutex &) = delete;

    /**
     * @brief Try to lock the mutex with specified timeout
     * 
     * @param delay timeout in ticks
     * @return true if the mutex was locked, otherwise false
     */
    bool tryLock(const TickType_t delay) {
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
    const SemaphoreHandle_t handle_;
    Mutex &operator=(const Mutex &) = delete;
};

#endif // FREERTOS_UTILS_MUTEX_HPP_