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

// Humidity Look Up Table
/*const int   HumidArrayinRH[20]  = {10,20,30,40,50,60,70,80,90};
const float HumidArrayinV[20]   = {0.74,0.95,1.31,1.68,2.02,2.37,2.69,2.99,3.19};*/

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
float temp;
temp = lm35_temp_conversion(val);
if(tempUnit == DEGC)
return temp;
else
return degreetoFaranheit(val);
}

/*** Method      : getHumi
**   Parameters  : None
**   Return      : float (Humidity in % RH)
**   Description : It will read the humidity in % RH
**/
float Sensors::getHumi(void)
{
 int val = pcf8591analogRead(HUMI);
 int humidRH;
 int voltage = adc_to_voltage(val);
 if(voltage >= 740 && voltage <= 950)
 {
	 humidRH = map(voltage,740,950,10,20);
	 return humidRH;
 }
 else if(voltage >= 950 && voltage <= 1310)
  {
	  humidRH = map(voltage,950,1310,20,30);
	  return humidRH;
  }
   else if(voltage >= 950 && voltage <= 1310)
   {
	   humidRH = map(voltage,950,1310,20,30);
	   return humidRH;
   }
    else if(voltage >= 1310 && voltage <= 1680)
    {
	    humidRH = map(voltage,1310,1680,30,40);
	    return humidRH;
    }
	 else if(voltage >= 1680 && voltage <= 2020)
	 {
		 humidRH = map(voltage,1680,2020,40,50);
		 return humidRH;
	 }
	 	 else if(voltage >= 2020 && voltage <= 2370)
	 	 {
		 	 humidRH = map(voltage,2020,2370,50,60);
		 	 return humidRH;
	 	 }
		  	 else if(voltage >= 2370 && voltage <= 2690)
		  	 {
			  	 humidRH = map(voltage,2370,2690,60,70);
			  	 return humidRH;
		  	 }
			   		  	 else if(voltage >= 2690 && voltage <= 2990)
			   		  	 {
				   		  	 humidRH = map(voltage,2690,2990,70,80);
				   		  	 return humidRH;
			   		  	 }
							  		  	 else if(voltage >= 2990 && voltage <= 3190)
							  		  	 {
								  		  	 humidRH = map(voltage,2370,2690,80,90);
								  		  	 return humidRH;
							  		  	 }
											 else
											 return 0;
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

int Sensors::getCurrent(void)
{
//	 int val = adc_to_voltage(pcf8591analogRead(CUR));
	 return pcf8591analogRead(CUR);
}
