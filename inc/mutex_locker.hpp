#ifndef static_mutex.hpp
#define static_mutex.hpp

#include <assert.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "config.h"
#include "static_mutex.hpp"

/**
 * @class MutexLocker
 * 
 * @brief Class for lock RAII-style operations with mutexes
 * 
 */
class MutexLocker {
public:
    /**
     * @brief Construct a new MutexLocker object with mutex refference
     * 
     * @param mutex mutex refference to work with
     */
    explicit MutexLocker(StaticMutex &mutex) : mutex_ref_{mutex} {
        mutex_ref_.lock();
    }

    /**
     * @brief Destroy the MutexLocker object
     * 
     */
    ~MutexLocker() {
        mutex_ref_.unlock();
    }

private:
    /**
     * @brief StaticMutex object refference
     * 
     */
    StaticMutex& mutex_ref_;
};


#endif // static_mutex.hpp