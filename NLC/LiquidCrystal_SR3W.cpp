
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif
#include "LiquidCrystal_SR3W.h"

#include "FastIO.h"

/*!
 @defined 
 @abstract   LCD_NOBACKLIGHT
 @discussion No BACKLIGHT MASK
 */
#define LCD_NOBACKLIGHT 0x00

/*!
 @defined 
 @abstract   LCD_BACKLIGHT
 @discussion BACKLIGHT MASK used when backlight is on
 */
#define LCD_BACKLIGHT   0xFF


// Default library configuration parameters used by class constructor with
// only the I2C address field.
// ---------------------------------------------------------------------------
/*!
 @defined 
 @abstract   Enable bit of the LCD
 @discussion Defines the IO of the expander connected to the LCD's Enable
 */
#define EN 4  // Enable bit

/*!
 @defined 
 @abstract   Read/Write bit of the LCD
 @discussion Defines the IO of the expander connected to the LCD's Rw pin
 */
#define RW 5  // Read/Write bit

/*!
 @defined 
 @abstract   Register bit of the LCD
 @discussion Defines the IO of the expander connected to the LCD's Register select pin
 */
#define RS 6  // Register select bit

/*!
 @defined 
 @abstract   LCD dataline allocation this library only supports 4 bit LCD control
 mode.
 @discussion D4, D5, D6, D7 LCD data lines pin mapping of the extender module
 */
#define D4 0
#define D5 1
#define D6 2
#define D7 3



LiquidCrystal_SR3W::LiquidCrystal_SR3W(uint8_t data, uint8_t clk, uint8_t strobe)
{
   init( data, clk, strobe, RS, RW, EN, D4, D5, D6, D7 );
}

LiquidCrystal_SR3W::LiquidCrystal_SR3W(uint8_t data, uint8_t clk, uint8_t strobe,
                                       uint8_t backlighPin, t_backlighPol pol)
{
   init( data, clk, strobe, RS, RW, EN, D4, D5, D6, D7 );
   setBacklightPin(backlighPin, pol);
}

LiquidCrystal_SR3W::LiquidCrystal_SR3W(uint8_t data, uint8_t clk, uint8_t strobe,
                                       uint8_t En, uint8_t Rw, uint8_t Rs, 
                                       uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7 )
{
   init( data, clk, strobe, Rs, Rw, En, d4, d5, d6, d7 );
}

LiquidCrystal_SR3W::LiquidCrystal_SR3W(uint8_t data, uint8_t clk, uint8_t strobe, 
                                       uint8_t En, uint8_t Rw, uint8_t Rs, 
                                       uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                                       uint8_t backlighPin, t_backlighPol pol)
{
   init( data, clk, strobe, Rs, Rw, En, d4, d5, d6, d7 );
   setBacklightPin(backlighPin, pol);
}


void LiquidCrystal_SR3W::send(uint8_t value, uint8_t mode)
{
   
   if ( mode != FOUR_BITS )
   {
      write4bits( (value >> 4), mode ); // upper nibble
   }   
   write4bits( (value & 0x0F), mode); // lower nibble


#if (F_CPU <= 16000000)
   // No need to use the delay routines on AVR since the time taken to write
   // on AVR with SR pin mapping even with fio is longer than LCD command execution.
   waitUsec(37); //goes away on AVRs
#else
   delayMicroseconds ( 37 );      // commands & data writes need > 37us to complete
#endif

}


void LiquidCrystal_SR3W::setBacklightPin ( uint8_t value, t_backlighPol pol = POSITIVE )
{
   _backlightPinMask = ( 1 << value );
   _backlightStsMask = LCD_NOBACKLIGHT;
   _polarity = pol;
   setBacklight (BACKLIGHT_OFF);     // Set backlight to off as initial setup
}

void LiquidCrystal_SR3W::setBacklight ( uint8_t value )
{
   // Check if backlight is available
   // ----------------------------------------------------
   if ( _backlightPinMask != 0x0 )
   {
      // Check for polarity to configure mask accordingly
      // ----------------------------------------------------------
      if  (((_polarity == POSITIVE) && (value > 0)) || 
           ((_polarity == NEGATIVE ) && ( value == 0 )))
      {
         _backlightStsMask = _backlightPinMask & LCD_BACKLIGHT;
      }
      else 
      {
         _backlightStsMask = _backlightPinMask & LCD_NOBACKLIGHT;
      }
      loadSR( _backlightStsMask );
   }
}


// PRIVATE METHODS
// -----------------------------------------------------------------------------

int LiquidCrystal_SR3W::init(uint8_t data, uint8_t clk, uint8_t strobe, 
                             uint8_t Rs, uint8_t Rw, uint8_t En,
                             uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
   _data       = fio_pinToBit(data);
   _clk        = fio_pinToBit(clk);
   _strobe     = fio_pinToBit(strobe);
   _data_reg   = fio_pinToOutputRegister(data);
   _clk_reg    = fio_pinToOutputRegister(clk);
   _strobe_reg = fio_pinToOutputRegister(strobe);
   
   // LCD pin mapping
   _backlightPinMask = 0;
   _backlightStsMask = LCD_NOBACKLIGHT;
   _polarity = POSITIVE;
   
   _En = ( 1 << En );
   _Rw = ( 1 << Rw );
   _Rs = ( 1 << Rs );
   
   // Initialise pin mapping
   _data_pins[0] = ( 1 << d4 );
   _data_pins[1] = ( 1 << d5 );
   _data_pins[2] = ( 1 << d6 );
   _data_pins[3] = ( 1 << d7 );
   
   _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
   
   return (1);
}

void LiquidCrystal_SR3W::write4bits(uint8_t value, uint8_t mode)
{
   uint8_t pinMapValue = 0;
   
   // Map the value to LCD pin mapping
   // --------------------------------
   for ( uint8_t i = 0; i < 4; i++ )
   {
      if ( ( value & 0x1 ) == 1 )
      {
         pinMapValue |= _data_pins[i];
      }
      value = ( value >> 1 );
   }
   
   // Is it a command or data
   // -----------------------
   mode = ( mode == DATA ) ? _Rs : 0;
   
   pinMapValue |= mode | _backlightStsMask;
   loadSR ( pinMapValue | _En );  // Send with enable high
   loadSR ( pinMapValue); // Send with enable low
}


void LiquidCrystal_SR3W::loadSR(uint8_t value) 
{
   // Load the shift register with information
   fio_shiftOut(_data_reg, _data, _clk_reg, _clk, value, MSBFIRST);
   
   // Strobe the data into the latch
   ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
   {
      fio_digitalWrite_HIGH(_strobe_reg, _strobe);
      fio_digitalWrite_SWITCHTO(_strobe_reg, _strobe, LOW);
   }
}
