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
#ifndef _AF_DCMotor_h_
#define _AF_DCMotor_h_

#include <inttypes.h>
#include <IDCMotor.h>


class AF_MotorShield;


class AF_DCMotor : public IDCMotor
{
    DECLARE_CLASSNAME;

    //**************************************************************************
    /// Default constructor.
    /// The constructor is private so that only the AF_MotorShield class can
    /// create instances (since it is a friend class).
    /// Call the Initialize() method to perform the necessary motor configuration.
    //**************************************************************************
    private: AF_DCMotor(void);

    /*--------------------------------------------------------------------------
    Public interface
    --------------------------------------------------------------------------*/

    //**************************************************************************
    /// Runs the motor in the specified mode. The commanded mode can be one of
    /// the DCMotorMode enum values (FORWARD, BACKWARD, BRAKE, or RELEASE).
    //**************************************************************************
    public: void Run(DCMotorMode command);

    /*--------------------------------------------------------------------------
    Properties
    --------------------------------------------------------------------------*/

    //**************************************************************************
    /// Gets the configured motor number (0 - 3).
    //**************************************************************************
    public: uint8_t MotorNum() { return _motorState.motorNum; };

    //**************************************************************************
    /// Gets the motor ID (motor number).
    //**************************************************************************
    public: uint16_t ID() { return _motorState.motorNum; };

    //**************************************************************************
    /// Gets or sets the speed of the motor. The speed is 0-255.
    //**************************************************************************
    public: uint8_t Speed() { return _motorState.speed; };
    public: void    Speed(uint8_t speed);

    //**************************************************************************
    /// Gets the current motor mode.
    /// Returns one of the DCMotorMode enum values (FORWARD, BACKWARD, BRAKE, or RELEASE).
    //**************************************************************************
    public: DCMotorMode Mode() { return (DCMotorMode)_motorState.mode; };

    /*--------------------------------------------------------------------------
    Internal implementation
    --------------------------------------------------------------------------*/

    //**************************************************************************
    /// Initializes the motor. The AF_MotorShield class calls this method to
    /// configure the motor.
    //**************************************************************************
    private: void Initialize(AF_MotorShield* controller, uint8_t motorNum, uint8_t pinPWM, uint8_t pin1, uint8_t pin2);

    /*--------------------------------------------------------------------------
    Internal state
    --------------------------------------------------------------------------*/
    private: struct
    {
        uint16_t motorNum : 2;  // The motor number (0 - 3)
        uint16_t pinPWM   : 4;  // The PWM pin for the motor
        uint16_t pin1     : 4;  // Motor pin 1
        uint16_t pin2     : 4;  // Motor pin 2
        uint16_t mode     : 2;  // Current motor mode (one of the DCMotorMode enum values)
        uint16_t speed    : 8;  // Current speed of the motor
    }
    _motorState;

    private: AF_MotorShield* _controller;

    friend class AF_MotorShield;
};

#endif
