/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->   http://www.adafruit.com/products/1438
*/

#include <Arduino.h>
#include <AF_MotorShield.h>


// Create the motor shield object with the default I2C address
AF_MotorShield AFMS; 

// Or, create it with a different I2C address (say for stacking)
// AF_MotorShield AFMS = AF_MotorShield(0x61); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree) to both motor ports
AF_StepperMotor *myMotor1 = AFMS.GetStepperMotor(1, 200);
AF_StepperMotor *myMotor2 = AFMS.GetStepperMotor(2, 200);


void setup() 
{
  Serial.begin(115200);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");

  AFMS.Begin();  // create with the default frequency 1.6KHz
  //AFMS.Begin(1000);  // OR with a different frequency, say 1KHz
  
  myMotor1->Speed(100);  // 100 rpm   
  myMotor2->Speed(200);  // 200 rpm   
}


void loop() 
{
  Serial.println("**** Single coil steps ****");
  Serial.println("Motor 1: 100 steps");
  myMotor1->step(100, SINGLE); 
  myMotor1->step(100, SINGLE); 
  Serial.println("Motor 2: 100 steps");
  myMotor2->step(100, SINGLE); 
  myMotor2->step(100, SINGLE); 

  Serial.println("**** Double coil steps ****");
  Serial.println("Motor 1: 100 steps");
  myMotor1->step(100, DOUBLE); 
  myMotor1->step(100, DOUBLE);
  Serial.println("Motor 2: 100 steps");
  myMotor2->step(100, DOUBLE); 
  myMotor2->step(100, DOUBLE);

  Serial.println("**** Interleave coil steps (half-stepping) ****");
  Serial.println("Motor 1: 100 steps");
  myMotor1->step(100, INTERLEAVE); 
  myMotor1->step(100, INTERLEAVE); 
  Serial.println("Motor 2: 100 steps");
  myMotor2->step(100, INTERLEAVE); 
  myMotor2->step(100, INTERLEAVE); 

  Serial.println("**** Microstep steps ****");
  Serial.println("Motor 1: +/-50 steps");
  myMotor1->step(50, MICROSTEP); 
  myMotor1->step(-50, MICROSTEP);
  Serial.println("Motor 2: +/-50 steps");
  myMotor2->step(50, MICROSTEP); 
  myMotor2->step(-50, MICROSTEP);
}
