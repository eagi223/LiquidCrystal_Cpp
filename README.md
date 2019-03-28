## Liquid Crystal - C++
This is a direct port of Arduino Liquid Crystal for C++ (written because I really could't find any non-Arduino C++ drivers for the Hitachi HD44780 compatible LCDs). All non-necessary dependencies were removed. Any critical dependencies are includes in the /dependencies directory and have interface template .cpp files as required. The example was writen for the ESP32, so there is a Gpio interface written specific to the ESP-IDF and a Time interface written specific to FreeRTOS. I've included templates for quickly adding interfaces for other SDKs which should be pretty straight forward. The steps to use the templates are as follows:

1) Add custom #defines into the Liquid_Crystal_Lib_Cfg.hpp file to use in your custom interface files and comment out the ESP32 defines that are already there:

```C++
    // #define GPIO_IMPLEMENT_ESP32
    #define GPIO_IMPLEMENT_SOMEOTHERSDK
    // #define TIME_IMPLEMENT_ESP32
    #define TIME_IMPLEMENT_SOMEOTHERSDK
```
*note: The names you use for these macros doesn't matter. You just have to make sure that you use the same naming in your interface files for the #ifdef so that the correct interface file will be used. You could also not use these macros at all and just delete all interface files but your own and not use the #ifdefs... up to you*

2) Implement inteface for the chip SKD that you are using. ESP32 interfaces are provided. Both Gpio.cpp and Time.cpp need implementations following the format of Gpio_template.cpp and Time_template.cpp. Make sure to change #ifdef GPIO_IMLEMENT_TEMPLATE and #ifdef TIME_IMPLEMENT_TEMPLATE to the new macros created in step 1.

3) Include this library in your project and make sure it is compiled and linked according to the standard way of compiling and linking external libraries for your SDK. For the ESP-IDF a component.mk or CMakeList.txt file is required.

4) Use the library just like the Arduino Liquid Crystal library!
```C++
#include "LiquidCrystal_C++.hpp"

void app_main(){
    // lcd pins:
    // rs = 27, en = 14, d4-d7 = 12, 13, 15, 2
    LiquidCrystal lcd(27, 14, 12, 13, 15, 2);
    lcd.begin(16,4); // Set the display up as 16 row, 4 column
    lcd.clear();
    lcd.print("hello, world!");

    int i = 0;
    while(true){
        lcd.print(i);
        i++;

        delay(100);
    }
}
```