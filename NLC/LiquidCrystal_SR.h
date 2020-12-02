
#ifndef _LIQUIDCRYSTAL_SR_
#define _LIQUIDCRYSTAL_SR_

#include <inttypes.h>
#include "LCD.h"
#include "FastIO.h"


// two-wire indicator constant
// ---------------------------------------------------------------------------
#define TWO_WIRE  204
#define SR_RS_BIT 0x04
#define SR_EN_BIT 0x80

class LiquidCrystal_SR : public LCD
{
public:
   /*!
    @method     
    @abstract   LCD SHIFT REGISTER constructors.
    @discussion Defines the pin assignment that the LCD will have.
    The constructor does not initialize the LCD. Assuming 1 line 8 pixel high 
    font.
    
    @param srdata[in]   pin for shiftregister data line.
    @param srclock[in]  pin for shiftregister clock line.
    @param enable[in]   optional direct enable pin for the LCD
    */
   LiquidCrystal_SR ( uint8_t srdata, uint8_t srclock, uint8_t enable=TWO_WIRE );
   
   /*!
    @function
    @abstract   Send a particular value to the LCD.
    @discussion Sends a particular value to the LCD for writing to the LCD or
    as an LCD command using the shift register.
    
    Users should never call this method.
    
    @param      value[in] Value to send to the LCD.
    @result     mode LOW - write to the LCD CGRAM, HIGH - write a command to
    the LCD.
    */
   virtual void send(uint8_t value, uint8_t mode);
   
   
   /*!
    @function
    @abstract   Sets the pin to control the backlight.
    @discussion Sets the pin in the device to control the backlight.
    @warning    Currently not supported
    
    @param      mode: backlight mode (HIGH|LOW)
    @param      pol: backlight polarity
    */
   void setBacklightPin ( uint8_t pin, t_backlighPol pol );
   
   /*!
    @function
    @abstract   Switch-on/off the LCD backlight.
    @discussion Switch-on/off the LCD backlight.
    The setBacklightPin has to be called before setting the backlight for
    this method to work. @see setBacklightPin.
    
    @param      mode: backlight mode (HIGH|LOW)
    */
   void setBacklight ( uint8_t mode );
   
private:
   
   /*!
    @method     
    @abstract   Initializes the LCD pin allocation
    @discussion Initializes the LCD pin allocation and configuration.
    */
   void init ( uint8_t srdata, uint8_t srclock, uint8_t enable, uint8_t lines, 
              uint8_t font );
   
   /*!
    * @method
    * @abstract takes care of shifting and the enable pulse
    */
   void shiftIt (uint8_t val);
   
   uint8_t _enable_pin;  // Enable Pin
   uint8_t _two_wire;    // two wire mode
   
   fio_register _srDataRegister; // Serial Data pin
   fio_bit _srDataBit;
   fio_register _srClockRegister; // Clock Pin
   fio_bit _srClockBit;
   fio_register _srEnableRegister; // Enable Pin
   fio_bit _srEnableBit;
   
};

#endif

