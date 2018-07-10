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
#include <RTL_Stdlib.h>
#include "AF_MotorShield.h"
#include "AF_DCMotor.h"


DEFINE_CLASSNAME(AF_DCMotor);


/*******************************************************************************
    DC MOTORS
*******************************************************************************/

AF_DCMotor::AF_DCMotor(void) 
{
    _controller = NULL;
    _motorState.motorNum = 0;
    _motorState.pinPWM = 0;
    _motorState.pin1 = 0;
    _motorState.pin2 = 0;
    _motorState.mode = RELEASE;
}


void AF_DCMotor::Initialize(AF_MotorShield* controller, uint8_t motorNum, uint8_t pinPWM, uint8_t pin1, uint8_t pin2)
{
    _controller = controller;
    _motorState.motorNum = motorNum;
    _motorState.pinPWM = pinPWM;
    _motorState.pin1 = pin1;
    _motorState.pin2 = pin2;
    
    Run(RELEASE);
}


void AF_DCMotor::Run(DCMotorMode cmd) 
{
    uint8_t pin1 = _motorState.pin1;
    uint8_t pin2 = _motorState.pin2;
    
    _motorState.mode = cmd;
    
    switch (cmd) 
    {
        case FORWARD:
            _controller->SetPin(pin2, LOW);  // take low first to avoid 'brake'
            _controller->SetPin(pin1, HIGH);
            break;
      
        case BACKWARD:
            _controller->SetPin(pin1, LOW);  // take low first to avoid 'brake'
            _controller->SetPin(pin2, HIGH);
            break;
      
        case BRAKE:
            _controller->SetPin(pin1, HIGH);
            _controller->SetPin(pin2, HIGH);
            break;
      
        case RELEASE:
            _controller->SetPin(pin1, LOW);
            _controller->SetPin(pin2, LOW);
            break;
    }
}


void AF_DCMotor::Speed(uint8_t speed) 
{
    _motorState.speed = speed;
    _controller->SetPWM(_motorState.pinPWM, speed * 16);
}


