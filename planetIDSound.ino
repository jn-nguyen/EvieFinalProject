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

enum planets {RED_MARS, ORANGE_VENUS, GREEN_MERCURY, YELLOW_JUPITER, GOLD_SATURN, PURPLE_URANUS, BLUE_NEPTUNE};

#define SD_ChipSelectPin 10

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");
  
  pixy.init();
}

void loop()
{ 
  int i; 
  // grab blocks!
  pixy.ccc.getBlocks();
  
  // If there are detect blocks, print them!
  if (pixy.ccc.numBlocks)
  {
    Serial.print("Detected ");

    int sig = pixy.ccc.blocks[i].m_signature;
    // planets planet;


    Serial.println(sig);

    if(sig == RED_MARS) //  mars
    {
      tmrpcm.speakerPin=9;

      if(!SD.begin(SD_ChipSelectPin))
      {
        Serial.println("SD fail");
        return;
      }

      tmrpcm.setVolume(5);
      tmrpcm.play("MARS.wav");

    }
    
    if(sig == ORANGE_VENUS)
    {

    }

    if(sig == GREEN_MERCURY)
    {

    }    

    if(sig == YELLOW_JUPITER)
    {

    }

    if(sig == GOLD_SATURN)
    {

    }

    if(sig == PURPLE_URANUS)
    {

    }

    if(sig == BLUE_NEPTUNE)
    {

    }

  }  
}

