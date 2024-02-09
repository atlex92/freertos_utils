#ifndef FREERTOS_UTILS_INTERRUPT_LOCKER_HPP_
#define FREERTOS_UTILS_INTERRUPT_LOCKER_HPP_

#include <freertos/FreeRTOS.h>

/**
 * @class InterruptLocker
 * 
 * @brief Class for crytical sections RAII-style operations
 * 
 */
class InterruptLocker {
public:
    /**
     * @brief Construct a new InterruptLocker object and enter crytical section
     * 
     */
    InterruptLocker() {
        time_critical_mutex_ = portMUX_INITIALIZER_UNLOCKED;
        portENTER_CRITICAL(&time_critical_mutex_);
    }
    /**
     * @brief Destroy the InterruptLocker object and exit crytical section
     * 
     */
    ~InterruptLocker() {
        portEXIT_CRITICAL(&time_critical_mutex_);
    }
private:
    /**
     * @brief Lock object
     * 
     */
    portMUX_TYPE time_critical_mutex_;
};

#endif // FREERTOS_UTILS_INTERRUPT_LOCKER_HPP_