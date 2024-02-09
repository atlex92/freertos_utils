#ifndef FREERTOS_UTILS_CONFIG_H_
#define FREERTOS_UTILS_CONFIG_H_

#define MCU_ESP32

#ifdef MCU_ESP32
    #define IS_IN_ISR() xPortInIsrContext()
#endif // MCU_ESP32

#endif // FREERTOS_UTILS_CONFIG_H_