
#ifndef _LIQUIDCRYSTAL_SR2W_
#define _LIQUIDCRYSTAL_SR2W_

#include <inttypes.h>
#include "LCD.h"
#include "FastIO.h"


// two-wire SR output bit constants
// ---------------------------------------------------------------------------

#define SR2W_BL_MASK 0x02
#define SR2W_RS_MASK 0x04
#define SR2W_DATA_MASK 0x78	// data bits are hard coded to be SR bits 6,5,4,3
#define SR2W_EN_MASK 0x80	// cannot ever be changed

class LiquidCrystal_SR2W : public LCD
{
public:
   /*!
    @method     
    @abstract   LCD 2 wire SHIFT REGISTER constructor.
    @discussion Defines the pin assignments that connect to the shift register.
    The constructor does not initialize the LCD. Assuming 1 line 8 pixel high 
    font.
    
    @param srdata[in]   Arduino pin for shift register data line.
    @param srclock[in]  Arduino pin for shift register clock line.
    @param blpol[in]    optional backlight polarity (default = POSITIVE)
    */
   LiquidCrystal_SR2W (uint8_t srdata, uint8_t srclock, t_backlighPol blpol = POSITIVE);
   
   /*!
    @function
    @abstract   Send a particular value to the LCD.
    @discussion Sends a particular value to the LCD for writing to the LCD or
    as an LCD command using the shift register.
    
    Users should never call this method.
    
    @param      value[in] Value to send to the LCD.
    @param      mode[in]  DATA=8bit data, COMMAND=8bit cmd, FOUR_BITS=4bit cmd
    the LCD.
    */
   virtual void send(uint8_t value, uint8_t mode);
   
   
   /*!
    @function
    @abstract   Switch-on/off the LCD backlight.
    @discussion Switch-on/off the LCD backlight.
    The setBacklightPin has to be called before setting the backlight for
    this method to work. @see setBacklightPin.
    
    @param      mode[in] backlight mode (0 off, non-zero on)
    */
   void setBacklight ( uint8_t mode );
   
private:
   
   /*!
    @method     
    @abstract   Initializes the LCD pin allocation
    @discussion Initializes the LCD pin allocation and configuration.
    */
   void init ( uint8_t srdata, uint8_t srclock, t_backlighPol blpol, uint8_t lines, uint8_t font );
   
   /*!
    * @method
    * @abstract takes care of shifting and the enable pulse
    */
   void loadSR (uint8_t val);
   
   fio_register _srDataRegister; // Serial Data pin
   fio_bit _srDataMask;
   fio_register _srClockRegister; // Clock Pin
   fio_bit _srClockMask;

   uint8_t _blPolarity;
   uint8_t _blMask;
};
#endif
