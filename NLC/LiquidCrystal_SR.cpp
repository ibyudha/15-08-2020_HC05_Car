
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif
#include "LiquidCrystal_SR.h"

#include "FastIO.h"


// CONSTRUCTORS
// ---------------------------------------------------------------------------
// Assuming 1 line 8 pixel high font
LiquidCrystal_SR::LiquidCrystal_SR (uint8_t srdata, uint8_t srclock, 
                                    uint8_t enable ) 
{
	init ( srdata, srclock, enable, 1, 0 );
}


// PRIVATE METHODS
// ---------------------------------------------------------------------------

//
// init
void LiquidCrystal_SR::init(uint8_t srdata, uint8_t srclock, uint8_t enable, 
                            uint8_t lines, uint8_t font)
{
   // Initialise private variables
   _two_wire = 0;
   
   _srDataRegister = fio_pinToOutputRegister(srdata);
   _srDataBit = fio_pinToBit(srdata);
   _srClockRegister = fio_pinToOutputRegister(srclock);
   _srClockBit = fio_pinToBit(srclock);
   
   if ((enable == TWO_WIRE) || (enable == srdata))
   {
      _two_wire = 1;
      _srEnableRegister = _srDataRegister;
      _srEnableBit = _srDataBit;
   }
   else
   {
      _srEnableRegister = fio_pinToOutputRegister(enable);
      _srEnableBit = fio_pinToBit(enable);
   }
   
   // Configure control pins as outputs
   // ------------------------------------------------------------------------
   
   _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
}

//
// shiftIt
void LiquidCrystal_SR::shiftIt(uint8_t val)
{
   if (_two_wire)
   {
      // Clear to get Enable LOW
      fio_shiftOut(_srDataRegister, _srDataBit, _srClockRegister, _srClockBit);
   }
   fio_shiftOut(_srDataRegister, _srDataBit, _srClockRegister, _srClockBit, val, MSBFIRST);
   
   // LCD ENABLE PULSE
   //
   // While this library is written with a shift register without an output
   // latch in mind, it can work in 3-wire mode with a shiftregister with a
   // latch. The shiftregister latch pin (STR, RCL or similar) is then
   // connected to the LCD enable pin. The LCD is (very likely) slower
   // to read the Enable pulse, and then reads the new contents of the SR.
   ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
   {
      fio_digitalWrite_HIGH(_srEnableRegister, _srEnableBit);
      delayMicroseconds (1);         // enable pulse must be >450ns               
      fio_digitalWrite_SWITCHTO(_srEnableRegister, _srEnableBit, LOW);
   } // end critical section
}

// PUBLIC METHODS
// ---------------------------------------------------------------------------


/************ low level data pushing commands **********/
//
// send
void LiquidCrystal_SR::send(uint8_t value, uint8_t mode)
{
   // Divide byte in two nibbles include the RS signal
   // and format it for shiftregister output wiring to the LCD
   // We are only interested in my COMMAND or DATA for myMode
   uint8_t myMode = ( mode == DATA ) ? SR_RS_BIT : 0; // RS bit; LOW: command.  HIGH: character.
   
   if ( mode != FOUR_BITS )
   {
      shiftIt(myMode | SR_EN_BIT | ((value >> 1) & 0x78)); // upper nibble
   }

   shiftIt(myMode | SR_EN_BIT | ((value << 3) & 0x78)); // lower nibble
   /*
    * Add some delay since this code is so fast it needs some added delay
    * even on AVRs because the shiftout is shorter than the LCD command execution time.
    */
#if (F_CPU <= 16000000)
   if(_two_wire)
   	delayMicroseconds ( 10 );
   else
   	delayMicroseconds ( 17 ); // 3 wire mode is faster so it must delay longer
#else
   delayMicroseconds ( 37 );      // commands & data writes need > 37us to complete
#endif

}

//
// setBacklightPin
void LiquidCrystal_SR::setBacklightPin ( uint8_t pin, t_backlighPol pol )
{ }

//
// setBacklight
void LiquidCrystal_SR::setBacklight ( uint8_t mode ) 
{ }

