#include "Liquid_Crystal_Lib_Cfg.hpp"

#ifdef GPIO_IMPLEMENT_TEMPLATE
#include "Gpio.hpp"

// Standard C++ Libraries
#include <string>
#include <iostream>

// ##### ADD INCLUDES YOU NEED FROM YOUR SDK HERE #####

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

// Public member functions
void Gpio::config(){
    // Call function/set registers/etc... to configure gpio
    // based on the member data set in the constructor

    // This should probably check to make sure pin is valid or
    // generic constructors that don't take a gpio number should be deleted
    // from Gpio.hpp
}

void Gpio::setMode(GpioMode mode){
    // Sets the mode for the gpio pin (ex: input, output, output open drain, etc...)
    // The GpioMode enum should be changed so that only valid options for your SDK
    // are allowed. Convert the passed in GpioMode to your skd specific modes and set
    // the configuration. 
}

void Gpio::setOutput(GpioLevel level){
    // Sets the output level for the gpio based on the GpioLevel enum (high or low). Convert
    // this as required for your SDK.
    
    // This should probably check that Gpio is set as an output first
}

void Gpio::setOutputInt(int level){
    // The same as Gpio::setOutput, but takes an int instead of a GpioLevel enum value.
    // Implementation is up to you, but 0 should mean digital low and 1 should mean digital high.
}

void Gpio::print(){
    std::cout << "###GPIO["<<unsigned(_pin)<<"] - ";
    std::cout << debug_mode_lookup_table[_mode] << " ";
    std::cout << ((_pullup_enabled) ? "PULL UP " : " ");
    std::cout << ((_pulldown_enabled) ? "PULL DOWN " : " ");
    std::cout << "INTR " << debug_mode_lookup_table[_interrupt_type] << "\n";
}

#endif /* GPIO_IMPLEMENT_ESP32 */