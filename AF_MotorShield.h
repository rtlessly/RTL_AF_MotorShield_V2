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
#include "utility/AF_MS_PWMServoDriver.h"
#include "AF_StepperMotor.h"
#include "AF_DCMotor.h"


class AF_MotorShield
{
    DECLARE_CLASSNAME;

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
