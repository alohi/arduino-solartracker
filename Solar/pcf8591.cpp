// PCF8591 Libary
// Modified by Ben Wagner
//
// Based on Original MCP23009 code by
// Code by Adafruit Industries/Limor Fried
// License: LGPL

extern "C"

#include <Wire.h>
#include <avr/pgmspace.h>
#include <WProgram.h>
#include "PCF8591.h"
#include <Arduino.h>

void pcf8591begin(void) 
{
  Wire.begin(PCF8591_ADDRESS);
}

void pcf8591Read(unsigned char *buff)
{
 Wire.beginTransmission(PCF8591_ADDRESS); // wake up PCF8591
 Wire.write(0x04); // control byte - read ADC0 then auto-increment
 Wire.endTransmission(); // end tranmission
 Wire.requestFrom(PCF8591_ADDRESS, 5);
 buff[0]=Wire.read();
 buff[0]=Wire.read();
 buff[1]=Wire.read();
 buff[2]=Wire.read();
 buff[3]=Wire.read();
}


