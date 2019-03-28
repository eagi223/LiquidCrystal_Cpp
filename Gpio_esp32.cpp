#include "Liquid_Crystal_Lib_Cfg.hpp"

#ifdef GPIO_IMPLEMENT_ESP32
#include "Gpio.hpp"

// Standard C++ Libraries
#include <string>
#include <iostream>

// ESP-IDF Libraries
#include "driver/gpio.h"

// Todo: Wrap this so it doesn't get compiled in production code
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

gpio_mode_t _getMode(GpioMode mode){
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

gpio_pullup_t _getPullupEn(bool pullupEn){
    return pullupEn ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
}

gpio_pulldown_t _getPulldownEn(bool pulldownEn){
    return pulldownEn ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE;
}

gpio_int_type_t _getIntrType(GpioInterrupt intrType){
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

bool _isValidPin(uint8_t pin, GpioMode mode){
    if( (mode == kGpioModeOutput) ||
        (mode == kGpioModeInputOutput) ||
        (mode == kGpioModeInputOutputOpenDrain)){
            return GPIO_IS_VALID_OUTPUT_GPIO(pin);
        } else {
            return GPIO_IS_VALID_GPIO(pin);
        }
}

bool _isOutput(GpioMode mode){
    return ((mode == kGpioModeOutput) || (mode == kGpioModeInputOutput) || (mode == kGpioModeInputOutputOpenDrain));
}

void Gpio::config(){
    if(_isValidPin(this->_pin, this->_mode)){
        gpio_config_t cfg = {
        cfg.pin_bit_mask = 1<<this->_pin,
        cfg.mode = _getMode(this->_mode),
        cfg.pull_up_en = _getPullupEn(this->_pullup_enabled),
        cfg.pull_down_en = _getPulldownEn(this->_pulldown_enabled),
        cfg.intr_type = _getIntrType(this->_interrupt_type),
        };

        ::gpio_config(&cfg);
        // print();
    }
}

void Gpio::setMode(GpioMode mode){
    if(_isValidPin(this->_pin, this->_mode)){
        // std::cout << "###GPIO[" << this->_pin << "] - set mode to:  " << debug_mode_lookup_table[mode] << std::endl;
        this->_mode = mode;
        ::gpio_set_direction((gpio_num_t)this->_pin, _getMode(this->_mode));
        // print();
    }
}

void Gpio::setOutput(GpioLevel level){
    if(_isOutput(this->_mode)){
        // std::cout << "###GPIO[" << this->_pin << "] - " << level << std::endl;
        uint32_t espLevel = (level == kGpioLevelHigh) ? 1 : 0;
        ::gpio_set_level((gpio_num_t)this->_pin, espLevel);
        // print();
    }
}

void Gpio::setOutputInt(int level){
    if(_isOutput(this->_mode)){
        // std::cout << "###GPIO[" << this->_pin << "] - " << level << std::endl;
        uint32_t espLevel = level;
        ::gpio_set_level((gpio_num_t)this->_pin, espLevel);
        // print();
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