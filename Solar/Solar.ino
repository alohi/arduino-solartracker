#include "gsmModem.h"
#include "sensors.h"
#include "config.h"
#include "TimerOne.h"
#include "appmsg.h"
#include <LiquidCrystal.h>
#include <Stepper.h>
#include <Wire.h>
#include "pcf8591.h"
#include "RTClib.h"
#include <Stepper.h>


unsigned char TempWarning = 0;
unsigned char HumidWarning = 0;

// Globals for Timer Calculation
volatile unsigned int Tms = 0;
volatile unsigned int Tss = 0;

unsigned char prevAlert = 0;

unsigned int angle = ANGLE_START_VAL;
unsigned char RunningMode = 0; 
/* 0 -> Day Mode
   1 -> Night Mode
   2 -> No sunlight
   3 -> Power Save
   4 -> Min/Max Save
   */
boolean displayClearFlag = false;
unsigned char rtcstringlen = 0;
boolean stepperRunReq = false;
boolean stepperDisable = false;

unsigned char rtcDisplayVal = 0;
/* 0 -> Date 
   1 -> Day Of Week
   2 -> Time
*/

unsigned char DisplayVal = 0;
/*
0 -> Temperature
1 -> Humidity
2 -> LDR Difference
3 -> Voltage
4 -> Current
5 -> Power
6 -> Running Mode (Night or Day)
7 -> Next Data Log Interval
*/

// Uncomment Below line once to set RTC time
// Note : Initially you have to uncomment and burn the program, then comment and reburn program
#define ADJUST_RTC

#ifdef ADJUST_RTC
#define _DATE "Apr 18 2014"
#define _TIME "12:44:00"
#endif

// Timer ISR
void timer1Isr(void)          // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  Tms++;
  if(Tms >= 1000)
  {
    Tms = 0;
    Tss++;
  }
}

// An instance of a LiquidCrystal class (Object)
LiquidCrystal lcd(LCD_RS,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7);
// An instance of gsm modem class
gsmModem myModem;
// An instance of Sensors class
Sensors mySensors;
// An instance for stepper motor
Stepper myStepper(STEPPER_STEPS,STEPPER_INA_1,STEPPER_INA_2,STEPPER_INB_1,STEPPER_INB_2);
// An instance of rtc
RTC_DS1307 rtc;



// Enable Motor
void enableMotor(void)
{
  #ifdef FORCE_DISABLE_MOTOR
  digitalWrite(STEPPER_EN1,LOW);
  digitalWrite(STEPPER_EN2,LOW);
  return;
  #endif
  digitalWrite(STEPPER_EN1,HIGH);
  digitalWrite(STEPPER_EN2,HIGH);
}

// Disable Motor
void disableMotor(void)
{
  digitalWrite(STEPPER_EN1,LOW);
  digitalWrite(STEPPER_EN2,LOW);
}

void testMotor(void)
{
	char c;
	enableMotor();
	if(Serial.available())
	{
		c = Serial.read();
		switch(c)
		{
			case 'a' : 	myStepper.step(1);
Serial.println("Motor Clock wise");
			break;
			case 'b' : 	myStepper.step(-1);
Serial.println("Motor anticlock wise");
			break;			
		}
	}

	
}



// Setup executes once on start up
void setup(void)
{ 
  // LDR Pin Configuration
  pinMode(LDR1,INPUT);
  pinMode(LDR2,INPUT);
  pinMode(LDR3,INPUT);
  pinMode(LDR4,INPUT);
  
  // Set Port Directions
  pinMode(EN1,OUTPUT);
  pinMode(EN2,OUTPUT);
  
  // Disable Motor
  digitalWrite(EN1,LOW);
  digitalWrite(EN2,LOW);
  
  // UART Initialization
  Serial.begin(BAUD);
  // LCD Initialization
  lcd.begin(16,2);
  // Sensors Initialization
  mySensors.begin();
  // TwoWire Initialization
  Wire.begin();
  // RTC DS1307 Initialization
  rtc.begin();
  
   if (! rtc.isrunning()) 
   {
	   #ifdef DEBUG
	   Serial.println("RTC is NOT running!");
	   #endif
	   	     rtc.adjust(DateTime(_DATE,_TIME));
   }
   else

   #ifdef DEBUG
   Serial.println("RTC is running");
  #endif
  // Adjust Date and time
  #ifdef ADJUST_RTC
  //"Apr 17 2014", time = "19:19:00"
  rtc.adjust(DateTime(_DATE,_TIME));
  #endif
    rtc.adjust(DateTime("Apr 18 2014","12:00:00"));
  // Initiate LCD cursor
  lcd.setCursor(0,0);
  
  // Stepper Motor speed settings
  myStepper.setSpeed(60);
   
  // Timer 1 Initialization
  Timer1.initialize(Timeus);
  Timer1.attachInterrupt(timer1Isr);
}

void testMotorCont(void)
{
	int i;
	enableMotor();
	while(1)
	{
		for(i=0;i<160;i++)
		{
			myStepper.step(1);
			delay(30);
		}
		/*		for(i=0;i<160;i++)
				{
					myStepper.step(-1);
					delay(30);
				}*/
		
	}
}

void loop(void)
{
	
// Sensor Values
float _humi,_ldr1,_ldr2,_ldr3,_ldr4,_temp;
// LDR Average (1 - 2, 3 - 4)
float _LDR1,_LDR2;
// LDR Difference (LDR2 - LDR1)
float _LDR;
// Solar Current (mA)
int   current;
// Solar Voltage (mV)
int   Voltage;
// Solar Power (mW)
int power;
// Alert Status
unsigned char Status;

// Global DateTime
unsigned char MM;  // Month (1 - 12)
unsigned char DD;  // Date
unsigned int  YY;  // Year
unsigned char DOF; // Day Of Week (1-7)
unsigned char HH;  // Hour
unsigned char MINU;  // Minute
unsigned char SS;  // Second

// Local DateTime
unsigned char mm;  // Month (1 - 12)
unsigned char dd;  // Date 
unsigned int  yy;  // Year 
unsigned char dof; // Day Of Week (1-7)
unsigned char hh;  // Hour
unsigned char minu;  // Minute
unsigned char ss;  // Second

// hour and minute for data logging
unsigned char dataLogHH;
unsigned char dataLogMM;

// LCD Update Time Value
unsigned char lSS;

// Seconds counter for stepper motor rotation
unsigned char stepperStopWatch = 0;


DateTime now = rtc.now();
while(1)
testMotor();

// Store Global DateTime
MM  = now.month();         // Month
DD  = now.day();           // Day
YY  = now.year();          // Year
DOF = now.dayOfWeek();     // Day Of Week
HH  = now.hour();          // Hour
MINU  = now.minute();        // Minute
SS  = now.second();        // Second


// Generate warnings
#ifndef NIGHT_SAVE_MODE
#warning "Night Save mode is disabled"
#endif

#ifndef SENSOR_ALERT
#warning "Sensor alert is disabled"
#endif

#ifndef DATA_LOGGING
#warning "Data Logging is disabled"
#endif

// Calculate next predicted interval for data logging
#ifdef DATA_LOGGING
if(MINU >= (60 - DATA_LOG_SMS_INTERVAL))
{
	dataLogMM = (MINU + DATA_LOG_SMS_INTERVAL) - 60;
	if(HH == 23)
	dataLogHH = 0;
	else
	dataLogHH = HH + 1;
}
else
{
	dataLogMM = MINU + DATA_LOG_SMS_INTERVAL;
	dataLogHH = HH;
}
#endif

// Boottest will not enabled if DEBUG macro is defined
#ifndef DEBUG
bootTest();
#endif

Timer1.detachInterrupt();
Timer1.stop();

enableMotor();

/*dataLogHH = 12;
dataLogMM = 00;*/

// Infinite loop (It will run continuosly)
while(1)
{
// Read All the datas
_humi   = mySensors.getHumi();
_temp   = mySensors.getTemp(DEGC);
_ldr1   = mySensors.getLight(1);
_ldr2   = mySensors.getLight(2);
_ldr3   = mySensors.getLight(3);
_ldr4   = mySensors.getLight(4);
current = mySensors.getCurrent();
Voltage = mySensors.getVoltage();
power = current * Voltage;

DateTime now = rtc.now();

while(1)
{
	testMotor();
}

// GET RTC Date and time
mm  = now.month();         // Month
dd  = now.day();           // Day
yy  = now.year();          // Year
dof = now.dayOfWeek();     // Day Of Week
hh  = now.hour();          // Hour
minu  = now.minute();        // Minute
ss  = now.second();        // Second

// Read LDR
_LDR1 = (_ldr2 + _ldr1) / 2;
_LDR2 = (_ldr4 + _ldr3) / 2;
// Calculate Difference
_LDR = _LDR1 - _LDR2;

lSS = ss;


/*/////////////////////////////////////////////////////////////
/////////////////////////////// Check for a mode///////////////
///////////////////////////////////////////////////////////////
*/
   /*
   0 -> Day Mode
   1 -> Night Mode
   2 -> No sunlight
   3 -> Power Save
   4 -> Min/Max Save
   */
#ifdef NO_SUNLIGHT_MODE
if(power >= POWER_OFFSET_VAL)
{
	RunningMode = 2;
	disableMotor();
	stepperDisable = true;
	angle = ANGLE_START_VAL;
}
#endif

#ifdef POWER_SAVE
if(power < POWER_LOW_THRESHOLD)
{
	RunningMode = 3;
	disableMotor();
	stepperDisable = true;
	angle = ANGLE_START_VAL;
}
#endif

#ifdef MIN_MAX_STOP
if(angle >= ANGLE_MAX)
{
	RunningMode = 4;
	disableMotor();
	stepperDisable = true;
	angle = ANGLE_START_VAL;
}
#endif

// Night Mode or Day Mode
#ifndef NIGHT_SAVE_MODE
// Stepper On Time (Day Mode)
if(hh >= STEPPER_ON_TIME_HH && minu >= STEPPER_ON_TIME_MM && hh <= STEPPER_OFF_TIME_HH && minu <= STEPPER_OFF_TIME_MM)
{
	RunningMode = 0;
	enableMotor();
}
// Stepper Off Time ( Night Mode)
if(hh >= STEPPER_OFF_TIME_HH && minu >= STEPPER_OFF_TIME_MM && hh <= STEPPER_ON_TIME_HH && minu <= STEPPER_ON_TIME_MM)
{
	RunningMode = 1;
	disableMotor();
}
#endif

// Sensor Alert condition is enabled if SENSOR_ALERT macro is enabled in config.h
#ifdef SENSOR_ALERT
if(_humi > HUMID_UPPER_LIMIT || _humi < HUMID_LOWER_LIMIT || _temp > TEMPR_UPPER_LIMIT || _temp < TEMPR_LOWER_LIMIT)
{
  #if ALERT_TYPE == 0
  myModem.connectCall(USER_NO);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(LCDMSG1);
  lcd.setCursor(0,1);
  lcd.print(LCDMSG2); 
  #elif ALERT_TYPE == 1 || ALERT_TYPE == 2 || ALERT_TYPE == 3
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(LCDMSG1);
  
  #else
  #error "Invalid Alert Type"
  #endif
 
 // Humidity
  if(_humi > HUMID_UPPER_LIMIT)
  {
    Status = 0;
	if(HumidWarning != 1)
	HumidWarning = 1;
  }
  else if(_humi < HUMID_LOWER_LIMIT)
  {
    Status = 1;
	if(HumidWarning != 2)
	HumidWarning = 2;
  }
  else
  HumidWarning = 0;
  
  // Temperature
  if(_temp > TEMPR_UPPER_LIMIT )
  {
	  if(TempWarning != 1)
	  TempWarning = 1;
      Status = 2;
  }
  else if(_temp < TEMPR_LOWER_LIMIT)
  {
	  if(TempWarning != 2)
	TempWarning = 2;
    Status = 3;
  }
  else
  {
     Status = 4;
	 TempWarning = 0;
  }
  
  if(TempWarning > 0 || HumidWarning > 0)
  {
   if(Status == 0)
   {
             #if ALERT_TYPE == 1
             myModem.sendSms(USER_NO,USERMSG1);
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG4);
             #elif ALERT_TYPE == 2
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG5);
             myModem.sendSms(DAQ_SERVER_NO,USERMSG1);
             #elif ALERT_TYPE == 3
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG6);
             myModem.sendSms(USER_NO,USERMSG1);
             myModem.sendSms(DAQ_SERVER_NO,USERMSG1);
             #endif
   }
   else if(Status == 1)
   {
             #if ALERT_TYPE == 1
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG4);
             myModem.sendSms(USER_NO,USERMSG2);
             #elif ALERT_TYPE == 2
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG5);             
             myModem.sendSms(DAQ_SERVER_NO,USERMSG2);
             #elif ALERT_TYPE == 3
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG6);
             myModem.sendSms(USER_NO,USERMSG2);
             myModem.sendSms(DAQ_SERVER_NO,USERMSG2);
             #endif
   }
    else if(Status == 2)
    {
             #if   ALERT_TYPE == 1
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG4);
             myModem.sendSms(USER_NO,USERMSG3);
             #elif ALERT_TYPE == 2
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG5);   
             myModem.sendSms(DAQ_SERVER_NO,USERMSG3);
             #elif ALERT_TYPE == 3
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG6);
             myModem.sendSms(USER_NO,USERMSG1);
             myModem.sendSms(DAQ_SERVER_NO,USERMSG3);
             #endif
    }
    else if(Status == 3)
        {
             #if   ALERT_TYPE == 1
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG4);
             myModem.sendSms(USER_NO,USERMSG4);
             #elif ALERT_TYPE == 2
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG5); 
             myModem.sendSms(DAQ_SERVER_NO,USERMSG4);
             #elif ALERT_TYPE == 3
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG6);
             myModem.sendSms(USER_NO,USERMSG4);
             myModem.sendSms(DAQ_SERVER_NO,USERMSG4);
             #endif
        }
    else
    {
      // Its invalid
      // Dummy entry
    }
  }
  
   // Put Invalid entry value for status
   Status = 10;  
   
   // Update Flag (So for next cycle lcd will be cleared)
   displayClearFlag = true;
}
#endif

#ifdef DATA_LOGGING
// Data Logging Condition
if(minu == dataLogMM && hh == dataLogHH)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(LCDMSG7);
  
  #if DATA_LOG_MODE == 0
  
  // Display LCD
  lcd.setCursor(0,1);
  lcd.print(LCDMSG9);
  
  	Serial.println("AT+CMGF=1");
  	delay(500);
  
  // Send Data to DAQ_NO
  Serial.print("AT+CMGS=\"");
  Serial.print(DAQ_SERVER_NO);
  Serial.write('"');
  Serial.println();
  delay(SMS_NO_TEXT_DELAY);

  // Send Current DateTime
  Serial.print(mm);   // Month
  Serial.write('\\');
  Serial.print(dd);   // Date
  Serial.write('\\');
  Serial.print(yy);   // Year
  Serial.write(' ');
  Serial.print(dof);  // Day Of Week
  Serial.write(' ');
  Serial.print(hh);   // Hour
  Serial.write(':');
  Serial.print(minu); // Minute
  Serial.write(':');
  Serial.print(ss);   // Second
Serial.write(',');
// Send Sensor Data
  Serial.print(_temp); // Temp
  Serial.write(',');
  Serial.print(_humi); // 
  Serial.write(',');
  Serial.print(_LDR1);
  Serial.write(',');
  Serial.print(_LDR2);
  Serial.write(',');
  Serial.print(Voltage);
  Serial.write(',');
  Serial.print(current);
  Serial.write(',');
  Serial.print(angle);
  Serial.write(0x1A);
 
  
  #elif DATA_LOG_MODE == 1
  
  // Display LCD
    lcd.setCursor(0,1);
    lcd.print(LCDMSG8);
  
    	Serial.println("AT+CMGF=1");
    	delay(500);
  
  // Send to User mobile
  Serial.print("AT+CMGS=\"");
  Serial.print(USER_NO);
  Serial.write('"');
  Serial.println();
  delay(SMS_NO_TEXT_DELAY);

  // Send Current DateTime
  Serial.print(mm);   // Month
  Serial.write('\\');
  Serial.print(dd);   // Date
  Serial.write('\\');
  Serial.print(yy);   // Year
  Serial.write(' ');
  Serial.print(dof);  // Day Of Week
  Serial.write(' ');
  Serial.print(hh);   // Hour
  Serial.write(':');
  Serial.print(minu); // Minute
  Serial.write(':');
  Serial.print(ss);   // Second
Serial.write(',');
  // Send Sensor Data
  Serial.print(_temp);
  Serial.write(',');
  Serial.print(_humi);
  Serial.write(',');
  Serial.print(_LDR1);
  Serial.write(',');
  Serial.print(_LDR2);
  Serial.write(',');
  Serial.print(Voltage);
  Serial.write(',');
  Serial.print(current);
  Serial.write(',');
  Serial.print(angle);
  Serial.write(0x1A);
  
  #elif DATA_LOG_MODE == 2
  
  // Display LCD
    lcd.setCursor(0,1);
    lcd.print(LCDMSG10);
  
    	Serial.println("AT+CMGF=1");
    	delay(500);
  
  // Send to both  
  Serial.print("AT+CMGS=\"");
  Serial.print(DAQ_SERVER_NO);
  Serial.write('"');
  Serial.println();
  delay(SMS_NO_TEXT_DELAY);
  
   // Send Current DateTime
   Serial.print(mm);   // Month
   Serial.write('\\');
   Serial.print(dd);   // Date
   Serial.write('\\');
   Serial.print(yy);   // Year
   Serial.write(' ');
   Serial.print(dof);  // Day Of Week
   Serial.write(' ');
   Serial.print(hh);   // Hour
   Serial.write(':');
   Serial.print(minu); // Minute
   Serial.write(':');
   Serial.print(ss);   // Second
Serial.write(',');
   // Send Sensor Data
   Serial.print(_temp);
   Serial.write(',');
   Serial.print(_humi);
   Serial.write(',');
   Serial.print(_LDR1);
   Serial.write(',');
   Serial.print(_LDR2);
   Serial.write(',');
   Serial.print(Voltage);
   Serial.write(',');
   Serial.print(current);
   Serial.write(',');
   Serial.print(angle);
   Serial.write(0x1A);
  
    delay(SMS_NO_TEXT_DELAY*2);

  	Serial.println("AT+CMGF=1");
  	delay(500);

     Serial.print("AT+CMGS=\"");
     Serial.print(USER_NO);
     Serial.write('"');
     Serial.println();
     delay(SMS_NO_TEXT_DELAY);

    // Send Current DateTime
    Serial.print(mm);   // Month
    Serial.write('\\');
    Serial.print(dd);   // Date
    Serial.write('\\');
    Serial.print(yy);   // Year
    Serial.write(' ');
    Serial.print(dof);  // Day Of Week
    Serial.write(' ');
    Serial.print(hh);   // Hour
    Serial.write(':');
    Serial.print(minu); // Minute
    Serial.write(':');
    Serial.print(ss);   // Second
Serial.write(',');
    // Send Sensor Data
    Serial.print(_temp);
    Serial.write(',');
    Serial.print(_humi);
    Serial.write(',');
    Serial.print(_LDR1);
    Serial.write(',');
    Serial.print(_LDR2);
    Serial.write(',');
    Serial.print(Voltage);
    Serial.write(',');
    Serial.print(current);
    Serial.write(',');
    Serial.print(angle);
    Serial.write(0x1A);
  
  // Through an error
  #else
  #error "Invalid 'DATA_LOG_MODE' macro"
  #endif	
  

if(minu >= (60 - DATA_LOG_SMS_INTERVAL))
{
	dataLogMM = (minu + DATA_LOG_SMS_INTERVAL) - 60;
	if(hh == 23)
	dataLogHH = 0;
	else
	dataLogHH = hh + 1;
}
else
{
	dataLogMM = minu + DATA_LOG_SMS_INTERVAL;
	dataLogHH = hh;
}
}

   // Update Flag (So for next cycle lcd will be cleared)
   displayClearFlag = true;
#endif


/*
// Clear LCD if required depending on flag
if(displayClearFlag == true)
{
	lcd.clear();
	displayClearFlag = false;
}
*/

    // Update Global RTC
	if(mm != MM || dd !=DD || yy != YY || dof != DOF || hh != HH || minu != MINU || ss != SS)
	{
		MM = mm;
		DD = dd;
		YY = yy;
		DOF = dof;
		HH = hh;
		MINU = minu;
		SS = ss;
	}

// LCD Update
if((lSS % LCD_UPDATE_RATE) == 0)
{

	/*
	0 -> Temperature
	1 -> Humidity
	2 -> LDR Difference
	3 -> Voltage
	4 -> Current
	5 -> Power
	6 -> Running Mode (Night or Day)
	7 -> Next Data Log Interval
	*/
	
/*if((lSS % RTC_LCD_UPDATE_RATE) == 0) 
{*/
	if(rtcDisplayVal == 0)
	{
		    rtcDisplayVal++;
			lcd.setCursor(0,0);
			lcd.print(LCDCLR);
			lcd.setCursor(0,0);
			lcd.print(mm);   // Month
			lcd.write('/');
			lcd.print(dd);   // Date
			lcd.write('/');
			lcd.print(yy);   // Year
	}
	else if(rtcDisplayVal == 1)
	{
		            rtcDisplayVal++;
					lcd.setCursor(0,0);
					lcd.print(LCDCLR);
					lcd.setCursor(0,0);
		switch(dof)
		{
			         case 0 : lcd.print("SUNDAY    ");
			         break;
					 case 1 : lcd.print("MONDAY    ");
					 break;
					 case 2 : lcd.print("TUESDAY   ");
					 break;
					 case 3 : lcd.print("WEDNESDAY ");
					 break;
					 case 4 : lcd.print("THURSDAY  ");
					 break;
					 case 5 : lcd.print("FRIDAY    ");
					 break;
					 case 6 : lcd.print("SATURDAY  ");
					 break;
		}
	}
		else
		{
			rtcDisplayVal = 0;
			lcd.setCursor(0,0);
			lcd.print(LCDCLR);
			lcd.setCursor(0,0);
	        lcd.print(hh % 12);   // Hour
	        lcd.write(':');
	        lcd.print(minu); // Minute
	        lcd.write(':');
	        lcd.print(ss);
		    lcd.print(" ");	
			if(hh < 12)	
			lcd.print("AM")  ;
			else
			lcd.print("PM");
		}
//}
	

	/*	if((lSS % SENSOR_LCD_VAL_UPDATE_RATE) == 0) 
		{*/
				delay(100);
				lcd.setCursor(0,1);
				lcd.print(LCDCLR);
				lcd.setCursor(0,1);
	if(DisplayVal == 0)
	{
		DisplayVal++;
		lcd.print("Temp:");
		lcd.print((int)_temp);
		lcd.print((char)223);
		lcd.print("C");
		 lcd.print("    ");
	}
		else if(DisplayVal == 1)
		{
			DisplayVal++;
			lcd.print("Humi:");
			lcd.print((int)_humi);
			lcd.write('%');
			lcd.write(' ');
			lcd.print("RH");
			lcd.print("    ");
		}
			else if(DisplayVal == 2)
			{
				DisplayVal++;
				lcd.print("LDR:");
				lcd.print((int)_LDR);
				 lcd.print(" %  ");
			}
						else if(DisplayVal == 3)
						{
							DisplayVal++;
							lcd.print("VOL:");
							lcd.print((int)Voltage);
							lcd.print(" mV");
							 lcd.print("    ");
						}
												else if(DisplayVal == 4)
												{
													DisplayVal++;
													lcd.print("CUR:");
													lcd.print((int)current);
													lcd.print(" mA");
													 lcd.print("    ");
												}
																								else if(DisplayVal == 5)
																								{
																									DisplayVal++;
																									lcd.print("Power:");
																									lcd.print((int)((Voltage * current)));
																									lcd.print(" mW");
																									lcd.print("    ");																								}
																																															else if(DisplayVal == 6)
																																															{
																																																DisplayVal++;
																																																lcd.print("Mode: ");
																																																   /*
   0 -> Day Mode
   1 -> Night Mode
   2 -> No sunlight
   3 -> Power Save
   4 -> Min/Max Save
   */
																																																if(RunningMode == 0)
																																																lcd.print("Day Mode");
																																																else if(RunningMode == 1)
																																																lcd.print("Night Mode");
																																																else if(RunningMode == 2)
																																																lcd.print("No Sun");
																																																else if(RunningMode == 3)
																																																lcd.print("Power Save");
																																																else if(RunningMode == 4)
																																																lcd.print("Min/Max");
																																																else
																																																lcd.print("error   ");
																																																lcd.print("     ");
																																							
																																															}
																																																							else if(DisplayVal == 7)
																																																							{
																																																								DisplayVal++;
																																																							    lcd.print("Next:");
																																																								lcd.print(dataLogHH % 12);
																																																								lcd.write(':');
																																																								lcd.print(dataLogMM);
																																																								lcd.write(' ');
																																																								if(dataLogHH < 12)
																																																								lcd.print("AM");
																																																								else
																																																								lcd.print("PM");
																																																								lcd.print("    ");
																																																							}
																																																												else if(DisplayVal == 8)
																																																												{
																																																													DisplayVal = 0;
																																																													lcd.print("Angle:");
																																																													lcd.print(angle);
																																																													lcd.print("    ");
																																																												}
																																																							
																																																					
		//}
																																																								
																																																							}
		////////////////////////////////////////////////////////////////
////////// Delay loop has to modify
////////// Now it is staying for STEP_DELAY__ ms 

#ifdef LDR_LOGIC

// Calculate Average
_LDR1 = (_ldr2 + _ldr1) / 2;
_LDR2 = (_ldr4 + _ldr3) / 2;
// Calculate Difference
_LDR = _LDR1 - _LDR2;

	if(_LDR > LDR_THRESHOLD1)
	{
		myStepper.step(-1);
		delay(STEP_DELAY__);
	   /* Start a stop watch
		stepperStopWatch = ss;+
		stepperRunReq = true;*/
		angle -= 1.8;
	}
	else if(_LDR < LDR_THRESHOLD2)
	{
		myStepper.step(1);
		/*stepperStopWatch = ss;
		stepperRunReq = true;*/
		delay(STEP_DELAY__);
		angle += 1.8;
	}
#endif				

// For debugging
#ifdef DEBUG
/*lcd.setCursor(0,0);
lcd.print(_LDR);
lcd.print("    ");*/
Serial.print(mm);
Serial.write('/');
Serial.print(dd);
Serial.write('/');
Serial.print(yy);
Serial.write(' ');
switch(dof)
{
	case 0 : Serial.print("SUN"); 
	         break;
			 	case 1 : Serial.print("MON");
			 	break;
				 	case 2 : Serial.print("TUE");
				 	break;
					 	case 3 : Serial.print("WED");
					 	break;
						 	case 4 : Serial.print("THU");
						 	break;
							 	case 5 : Serial.print("FRI");
							 	break;
								 	case 6 : Serial.print("SAT");
								 	break;
}
Serial.write(' ');
Serial.print(hh,DEC);
Serial.write(':');
Serial.print(minu);
Serial.write(':');
Serial.print(ss);
Serial.write(0x09);
Serial.print(dataLogHH);
Serial.write(0x09);
Serial.print(dataLogMM);
Serial.write(0x09);
Serial.print(_temp);
Serial.write(0x09);
Serial.print(_humi);
Serial.write(0x09);
Serial.print(_LDR);
Serial.write(0x09);
Serial.print(Voltage);
Serial.write(0x09);
Serial.print(current);
Serial.write(0x09);
Serial.println();
delay(_DEBUG_UART_PRINT_DELAY_);
#endif
}
}

void bootTest(void)
{
  unsigned char i = 0;
  lcd.setCursor(0,0);
  lcd.print(BOOTMSG1);
  
  
  lcd.setCursor(0,1);
  for(i = 0;i<16;i++)
  {
      lcd.write('.');
      delay(LCD_INITIAL_SLOW_DELAY);
  }
  delay(StartUpDelay);
  lcd.clear();
  
  #ifdef DEBUG
  Serial.println();
  #endif
  
  // GSM Modem test
  // If detected
  bAck:
    Tms = 0;
	Tss = 0;
	Timer1.initialize(Timeus);
  Timer1.start();
  if(myModem.detectModem() == 1)         
  {
  lcd.clear();
  lcd.print(BOOTMSG2);
  
  #ifdef DEBUG
  Serial.println(BOOTMSG2);
  #endif
  
  Tss = 0;
  #ifdef STARTUP_ALERT_SMS == 1
  myModem.sendSms(DAQ_SERVER_NO,START_UP_ALERT_SMS);
  #elif  STARTUP_ALERT_SMS == 2
  myModem.sendSms(USER_NO,START_UP_ALERT_SMS);
  #elif  STARTUP_ALERT_SMS == 3
  myModem.sendSms(DAQ_SERVER_NO,START_UP_ALERT_SMS);
  myModem.sendSms(USER_NO,START_UP_ALERT_SMS);
  
  // Through an error
  #else
  #error "Invalid value for 'STARTUP_ALERT_SMS' macro"
  
  #endif
  }
  // If not detected
  else
  {
  lcd.print(BOOTMSG3);

  #ifdef DEBUG
  Serial.println(BOOTMSG3);
  #endif  

  lcd.setCursor(0,1);
  lcd.print(BOOTMSG4);
  
  #ifdef DEBUG
  Serial.println(BOOTMSG4);
  #endif
  Timer1.stop();
  goto bAck;
  }
  Timer1.stop();
  delay(StartUpDelay);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(BOOTMSG5);

  #ifdef DEBUG
  Serial.println(BOOTMSG5);
  #endif
  
  lcd.setCursor(0,1);
  lcd.print(BOOTMSG6);

  #ifdef DEBUG
  Serial.println(BOOTMSG6);
  #endif
  
  delay(StartUpDelay);
}
