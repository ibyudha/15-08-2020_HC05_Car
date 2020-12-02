#ifndef _AFMotor_h_
#define _AFMotor_h_

#include <inttypes.h>
#if defined(__AVR__)
    #include <avr/io.h>
    #define MICROSTEPS 16                
    #define MOTOR12_64KHZ _BV(CS20)    
    #define MOTOR12_8KHZ _BV(CS21)    
    #define MOTOR12_2KHZ _BV(CS21) | _BV(CS20)  
    #define MOTOR12_1KHZ _BV(CS22)            
    #define MOTOR34_64KHZ _BV(CS00)    
    #define MOTOR34_8KHZ _BV(CS01)            
    #define MOTOR34_1KHZ _BV(CS01) | _BV(CS00)  
    #define DC_MOTOR_PWM_RATE   MOTOR34_8KHZ   
    #define STEPPER1_PWM_RATE   MOTOR12_64KHZ 
    #define STEPPER2_PWM_RATE   MOTOR34_64KHZ   
    
#elif defined(__PIC32MX__)

    #define MICROSTEPS 16       
    #define MOTOR12_312KHZ  0  
    #define MOTOR12_156KHZ  1   
    #define MOTOR12_64KHZ   2 
    #define MOTOR12_39KHZ   3  
    #define MOTOR12_19KHZ   4   
    #define MOTOR12_8KHZ    5  
    #define MOTOR12_4_8KHZ  6  
    #define MOTOR12_2KHZ    7  
    #define MOTOR12_1KHZ    7   
    #define MOTOR34_312KHZ  0   
    #define MOTOR34_156KHZ  1  
    #define MOTOR34_64KHZ   2  
    #define MOTOR34_39KHZ   3  
    #define MOTOR34_19KHZ   4  
    #define MOTOR34_8KHZ    5  
    #define MOTOR34_4_8KHZ  6  
    #define MOTOR34_2KHZ    7  
    #define MOTOR34_1KHZ    7  
    #define DC_MOTOR_PWM_RATE   MOTOR34_39KHZ
    #define STEPPER1_PWM_RATE   MOTOR12_39KHZ
    #define STEPPER2_PWM_RATE   MOTOR34_39KHZ
    
#endif
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR4_A 0
#define MOTOR4_B 6
#define MOTOR3_A 5
#define MOTOR3_B 7
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4
#define SINGLE 1
#define DOUBLE 2
#define INTERLEAVE 3
#define MICROSTEP 4
#define MOTORLATCH 12
#define MOTORCLK 4
#define MOTORENABLE 7
#define MOTORDATA 8

class AFMotorController
{
  public:
    AFMotorController(void);
    void enable(void);
    friend class AF_DCMotor;
    void latch_tx(void);
    uint8_t TimerInitalized;
};

class AF_DCMotor
{
 public:
  AF_DCMotor(uint8_t motornum, uint8_t freq = DC_MOTOR_PWM_RATE);
  void run(uint8_t);
  void setSpeed(uint8_t);

 private:
  uint8_t motornum, pwmfreq;
};

class AF_Stepper {
 public:
  AF_Stepper(uint16_t, uint8_t);
  void step(uint16_t steps, uint8_t dir,  uint8_t style = SINGLE);
  void setSpeed(uint16_t);
  uint8_t onestep(uint8_t dir, uint8_t style);
  void release(void);
  uint16_t revsteps;
  uint8_t steppernum;
  uint32_t usperstep, steppingcounter;
 private:
  uint8_t currentstep;

};

uint8_t getlatchstate(void);

#endif
