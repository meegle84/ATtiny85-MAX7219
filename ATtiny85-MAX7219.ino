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
#include "fonts.h"

int dx=0;
int dy=0;
unsigned long cnt = 0;
unsigned long start;

// ----------------------------------
void setup() 
{
  Serial.begin (9600);
  initMAX7219();
  sendCmdAll(CMD_SHUTDOWN,1);
  sendCmdAll(CMD_INTENSITY,0);
//  clr();
//  showDigit(2, 0, dig6x8);
//  showDigit(3, 8, dig6x8);
//  showDigit(4, 16, dig6x8);
//  showDigit(5, 24, dig6x8);
//  refreshAll();
//  delay(2000);
//  cnt = random(0,9999) * (long)random(0,9999);
  cnt = 12345678;
  start = millis();
}

// ----------------------------------

byte dig[8]={0};
byte digold[8]={0};
byte digtrans[8]={0};
byte digpos4[4]={1,9,17,25};
byte digpos6[6]={1,6,11,16,21,26};
byte digpos8[8]={0,4,8,12,16,20,24,28};
byte del=0;
byte num;
byte *digpos;
const uint8_t *fnt;
int mode=0;

void loop()
{
  fnt = font;
  printStringWithShift("Porque no funciona?", 64);
}


void counter()
{
  int i;
  if(del==0) {
    long v = cnt;
    for(i=0; i<num; i++) digold[i] = dig[i];
    for(i=0; i<num; i++) {
      dig[num-1-i] = v % 10;
      v /= 10;
    }
    del=12;
    cnt++;
    for(i=0; i<num; i++)  digtrans[i] = (dig[i]==digold[i]) ? 0 : 10;
  } else
    del--;
  
  clr();
  for(i=0; i<num; i++) {
    if(digtrans[i]==0) {
      dy=0;
      showDigit(dig[i], digpos[i], fnt);
    } else {
      dy=10-digtrans[i];
      showDigit(digold[i], digpos[i], fnt);
      dy=-digtrans[i];
      showDigit(dig[i], digpos[i], fnt);
      digtrans[i]--;
    }
  }
  refreshAll();
  delay(40);
}

// ----------------------------------

void showDigit(char ch, int col, const uint8_t *data)
{
  if(dy<-8 | dy>8) return;
  int len = pgm_read_byte(data);
  int w = pgm_read_byte(data + 1 + ch * len);
  col += dx;
  for (int i = 0; i < w; i++)
    if(col+i>=0 && col+i<32) {
      byte v = pgm_read_byte(data + 1 + ch * len + 1 + i);
      if(!dy) scr[col + i] = v; else scr[col + i] |= dy>0 ? v>>dy : v<<-dy;
    }
}

void setCol(int col, byte v)
{
  if(dy<-8 | dy>8) return;
  col += dx;
  if(col>=0 && col<32)
    if(!dy) scr[col] = v; else scr[col] |= dy>0 ? v>>dy : v<<-dy;
}

int showChar(char ch, const uint8_t *data)
{
  int len = pgm_read_byte(data);
  int i,w = pgm_read_byte(data + 1 + ch * len);
  for (i = 0; i < w; i++)
    scr[NUM_MAX*8 + i] = pgm_read_byte(data + 1 + ch * len + 1 + i);
  scr[NUM_MAX*8 + i] = 0;
  return w;
}

// ---------------------------------------------
void printCharWithShift(unsigned char c, int shift_speed) {
  //c = convertPolish(c);
  //if (c < ' ' || c > '~'+22) return;
  c -= 32;
  int w = showChar(c, fnt);
  for (int i=0; i<w+1; i++) {
    delay(shift_speed);
    scrollLeft();
    refreshAll();
  }
}

// ---------------------------------------------
void printStringWithShift(char* s, int shift_speed){
  while (*s) {
    printCharWithShift(*s, shift_speed);
    s++;
  }
}
