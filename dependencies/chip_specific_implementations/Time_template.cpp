/**
 * A basic template to implement this functionality for whatever system you are using
 * 
 * You need to:
 *  -Add "#define TIME_IMPLEMENT_WHATEVERYOUWANTTOCALLIT" to Liquid_Crystal_Lib_Cfg.hpp
 *  -Remove/Comment Out "#define TIME_IMPLEMENT_ESP32" from Liquid_Crystal_Lib_Cfg.hpp
 *  -Change "#ifdef TIME_IMPLEMENT_TEMPLATE" below to "#ifdef TIME_IMPLEMENT_****" that you added to 
 *      Liquid_Crystal_Lib_Cfg.hpp in step 1
 *  -Add your implementation in the commented function(s)
 * 
 * March 28, 2019
 * Ethan Gibson
 */

// Include the config file to check if this is the correct implementation
#include "Liquid_Crystal_Lib_Cfg.hpp"

#ifdef TIME_IMPLEMENT_TEMPLATE
#include "Time.hpp"

// FreeRTOS dependencies
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void delay_us(unsigned int time_to_delay_us){
    // Call whatever native delay function you use for your system and make sure the units are
    // in 
}

#endif