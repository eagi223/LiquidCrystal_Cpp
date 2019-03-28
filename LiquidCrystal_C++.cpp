#include "LiquidCrystal_C++.hpp"

// Standard C/C++ libs
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

// Arduino replacement libs (included in project)
#include "Time.hpp"

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//
// Note, however, that resetting the microcontroller doesn't reset the LCD, so we
// can't assume that its in that state when an app starts (and the
// LiquidCrystal constructor is called).

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
                 uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                 uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
  init(0, rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7);
}

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t enable,
                 uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                 uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
  init(0, rs, 255, enable, d0, d1, d2, d3, d4, d5, d6, d7);
}

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
                 uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
    // Set unused pins to 255 since the is (most likely) not a valid GPIO number
    // If using a microcontroller that does need 255 as a GPIO... this needs to change.
    // Either increase the size of the pin parameter type (ex: uint16_t) or change to 
    // ints and use -1 for the invalid state.
  init(1, rs, rw, enable, d0, d1, d2, d3, 0xff, 0xff, 0xff, 0xff);
}

LiquidCrystal::LiquidCrystal(uint8_t rs,  uint8_t enable,
                 uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
    // Set unused pins to 255 since the is (most likely) not a valid GPIO number
    // If using a microcontroller that does need 255 as a GPIO... this needs to change.
    // Either increase the size of the pin parameter type (ex: uint16_t) or change to 
    // ints and use -1 for the invalid state.
  init(1, rs, 0xff, enable, d0, d1, d2, d3, 0xff, 0xff, 0xff, 0xff);
}

void LiquidCrystal::init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
             uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
             uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
    // Create Gpio objects with pin specified but default settings otherwise
    _rs_pin = Gpio(rs);
    _rw_pin = Gpio(rw);
    _enable_pin = Gpio(enable);
  
    // If a pin is 255 (meaning invalid/not-used) we create a generic Gpio object with no pin number associated.
    // TODO: Change this to not create a Gpio object at all if a pin isn't used. Just have to make sure
    // that no member functions here try to use Gpio methods on a _data_pins index that isn't valid
    _data_pins[0] = (d0 != 0xff) ? Gpio(d0) : Gpio(); 
    _data_pins[1] = (d1 != 0xff) ? Gpio(d1) : Gpio(); 
    _data_pins[2] = (d2 != 0xff) ? Gpio(d2) : Gpio(); 
    _data_pins[3] = (d3 != 0xff) ? Gpio(d3) : Gpio(); 
    _data_pins[4] = (d4 != 0xff) ? Gpio(d4) : Gpio(); 
    _data_pins[5] = (d5 != 0xff) ? Gpio(d5) : Gpio(); 
    _data_pins[6] = (d6 != 0xff) ? Gpio(d6) : Gpio(); 
    _data_pins[7] = (d7 != 0xff) ? Gpio(d7) : Gpio(); 

    // Configure Gpio (defaults to disabled)
    _rw_pin.config();
    _enable_pin.config();
    for( int dpIndex = 0; dpIndex < 8; dpIndex++ ){
        _data_pins[dpIndex].config();
    }

  if (fourbitmode)
    _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  else 
    _displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
  
    begin(16, 1);  
}

void LiquidCrystal::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;

  setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);  

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }

    _rs_pin.setMode(kGpioModeOutput);

  // we can save 1 pin by not using RW. Indicate by passing 255 instead of pin#
  if (_rw_pin.getPin() != 255) { 
    _rw_pin.setMode(kGpioModeOutput);
  }
  _enable_pin.setMode(kGpioModeOutput);
  
  // Do these once, instead of every time a character is drawn for speed reasons.
  for (int i=0; i<((_displayfunction & LCD_8BITMODE) ? 8 : 4); ++i)
  {
      _data_pins[i].setMode(kGpioModeOutput);
   } 

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
  delay_us(50000); 
  // Now we pull both RS and R/W low to begin commands
  _rs_pin.setOutput(kGpioLevelLow);
  _enable_pin.setOutput(kGpioLevelLow);
  if (_rw_pin.getPin() != 255) { 
    _rw_pin.setOutput(kGpioLevelLow);
  }
  
  //put the LCD into 4 bit or 8 bit mode
  if (! (_displayfunction & LCD_8BITMODE)) {
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    write4bits(0x03);
    delay_us(4500); // wait min 4.1ms

    // second try
    write4bits(0x03);
    delay_us(4500); // wait min 4.1ms
    
    // third go!
    write4bits(0x03); 
    delay_us(150);

    // finally, set to 4-bit interface
    write4bits(0x02); 
  } else {
    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    command(LCD_FUNCTIONSET | _displayfunction);
    delay_us(4500);  // wait more than 4.1ms

    // second try
    command(LCD_FUNCTIONSET | _displayfunction);
    delay_us(150);

    // third go
    command(LCD_FUNCTIONSET | _displayfunction);
  }

  // finally, set # lines, font size, etc.
  command(LCD_FUNCTIONSET | _displayfunction);  

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
  display();

  // clear it off
  clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);

}

void LiquidCrystal::setRowOffsets(int row0, int row1, int row2, int row3)
{
  _row_offsets[0] = row0;
  _row_offsets[1] = row1;
  _row_offsets[2] = row2;
  _row_offsets[3] = row3;
}

/********** high level commands, for the user! */
void LiquidCrystal::clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delay_us(2000);  // this command takes a long time!
}

void LiquidCrystal::home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  delay_us(2000);  // this command takes a long time!
}

void LiquidCrystal::setCursor(uint8_t col, uint8_t row)
{
  const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/0
  }
  if ( row >= _numlines ) {
    row = _numlines - 1;    // we count rows starting w/0
  }
  
  command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidCrystal::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystal::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystal::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystal::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystal::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystal::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystal::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystal::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystal::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
//   std::cout << "char map: ";
  for (int i=0; i<8; i++) {
    write(charmap[i]);
    // std::cout << i << ":" << (unsigned)charmap[i] << std::endl;
  }
}

/*********** mid level commands, for sending data/cmds */

inline void LiquidCrystal::command(uint8_t value) {
  send(value, kGpioLevelLow);
}

inline size_t LiquidCrystal::write(uint8_t value) {
  send(value, kGpioLevelHigh);
  return 1; // assume sucess
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void LiquidCrystal::send(uint8_t value, enum GpioLevel mode) {
  _rs_pin.setOutput(mode);

  // if there is a RW pin indicated, set it low to Write
  if (_rw_pin.getPin() != 255) { 
    _rw_pin.setOutput(kGpioLevelLow);
  }
  
  if (_displayfunction & LCD_8BITMODE) {
    write8bits(value); 
  } else {
    write4bits(value>>4);
    write4bits(value);
  }
}

void LiquidCrystal::pulseEnable(void) {
  _enable_pin.setOutput(kGpioLevelLow);
  delay_us(1);    
  _enable_pin.setOutput(kGpioLevelHigh);
  delay_us(1);    // enable pulse must be >450ns
  _enable_pin.setOutput(kGpioLevelLow);
  delay_us(100);   // commands need > 37us to settle
}

void LiquidCrystal::write4bits(uint8_t value) {
  for (int i = 0; i < 4; i++) {
    _data_pins[i].setOutputInt((value >> i) & 0x01);
  }

  pulseEnable();
}

void LiquidCrystal::write8bits(uint8_t value) {
  for (int i = 0; i < 8; i++) {
    _data_pins[i].setOutputInt((value >> i) & 0x01);
  }
  
  pulseEnable();
}