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
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>
#include <Debug.h>
#include "AF_MotorShield.h"
#include <AF_MS_PWMServoDriver.h>

#if defined(ARDUINO_SAM_DUE)
 #define WIRE Wire1
#else
 #define WIRE Wire
#endif

static DebugHelper Debug("AF_MotorShield");


AF_MotorShield::AF_MotorShield(uint8_t addr) 
{
    _addr = addr;
    _pwm = AF_MS_PWMServoDriver(_addr);
}


void AF_MotorShield::Begin(uint16_t freq) 
{
    // initialize PWM w/_freq
    WIRE.begin();
    _pwm.begin();
    _freq = freq;
    _pwm.setPWMFreq(_freq);  // This is the maximum PWM frequency

    for (uint8_t i=0; i < 16; i++)  _pwm.setPWM(i, 0, 0);

    // Initialize DC motors
    _dcMotors[0].Initialize(this, 0,  8, 10,  9);
    _dcMotors[1].Initialize(this, 1, 13, 11, 12);
    _dcMotors[2].Initialize(this, 2,  2,  4,  3);
    _dcMotors[3].Initialize(this, 3,  7,  5,  6);

    // Initialize stepper motors
    _stepperMotors[0].Initialize(this, 0, 8, 10, 9, 13, 11, 12);
    _stepperMotors[1].Initialize(this, 1, 2,  4, 3,  7,  5,  6);
}


void AF_MotorShield::SetPWM(uint8_t pin, uint16_t value) 
{
    if (value > 4095) 
        _pwm.setPWM(pin, 4096, 0);
    else 
        _pwm.setPWM(pin, 0, value);
}


void AF_MotorShield::SetPin(uint8_t pin, boolean value) 
{
    if (value == LOW)
        _pwm.setPWM(pin, 0, 0);
    else
        _pwm.setPWM(pin, 4096, 0);
}


AF_DCMotor *AF_MotorShield::GetDCMotor(uint8_t motorNum) 
{
    if (motorNum >= 4) return NULL;  // Only allow motor numbers 0, 1, 2, and 3
 
    return &_dcMotors[motorNum];
}


AF_StepperMotor *AF_MotorShield::GetStepperMotor(uint8_t motorNum, uint16_t steps) 
{
    if (motorNum >= 2) return NULL;  // Only allow motor numbers 0 and 1

    _stepperMotors[motorNum]._stepsPerRev = steps;
  
    return &_stepperMotors[motorNum];
}


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
    Debug.Log("Run => motor=%i, steps=%i, mode=%i", _motorState.motorNum, steps, mode);

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
            Debug.Log("OneStep_Double => motor=%i, dir=%1", _motorState.motorNum, dir);

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
            Debug.Log("OneStep_Double => motor=%i, dir=%1", _motorState.motorNum, dir);

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
            Debug.Log("OneStep_Interleave => motor=%i, dir=%1", _motorState.motorNum, dir);

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
            Debug.Log("OneStep_Microstep => motor=%i, dir=%1", _motorState.motorNum, dir);
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
            
            Debug.Log("OneStep_Microstep => _currentStep=%l, phase=%i, microStep=%i", _currentStep, phase, microStep);

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

            Debug.Log("OneStep_Microstep => PWMA=%i, PWMB=%i, latchState=%i", pwmA, pwmB, latchState);

            break;            
    }
    
    _controller->SetPWM(_motorState.pinPWMA, pwmA*16);
    _controller->SetPWM(_motorState.pinPWMB, pwmB*16);
    _controller->SetPin(_motorState.pinA2, (latchState & 0x1)  ? HIGH : LOW);
    _controller->SetPin(_motorState.pinB1, (latchState & 0x2)  ? HIGH : LOW);
    _controller->SetPin(_motorState.pinA1, (latchState & 0x4)  ? HIGH : LOW);
    _controller->SetPin(_motorState.pinB2, (latchState & 0x8)  ? HIGH : LOW);
}

