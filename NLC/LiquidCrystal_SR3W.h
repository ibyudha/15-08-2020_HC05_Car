
#ifndef _LIQUIDCRYSTAL_SR3W_H_
#define _LIQUIDCRYSTAL_SR3W_H_

#include <inttypes.h>
#include "LCD.h"
#include "FastIO.h"


class LiquidCrystal_SR3W : public LCD 
{
public:
   
   /*!
    @method     
    @abstract   Class constructor. 
    @discussion Initializes class variables and defines the IO driving the 
    shift register. The constructor does not initialize the LCD.
    Default configuration:
       Shift register      LCD
       QA - 0              DB4
       QB - 1              DB5
       QC - 2              DB6
       QD - 3              DB7
       QE - 4              E
       QF - 5              
       QG - 6              Rs
       GND                 Rw
    
    @param      strobe[in] digital IO connected to shiftregister strobe pin.
    @param      data[in] digital IO connected to the shiftregister data pin.
    @param      clk[in] digital IO connected to the shiftregister clock pin.
    */
   LiquidCrystal_SR3W(uint8_t data, uint8_t clk, uint8_t strobe);
   // Constructor with backlight control
   LiquidCrystal_SR3W(uint8_t data, uint8_t clk, uint8_t strobe, 
                      uint8_t backlighPin, t_backlighPol pol);   
   
   /*!
    @method     
    @abstract   Class constructor. 
    @discussion Initializes class variables and defines the control lines of
    the LCD and the shiftregister. The constructor does not initialize the LCD.
    
    @param      strobe[in] digital IO connected to shiftregister strobe pin.
    @param      data[in] digital IO connected to shiftregister data pin.
    @param      clk[in] digital IO connected to shiftregister clock pin.
    @param      En[in] LCD En (Enable) pin connected to SR output pin.
    @param      Rw[in] LCD Rw (Read/write) pin connected to SR output pin.
    @param      Rs[in] LCD Rs (Reg Select) pin connected to SR output pin.
    @param      d4[in] LCD data 4 pin map to the SR output pin.
    @param      d5[in] LCD data 5 pin map to the SR output pin.
    @param      d6[in] LCD data 6 pin map to the SR output pin.
    @param      d7[in] LCD data 7 pin map to the SR output pin.
    */
   LiquidCrystal_SR3W(uint8_t data, uint8_t clk, uint8_t strobe, 
                      uint8_t En, uint8_t Rw, uint8_t Rs, 
                      uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7 );
   // Constructor with backlight control
   LiquidCrystal_SR3W( uint8_t data, uint8_t clk, uint8_t strobe,
                      uint8_t En, uint8_t Rw, uint8_t Rs, 
                      uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                      uint8_t backlighPin, t_backlighPol pol);
   
   /*!
    @function
    @abstract   Send a particular value to the LCD.
    @discussion Sends a particular value to the LCD for writing to the LCD or
    as an LCD command.
    
    Users should never call this method.
    
    @param      value[in] Value to send to the LCD.
    @param      mode[in] DATA - write to the LCD CGRAM, COMMAND - write a 
    command to the LCD.
    */
   virtual void send(uint8_t value, uint8_t mode);
   
   /*!
    @function
    @abstract   Sets the pin to control the backlight.
    @discussion Sets the pin in the device to control the backlight. This device
    doesn't support dimming backlight capability.
    
    @param      value: pin mapped on the 74HC595N (0, .., 7) for (Qa0, .., Qh7)
    respectively.
    @param      pol: polarity POSITIVE|NEGATIVE.
    */
   void setBacklightPin ( uint8_t value, t_backlighPol pol );
   
   /*!
    @function
    @abstract   Switch-on/off the LCD backlight.
    @discussion Switch-on/off the LCD backlight.
    The setBacklightPin has to be called before setting the backlight for
    this method to work. @see setBacklightPin.
    
    @param      value: backlight mode (HIGH|LOW)
    */
   void setBacklight ( uint8_t value );
   
private:
   
   /*!
    @method     
    @abstract   Initializes the LCD class
    @discussion Initializes the LCD class and IO expansion module.
    */
   int  init(uint8_t data, uint8_t clk, uint8_t strobe, 
             uint8_t Rs, uint8_t Rw, uint8_t En,
             uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
   
   /*!
    @method     
    @abstract   Writes an 4 bit value to the LCD.
    @discussion Writes 4 bits (the least significant) to the LCD control data lines.
    @param      value[in] Value to write to the LCD
    @param      more[in]  Value to distinguish between command and data.
    COMMAND == command, DATA == data.
    */
   void write4bits(uint8_t value, uint8_t mode);
   
   /*!
    @function
    @abstract   load into the shift register a byte
    @discussion loads into the shift register a byte
    @param      value[in]: value to be loaded into the shiftregister.
    */
   void loadSR(uint8_t value);
   
   
   fio_bit      _strobe;           // shift register strobe pin
   fio_register _strobe_reg;       // SR strobe pin MCU register
   fio_bit      _data;             // shift register data pin
   fio_register _data_reg;         // SR data pin MCU register
   fio_bit      _clk;              // shift register clock pin
   fio_register _clk_reg;          // SR clock pin MCU register
   uint8_t      _En;               // LCD expander word for enable pin
   uint8_t      _Rw;               // LCD expander word for R/W pin
   uint8_t      _Rs;               // LCD expander word for Register Select pin
   uint8_t      _data_pins[4];     // LCD data lines
   uint8_t      _backlightPinMask; // Backlight IO pin mask
   uint8_t      _backlightStsMask; // Backlight status mask
   
};

#endif

