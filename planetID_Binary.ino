//
// begin license header
//
// ArdCPU
//
// https://docs.pixycam.com/wiki/doku.php?id=wiki:v2:hooking_up_pixy_to_a_microcontroller_-28like_an_arduino-29
//
 
#include <Pixy2.h>
#include <TMRpcm.h>
#include <pcmConfig.h>
#include <pcmRF.h>
#include "SD.h"
#define SD_ChipSelectPin 10
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;

// This is the main Pixy object
Pixy2 pixy;

enum planets {NONE, RED_MARS, ORANGE_VENUS, YELLOW_JUPITER, GREEN_MERCURY, BLUE_NEPTUNE, PURPLE_URANUS, GOLD_SATURN};
planets solarSystem[] = {NONE, RED_MARS, ORANGE_VENUS, YELLOW_JUPITER, GREEN_MERCURY, BLUE_NEPTUNE, PURPLE_URANUS, GOLD_SATURN};
// int binary[] = {000, 001, 010, 011, 100, 101, 110, 111};
int bits[3];

planets identity;
int code;
int center = 160;
int steering = 90;

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");
 
  pinMode(13, OUTPUT);  
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);  

  pinMode(2, OUTPUT); // electromagnet
  pinMode(3, OUTPUT); // servo

  pinMode(5, OUTPUT); // steering
  pinMode(9, OUTPUT); // motor

  pixy.init();
}

void loop()
{
  int i;
  // grab blocks!
  pixy.ccc.getBlocks();
 
  // If there are detect blocks, print them!
 
  if (pixy.ccc.numBlocks && pixy.ccc.blocks[0].m_age>120)
  {
    Serial.print("Detected Sig: ");
    int sig = pixy.ccc.blocks[i].m_signature;
    Serial.println(sig);


    identity = solarSystem[sig];
    Serial.print("Found the planet: ");
    Serial.println(identity);


   // code = binary[sig];
   // Serial.print("Code Number: ");
   // Serial.println(code);


    bits[0] = bitRead(sig, 0);
    bits[1] = bitRead(sig, 1);
    bits[2] = bitRead(sig, 2);


    digitalWrite(13, bits[2]);
    digitalWrite(12, bits[1]);
    digitalWrite(11, bits[0]);


    Serial.print("BitCode: ");
    Serial.print(bits[2]);
    Serial.print(bits[1]);
    Serial.println(bits[0]);


    // steering
    int pos = pixy.ccc.blocks[0].m_x;
    int error = center - pos;
    int dir = map(error, -160, 160, 180, 0);
    steering = steering + dir;
    analogWrite(5, steering);


    // motor
    int size = pixy.ccc.blocks[0].m_width; // width between 0 and 316
    if(size < 275)
    {
      analogWrite(9, 100);
    }
    else
    {
     analogWrite(9, 0);
     // servo and electromagnet
    }
  }
 }


