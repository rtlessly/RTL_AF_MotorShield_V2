/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/


#include <Wire.h>
#include <AF_MotorShield.h>

// Create the motor shield object with the default I2C address
AF_MotorShield AFMS = AF_MotorShield(); 

// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
AF_StepperMotor *myMotor1 = AFMS.GetStepperMotor(0, 200);
AF_StepperMotor *myMotor2 = AFMS.GetStepperMotor(1, 200);


void setup() 
{
  Serial.begin(115200);
  Serial.println("Stepper test!");

  AFMS.Begin();  // create with the default frequency 1.6KHz
  //AFMS.Begin(1000);  // OR with a different frequency, say 1KHz
  
  myMotor1->Speed(100);  // rpm   
  myMotor2->Speed(200);  // rpm   
}


void loop() 
{
  Serial.println("**** Single coil steps ****");
  Serial.println("Motor 1");
  myMotor1->Run(400, AF_StepperMotor::SINGLE); 
  //myMotor1->Run(-400, AF_StepperMotor::SINGLE); 
  
  Serial.println("Motor 2");
  myMotor2->Run(400, AF_StepperMotor::SINGLE); 
  //myMotor2->Run(-400, AF_StepperMotor::SINGLE); 

  Serial.println("**** Double coil steps ****");
  Serial.println("Motor 1");
  //myMotor1->Run(400, AF_StepperMotor::DOUBLE); 
  myMotor1->Run(-400, AF_StepperMotor::DOUBLE);
  
  Serial.println("Motor 2");
  //myMotor2->Run(400, AF_StepperMotor::DOUBLE); 
  myMotor2->Run(-400, AF_StepperMotor::DOUBLE);

  Serial.println("**** Interleave coil steps (half-stepping) ****");
  Serial.println("Motor 1");
  myMotor1->Run(400, AF_StepperMotor::INTERLEAVE); 
  //myMotor1->Run(-400, AF_StepperMotor::INTERLEAVE); 
  
  Serial.println("Motor 2");
  myMotor2->Run(400, AF_StepperMotor::INTERLEAVE); 
  //myMotor2->Run(-400, AF_StepperMotor::INTERLEAVE); 

  Serial.println("**** Micro-stepping ****");
  Serial.println("Motor 1");
  //myMotor1->Run(400, AF_StepperMotor::MICROSTEP); 
  myMotor1->Run(-400, AF_StepperMotor::MICROSTEP);
  
  Serial.println("Motor 2");
  //myMotor2->Run(400, AF_StepperMotor::MICROSTEP); 
  myMotor2->Run(-400, AF_StepperMotor::MICROSTEP);
}


