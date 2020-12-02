#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #if defined(__AVR__)
    #include <avr/io.h>
  #endif
  #include "WProgram.h"
#endif

#include "AFMotor.h"



static uint8_t latch_state;

#if (MICROSTEPS == 8)
uint8_t microstepcurve[] = {0, 50, 98, 142, 180, 212, 236, 250, 255};
#elif (MICROSTEPS == 16)
uint8_t microstepcurve[] = {0, 25, 50, 74, 98, 120, 141, 162, 180, 197, 212, 225, 236, 244, 250, 253, 255};
#endif

AFMotorController::AFMotorController(void) {
    TimerInitalized = false;
}

void AFMotorController::enable(void) {
  pinMode(MOTORLATCH, OUTPUT);
  pinMode(MOTORENABLE, OUTPUT);
  pinMode(MOTORDATA, OUTPUT);
  pinMode(MOTORCLK, OUTPUT);
  latch_state = 0;
  latch_tx();
  digitalWrite(MOTORENABLE, LOW);
}


void AFMotorController::latch_tx(void) {
  uint8_t i;

  digitalWrite(MOTORLATCH, LOW);
  digitalWrite(MOTORDATA, LOW);

  for (i=0; i<8; i++) {
    digitalWrite(MOTORCLK, LOW);

    if (latch_state & _BV(7-i)) {

      digitalWrite(MOTORDATA, HIGH);
    } else {

      digitalWrite(MOTORDATA, LOW);
    }

    digitalWrite(MOTORCLK, HIGH);
  }

  digitalWrite(MOTORLATCH, HIGH);
}

static AFMotorController MC;

inline void initPWM1(uint8_t freq) {
#if defined(__AVR_ATmega8__) || \
    defined(__AVR_ATmega48__) || \
    defined(__AVR_ATmega88__) || \
    defined(__AVR_ATmega168__) || \
    defined(__AVR_ATmega328P__)
    TCCR2A |= _BV(COM2A1) | _BV(WGM20) | _BV(WGM21); 
    TCCR2B = freq & 0x7;
    OCR2A = 0;
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

    TCCR1A |= _BV(COM1A1) | _BV(WGM10); 
    TCCR1B = (freq & 0x7) | _BV(WGM12);
    OCR1A = 0;
#elif defined(__PIC32MX__)
    #if defined(PIC32_USE_PIN9_FOR_M1_PWM)

        pinMode(9, OUTPUT);
        pinMode(11, INPUT);
        if (!MC.TimerInitalized)
        {   
            T2CON = 0x8000 | ((freq & 0x07) << 4); 
            TMR2 = 0x0000;
            PR2 = 0x0100;
            MC.TimerInitalized = true;
        }
  
        OC4CON = 0x8006; 
        OC4RS = 0x0000;
        OC4R = 0x0000;
    #elif defined(PIC32_USE_PIN10_FOR_M1_PWM)
        pinMode(10, OUTPUT);
        pinMode(11, INPUT);
        if (!MC.TimerInitalized)
        {  
            T2CON = 0x8000 | ((freq & 0x07) << 4);
            TMR2 = 0x0000;
            PR2 = 0x0100;
            MC.TimerInitalized = true;
        }
  
        OC5CON = 0x8006;
        OC5RS = 0x0000;
        OC5R = 0x0000;
    #else
   
        digitalWrite(11, LOW);
    #endif
#else
   #error "This chip is not supported!"
#endif
    #if !defined(PIC32_USE_PIN9_FOR_M1_PWM) && !defined(PIC32_USE_PIN10_FOR_M1_PWM)
        pinMode(11, OUTPUT);
    #endif
}

inline void setPWM1(uint8_t s) {
#if defined(__AVR_ATmega8__) || \
    defined(__AVR_ATmega48__) || \
    defined(__AVR_ATmega88__) || \
    defined(__AVR_ATmega168__) || \
    defined(__AVR_ATmega328P__)
 
    OCR2A = s;
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

    OCR1A = s;
#elif defined(__PIC32MX__)
    #if defined(PIC32_USE_PIN9_FOR_M1_PWM)
  
        OC4RS = s;
    #elif defined(PIC32_USE_PIN10_FOR_M1_PWM)

        OC5RS = s;
    #else

        if (s > 127)
        {
            digitalWrite(11, HIGH);
        }
        else
        {
            digitalWrite(11, LOW);
        }
    #endif
#else
   #error "This chip is not supported!"
#endif
}

inline void initPWM2(uint8_t freq) {
#if defined(__AVR_ATmega8__) || \
    defined(__AVR_ATmega48__) || \
    defined(__AVR_ATmega88__) || \
    defined(__AVR_ATmega168__) || \
    defined(__AVR_ATmega328P__)
    TCCR2A |= _BV(COM2B1) | _BV(WGM20) | _BV(WGM21); 
    TCCR2B = freq & 0x7;
    OCR2B = 0;
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    TCCR3A |= _BV(COM1C1) | _BV(WGM10);
    TCCR3B = (freq & 0x7) | _BV(WGM12);
    OCR3C = 0;
#elif defined(__PIC32MX__)
    if (!MC.TimerInitalized)
    { 
        T2CON = 0x8000 | ((freq & 0x07) << 4); 
        TMR2 = 0x0000;
        PR2 = 0x0100;
        MC.TimerInitalized = true;
    }

    OC1CON = 0x8006;
    OC1RS = 0x0000;
    OC1R = 0x0000;
#else
   #error "This chip is not supported!"
#endif

    pinMode(3, OUTPUT);
}

inline void setPWM2(uint8_t s) {
#if defined(__AVR_ATmega8__) || \
    defined(__AVR_ATmega48__) || \
    defined(__AVR_ATmega88__) || \
    defined(__AVR_ATmega168__) || \
    defined(__AVR_ATmega328P__)
    OCR2B = s;
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    OCR3C = s;
#elif defined(__PIC32MX__)
    OC1RS = s;
#else
   #error "This chip is not supported!"
#endif
}

inline void initPWM3(uint8_t freq) {
#if defined(__AVR_ATmega8__) || \
    defined(__AVR_ATmega48__) || \
    defined(__AVR_ATmega88__) || \
    defined(__AVR_ATmega168__) || \
    defined(__AVR_ATmega328P__)
    TCCR0A |= _BV(COM0A1) | _BV(WGM00) | _BV(WGM01);
    OCR0A = 0;
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    TCCR4A |= _BV(COM1A1) | _BV(WGM10);
    TCCR4B = (freq & 0x7) | _BV(WGM12);
    OCR4A = 0;
#elif defined(__PIC32MX__)
    if (!MC.TimerInitalized)
    {
        T2CON = 0x8000 | ((freq & 0x07) << 4); 
        TMR2 = 0x0000;
        PR2 = 0x0100;
        MC.TimerInitalized = true;
    }
    OC3CON = 0x8006;
    OC3RS = 0x0000;
    OC3R = 0x0000;
#else
   #error "This chip is not supported!"
#endif
    pinMode(6, OUTPUT);
}

inline void setPWM3(uint8_t s) {
#if defined(__AVR_ATmega8__) || \
    defined(__AVR_ATmega48__) || \
    defined(__AVR_ATmega88__) || \
    defined(__AVR_ATmega168__) || \
    defined(__AVR_ATmega328P__)
    OCR0A = s;
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    OCR4A = s;
#elif defined(__PIC32MX__)
    OC3RS = s;
#else
   #error "This chip is not supported!"
#endif
}



inline void initPWM4(uint8_t freq) {
#if defined(__AVR_ATmega8__) || \
    defined(__AVR_ATmega48__) || \
    defined(__AVR_ATmega88__) || \
    defined(__AVR_ATmega168__) || \
    defined(__AVR_ATmega328P__)
    TCCR0A |= _BV(COM0B1) | _BV(WGM00) | _BV(WGM01);
    OCR0B = 0;
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    TCCR3A |= _BV(COM1A1) | _BV(WGM10); 
    TCCR3B = (freq & 0x7) | _BV(WGM12);

    OCR3A = 0;
#elif defined(__PIC32MX__)
    if (!MC.TimerInitalized)
    {  
        T2CON = 0x8000 | ((freq & 0x07) << 4); 
        TMR2 = 0x0000;
        PR2 = 0x0100;
        MC.TimerInitalized = true;
    }

    OC2CON = 0x8006; 
    OC2RS = 0x0000;
    OC2R = 0x0000;
#else
   #error "This chip is not supported!"
#endif
    pinMode(5, OUTPUT);
}

inline void setPWM4(uint8_t s) {
#if defined(__AVR_ATmega8__) || \
    defined(__AVR_ATmega48__) || \
    defined(__AVR_ATmega88__) || \
    defined(__AVR_ATmega168__) || \
    defined(__AVR_ATmega328P__)
    OCR0B = s;
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    OCR3A = s;
#elif defined(__PIC32MX__)
    OC2RS = s;
#else
   #error "This chip is not supported!"
#endif
}

AF_DCMotor::AF_DCMotor(uint8_t num, uint8_t freq) {
  motornum = num;
  pwmfreq = freq;

  MC.enable();

  switch (num) {
  case 1:
    latch_state &= ~_BV(MOTOR1_A) & ~_BV(MOTOR1_B); 
    MC.latch_tx();
    initPWM1(freq);
    break;
  case 2:
    latch_state &= ~_BV(MOTOR2_A) & ~_BV(MOTOR2_B);
    MC.latch_tx();
    initPWM2(freq);
    break;
  case 3:
    latch_state &= ~_BV(MOTOR3_A) & ~_BV(MOTOR3_B);
    MC.latch_tx();
    initPWM3(freq);
    break;
  case 4:
    latch_state &= ~_BV(MOTOR4_A) & ~_BV(MOTOR4_B); 
    MC.latch_tx();
    initPWM4(freq);
    break;
  }
}

void AF_DCMotor::run(uint8_t cmd) {
  uint8_t a, b;
  switch (motornum) {
  case 1:
    a = MOTOR1_A; b = MOTOR1_B; break;
  case 2:
    a = MOTOR2_A; b = MOTOR2_B; break;
  case 3:
    a = MOTOR3_A; b = MOTOR3_B; break;
  case 4:
    a = MOTOR4_A; b = MOTOR4_B; break;
  default:
    return;
  }
  
  switch (cmd) {
  case FORWARD:
    latch_state |= _BV(a);
    latch_state &= ~_BV(b); 
    MC.latch_tx();
    break;
  case BACKWARD:
    latch_state &= ~_BV(a);
    latch_state |= _BV(b); 
    MC.latch_tx();
    break;
  case RELEASE:
    latch_state &= ~_BV(a);   
    latch_state &= ~_BV(b); 
    MC.latch_tx();
    break;
  }
}

void AF_DCMotor::setSpeed(uint8_t speed) {
  switch (motornum) {
  case 1:
    setPWM1(speed); break;
  case 2:
    setPWM2(speed); break;
  case 3:
    setPWM3(speed); break;
  case 4:
    setPWM4(speed); break;
  }
}
AF_Stepper::AF_Stepper(uint16_t steps, uint8_t num) {
  MC.enable();

  revsteps = steps;
  steppernum = num;
  currentstep = 0;

  if (steppernum == 1) {
    latch_state &= ~_BV(MOTOR1_A) & ~_BV(MOTOR1_B) &
      ~_BV(MOTOR2_A) & ~_BV(MOTOR2_B);
    MC.latch_tx();

    pinMode(11, OUTPUT);
    pinMode(3, OUTPUT);
    digitalWrite(11, HIGH);
    digitalWrite(3, HIGH);
    initPWM1(STEPPER1_PWM_RATE);
    initPWM2(STEPPER1_PWM_RATE);
    setPWM1(255);
    setPWM2(255);

  } else if (steppernum == 2) {
    latch_state &= ~_BV(MOTOR3_A) & ~_BV(MOTOR3_B) &
      ~_BV(MOTOR4_A) & ~_BV(MOTOR4_B);
    MC.latch_tx();
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    initPWM3(STEPPER2_PWM_RATE);
    initPWM4(STEPPER2_PWM_RATE);
    setPWM3(255);
    setPWM4(255);
  }
}

void AF_Stepper::setSpeed(uint16_t rpm) {
  usperstep = 60000000 / ((uint32_t)revsteps * (uint32_t)rpm);
  steppingcounter = 0;
}

void AF_Stepper::release(void) {
  if (steppernum == 1) {
    latch_state &= ~_BV(MOTOR1_A) & ~_BV(MOTOR1_B) &
      ~_BV(MOTOR2_A) & ~_BV(MOTOR2_B);
    MC.latch_tx();
  } else if (steppernum == 2) {
    latch_state &= ~_BV(MOTOR3_A) & ~_BV(MOTOR3_B) &
      ~_BV(MOTOR4_A) & ~_BV(MOTOR4_B);
    MC.latch_tx();
  }
}

void AF_Stepper::step(uint16_t steps, uint8_t dir,  uint8_t style) {
  uint32_t uspers = usperstep;
  uint8_t ret = 0;

  if (style == INTERLEAVE) {
    uspers /= 2;
  }
 else if (style == MICROSTEP) {
    uspers /= MICROSTEPS;
    steps *= MICROSTEPS;
#ifdef MOTORDEBUG
    Serial.print("steps = "); Serial.println(steps, DEC);
#endif
  }

  while (steps--) {
    ret = onestep(dir, style);
    delay(uspers/1000); // in ms
    steppingcounter += (uspers % 1000);
    if (steppingcounter >= 1000) {
      delay(1);
      steppingcounter -= 1000;
    }
  }
  if (style == MICROSTEP) {
    while ((ret != 0) && (ret != MICROSTEPS)) {
      ret = onestep(dir, style);
      delay(uspers/1000); // in ms
      steppingcounter += (uspers % 1000);
      if (steppingcounter >= 1000) {
	delay(1);
	steppingcounter -= 1000;
      } 
    }
  }
}

uint8_t AF_Stepper::onestep(uint8_t dir, uint8_t style) {
  uint8_t a, b, c, d;
  uint8_t ocrb, ocra;

  ocra = ocrb = 255;

  if (steppernum == 1) {
    a = _BV(MOTOR1_A);
    b = _BV(MOTOR2_A);
    c = _BV(MOTOR1_B);
    d = _BV(MOTOR2_B);
  } else if (steppernum == 2) {
    a = _BV(MOTOR3_A);
    b = _BV(MOTOR4_A);
    c = _BV(MOTOR3_B);
    d = _BV(MOTOR4_B);
  } else {
    return 0;
  }

  if (style == SINGLE) {
    if ((currentstep/(MICROSTEPS/2)) % 2) { 
      if (dir == FORWARD) {
	currentstep += MICROSTEPS/2;
      }
      else {
	currentstep -= MICROSTEPS/2;
      }
    } else {      
      if (dir == FORWARD) {
	currentstep += MICROSTEPS;
      }
      else {
	currentstep -= MICROSTEPS;
      }
    }
  } else if (style == DOUBLE) {
    if (! (currentstep/(MICROSTEPS/2) % 2)) { 
      if (dir == FORWARD) {
	currentstep += MICROSTEPS/2;
      } else {
	currentstep -= MICROSTEPS/2;
      }
    } else {  
      if (dir == FORWARD) {
	currentstep += MICROSTEPS;
      } else {
	currentstep -= MICROSTEPS;
      }
    }
  } else if (style == INTERLEAVE) {
    if (dir == FORWARD) {
       currentstep += MICROSTEPS/2;
    } else {
       currentstep -= MICROSTEPS/2;
    }
  } 

  if (style == MICROSTEP) {
    if (dir == FORWARD) {
      currentstep++;
    } else {
      currentstep--;
    }

    currentstep += MICROSTEPS*4;
    currentstep %= MICROSTEPS*4;

    ocra = ocrb = 0;
    if ( (currentstep >= 0) && (currentstep < MICROSTEPS)) {
      ocra = microstepcurve[MICROSTEPS - currentstep];
      ocrb = microstepcurve[currentstep];
    } else if  ( (currentstep >= MICROSTEPS) && (currentstep < MICROSTEPS*2)) {
      ocra = microstepcurve[currentstep - MICROSTEPS];
      ocrb = microstepcurve[MICROSTEPS*2 - currentstep];
    } else if  ( (currentstep >= MICROSTEPS*2) && (currentstep < MICROSTEPS*3)) {
      ocra = microstepcurve[MICROSTEPS*3 - currentstep];
      ocrb = microstepcurve[currentstep - MICROSTEPS*2];
    } else if  ( (currentstep >= MICROSTEPS*3) && (currentstep < MICROSTEPS*4)) {
      ocra = microstepcurve[currentstep - MICROSTEPS*3];
      ocrb = microstepcurve[MICROSTEPS*4 - currentstep];
    }
  }

  currentstep += MICROSTEPS*4;
  currentstep %= MICROSTEPS*4;

#ifdef MOTORDEBUG
  Serial.print("current step: "); Serial.println(currentstep, DEC);
  Serial.print(" pwmA = "); Serial.print(ocra, DEC); 
  Serial.print(" pwmB = "); Serial.println(ocrb, DEC); 
#endif

  if (steppernum == 1) {
    setPWM1(ocra);
    setPWM2(ocrb);
  } else if (steppernum == 2) {
    setPWM3(ocra);
    setPWM4(ocrb);
  }


  latch_state &= ~a & ~b & ~c & ~d; 

  if (style == MICROSTEP) {
    if ((currentstep >= 0) && (currentstep < MICROSTEPS))
      latch_state |= a | b;
    if ((currentstep >= MICROSTEPS) && (currentstep < MICROSTEPS*2))
      latch_state |= b | c;
    if ((currentstep >= MICROSTEPS*2) && (currentstep < MICROSTEPS*3))
      latch_state |= c | d;
    if ((currentstep >= MICROSTEPS*3) && (currentstep < MICROSTEPS*4))
      latch_state |= d | a;
  } else {
    switch (currentstep/(MICROSTEPS/2)) {
    case 0:
      latch_state |= a; 
      break;
    case 1:
      latch_state |= a | b; 
      break;
    case 2:
      latch_state |= b; 
      break;
    case 3:
      latch_state |= b | c; 
      break;
    case 4:
      latch_state |= c; 
      break; 
    case 5:
      latch_state |= c | d;
      break;
    case 6:
      latch_state |= d;
      break;
    case 7:
      latch_state |= d | a;
      break;
    }
  }

 
  MC.latch_tx();
  return currentstep;
}

