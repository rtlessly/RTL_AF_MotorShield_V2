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

#include <Arduino.h>
#include <Wire.h>
#include <RTL_Stdlib.h>
#include "AF_MotorShield2.h"


#if defined(ARDUINO_SAM_DUE)
 #define WIRE Wire1
#else
 #define WIRE Wire
#endif


// Motor port configurations for DC and stepper motors
static const uint8_t DCMOTOR_0 = 0b00000001;    // DC motor 0 uses port 0
static const uint8_t DCMOTOR_1 = 0b00000010;    // DC motor 1 uses port 1
static const uint8_t DCMOTOR_2 = 0b00000100;    // DC motor 2 uses port 2
static const uint8_t DCMOTOR_3 = 0b00001000;    // DC motor 3 uses port 3
static const uint8_t STEPPER_0 = 0b00000011;    // Stepper motor 0 uses ports 0 & 1
static const uint8_t STEPPER_1 = 0b00001100;    // Stepper motor 1 uses ports 2 & 3

DEFINE_CLASSNAME(AF_MotorShield2);


AF_MotorShield2::AF_MotorShield2(uint8_t addr) 
{
    _addr = addr;
    _pwm = AF_MS_PWMServoDriver(_addr);
}


void AF_MotorShield2::Begin(uint16_t freq) 
{
    // initialize PWM w/_freq
    WIRE.begin();
    _pwm.begin();
    _freq = freq;
    _pwm.setPWMFreq(_freq);  // This is the maximum PWM frequency

    for (uint8_t i=0; i < 16; i++)  _pwm.setPWM(i, 0, 0);

    // // Initialize DC motors
    // _dcMotors[0].Initialize(this, 0,  8, 10,  9);
    // _dcMotors[1].Initialize(this, 1, 13, 11, 12);
    // _dcMotors[2].Initialize(this, 2,  2,  4,  3);
    // _dcMotors[3].Initialize(this, 3,  7,  5,  6);

    // // Initialize stepper motors
    // _stepperMotors[0].Initialize(this, 0, 8, 10, 9, 13, 11, 12);
    // _stepperMotors[1].Initialize(this, 1, 2,  4, 3,  7,  5,  6);
}


bool AF_MotorShield2::Attach(AF_DCMotor2& motor, uint8_t motorID)
{
    uint8_t portMask = (0x01 << motorID);       // Set motor port allocation bit
    
    if (motorID > 3) return false;              // Only allow motor numbers 0 - 3
    if (motor.IsAttached()) return false;       // Only if motor is not already attached
    if (_ports & portMask) return false;        // Only if motor port not already in use
    
    _ports |= portMask;                         // Mark motor port as allocated
    
    switch (motorID)                            // Configure motor for port
    {
        case 0:
            motor.Configure(this, 0,  8, 10,  9);
            break;

        case 1:
            motor.Configure(this, 1, 13, 11, 12);
            break;

        case 2:
            motor.Configure(this, 2,  2,  4,  3);
            break;

        case 3:
            motor.Configure(this, 3,  7,  5,  6);
            break;
    }
    
    return true;
}


void AF_MotorShield2::Detach(AF_DCMotor2& motor)
{
    if (!motor.IsAttachedTo(this)) return;          // If motor is not attached to this controller then exit
    
    uint8_t portMask = ~(0x01 << motor.GetID());    // Reset motor port allocation bit

    motor.Configure(NULL, 0,  0,  0,  0);           // De-configure motor
    _ports &= portMask;                             // Free port in port allocation map
}


void AF_MotorShield2::SetPWM(uint8_t pin, uint16_t value) 
{
    if (value > 4095) 
        _pwm.setPWM(pin, 4096, 0);
    else 
        _pwm.setPWM(pin, 0, value);
}


void AF_MotorShield2::SetPin(uint8_t pin, boolean value) 
{
    if (value == LOW)
        _pwm.setPWM(pin, 0, 0);
    else
        _pwm.setPWM(pin, 4096, 0);
}

