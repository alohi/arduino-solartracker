// PCF8591 Libary
// Modified by Ben Wagner
//
// Based on Original MCP23009 code by
// Code by Adafruit Industries/Limor Fried
// License: LGPL

#include <Wire.h>
#include <avr/pgmspace.h>
#include <WProgram.h>
#include "PCF8591.h"
#include <Arduino.h>

void pcf8591beginWithAdd(uint8_t addr)
{
/* if (addr > 7) {
    addr = 7;
  }*/
  Wire.begin();

  //Send registers (defaults)
  //1st Byte
  Wire.beginTransmission(PCF8591_ADDRESS);
  //2nd Byte
  Wire.write(0x00); //No registers set....  Don't want anything special
  Wire.endTransmission();
}

void pcf8591begin(void) 
{
  pcf8591beginWithAdd(0);
}

int pcf8591analogRead(uint8_t p) 
{
  volatile uint8_t retValue=0;
  // read the current GPIO
  Wire.beginTransmission(PCF8591_ADDRESS);
  Wire.write(p);
  Wire.endTransmission();        // stop transmitting
  delayMicroseconds(100);
  Wire.requestFrom(PCF8591_ADDRESS, 1);
while(Wire.available())
{
	retValue = Wire.read();
}


  Wire.endTransmission();
  return (retValue);
}


