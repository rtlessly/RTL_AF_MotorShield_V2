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
 
 This adaptation was written by R. Terry Lessly 2018-05-04.
 ******************************************************************/
#define DEBUG 0

#include <Arduino.h>
#include <RTL_Stdlib.h>
#include <RTL_Math.h>
#include "AF_MotorShield2.h"
#include "AF_DCMotor2.h"


DEFINE_CLASSNAME(AF_DCMotor2);


AF_DCMotor2::AF_DCMotor2(AF_MotorShield2& controller, uint8_t motorID) 
{
    controller.Attach(*this, motorID);
}


AF_DCMotor2::AF_DCMotor2(void) 
{
    Configure(NULL, 0, 0, 0, 0);
}


void AF_DCMotor2::Configure(AF_MotorShield2* controller, uint8_t motorID, uint8_t pinPWM, uint8_t pin1, uint8_t pin2)
{
    _controller = controller;
    _motorState.motorID = motorID;
    _motorState.pinPWM = pinPWM;
    _motorState.pin1 = pin1;
    _motorState.pin2 = pin2;
    _speed = 0;
}


void AF_DCMotor2::Run(int16_t speed) 
{
    uint8_t pin1 = _motorState.pin1;
    uint8_t pin2 = _motorState.pin2;

    speed = constrain(speed, -255, 255);

    // If direction changed then reconfigure motor
    if (SIGN(_speed) != SIGN(speed))
    {
        if (speed > 0)                       // Going forward
        {
            _controller->SetPin(pin2, LOW);  // take pin 2 low first to avoid 'brake'
            _controller->SetPin(pin1, HIGH);
        }
        else if (speed < 0)                  // Going backward
        {
            _controller->SetPin(pin1, LOW);  // take pin 1 low first to avoid 'brake'
            _controller->SetPin(pin2, HIGH);
        }
        else                                 // Stopped
        {
            _controller->SetPin(pin1, LOW);  // Take both pins low to disable motor
            _controller->SetPin(pin2, LOW);
        }
    }
    
    // Finally, set the motor speed
    _speed = speed;
    _controller->SetPWM(_motorState.pinPWM, abs(_speed) * 16);   // convert speed to range 0-4096 for PWM
}
