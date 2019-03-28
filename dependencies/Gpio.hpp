/**
 *  Generic GPIO configuration class
 * 
 *  Only the features needed for Liquid Crystal were implemented. More functionality
 *  will be implemented in a seperate repo.
 * 
 *  March 28, 2019
 *  Ethan Gibson
 */

#ifndef GPIO_HPP_
#define GPIO_HPP_

#include <stdint.h>

enum GpioMode {
    kGpioModeDisabled,
    kGpioModeInput,
    kGpioModeOutput,
    kGpioModeOutputOpenDrain,
    kGpioModeInputOutputOpenDrain,
    kGpioModeInputOutput,
    kGpioModeMax
};

enum GpioInterrupt {
    kGpioInterruptDisabled,
    kGpioInterruptPosEdge,
    kGpioInterruptNegEdge,
    kGpioInterruptAnyEdge,
    kGpioInterruptLowLevel,
    kGpioInterruptHighLevel,
    kGpioInterruptMax
};

enum GpioLevel {
    kGpioLevelLow,
    kGpioLevelHigh,
    kGpioLevelMax
};

/**
 * @brief Generial digital IO class to store configuration for a pin
 * 
 * Actual implementation of functions is done by the wrapper for the specific chipset
 */
class Gpio
{
    private:
        // Private data
        uint8_t _pin;
        enum GpioMode _mode;
        bool _pullup_enabled;
        bool _pulldown_enabled;
        enum GpioInterrupt _interrupt_type;

    public:
        // Constructors
        Gpio()
            :_pin(0xff),
            _mode(kGpioModeMax),
            _pullup_enabled(false),
            _pulldown_enabled(false),
            _interrupt_type(kGpioInterruptMax)
            {}

        Gpio(uint8_t pin)
            :_pin(pin),
            _mode(kGpioModeDisabled),
            _pullup_enabled(false),
            _pulldown_enabled(false),
            _interrupt_type(kGpioInterruptDisabled)
            {}

        Gpio(uint8_t pin, enum GpioMode mode)
            :_pin(pin),
            _mode(mode),
            _pullup_enabled(false),
            _pulldown_enabled(false),
            _interrupt_type(kGpioInterruptDisabled)
            {}

        Gpio(uint8_t pin, enum GpioMode mode, bool pullup_enabled, bool pulldown_enabled, enum GpioInterrupt interrupt_type )
            :_pin(pin),
            _mode(mode),
            _pullup_enabled(pullup_enabled),
            _pulldown_enabled(pulldown_enabled),
            _interrupt_type(interrupt_type)
            {}

        Gpio(enum GpioMode mode, bool pullup_enabled, bool pulldown_enabled, enum GpioInterrupt interrupt_type )
            :_pin(0xff),
            _mode(mode),
            _pullup_enabled(pullup_enabled),
            _pulldown_enabled(pulldown_enabled),
            _interrupt_type(interrupt_type)
            {}

        // Set configuration based on member data
        void config();

        // Getters
        uint8_t getPin() {return this->_pin;}
        enum GpioMode getMode() {return this->_mode;}

        // Setters
        void setMode(enum GpioMode mode);
        void setOutput(enum GpioLevel level);
        void setOutputInt(int level);

        // Display the current member data (which isn't set until config() has been called)
        void print();

        // Operator overloads
        void operator = (const Gpio &G) {
            _pin = G._pin;
            _mode = G._mode;
            _pullup_enabled = G._pullup_enabled;
            _pulldown_enabled = G._pulldown_enabled;
            _interrupt_type = G._interrupt_type;
        }
};

#endif /* GPIO_HPP_ */