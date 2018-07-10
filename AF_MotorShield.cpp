/******************************************************************
 This library is for the Adafruit Motor Shield V2 for Arduino. It
 is adapted from the Adafruit library for the Motor Shield V2. 
 The library supports DC motors & Stepper motors with micro-stepping 
 as well as stacking-support. 

 It will only work with Adafruit Motor Shield V2. 
 See https://www.adafruit.com/products/1483
 
 The original Adafruit library was written by Limor Fried/Ladyada for
 Adafruit Industries. BSD license, check AdafruitLicense.txt for more 
 information.

Original Copyright (c) 2012, Adafruit Industries.  All rights reserved.
 
 This adaptation was written by R. Terry Lessly 2016-11-07.
 ******************************************************************/
#define DEBUG 0

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
#include <Wire.h>
#include <RTL_Stdlib.h>
#include "AF_MotorShield.h"


#if defined(ARDUINO_SAM_DUE)
 #define WIRE Wire1
#else
 #define WIRE Wire
#endif


DEFINE_CLASSNAME(AF_MotorShield);


AF_MotorShield::AF_MotorShield(uint8_t addr) 
{
    _addr = addr;
    _pwm = AF_MS_PWMServoDriver(_addr);
}


void AF_MotorShield::Begin(uint16_t freq) 
{
    // initialize PWM w/_freq
    WIRE.begin();
    _pwm.begin();
    _freq = freq;
    _pwm.setPWMFreq(_freq);  // This is the maximum PWM frequency

    for (uint8_t i=0; i < 16; i++)  _pwm.setPWM(i, 0, 0);

    // Initialize DC motors
    _dcMotors[0].Initialize(this, 0,  8, 10,  9);
    _dcMotors[1].Initialize(this, 1, 13, 11, 12);
    _dcMotors[2].Initialize(this, 2,  2,  4,  3);
    _dcMotors[3].Initialize(this, 3,  7,  5,  6);

    // Initialize stepper motors
    _stepperMotors[0].Initialize(this, 0, 8, 10, 9, 13, 11, 12);
    _stepperMotors[1].Initialize(this, 1, 2,  4, 3,  7,  5,  6);
}


void AF_MotorShield::SetPWM(uint8_t pin, uint16_t value) 
{
    if (value > 4095) 
        _pwm.setPWM(pin, 4096, 0);
    else 
        _pwm.setPWM(pin, 0, value);
}


void AF_MotorShield::SetPin(uint8_t pin, boolean value) 
{
    if (value == LOW)
        _pwm.setPWM(pin, 0, 0);
    else
        _pwm.setPWM(pin, 4096, 0);
}


AF_DCMotor *AF_MotorShield::GetDCMotor(uint8_t motorNum) 
{
    if (motorNum >= 4) return NULL;  // Only allow motor numbers 0, 1, 2, and 3
 
    return &_dcMotors[motorNum];
}


AF_StepperMotor *AF_MotorShield::GetStepperMotor(uint8_t motorNum, uint16_t steps) 
{
    if (motorNum >= 2) return NULL;  // Only allow motor numbers 0 and 1

    _stepperMotors[motorNum]._stepsPerRev = steps;
  
    return &_stepperMotors[motorNum];
}
