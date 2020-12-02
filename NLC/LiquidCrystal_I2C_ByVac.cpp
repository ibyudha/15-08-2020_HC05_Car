
#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif
#include <inttypes.h>
#include "LiquidCrystal_I2C_ByVac.h"

// CONSTRUCTORS
// ---------------------------------------------------------------------------
LiquidCrystal_I2C_ByVac::LiquidCrystal_I2C_ByVac( uint8_t lcd_Addr )
{
   _Addr = lcd_Addr;
   _polarity == NEGATIVE;
}

// PUBLIC METHODS
// ---------------------------------------------------------------------------

//
// begin
void LiquidCrystal_I2C_ByVac::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) 
{
   Wire.begin();
   LCD::begin ( cols, lines, dotsize );   
}

// User commands - users can expand this section
//----------------------------------------------------------------------------
// Turn the integrated backlight off/on

// setBacklight
void LiquidCrystal_I2C_ByVac::setBacklight( uint8_t value ) 
{
  Wire.beginTransmission(_Addr);
  Wire.write(0x03); 					//  ByVac command code 0x03 for backlight
  if (value==0) Wire.write(1); else Wire.write((byte)0); 	// 1 for off since polarity is NEGATIVE
  Wire.endTransmission();
}

// Turn the contrast off/on

// setContrast
void LiquidCrystal_I2C_ByVac::setContrast( uint8_t value ) 
{
  Wire.beginTransmission(_Addr);
  Wire.write(0x05); 					//  ByVac command code 0x05 for contrast
  if (value==0) Wire.write((byte)0); else Wire.write(1); 
  Wire.endTransmission();
}

// PRIVATE METHODS
// ---------------------------------------------------------------------------

//
// init
int LiquidCrystal_I2C_ByVac::init()
{
   int status = 0;
   
   // ByVac backpack initialized by onboard firmware
   // ------------------------------------------------------------------------
   status=1;
   return ( status );
}

// low level data pushing commands
//----------------------------------------------------------------------------

//
// send - write either command or data
void LiquidCrystal_I2C_ByVac::send(uint8_t value, uint8_t mode) 
{
  Wire.beginTransmission(_Addr);
  Wire.write(mode+1); // map COMMAND (0) -> ByVac command code 0x01/ DATA  (1) ->  ByVac command code 0x02
  Wire.write(value);
  Wire.endTransmission();
}
