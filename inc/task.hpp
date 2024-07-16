#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <string>

/**
 * @class Task
 * 
 * @brief C++ wrapper for static task operations
 * 
 */
class Task {
public:
	/**
	 * @brief default task priority
	 * 
	 */
	static constexpr uint32_t kTaskDefaultPriority {1U};

	/**
	 * @brief Construct a new Task object
	 * 
	 * @param task_name name of task
	 * @param stack_size stack size
	 * @param priority task priority
	 * @param core_id core id 
	 */
	explicit Task(const std::string& task_name = "Task", const uint16_t stack_size = configMINIMAL_STACK_SIZE, const uint8_t priority = kTaskDefaultPriority, const BaseType_t core_id = 0);

	/**
	 * @brief Virtual destructor
	 * 
	 */
	virtual ~Task();

	/**
	 * @brief Check if task is running currently
	 * 
	 * @return true if the task is running, otherwise false
	 */
	bool isRunning() const;

	/**
	 * @brief Pause the task
	 * 
	 */
	void suspend();

	/**
	 * @brief Resume the task
	 * 
	 */
	void resume();

	/**
	 * @brief Start task
	 * 
	 * @param task_data argument to pass to the task 
	 */
	void start(void* task_data = nullptr);

	/**
	 * @brief Stop the task
	 * 
	 */
	void stop();

	/**
	 * @brief Task main function to execute
	 * 
	 * @param data argument passed to the task 
	 */
	virtual void run(void* data) = 0;

	/**
	 * @brief Put the task into idle state for given period
	 * 
	 * @param ms period in ms to pause the task
	 */
	static void delay(const uint32_t ms);

	/**
	 * @brief Callback handler for task stop operation
	 * 
	 */
	virtual void onStop();

private:
	/**
	 * @brief Common staic task function
	 * 
	 * @param data argument passed into the task
	 */
	static void runTask(void* data);

protected:
	/**
	 * @brief Internal task descriptor @see TaskHandle_t
	 * 
	 */
	TaskHandle_t task_descr_{};
private:
	/**
	 * @brief flag shows if the task is running
	 * 
	 */
	volatile bool is_running_{false};

	/**
	 * @brief Task's internal data parameter
	 * 
	 */
	void* task_arg_{nullptr};

	/**
	 * @brief Task's name
	 * 
	 */
	std::string task_name_;

	/**
	 * @brief Task's stack size
	 * 
	 */
	uint16_t stack_size_{};

	/**
	 * @brief Task's priority
	 * 
	 */
	uint8_t priority_{};

	/**
	 * @brief Core which task is pinned to
	 * 
	 */
	BaseType_t core_id_{};
};
