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
analogWrite(mag_pin, LOW);
armservo.write(0); //arm is at 0deg when no input

Serial.println("arm 0, mag off");
boolean_arm = analogRead(arm_input);

if(boolean_arm > LOW)
{
Serial.println("action activated");
digitalWrite(mag_pin, HIGH); //to be on for a while before moving arm 

ArmActionProtocol(); 
Serial.println("mag on");
//delay(4000); //wait for magnet
}
}

//function to raise arm and keep it there
void ArmActionProtocol(){

//armservo.write(180); //raise arm
//Serial.println("raised arm");

if(boolean_arm > LOW)
ArmActionProtocol(); //recursive function call to keep arm raised if it is still meeting threshold; if not true will return to the loop body
} 