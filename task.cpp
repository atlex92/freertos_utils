
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "task.hpp"
#include "interrupt_locker.hpp"
#include "esp_log.h"

static const char* const TAG {"Task"};

Task::Task(const std::string& taskName, const uint16_t stackSize, const uint8_t priority, const BaseType_t coreID)
	:	is_running_(false),
		task_name_(taskName),
		stack_size_(stackSize),
		priority_(priority),
		core_id_(coreID) {
}

Task::~Task() {

}

void Task::delay(const uint32_t ms) {
	vTaskDelay(pdMS_TO_TICKS(ms));
}

void Task::runTask(void* pTaskInstance) {
	Task* pTask = static_cast<Task*> (pTaskInstance);
	pTask->run(pTask->task_arg_);
	pTask->stop();
}

void Task::start(void* taskData) {

	if (task_descr_ != nullptr) {
		ESP_LOGE(TAG, "Task::start - There might be a task with name: %s already running!", task_name_.c_str());
		delay(500);
		assert(false);
	}
	task_arg_ = taskData;

	{
		/*
			Safe updating is_running_ flag, context switches are excluded for a while
		*/
		InterruptLocker lock;
		// assert(pdPASS == ::xTaskCreate(&runTask, task_name_.c_str(), stack_size_, this, priority_, &task_descr_));
		assert(pdPASS ==::xTaskCreatePinnedToCore(&runTask, task_name_.c_str(), stack_size_, this, priority_, &task_descr_, core_id_));
		is_running_ = true;
	}
}

void Task::stop() {

	if (nullptr == task_descr_) {
		return;
	}
	TaskHandle_t temp = task_descr_;
	onStop();
	task_descr_ = nullptr;
	is_running_ = false;
	vTaskDelete(temp);
}

void Task::suspend() {
	if (task_descr_ == nullptr) {
		return;
	}
	if (false == is_running_) {
		return;
	}
	{
		/*
			Safe updating is_running_ flag, context switches are excluded for a while
		*/
		InterruptLocker lock;
		vTaskSuspend(task_descr_);
		is_running_ = false;
	}
}

void Task::resume() {
	if (task_descr_ == nullptr) {
		return;
	}
	if (true == is_running_) {
		return;
	}
	{
		/*
			Safe updating is_running_ flag, context switches are excluded for a while
		*/
		InterruptLocker lock;
		vTaskResume(task_descr_);
		is_running_ = true;
	}
}

void Task::onStop() {

}

bool Task::isRunning()const {
	return is_running_;
}
