

#ifndef _LIQUIDCRYSTAL_SR1W_
#define _LIQUIDCRYSTAL_SR1W_

#include <inttypes.h>
#include "LCD.h"
#include "FastIO.h"

// 1-wire SR timing constants
// ---------------------------------------------------------------------------

// NOTE:
//  The 1.5k resistor (1.2k - 1.8k with a 20% tolerance)
//   takes between 2.376uS and 4.36uS to fully charge or discharge
//	 the 2.2n capacitor (1.98n - 2.42n with a 10% tolerance).
//	We round this up to a 5uS delay to provide an additional safety margin.

#define SR1W_DELAY_US		5
#define SR1W_DELAY()		{ delayMicroseconds(SR1W_DELAY_US); numDelays++; }

// 1-wire SR output bit constants
// ---------------------------------------------------------------------------

#define SR1W_UNUSED_MASK	0x01	// Set unused bit(s) to '1' as they are slightly faster to clock in.
#define SR1W_D7_MASK		0x02
#define SR1W_D6_MASK		0x04
#define SR1W_D5_MASK		0x08
#define SR1W_D4_MASK		0x10
#define SR1W_BL_MASK		0x20
#define SR1W_RS_MASK		0x40
#define SR1W_EN_MASK		0x80	// This cannot be changed. It has to be the first thing shifted in.

#define SR1W_ATOMIC_WRITE_LOW(reg, mask)	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { *reg &= ~mask; }
#define SR1W_ATOMIC_WRITE_HIGH(reg, mask)	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { *reg |= mask; }


typedef enum { SW_CLEAR, HW_CLEAR } t_sr1w_circuitType;

class LiquidCrystal_SR1W : public LCD
{
public:
   /*!
    @method     
    @abstract   LCD 1 wire SHIFT REGISTER constructor.
    @discussion Defines the pin assignments that connect to the shift register.
    The constructor does not initialize the LCD. Assuming 1 line 8 pixel high 
    font.
    
    @param srdata[in]       Arduino pin for shift register.
    @param circuitType[in]  optionally select an alternate circuit type
    @param blpol[in]        optional backlight polarity (default = POSITIVE)
    */
   LiquidCrystal_SR1W (uint8_t srdata, t_sr1w_circuitType circuitType, 
                       t_backlighPol blpol = POSITIVE);
   
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
   void init ( uint8_t srdata, t_sr1w_circuitType circuitType, t_backlighPol blpol, 
               uint8_t lines, uint8_t font );
   
   /*!
    @method     
    @abstract Clears the shift register to ensure the Latch/Enable pins aren't 
    triggered accidentally.
    */
   uint8_t clearSR ();
   
   /*!
    * @method
    * @abstract takes care of shifting and the enable pulse
    */
   uint8_t loadSR (uint8_t val);
   
   fio_register _srRegister; // Serial PIN
   fio_bit _srMask;
   
   t_sr1w_circuitType _circuitType;
   
   uint8_t _blPolarity;
   uint8_t _blMask;
};
#endif