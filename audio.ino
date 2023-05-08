#include <TMRpcm.h>
#include <pcmConfig.h>
#include <pcmRF.h>

#include "SD.h"
#define SD_ChipSelectPin 10
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;

// int bits[3];

int currBitZero;
int currBitOne;
int currBitTwo;

int prevBitZero = 0;
int prevBitOne = 0;
int prevBitTwo = 0;

long int count = 0;

bool same = false;

void setup()
{
  tmrpcm.speakerPin=9;

  Serial.begin(115200);
  Serial.print("Starting...\n");
  if(!SD.begin(SD_ChipSelectPin))
  {
    Serial.println("SD fail");
    return;
  }
  tmrpcm.setVolume(5);
}

void loop() {

  // put your main code here, to run repeatedly:
  currBitTwo = digitalRead(7);
  currBitOne = digitalRead(6);
  currBitZero = digitalRead(5);
  
  // update counter
  count = count + 1;

    Serial.print("Previous BitCode: ");
    Serial.print(prevBitTwo);
    Serial.print(prevBitOne);
    Serial.println(prevBitZero);
    Serial.print("Current BitCode: ");
    Serial.print(currBitTwo);
    Serial.print(currBitOne);
    Serial.println(currBitZero);
    

  if(currBitTwo == prevBitTwo && currBitOne == prevBitOne && currBitZero == prevBitZero)
  {
      same = true;
  }
  else if(currBitTwo != prevBitTwo || currBitOne != prevBitOne || currBitZero != prevBitZero)
  {
    same = false;
  }

  Serial.print("same: ");
  Serial.println(same);


  if(same == 0 && currBitTwo == 0 && currBitOne == 0 && currBitZero == 1)
  {
    Serial.println("playing Mars, The Bringer of War");
    tmrpcm.play("MARS.wav");  
    delay(5000);
  }
  else if(same == 0 && currBitTwo == 0 && currBitOne == 1 && currBitZero == 0)
  {
    Serial.println("playing Venus, The Bringer of Peace");
    tmrpcm.play("VENUS.wav"); 
    delay(5000);
  }
  else if(same == 0 && currBitTwo == 0 && currBitOne == 1 && currBitZero == 1)
  {
    Serial.println("playing Jupiter, The Bringer of Jollity");    
    tmrpcm.play("JUPITER.wav");
    delay(5000);
  }
  else if(same == 0 && currBitTwo == 1 && currBitOne == 0 && currBitZero == 0)
  {
    Serial.println("playing Mercury, The Winged Messenger");
    tmrpcm.play("MERCURY.wav");
    delay(5000);
  }
  else if(same == 0 && currBitTwo == 1 && currBitOne == 0 && currBitZero == 1)
  {
    Serial.println("playing Neptune, The Mystic");
    tmrpcm.play("NEPTUNE.wav");
    delay(5000);
  }
  else if(same == 0 && currBitTwo == 1 && currBitOne == 1 && currBitZero == 0)
  {
    Serial.println("playing Uranus, The Magician");
    tmrpcm.play("URANUS.wav");
    delay(5000);
  }
  else if(same == 0 && currBitTwo == 1 && currBitOne == 1 && currBitZero == 1)
  {
    Serial.println("playing Saturn, The Bringer of Old Age");
    tmrpcm.play("SATURN.wav");
    delay(5000);
  }
  
  prevBitTwo = currBitTwo;
  prevBitOne = currBitOne;
  prevBitZero = currBitZero;
  
}
