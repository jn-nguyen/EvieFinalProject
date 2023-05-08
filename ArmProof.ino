//ArdArm Code
//This unit is the arduino uno used to actuate the servo of the arm
//The functions of this unit can be combined with Ard_IdArd which is the unit that identifies and audio outputs based on the camera identification


#include <SPI.h>
#include <Pixy2.h>
#include <PIDLoop.h>
#include <Servo.h>
#include <SPI.h>     // include Arduino SPI library
#include <SD.h>      // include Arduino SD library
#include "TMRpcm.h"  // include TMRpcm library


#define arm_input  A0
#define arm_pin 6
#define mag_pin 7

Servo armservo;
int boolean_arm;

void setup(){
  armservo.attach(arm_pin);
  Serial.begin(115200);
  Serial.print("Starting...\n");

}

void loop(){

//if(readanalog(input_pin)
armservo.write(0); //arm is at 0deg when no input
Serial.println("lowered arm");
delay(2000); //wait for magnet to be on for a while before moving arm 
armservo.write(180); //raise arm
Serial.println("raised arm");
delay(2000);
} 