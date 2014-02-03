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
#include "pcf8591.h"

void Sensors::begin(void)
{
    pcf8591begin();
}

/*** Method      : getTemp
**   Parameters  : unsigned char (Unit -> DEGC,DEGF)
**   Return      : float (Temperature in Degree Celcius, Or in Degree Faranheit)
**   Description : It will read the temperature using DS1820
**/
float Sensors::getTemp(unsigned char tempUnit)
{
int val = pcf8591analogRead(TEMP);
return val;
}

/*** Method      : getHumi
**   Parameters  : None
**   Return      : float (Humidity in % RH)
**   Description : It will read the humidity in % RH
**/
float Sensors::getHumi(void)
{
 int val = pcf8591analogRead(HUMI);
 return val;
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
