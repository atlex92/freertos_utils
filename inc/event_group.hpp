#pragma once
#include <assert.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include "config.h"

/**
 * @class EventGroup
 * 
 * @brief C++ wrapper for event group operations
 * 
 */
class EventGroup {
public:
    /**
     * @brief Construct a new EventGroup object
     * 
     */
    EventGroup() : handle_{xEventGroupCreate()} {
        assert(nullptr != handle_);
    }

    /**
     * @brief Wait for at least one bit in event group from specified bits
     *
     * @param bits_to_wait bits to wait for
     * @param ticks_to_wait time in ticks to wait for
     * @param clear option to clear bits after reading from the event group
     * @return actual bits were read or zero, if no bits were read from the group
     */
    EventBits_t waitForAny(const EventBits_t bits_to_wait, const uint32_t ticks_to_wait, const bool clear = true) {
        return xEventGroupWaitBits(handle_, bits_to_wait, clear, pdFALSE, ticks_to_wait);
    }

    /**
     * @brief Wait for all specified bits
     *
     * @param bits_to_wait bits to wait for
     * @param ticks_to_wait time in ticks to wait for
     * @param clear option to clear bits after reading from the event group
     * @return actual bits were read or zero, if no bits were read from the group
     */
    EventBits_t waitForAll(const EventBits_t bits_to_wait, const uint32_t ticks_to_wait, const bool clear = true) {
        return xEventGroupWaitBits(handle_, bits_to_wait, clear, pdTRUE, ticks_to_wait);
    }

    /**
     * @brief Set specifeied bits in the event group
     *
     * @param bits_to_set bits to set
     * @return bits value at the moment of time when
     * this function returns (before any other task can clear these bits)
     */
    EventBits_t setBits(const EventBits_t bits_to_set) {
        EventBits_t ret;
        const bool isr{IS_IN_ISR()};
        if (isr) {
            ret = xEventGroupSetBitsFromISR(handle_, bits_to_set, NULL);
        } else {
            ret = xEventGroupSetBits(handle_, bits_to_set);
        }
        return ret;
    }

    /**
     * @brief Clear specified bits in the event group
     *
     * @param bits_to_clear bits to clear
     * @return bits value before clear procedure
     */
    EventBits_t clearBits(const EventBits_t bits_to_clear) {
        EventBits_t ret;
        const bool isr{IS_IN_ISR()};
        if (isr) {
            ret = xEventGroupClearBitsFromISR(handle_, bits_to_clear);
        } else {
            ret = xEventGroupClearBits(handle_, bits_to_clear);
        }
        return ret;
    }

    /**
     * @brief Get current bits value
     *
     * @return current bits value
     */
    EventBits_t bits() const {
        EventBits_t ret;
        const bool isr{IS_IN_ISR()};
        if (isr) {
            ret = xEventGroupGetBitsFromISR(handle_);
        } else {
            ret = xEventGroupGetBits(handle_);
        }
        return ret;
    }

    /**
     * @brief Get raw event group handler @see EventGroupHandle_t
     * 
     * @return EventGroupHandle_t internal raw handler
     */
    EventGroupHandle_t raw() const {
        return handle_;
    }

    /**
     * @brief Helper method to calculate bits value according to bit number
     *
     * @param bit_index bit index (starting from 0)
     * @return bits value
     */
    template <const uint8_t bit_index>
    static constexpr EventBits_t bitToBits() {
        static_assert(bit_index <= 31u, "invalid bit index");
        return static_cast<EventBits_t>(1UL << bit_index);
    }
private:
    /**
     * @brief Internal event group handler
     * 
     */
    EventGroupHandle_t handle_{nullptr};
};
