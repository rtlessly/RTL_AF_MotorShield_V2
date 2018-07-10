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
#include "AF_StepperMotor.h"


DEFINE_CLASSNAME(AF_StepperMotor);


/*******************************************************************************
    STEPPER MOTORS
*******************************************************************************/

AF_StepperMotor::AF_StepperMotor(void) 
{
    _motorState.motorNum = 0;
    _stepsPerRev = 0;
    _currentStep = 0;
}


void AF_StepperMotor::Initialize(AF_MotorShield* controller, uint8_t motorNum, uint8_t pinPWMA, uint8_t pinA1, uint8_t pinA2, uint8_t pinPWMB, uint8_t pinB1, uint8_t pinB2)
{
    _controller = controller;
    _motorState.motorNum = motorNum;
    
    _motorState.pinPWMA = pinPWMA; 
    _motorState.pinA1 = pinA1; 
    _motorState.pinA2 = pinA2;
    
    _motorState.pinPWMB = pinPWMB; 
    _motorState.pinB1 = pinB1; 
    _motorState.pinB2 = pinB2; 
    
    Release();
}


void AF_StepperMotor::Mode(MotorMode mode) 
{ 
    _motorState.mode = mode; 
}


void AF_StepperMotor::Speed(uint16_t rpm) 
{
    _usPerStep = 60000000 / ((uint32_t)_stepsPerRev * (uint32_t)rpm);
}


uint16_t AF_StepperMotor::Speed() 
{ 
    uint16_t rpm = 60000000 / ((uint32_t)_stepsPerRev * (uint32_t)_usPerStep); 
    
    return rpm;
}

void AF_StepperMotor::Release(void) 
{
    _controller->SetPWM(_motorState.pinPWMA, 0);
    _controller->SetPin(_motorState.pinA1, LOW);
    _controller->SetPin(_motorState.pinA2, LOW);

    _controller->SetPWM(_motorState.pinPWMB, 0);
    _controller->SetPin(_motorState.pinB1, LOW);
    _controller->SetPin(_motorState.pinB2, LOW);
}


void AF_StepperMotor::Run(int32_t steps, MotorMode mode, uint16_t speed) 
{
    TRACE(Logger(_classname_, __func__, this) << F("[") << _motorState.motorNum 
                                              << F("] steps=") << steps 
	                                          << F(", mode=") << mode 
                                              << endl);

    if (speed > 0) Speed(speed);
    
    Mode(mode);
    
    MotorDirection dir = (steps > 0) ? FORWARD : BACKWARD;
    uint32_t stepInterval = _usPerStep;

    steps = abs(steps);
    
    if (mode == INTERLEAVE) 
    {
        // In interleave mode the motor runs in half-steps, so the step interval
        // has to be halved to maintain the desired RPM.
        // NOTE: The step count will be for half-steps instead of full steps, so
        //       the motor will run at the correct RPM, but only travel half as far.
        stepInterval /= 2;
    }
    else if (mode == MICROSTEP) 
    {
        // Micro-stepping mode takes multiple micro-steps to complete a full step, 
        // so adjust step count and step interval appropriately.
        // NOTE: Here we adjust both the number of steps and the step interval, so
        //       the motor will run at the correct RPM and travel the full distance.
        //       HOWEVER, at even moderate RPMs, the micro-step interval may become 
        //       shorter than the step loop time, so the RPM is effectively limited
        //       by how fast the CPU can complete the step loop.
        stepInterval /= MICROSTEPS;
        steps *= MICROSTEPS;
    }
        
    while (steps--) 
    {
        OneStep(dir);
        delayMicroseconds(stepInterval);
    }
}


#if (MICROSTEPS == 8)
static uint8_t microstepcurve[] = {0,     50,     98,      142,      180,      212,      236,      250,      255};
#elif (MICROSTEPS == 16)
static uint8_t microstepcurve[] = {0, 25, 50, 74, 98, 120, 141, 162, 180, 197, 212, 225, 236, 244, 250, 253, 255};
#endif


void AF_StepperMotor::OneStep(int dir) 
{
    uint8_t latchState = 0;
    uint8_t pwmA = 255;
    uint8_t pwmB = 255;

    switch(_motorState.mode)
    {
        case SINGLE:
            //OneStep_Single(dir);
            TRACE(Logger(_classname_, __func__, this) << "[" << _motorState.motorNum << F("] dir=") << dir << endl);

            // Increment/decrement step number, but constrain to 0-3. 
            // If dir=+1 then it steps 0-1-2-3 order, if dir=-1 then it steps 3-2-1-0 order
            _currentStep = ((_currentStep + dir) + 4) % 4;

            switch (_currentStep) 
            {
                case 0:
                    latchState |= 0x3; // energize coil 1+2
                    break;
                    
                case 1:
                    latchState |= 0x6; // energize coil 2+3
                    break;

                case 2:
                    latchState |= 0xC; // energize coil 3+4
                    break;
            
                case 3:
                    latchState |= 0x9; // energize coil 1+4
                    break;
            }

            break;
            
        case DOUBLE:
            //OneStep_Double(dir);
            TRACE(Logger(_classname_, __func__, this) << "[" << _motorState.motorNum << F("] dir=") << dir << endl);

            // Increment/decrement step number, but constrain to 0-3. 
            // If dir=+1 then it steps 0-1-2-3 order, if dir=-1 then it steps 3-2-1-0 order
            _currentStep = ((_currentStep + dir) + 4) % 4;    

            switch (_currentStep) 
            {
                case 0:
                    latchState |= 0x3; // energize coil 1+2
                    break;
                    
                case 1:
                    latchState |= 0x6; // energize coil 2+3
                    break;

                case 2:
                    latchState |= 0xC; // energize coil 3+4
                    break;
            
                case 3:
                    latchState |= 0x9; // energize coil 1+4
                    break;
            }

            break;
            
        case INTERLEAVE:
            //OneStep_Interleave(dir);
            TRACE(Logger(_classname_, __func__, this) << "[" << _motorState.motorNum << F("] dir=") << dir << endl);

            // Increment/decrement step number, but constrain to 0-7. 
            // If dir=+1 then it steps 0-1-2-3-4-5-6-7 order, if dir=-1 then it steps 7-6-5-4-3-2-1-0 order
            _currentStep = ((_currentStep + dir) + 8) % 8;

            switch (_currentStep) 
            {
                case 0:
                    latchState |= 0x1; // energize coil 1 only
                    break;
                    
                case 1:
                    latchState |= 0x3; // energize coil 1+2
                    break;
                    
                case 2:
                    latchState |= 0x2; // energize coil 2 only
                    break;
                    
                case 3:
                    latchState |= 0x6; // energize coil 2+3
                    break;
                    
                case 4:
                    latchState |= 0x4; // energize coil 3 only
                    break; 
                    
                case 5:
                    latchState |= 0xC; // energize coil 3+4
                    break;
                    
                case 6:
                    latchState |= 0x8; // energize coil 4 only
                    break;
            
                case 7:
                    latchState |= 0x9; // energize coil 1+4
                    break;
            }
            
            break;
            
        case MICROSTEP:
            //OneStep_Microstep(dir);
            TRACE(Logger(_classname_, __func__, this) << F("[") << _motorState.motorNum << F("] dir=") << dir << endl);

            // There are 16 (or 8) micro-steps per full motor step, and there are 4 steps 
            // (or phases) to complete a full cycle. The 4 phases are:
            //      Phase 0: coil A decreasing +, coil B increasing +
            //      Phase 1: coil A increasing -, coil B decreasing +
            //      Phase 2: coil A decreasing -, coil B increasing -
            //      Phase 3: coil A increasing +, coil B decreasing -
            //
            // To keep track of both the phase and micro-step with a single variable,
            // _currentStep, we count micro-steps in a full cycle, so there are 16*4 
            // (or 8*4) micro-steps per cycle.
            _currentStep = ((_currentStep + dir) + (MICROSTEPS*4)) % (MICROSTEPS*4);
            
            uint8_t phase = _currentStep / MICROSTEPS;
            uint8_t microStep = _currentStep % MICROSTEPS;
            
            TRACE(Logger(_classname_, __func__, this) << F("[") << _motorState.motorNum << F("] _currentStep=") << _currentStep 
			                                          << F(", phase=") << phase << F(", microStep=") << microStep << endl);

            switch (phase)
            {
                case 0:
                    pwmA = microstepcurve[MICROSTEPS - microStep];
                    pwmB = microstepcurve[microStep];
                    latchState |= 0x03;
                    break;
                    
                case 1:
                    pwmA = microstepcurve[microStep];
                    pwmB = microstepcurve[MICROSTEPS - microStep];
                    latchState |= 0x06;
                    break;
                
                case 2:
                    pwmA = microstepcurve[MICROSTEPS - microStep];
                    pwmB = microstepcurve[microStep];
                    latchState |= 0x0C;
                    break;
                
                case 3:
                    pwmA = microstepcurve[microStep];
                    pwmB = microstepcurve[MICROSTEPS - microStep];
                    latchState |= 0x09;
                    break;
            }

            TRACE(Logger(_classname_, __func__, this) << F("[") << _motorState.motorNum << F("] PWMA=") << pwmA << F(", PWMB=") 
			                                          << pwmB << F(", latchState=") << latchState << endl);

            break;            
    }
    
    _controller->SetPWM(_motorState.pinPWMA, pwmA*16);
    _controller->SetPWM(_motorState.pinPWMB, pwmB*16);
    _controller->SetPin(_motorState.pinA2, (latchState & 0x1)  ? HIGH : LOW);
    _controller->SetPin(_motorState.pinB1, (latchState & 0x2)  ? HIGH : LOW);
    _controller->SetPin(_motorState.pinA1, (latchState & 0x4)  ? HIGH : LOW);
    _controller->SetPin(_motorState.pinB2, (latchState & 0x8)  ? HIGH : LOW);
}

