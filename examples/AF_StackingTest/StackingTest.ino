/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->   http://www.adafruit.com/products/1438
*/

#include <AF_MotorShield.h>

AF_MotorShield AFMSbot(0x61); // Rightmost jumper closed
AF_MotorShield AFMStop(0x60); // Default address, no jumpers

// On the bottom shield connect a stepper to port M3/M4 with 200 steps
AF_StepperMotor *myStepper1 = AFMSbot.GetStepperMotor(2, 200);

// On the top shield, connect two steppers, each with 200 steps
AF_StepperMotor *myStepper2 = AFMStop.GetStepperMotor(1, 200);
AF_StepperMotor *myStepper3 = AFMStop.GetStepperMotor(2, 200);

// And a DC Motor to port M1
AF_DCMotor *myMotor1 = AFMSbot.GetDCMotor(1);


void setup() 
{
  while (!Serial);
  
  Serial.begin(9600);
  Serial.println("Stacking Test");

  AFMSbot.Begin(); // Start the bottom shield
  AFMStop.Begin(); // Start the top shield
   
  // turn on the DC motor
  myMotor1->Speed(200);
  myMotor1->Run(AF_DCMotor::RELEASE);
  
  // Configure stepper motores
  myStepper1->Mode(AF_StepperMotor::INTERLEAVE);
  myStepper2->Mode(AF_StepperMotor::DOUBLE);
  myStepper3->Mode(AF_StepperMotor::MICROSTEP);
}


int i;


void loop() 
{
  myMotor1->run(AF_DCMotor::FORWARD);
  
  for (i=0; i<255; i++) 
  {
    myMotor1->Speed(i);  
    myStepper1->OneStep(AF_StepperMotor::FORWARD);
    myStepper2->OneStep(AF_StepperMotor::BACKWARD);
    myStepper3->OneStep(AF_StepperMotor::FORWARD);
    delay(3);
  }
 
  for (i=255; i!=0; i--) 
  {
    myMotor1->Speed(i);  
    myStepper1->OneStep(AF_StepperMotor::BACKWARD);
    myStepper2->OneStep(AF_StepperMotor::FORWARD);
    myStepper3->OneStep(AF_StepperMotor::BACKWARD);
    delay(3);
  }
 
  myMotor1->Run(AF_DCMotor::BACKWARD);
  
  for (i=0; i<255; i++) 
  {
    myMotor1->Speed(i);  
    myStepper1->OneStep(AF_StepperMotor::FORWARD);
    myStepper2->OneStep(AF_StepperMotor::BACKWARD);
    myStepper3->OneStep(AF_StepperMotor::FORWARD);
    delay(3);
  }
 
  for (i=255; i!=0; i--) 
  {
    myMotor1->Speed(i);  
    myStepper1->OneStep(AF_StepperMotor::BACKWARD);
    myStepper2->OneStep(AF_StepperMotor::FORWARD);
    myStepper3->OneStep(AF_StepperMotor::BACKWARD);
    delay(3);
  }
}
