#include <SPI.h>
#include <Pixy2.h>
#include <PIDLoop.h>
#include <Servo.h>
#include <SPI.h>     // include Arduino SPI library
#include <SD.h>      // include Arduino SD library
#include "TMRpcm.h"  // include TMRpcm library

// OTHER DECLARATIONS
 Servo armservo;

void setup()
{
  //servos
  armservo.attach(9);
 }

void loop()
{  

   ////////////////// TEST BENCH ////////////////
   //MOVES ARM ATTEACED TO PIN 9
   //ATTACH PWR TO 5V; SERVO TO PIN 9; GND TO ARDUINO_GND
   armservo.write(0);
   delay(2000);
    armservo.write(180);
   delay(2000);   
  /*
   //SERVO STEERING ATTACHED TO PIN 6
   //SERVO TO PIN 6;
   SteerControl(0);
   delay(1000);
   SteerControl(90);
   delay(2000);
   SteerControl(0);
   delay(2000);
   */
}

  //******************* ARM ACTUATION FUNCTION ******************//
  

