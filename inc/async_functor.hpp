#pragma once

#include <functional>
#include "task.hpp"

/**
 * @class AsyncFunctor
 *
 * @brief C++ wrapper for asynchronus operation
 *
 */
class AsyncFunctor : public Task {
public:
  using function_t = std::function<void()>;
  /** Constructs new AsyncFunctor object
   * @param function functor object to call
   * @param task_name task name
   * @param stack_size task stack size
   * @param priority task priority
   */
  explicit AsyncFunctor(function_t function, const std::string& task_name = "AsyncFunctor",
                        const uint16_t stack_size = configMINIMAL_STACK_SIZE,
                        const uint8_t priority = kTaskDefaultPriority)
  : Task(task_name, stack_size, priority), _func{function} {
  }

private:
  /**
   * @brief function to execute
   *
   * @param args argument passed to the task
   */
  void run(void* args) override {
    if (_func) {
      _func();
    }
  }
  /**
   * @brief Functor object
   */
  function_t _func = nullptr;
};