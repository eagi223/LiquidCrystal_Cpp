#include "Liquid_Crystal_Lib_Cfg.hpp"

#ifdef GPIO_IMPLEMENT_ESP32
#include "Gpio.hpp"

// Standard C++ Libraries
#include <string>
#include <iostream>

// ESP-IDF Libraries
#include "driver/gpio.h"


// Lookup tables for meaningful debug messages
const std::string debug_mode_lookup_table[kGpioModeMax] = {
    "DISABLED",
    "INPUT",
    "OUTPUT",
    "OUTPUT OD",
    "INPUT OUTPUT OD",
    "INPUT OUTPUT"
};

const std::string debug_intr_loopup_table[kGpioInterruptMax] = {
    "DISABLED",
    "POS EDGE",
    "NEG EDGE",
    "ANY EDGE",
    "LOW LEVEL",
    "HIGH LEVEL"
};


// Private member functions
static gpio_mode_t getMode(GpioMode mode){
    gpio_mode_t espGpioMode = GPIO_MODE_DISABLE;
    switch(mode){
        case kGpioModeDisabled:
            espGpioMode = GPIO_MODE_DISABLE;
            break;

        case kGpioModeInput:
            espGpioMode = GPIO_MODE_INPUT;
            break;

        case kGpioModeOutput:
            espGpioMode = GPIO_MODE_OUTPUT;
            break;

        case kGpioModeOutputOpenDrain:
            espGpioMode = GPIO_MODE_OUTPUT_OD;
            break;

        case kGpioModeInputOutputOpenDrain:
            espGpioMode = GPIO_MODE_INPUT_OUTPUT_OD;
            break;

        case kGpioModeInputOutput:
            espGpioMode = GPIO_MODE_INPUT_OUTPUT;
            break;

        default:
            break;
    }

    return espGpioMode;
}

static gpio_pullup_t getPullupEn(bool pullupEn){
    return pullupEn ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
}

static gpio_pulldown_t getPulldownEn(bool pulldownEn){
    return pulldownEn ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE;
}

static gpio_int_type_t getIntrType(GpioInterrupt intrType){
    gpio_int_type_t espIntrType = GPIO_INTR_DISABLE;
    switch(intrType){
        case kGpioInterruptDisabled:
            espIntrType = GPIO_INTR_DISABLE;
            break;
        
        case kGpioInterruptPosEdge:
            espIntrType = GPIO_INTR_POSEDGE;
            break;

        case kGpioInterruptNegEdge:
            espIntrType = GPIO_INTR_NEGEDGE;
            break;

        case kGpioInterruptAnyEdge:
            espIntrType = GPIO_INTR_ANYEDGE;
            break;

        case kGpioInterruptLowLevel:
            espIntrType = GPIO_INTR_LOW_LEVEL;
            break;

        case kGpioInterruptHighLevel:
            espIntrType = GPIO_INTR_HIGH_LEVEL;
            break;

        default:
            break;
    }

    return espIntrType;
}

static bool isValidPin(uint8_t pin, GpioMode mode){
    if( (mode == kGpioModeOutput) ||
        (mode == kGpioModeInputOutput) ||
        (mode == kGpioModeInputOutputOpenDrain)){
            return GPIO_IS_VALID_OUTPUT_GPIO(pin);
        } else {
            return GPIO_IS_VALID_GPIO(pin);
        }
}

static bool isOutput(GpioMode mode){
    return ((mode == kGpioModeOutput) || (mode == kGpioModeInputOutput) || (mode == kGpioModeInputOutputOpenDrain));
}


// Public member functions
void Gpio::config(){
    if(::isValidPin(this->_pin, this->_mode)){
        gpio_config_t cfg = {
        cfg.pin_bit_mask = 1<<this->_pin,
        cfg.mode = ::getMode(this->_mode),
        cfg.pull_up_en = ::getPullupEn(this->_pullup_enabled),
        cfg.pull_down_en = ::getPulldownEn(this->_pulldown_enabled),
        cfg.intr_type = ::getIntrType(this->_interrupt_type),
        };

        ::gpio_config(&cfg);
    }
}

void Gpio::setMode(GpioMode mode){
    if(::isValidPin(this->_pin, this->_mode)){
        this->_mode = mode;
        ::gpio_set_direction((gpio_num_t)this->_pin, ::getMode(this->_mode));
    }
}

void Gpio::setOutput(GpioLevel level){
    if(::isOutput(this->_mode)){
        uint32_t espLevel = (level == kGpioLevelHigh) ? 1 : 0;
        ::gpio_set_level((gpio_num_t)this->_pin, espLevel);
    }
}

void Gpio::setOutputInt(int level){
    if(::isOutput(this->_mode)){
        uint32_t espLevel = level;
        ::gpio_set_level((gpio_num_t)this->_pin, espLevel);
    }    
}

void Gpio::print(){
    std::cout << "###GPIO["<<unsigned(_pin)<<"] - ";
    std::cout << debug_mode_lookup_table[_mode] << " ";
    std::cout << ((_pullup_enabled) ? "PULL UP " : " ");
    std::cout << ((_pulldown_enabled) ? "PULL DOWN " : " ");
    std::cout << "INTR " << debug_mode_lookup_table[_interrupt_type] << "\n";
}

#endif /* GPIO_IMPLEMENT_ESP32 */