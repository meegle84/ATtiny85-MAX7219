/*
 Quick LedMatrix example by Pawel A. Hernik
 
 ARDUINO PINOUT:
 Pin 13 - DataIn
 Pin 12 - CLK
 Pin 11 - LOAD/CS
 
 DIGISPARK ATTINY85 PINOUT:
 Pin 2 - DataIn
 Pin 1 - CLK
 Pin 0 - LOAD/CS
*/

#define NUM_MAX 2

#include "Arduino.h"

#if defined( __AVR_ATtinyX5__ )
  #define DIN_PIN 2
  #define CS_PIN  1
  #define CLK_PIN 0
#else
  #define DIN_PIN 13
  #define CS_PIN  12
  #define CLK_PIN 11
#endif

#include "max7219.h"

unsigned long start;

// ----------------------------------
void setup() 
{
  Serial.begin (9600);
  initMAX7219();
  sendCmdAll(CMD_SHUTDOWN,1);
  sendCmdAll(CMD_INTENSITY,0);
  start = millis();
}
// ----------------------------------
int mode=0;

void loop()
{
  printStringWithShift("funciona?", 64);
}
