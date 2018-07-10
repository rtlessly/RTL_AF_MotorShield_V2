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
#ifndef _AF_DCMotor2_h_
#define _AF_DCMotor2_h_

#include <inttypes.h>
#include <RTL_Stdlib.h>
#include <IDCMotor2.h>


class AF_MotorShield2;


class AF_DCMotor2 : public IDCMotor2
{
    DECLARE_CLASSNAME;

    friend class AF_MotorShield2;

    /*--------------------------------------------------------------------------
    Constructors
    --------------------------------------------------------------------------*/

    //**************************************************************************
    /// Creates the motor instance and attaches it to the specified motor controller
    /// with the specified motor number.
    //**************************************************************************
    public: AF_DCMotor2(AF_MotorShield2& controller, uint8_t motorID);

    //**************************************************************************
    /// Default constructor.
    /// Creates a motor instance that is unattached to a controller. Use the 
    /// AF_MotorShield2::Attach() method to attach this motor to a controller.
    //**************************************************************************
    public: AF_DCMotor2(void);

    
    /*--------------------------------------------------------------------------
    Public methods
    --------------------------------------------------------------------------*/

    //**************************************************************************
    /// Runs the motor at the specified speed. 
    /// 
    /// Parameter: speed
    /// The speed to run the motor. The speed is constrained to be in the range
    /// +/- 255, where +255 is max forward speed, -255 is max backward speed, 
    /// and 0 is stopped.
    /// 
    /// Note: Other than the fact that they should be opposite directions, the 
    /// concept of "forward" and "backward" is implementation dependent, i.e., 
    /// the implementation can decide which direction is "forward" or "backward" 
    /// by wiring the motor appropriately.
    //**************************************************************************
    public: void Run(int16_t speed);

    
    /*--------------------------------------------------------------------------
    Public properties
    --------------------------------------------------------------------------*/

    //**************************************************************************
    /// Gets the motor ID (0 to 3).
    //**************************************************************************
    public: uint16_t GetID() { return _motorState.motorID; };

    //**************************************************************************
    /// Gets the current speed of the motor. 
    ///
    /// Returns: 
    /// The currently set speed of the motor, in the range +/- 255, where +255 
    /// is max forward speed, -255 is max backward speed, and 0 is stopped.
    //**************************************************************************
    public: int16_t GetSpeed() { return _speed; };

    //**************************************************************************
    /// Indicates if the motor is attached to a controller. 
    ///
    /// Returns: 
    /// True if the motor is attached; otherwise, false is returned.
    //**************************************************************************
    public: bool IsAttached() { return _controller != NULL; }

    //**************************************************************************
    /// Determines if the motor is attached to a specific controller. 
    ///
    /// Returns: 
    /// True if the motor is attached to the specified controller; otherwise, 
    /// false is returned.
    //**************************************************************************
    public: bool IsAttachedTo(AF_MotorShield2* controller) { return _controller == controller; }
    
    
    /*--------------------------------------------------------------------------
    Internal methods
    --------------------------------------------------------------------------*/

    //**************************************************************************
    /// The AF_MotorShields class calls this method to configure the motor.
    //**************************************************************************
    private: void Configure(AF_MotorShield2* controller, uint8_t motorID, uint8_t pinPWM, uint8_t pin1, uint8_t pin2);

    
    /*--------------------------------------------------------------------------
    Internal state
    --------------------------------------------------------------------------*/
    private: struct
    {
        uint16_t motorID : 2;   // The motor number (0 - 3)
        uint16_t pinPWM  : 4;   // The PWM pin for the motor
        uint16_t pin1    : 4;   // Motor pin 1
        uint16_t pin2    : 4;   // Motor pin 2
    }
    _motorState;

    private: int16_t _speed;    // Current speed of the motor

    private: AF_MotorShield2* _controller;
};

#endif
