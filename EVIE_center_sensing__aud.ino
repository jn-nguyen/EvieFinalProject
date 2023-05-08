#include <SPI.h>
#include <Pixy2.h>
#include <PIDLoop.h>
#include <Servo.h>
#include <SPI.h>     // include Arduino SPI library
#include <SD.h>      // include Arduino SD library
#include "TMRpcm.h"  // include TMRpcm library



TMRpcm tmrpcm;

Pixy2 pixy;
PIDLoop panLoop(350, 0, 600, true);
PIDLoop tiltLoop(500, 0, 700, true);
PIDLoop turnLoop(300, 600, 300, false);
PIDLoop translateLoop(400, 800, 300, false);

/* ABOVE FROM ZUMO_CHASE; THIS FROM PANTILT (EXAMPLES)
PIDLoop panLoop(400, 0, 400, true);
PIDLoop tiltLoop(500, 0, 500, true);
*/

enum arm {LIFT, RESET};
enum planets {RED_MARS, ORANGE_SATURN, YELLOW_JUPITER, GREEN_EARTH, BLUE_NEPTUNE, PURPLE_URANUS, GOLD_MERCURY};

// PIN ASSIGNMENT FOR PWM
#define motor     3
#define SD_ChipSelectPin 10

//PIN 9 IS FOR THE ARM
//PIN 6 IS FOR STEERING
//PIN4 IS FOR AUDIO
    //available: 10, 11

// OTHER DECLARATIONS
 Servo armservo;
 Servo steerservo;

// TRIAL-DETERMINED CONSTANTS
int pwm_motorFastest = 250;
int pwm_motorJustStop = 30;


//PID constants??
int32_t pixyX = PIXY_RCS_CENTER_POS;
int32_t pixyY =PIXY_RCS_CENTER_POS - PIXY_RCS_CENTER_POS/2 + PIXY_RCS_CENTER_POS/8;

void setup()
{
  //audio
  tmrpcm.speakerPin=5;
  Serial.begin(9600); //IDK WHY THIS IS DIFFERENT BUT I DONT WANT TO FUCK WITH IT - CAN PROB BE CHANGED TO 115200 WITHOUT ISSUE
  if(!SD.begin(SD_ChipSelectPin))
  {
    Serial.println("SD fail");
    return;
  }
  tmrpcm.setVolume(6);
  tmrpcm.play("test.wav");


  //servos
  armservo.attach(9);
  steerservo.attach(6);


  //PAN TILT
  Serial.begin(115200);
  Serial.print("Starting...\n");
 
  //initialize the pixy object 
  pixy.init();
  // Use color connected components program for the pan tilt
  pixy.changeProg("color_connected_components");



 }

void loop()
{  

   ////////////////// TEST BENCH ////////////////

   /*
   // MOTOR GOES VROOM
   // ATTACH MOTOR TO PIN 3; GND TO ARDUINO_GND
   analogWrite(motor, pwm_motorFast);

   */

   //MOVES ARM ATTEACED TO PIN 9
   //ATTACH PWR TO 5V; SERVO TO PIN 9; GND TO ARDUINO_GND
   ArmControl(LIFT);
   delay(2000);
   ArmControl(RESET);
   delay(2000);
   

   //SERVO STEERING ATTACHED TO PIN 6
   //SERVO TO PIN 6;
   SteerControl(0);
   delay(1000);
   SteerControl(90);
   delay(2000);
   SteerControl(0);
   delay(2000);

   
   

  //******************* Movement and Tracking ***********//
  static int16_t index = -1;
  int32_t panOffset, tiltOffset, headingOffset, locomotion;
  Block *block=NULL;
  
  //******************* PAN-TILT++ ******************//
  
  static int i = 0;
  int j;
  char buf[64]; 

  
  // get active blocks from Pixy
  pixy.ccc.getBlocks();

    if (index==-1) // search....
  {
    Serial.println("Searching for block...");
    index = acquireBlock();
    if (index>=0)
      Serial.println("Found block!");
 }

   // If we've found a block, find it, track it
  if (index>=0)
     block = trackBlock(index);

  // If we're able to track it, move motors
  if (block)
  {
    
    // calculate pan and tilt "errors" with respect to first object (blocks[0]), 
    // which is the biggest object (they are sorted by size).  
    panOffset = (int32_t)pixy.frameWidth/2 - (int32_t)pixy.ccc.blocks[0].m_x;
    tiltOffset = (int32_t)pixy.ccc.blocks[0].m_y - (int32_t)pixy.frameHeight/2;  
  
    // update loops
    panLoop.update(panOffset);
    tiltLoop.update(tiltOffset);
  
    // set pan and tilt servos  
    pixy.setServos(panLoop.m_command, tiltLoop.m_command);
    
    // calculate translate and rotate errors
    panOffset += panLoop.m_command - pixyX;
    tiltOffset += tiltLoop.m_command - pixyY;

    turnLoop.update(panOffset);
    translateLoop.update(-tiltOffset);

    // !!THIS IS THE MOST SUS PART
    //MAYBE MOTOR BASED ON TRANSLATE LOOP AND STEERING ON ROTATE?
    
    locomotion = turnLoop.m_command + translateLoop.m_command;
    //steer
    SteerControl(turnLoop.m_command);
    // set wheel velocities
    MotorControl(translateLoop.m_command);

    // print the block we're tracking -- wait until end of loop to reduce latency
    block->print();
  }  
  
  else // no object detected, go into reset state
  {
  turnLoop.reset();
  translateLoop.reset();
  MotorControl(0);
  index = -1; // set search state
  CameraSweep();
  }
}

  //******************* CAMERA SWEEP FUNCTION ******************/
  // If camera doesnt see any objects, looks around to find one
  void CameraSweep()  { //THIS DOESNT ACTUALLY DO MUCH OF ANYTHING LOL

    //reset
    panLoop.reset();
    tiltLoop.reset();
    pixy.setServos(panLoop.m_command, tiltLoop.m_command);
    
    //left ot right sweep

    //top to bottom sweep
    
}

  //******************* ARM ACTUATION FUNCTION ******************//
  
  void ArmControl(arm instr)  {

  if(instr == RESET)
  {
  armservo.write(0); ;
  }

  if(instr == LIFT)
  {
  armservo.write(180); 
  }
    
}


  //******************* MOTOR FUNCTION ******************//
  
  void MotorControl(int speed)  {

  int executedSpeed;

  if(speed > pwm_motorFastest)
  executedSpeed = pwm_motorFastest;

  else if(speed < pwm_motorJustStop)
  executedSpeed = 0;

  else
  executedSpeed = speed;
    
  analogWrite(motor,speed);
    
}

  //******************* STEER FUNCTION ******************//
  
  void SteerControl(int steer)  {
  
  int steerServo; //value between 0 and 180 for writing to servo
  steerServo = map(steer, 0, 255,0,180);
  armservo.write(steerServo);
    
}

//************ Moving and Tracking Helper Functions ***********//

// Take the biggest block (blocks[0]) that's been around for at least 30 frames (1/2 second)
// and return its index, otherwise return -1
int16_t acquireBlock()
{
  if (pixy.ccc.numBlocks && pixy.ccc.blocks[0].m_age>30)
    return pixy.ccc.blocks[0].m_index;

  return -1;
}



// Find the block with the given index.  In other words, find the same object in the current
// frame -- not the biggest object, but he object we've locked onto in acquireBlock()
// If it's not in the current frame, return NULL
Block *trackBlock(uint8_t index)
{
  uint8_t i;

  for (i=0; i<pixy.ccc.numBlocks; i++)
  {
    if (index==pixy.ccc.blocks[i].m_index)
      return &pixy.ccc.blocks[i];
  }

  return NULL;
}
