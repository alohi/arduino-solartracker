/***************************************************************************************************************
    This file is part of Solar Tracker using Arduino.

    Solar Tracker using Arduino is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Library for 8051 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Solar Tracker using Arduino.  If not, see <http://www.gnu.org/licenses/>.
/**************************************************************************************************************/



/***
**   File       : sensors.cpp
**   Author     : Sriharsha
**   Website    : www.zuna.in
**   Email      : helpzuna@gmail.com
**   Description: This file contains sensor function for Solar Tracker using Arduino
***/

#include "config.h"
#include "sensors.h"
#include "OneWire.h"

// An instnce of DS1820
OneWire ds(TEMP);

// Globals for 
byte addr[8];

/*** Method      : beginTemp
**   Parameters  : None
**   Return      : None
**   Description : It will initiate the DS1820
**/
void Sensors::beginTemp(void)
{
  ds.search(addr);
  ds.reset_search();
}

/*** Method      : getTemp
**   Parameters  : unsigned char (Unit -> DEGC,DEGF)
**   Return      : float (Temperature in Degree Celcius, Or in Degree Faranheit)
**   Description : It will read the temperature using DS1820
**/
float Sensors::getTemp(unsigned char tempUnit)
{
  byte i;
  byte data[12];
  float celsius, fahrenheit;
  
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
// Delay 750ms removed
  
  ds.reset();
  ds.select(addr);    
  ds.write(0xBE);        
  for ( i = 0; i < 9; i++)
  {          
    data[i] = ds.read();
  }

    unsigned int raw = (data[1] << 8) | data[0];

    raw = raw << 3; 
    if (data[7] == 0x10) 
    {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }

  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  if(tempUnit == DEGC)
  return celsius;
  else
  return fahrenheit;
}

/*** Method      : getHumi
**   Parameters  : None
**   Return      : float (Humidity in % RH)
**   Description : It will read the humidity in % RH
**/
float Sensors::getHumi(void)
{
 return calcHum(analogRead(HUMI));
}

unsigned long Sensors::getLux(unsigned int avg)
{
//
}

/*** Method      : getLight
**   Parameters  : unsigned char (Channel : 1,2,3,4)
**   Return      : float (Light Intensity in lux)
**   Description : It will read the LDR in lux
**/
unsigned long Sensors::getLight(unsigned char Ch)
{
 switch(Ch)
 {
   case 1 : return calcLight(analogRead(LDR1));
            break;
   case 2 : return calcLight(analogRead(LDR2));
            break;
   case 3 : return calcLight(analogRead(LDR3));
            break;
   case 4 : return calcLight(analogRead(LDR4));
            break;
   default: return ChannelInvalid;
            break;
 }
}
