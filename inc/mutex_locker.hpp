#pragma once

#include <assert.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "config.h"
#include "mutex.hpp"

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
  explicit MutexLocker(Mutex& mutex) : mutex_ref_{mutex} {
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
   * @brief Mutex object refference
   *
   */
  Mutex& mutex_ref_;
};
