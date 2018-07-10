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
#include <RTL_Stdlib.h>
#include "utility/AF_MS_PWMServoDriver.h"
//#include "AF_StepperMotor2.h"
#include "AF_DCMotor2.h"


class AF_MotorShield2
{
    DECLARE_CLASSNAME;

    // Declare the motor classes as friends so they can access the SetPin() and SetPWM() methods
    friend class AF_DCMotor2;
    friend class AF_StepperMotor2;

    /*--------------------------------------------------------------------------
    Constructors
    --------------------------------------------------------------------------*/

    //**************************************************************************
    /// Constructor.
    /// The addr parameter must match the I2C address of the board you want to
    /// control. The default value is 0x60, which is the default (factory) board
    /// address. If the board address has been altered (by soldering a bridge across
    /// any combination of the address jumper pads on the board) then this parameter
    /// must match the new address of the board.
    //**************************************************************************
    public: AF_MotorShield2(uint8_t addr = 0x60);

    
    /*--------------------------------------------------------------------------
    Public methods
    --------------------------------------------------------------------------*/

    //**************************************************************************
    /// Initializes the MotorShield.
    /// The freq parameter sets the PWM frequency for the board. If not specified,
    /// it defaults to 1600 Hz.
    //**************************************************************************
    public: void Begin(uint16_t freq = 1600);

    //**************************************************************************
    /// Attaches a DC motor to the MotorShield.
    //**************************************************************************
    public: bool Attach(AF_DCMotor2& motor, uint8_t motorID);

    //**************************************************************************
    /// Detaches a DC motor from the MotorShield.
    //**************************************************************************
    public: void Detach(AF_DCMotor2& motor);

    //**************************************************************************
    /// Attach a stepper motor to one of 2 stepper motor ports on the MotorShield.
    //**************************************************************************
    //public: bool Attach(AF_StepperMotor2& motor, uint8_t motorNum, uint16_t steps);

    
    /*--------------------------------------------------------------------------
    Internal methods
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
    private: uint8_t  _addr;            // I2C address
    private: uint8_t  _ports;           // Allocation bits for 4 motor ports (uses 4 LS bits)
    private: uint16_t _freq;            // PWM frequency
    private: AF_MS_PWMServoDriver _pwm; // Helper class for PWM
};

#endif
