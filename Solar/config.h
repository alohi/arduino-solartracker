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
#define DEBUG
//#define RELEASE

// Includes
#include <Arduino.h>

// Default Time in minutes for GSM Modem Data Logging
#define DATA_LOG_SMS_INTERVAL 15

#define EN1  7
#define EN2  8

// Define Mobile Number
#define DAQ_SERVER_NO "9980524441"


#define USER_NO       "9342833087"

#define STEP_DELAY__   3000

// Pin Definition for LDR's
#define LDR1 A0
#define LDR2 A1
#define LDR3 A2
#define LDR4 A3

#define LDR_THRESHOLD1 +40
#define LDR_THRESHOLD2 -40

// External ADC
///////////////////////////////////////////////////////////////////////////////////////
																					 //
// Pin Definition for Humidity Sensor (HSM-20G)                                      //
#define HUMI  1 // Error fixed                                                       //                         
                                                                                     //
// Pin Definition for Temperature Sensor (DS1820)                                    //
#define TEMP  0   // Error fixed                                                     //                        
                                                                                     //
// Voltage and current input for ADC (For Solar Power Calculations)                  //
#define CUR   2                                                                      //
#define VOL   3                                                                      //
///////////////////////////////////////////////////////////////////////////////////////

// Pin definition for Stepper Motor Control      
#define STEPPER_INA_1 2
#define STEPPER_INA_2 3
#define STEPPER_INB_1 4
#define STEPPER_INB_2 5
#define STEPPER_EN1   6
#define STEPPER_EN2   7

#define STEPEER_OFF_TIME 18
#define STEPPER_ON_TIME  6

// Stepper Motor Speed in RPM
#define STEPPER_SPEED_RPM 20                                
#define STEPER_STEPS      200
#define STEPPER_STEPS     200

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

// Return value for method getTemp if channel number is invalid
#define ChannelInvalid 0xFF

// Hardware timer initial time in micro seconds
#define Timeus 1000

// Delay for startup status indication for each test
#define StartUpDelay 2000

// 
#define STARTUP_ALERT_SMS 1
// 0 -> Disable, 1 -> To Daq server, 2 -> To user, 3 -> Both daq server and user

// Alert type
#define ALERT_TYPE        0
//0 -> Call to user when system fails, 1 -> Send sms to user, 2 -> Send Sms to DAQ server, 3 -> Send sms to both user and DAQ server

#define DATA_LOG_MODE     0
//0 -> Send Sms to DAQ server,  2 -> Send sms to user, 3 -> Send sms to both user and DAQ server

extern volatile unsigned int ss;

// Temperature units
#define DEGC 0
#define DEGF 1


// Voltage Mul factor
#define SOLAR_MUL_FACTOR 4

#define NIGHT_SAVE_MODE 
//#define SENSOR_ALERT
#define DATA_LOGGING

// Upper and lower limits
#define HUMID_UPPER_LIMIT      70
#define HUMID_LOWER_LIMIT      20
#define TEMPR_UPPER_LIMIT      100
#define TEMPR_LOWER_LIMIT      10

#define LCD_INITIAL_SLOW_DELAY 300

#ifdef DEBUG
#define _DEBUG_UART_PRINT_DELAY_ 500
#endif

#endif





