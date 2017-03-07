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
#ifndef _AF_MotorShield_h_
#define _AF_MotorShield_h_

#include <inttypes.h>
#include <Wire.h>
#include <IDCMotor.h>
#include <IStepperMotor.h>
#include "utility/AF_MS_PWMServoDriver.h"


class AF_MotorShield;


class AF_DCMotor : public IDCMotor
{
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



#define MICROSTEPS 8         // 8 or 16

class AF_StepperMotor : public IStepperMotor
{
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


class AF_MotorShield
{
    //**************************************************************************
    /// Constructor.
    /// The addr parameter must match the I2C address of the board you want to
    /// control. The default value is 0x60, which is the default (factory) board
    /// address. If the board address has been altered (by soldering a bridge across
    /// any combination of the address jumper pads on the board) then this parameter
    /// must match the new address of the board.
    //**************************************************************************
    public: AF_MotorShield(uint8_t addr = 0x60);

    /*--------------------------------------------------------------------------
    Public interface
    --------------------------------------------------------------------------*/

    //**************************************************************************
    /// Initializes the MotorShield.
    /// The freq parameter sets the PWM frequency for the board. If not specified,
    /// it defaults to 1600 Hz.
    //**************************************************************************
    public: void Begin(uint16_t freq = 1600);

    //**************************************************************************
    /// Factory method to get one of four DC motors attached to the MotorShield.
    //**************************************************************************
    public: AF_DCMotor* GetDCMotor(uint8_t motorNum);

    //**************************************************************************
    /// Factory method to get one of two stepper motors attached to the MotorShield.
    //**************************************************************************
    public: AF_StepperMotor* GetStepperMotor(uint8_t motorNum, uint16_t steps);

    /*--------------------------------------------------------------------------
    Internal implementation
    --------------------------------------------------------------------------*/

    //**************************************************************************
    /// Internal method to set a value on a PWM pin.
    //**************************************************************************
    private: void SetPWM(uint8_t pin, uint16_t value);

    //**************************************************************************
    /// Internal method to set a value on a digital pin.
    //**************************************************************************
    private: void SetPin(uint8_t pin, boolean value);

    /*--------------------------------------------------------------------------
    Internal state
    --------------------------------------------------------------------------*/
    private: uint8_t  _addr;
    private: uint16_t _freq;
    private: AF_MS_PWMServoDriver _pwm;
    private: AF_DCMotor _dcMotors[4];
    private: AF_StepperMotor _stepperMotors[2];

    // Declare the motor classes as friends so they can access the SetPin() and SetPWM() methods
    friend class AF_DCMotor;
    friend class AF_StepperMotor;
};

#endif
