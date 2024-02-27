#ifndef FREERTOS_UTILS_STATIC_BINARY_SEMAPHORE_HPP_
#define FREERTOS_UTILS_STATIC_BINARY_SEMAPHORE_HPP_

#include <assert.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "config.h"

/**
 * @class BinarySemaphore
 * 
 * @brief C++ wrapper for binary semaphore operations
 * 
 */
class BinarySemaphore {
public:
    /**
     * @brief Construct a new BinarySemaphore object
     * 
     */
    BinarySemaphore() :handle_{xSemaphoreCreateBinary()} {
        assert(handle_ != nullptr);
    }

    /**
     * @brief Try to take the semaphore with specified timeout
     * 
     * @param delay maximum timeout specified in ticks
     * @return true if the semaphore was taken, othewise false
     */
    bool tryTake(const TickType_t delay) {
        assert(!IS_IN_ISR());
        return (xSemaphoreTake(handle_, delay) != pdFALSE);
    }

    /**
     * @brief Try to take the semaphore without timeout
     * 
     */
    void take() {
        while (!tryTake(portMAX_DELAY)) {
        }
    }

    /**
     * @brief Try to give the semaphore
     * 
     * @return true if the semaphore was given, otherwise false
     */
    bool tryGive() const {
        return semaphoreGiveFromAnywhere();
    }

    /**
     * @brief Give untaken semaphore
     * 
     */
    void give() const {
        assert(tryGive());
    }

    /**
     * @brief Get internal handler @see SemaphoreHandle_t
     * 
     * @return internal handler @see SemaphoreHandle_t 
     */
    SemaphoreHandle_t raw() const {
        return handle_;
    }

private:
    /**
     * @brief Raw handler @see SemaphoreHandle_t
     * 
     */
    SemaphoreHandle_t handle_{nullptr};

    /**
     * @brief Internal data storage
     * 
     */

    BinarySemaphore(const BinarySemaphore &) = delete;
    BinarySemaphore(BinarySemaphore &&) = delete;
    BinarySemaphore &operator=(const BinarySemaphore &) = delete;

    /**
     * @brief Release a semaphore handle_. This function can be called from any context.
     *
     * @return true if semaphore is released successfully, otherwise false.
     */
    bool semaphoreGiveFromAnywhere() const {
        bool result{false};
        if (IS_IN_ISR()) {
            BaseType_t reschedule{pdFALSE};
            result = (xSemaphoreGiveFromISR(handle_, &reschedule) == pdTRUE);
            portYIELD_FROM_ISR(reschedule);
        } else {
            result = (xSemaphoreGive(handle_) == pdTRUE);
        }
        return result;
    }
};

#endif  // FREERTOS_UTILS_STATIC_BINARY_SEMAPHORE_HPP_
