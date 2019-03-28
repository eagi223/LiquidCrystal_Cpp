/**
 * Basic delay function implementation using FreeRTOS
 * 
 * Depends on FreeRTOS specifically for the ESP32 
 * 
 * March 28, 2019
 * Ethan Gibson
 */

// Include the config file to check if this is the correct implementation
#include "Liquid_Crystal_Lib_Cfg.hpp"

#ifdef TIME_IMPLEMENT_ESP32
#include "Time.hpp"

// FreeRTOS dependencies
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void delay_us(unsigned int time_to_delay_us){
    unsigned int time_to_delay_ms = (time_to_delay_us/1000)/portTICK_PERIOD_MS;

    if( time_to_delay_ms == 0 ){
        time_to_delay_ms = 1;
    }

    vTaskDelay((time_to_delay_us/1000)/portTICK_PERIOD_MS);
}

#endif