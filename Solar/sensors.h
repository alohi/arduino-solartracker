/***************************************************************************************************************
    This file is part of Solar Tracker using Arduino.

    Solar Tracker using Arduino is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Solar Tracker using Arduino is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Solar Tracker using Arduino.  If not, see <http://www.gnu.org/licenses/>.
/**************************************************************************************************************/



/***
**   File       : sensors.h
**   Author     : Sriharsha
**   Website    : www.zuna.in
**   Email      : helpzuna@gmail.com
**   Description: This is driver include file for sensors
***/

#ifndef __sensors_h__
#define __sensors_h__



// Define a class
class Sensors
{
  public:
  float getTemp(unsigned char);
  float getHumi(void);
  void begin(void);
  unsigned long getLight(unsigned char);
  unsigned long getLux(unsigned int);
  int getCurrent(void);
};

#define adc_to_voltage(val)  val * (5000/255)
#define lm35_temp_conversion(val) adc_to_voltage(val) / 10
#define degreetoFaranheit(val) (val * (9/5)) + 32

#endif
