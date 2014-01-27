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
**   File       : config.h
**   Author     : Sriharsha
**   Website    : www.zuna.in
**   Email      : helpzuna@gmail.com
**   Description: This is the Configuration file for Solar Tracker using Arduino
***/

// To avoid multiple includes
#ifndef __config_h__
#define __config_h__

// Build Configuration
//#define DEBUG
//#define RELEASE

// Includes
#include <Arduino.h>

// Default Time in seconds for GSM Modem Data Logging
#define GSM_TIME_OUT 900

// Define Mobile Number
#define DAQ_SERVER_NO "9342833087"
#define USER_NO       "9342833087"

// Pin Definition for LDR's
#define LDR1 A0
#define LDR2 A1
#define LDR3 A2
#define LDR4 A3


// Pin Definition for Humidity Sensor (HSM-20G)
#define HUMI A4

// Pin Definition for Temperature Sensor (DS1820)
#define TEMP 2

// Pin Definition for LCD (Hitachi 44780)
#define LCD_RS 9
#define LCD_EN 8
#define LCD_D4 10
#define LCD_D5 11
#define LCD_D6 12
#define LCD_D7 13

// Baud Rate definition
#define BAUD   9600

// Temperture Calculations
#define tempMulC         0.488
#define calcDC(val)     (val   * tempMulC)
#define calcDF(val)     (((val * tempMulC) * 1.8) + 32)

// Humidity Calculations
#define calcHum(val)    val

// Light Intensity Calculations
#define calcLight(val)  val

// Returm value for method getTemp if channel number is invalid
#define ChannelInvalid 0xFF

// Temperature units
#define DEGC 0
#define DEGF 1

#endif





