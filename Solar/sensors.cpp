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
unsigned char buff[10];
unsigned int Avg = 0;
unsigned int i;
int val;
float temp;
for(i=0;i<=100;i++)
{
pcf8591Read(buff);
//val = (int)buff[TEMP];
Avg = Avg + (int)buff[TEMP];
}
val = Avg / 100;
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
 int val;
 int humidRH;
 int voltage;
 unsigned int Avg = 0;
 unsigned int i;
 unsigned char buff[10];
 float temp;
 
 for(i=0;i<=HUMI_SAMPLE_VAL;i++)
 {
 pcf8591Read(buff);
 Avg = Avg + (int)buff[HUMI];
// val = (int)buff[HUMI];
 }
 val = Avg / HUMI_SAMPLE_VAL;
 voltage = adc_to_voltage(val);
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
 unsigned long ldr = 0;
 unsigned char i;
 switch(Ch)
 {
   case 1 : for(i=0;i<LDR_DATA_SAMPLES;i++)
            ldr = ldr + calcLight(analogRead(LDR1));
	    return ldr/LDR_DATA_SAMPLES;
            break;
   case 2 : for(i=0;i<LDR_DATA_SAMPLES;i++)
            ldr = ldr + calcLight(analogRead(LDR2));
            return ldr/LDR_DATA_SAMPLES;
            break;
   case 3 : for(i=0;i<LDR_DATA_SAMPLES;i++)
            ldr = ldr + calcLight(analogRead(LDR3));
            return ldr/LDR_DATA_SAMPLES;
            break;
   case 4 : for(i=0;i<LDR_DATA_SAMPLES;i++)
            ldr = ldr + calcLight(analogRead(LDR4));
            return ldr/LDR_DATA_SAMPLES;
            break;
   default: return ChannelInvalid;
            break;
 }
}

int Sensors::getCurrent(void)
{
 int val;
 int cur;
 int v;
 unsigned int i;
 unsigned int Avg = 0;
 unsigned char buff[10];
 
 for(i=0;i<=CURRENT_SAMPLE_VAL;i++)
 {
 pcf8591Read(buff);
 Avg = Avg + (int)buff[CUR];
 }
 val = Avg / CURRENT_SAMPLE_VAL;
 v = adc_to_voltage(val);
 cur = map(v,2432,3200,0,1100);
 return cur;
}

unsigned int Sensors::getVoltage(void)
{
 int v;
 int val;
 unsigned char buff[10];
 unsigned int Avg = 0;
  unsigned int i;
  
 for(i=0;i<=VOLTAGE_SAMPLE;i++)
 {
 pcf8591Read(buff);
 Avg = Avg + (int)buff[VOL];
 }
 val = Avg / VOLTAGE_SAMPLE;
 v = adc_to_voltage(val);  
 return v * SOLAR_MUL_FACTOR;
}




