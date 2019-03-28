#include "Liquid_Crystal_Lib_Cfg.hpp"
#ifdef TIME_IMPLEMENT_ESP32
#include "Time.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <iostream>

void delay_us(unsigned int time_to_delay_us){
    unsigned int time_to_delay_ms = (time_to_delay_us/1000)/portTICK_PERIOD_MS;

    if( time_to_delay_ms == 0 ){
        time_to_delay_ms = 1;
    }

    vTaskDelay((time_to_delay_us/1000)/portTICK_PERIOD_MS);
    // std::cout << "delay: " << time_to_delay_us << std::endl;
}


#endif