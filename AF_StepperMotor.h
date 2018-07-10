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
#ifndef _AF_StepperMotor_h_
#define _AF_StepperMotor_h_

#include <inttypes.h>
#include <IStepperMotor.h>


class AF_MotorShield;


#define MICROSTEPS 8         // 8 or 16

class AF_StepperMotor : public IStepperMotor
{
    DECLARE_CLASSNAME;

    //**************************************************************************
    /// Default constructor.
    /// The constructor is private so that only the AF_MotorShield class can
    /// create instances (since it is a friend class).
    /// Call the Initialize() method to perform the necessary motor configuration.
    //**************************************************************************
    private: AF_StepperMotor(void);

    /*--------------------------------------------------------------------------
    Public methods
    --------------------------------------------------------------------------*/

    //**************************************************************************
    /// Runs the motor for the specified number of steps, at the specified RPM,
    //  and in the specified mode. The default mode, if not specified, is SINGLE
    /// (single-coil). If the steps parameter is negative, the direction is BACKWARD,
    /// otherwise it is FORWARD.
    ///
    /// NOTE: This is a blocking call - it will not return until the run has
    ///       completed. In the meantime, your Arduino will be doing nothing else.
    ///       This method is provided primarily for simple testing of stepper
    ///       motors. The StepeprMotorController class provides an interface for
    ///       more robust, non-blocking control of the motor.
    ///
    /// NOTE: The number of steps is always full motor steps for SINGLE, DOUBLE,
    ///       and MICROSTEP modes, and half-steps for INTERLEAVE mode.
    //**************************************************************************
    public: void Run(int32_t steps, MotorMode mode = SINGLE, uint16_t speed=0);

    //**************************************************************************
    /// Advances the motor exactly one step in the specified direction.
    //**************************************************************************
    public: void OneStep(int dir);

    //**************************************************************************
    /// Releases the motor by setting all drive lines to LOW. In this configuration
    /// the motor will free-run.
    //**************************************************************************
    public: void Release(void);

    /*--------------------------------------------------------------------------
    Properties
    --------------------------------------------------------------------------*/

    //**************************************************************************
    /// Gets the motor ID (motor number).
    //**************************************************************************
    public: uint16_t ID() { return _motorState.motorNum; };

    //**************************************************************************
    /// Gets the number of steps per one motor revolution.
    //**************************************************************************
    public: uint16_t StepsPerRev() { return _stepsPerRev; };

    //**************************************************************************
    /// Gets or sets the motor mode.
    //**************************************************************************
    public: MotorMode Mode() { return (MotorMode)_motorState.mode; };
    public: void Mode(MotorMode mode);

    //**************************************************************************
    /// Gets or sets the speed of the motor in RPM.
    //**************************************************************************
    public: uint16_t Speed();
    public: void Speed(uint16_t rpm);

    /*--------------------------------------------------------------------------
    Internal implementation
    --------------------------------------------------------------------------*/

    //**************************************************************************
    /// Initializes the motor. The AF_MotorShield class calls this method to
    /// configure the motor.
    //**************************************************************************
    private: void Initialize(AF_MotorShield* controller, uint8_t motorNum, uint8_t pinPWMA, uint8_t pinA1, uint8_t pinA2, uint8_t pinPWMB, uint8_t pinB1, uint8_t pinB2);

    /*--------------------------------------------------------------------------
    Internal state
    --------------------------------------------------------------------------*/

    private: struct
    {
        uint16_t motorNum :  2; // The motor number (0 - 1)
        uint16_t mode     :  2; // The motor mode
        uint16_t pinPWMA  :  4; // The PWM pin for the 'A' coil of the motor
        uint16_t pinA1    :  4; // Motor 'A' coil pin 1
        uint16_t pinA2    :  4; // Motor 'A' coil pin 2
        uint16_t pinPWMB  :  4; // The PWM pin for the 'B' coil of the motor
        uint16_t pinB1    :  4; // Motor 'B' coil pin 1
        uint16_t pinB2    :  4; // Motor 'B' coil pin 2
    }
    _motorState;

    private: uint8_t  _currentStep;  // current step number
    private: uint16_t _stepsPerRev;  // Number of steps per motor revolution
    private: uint32_t _usPerStep;    // microseconds per step

    private: AF_MotorShield* _controller;

    friend class AF_MotorShield;
};

#endif
