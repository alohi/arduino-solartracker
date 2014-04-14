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


// Globals for Timer Calculation
volatile unsigned int ms = 0;
volatile unsigned int ss = 0;

unsigned int angle = 0;
boolean motorStatus = false;
boolean StepperStatus = true;
boolean lcdUpdateFlag = false;

#define ADJUST_RTC

#ifdef ADJUST_RTC
#define DATE "Feb 03 2014"
#define TIME "22:08:00"
#endif

void timer1Isr(void)          // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  ms++;
  if(ms >= 1000)
  {
    ms = 0;
    ss++;
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


void _test_sensors(void)
{
	
}

void enableMotor(void)
{
  digitalWrite(STEPPER_EN1,HIGH);
  digitalWrite(STEPPER_EN2,HIGH);
}

void disableMotor(void)
{
  digitalWrite(STEPPER_EN1,LOW);
  digitalWrite(STEPPER_EN2,LOW);
}

void testMotor(void)
{
  char c;
  while(1)
  {
    if(Serial.available())
    {
      c = Serial.read();
      if(c == 'f')
      {
        Serial.println("Motor FW");
      myStepper.step(1);
      }
      else if(c == 'b')
      {
        Serial.println("Motor BW");
      myStepper.step(-1);
      }
    }
  }
}

void motorRotate(void)
{
  unsigned char i;
  for(i=0;i<5  ;i++)
  {
  myStepper.step(5);
  delay(2000);
  }
    for(i=0;i<5;i++)
  {
  myStepper.step(-5);
  delay(2000);
  }
}

void setup(void)
{ 
  // LDR Pin Configuration
  pinMode(LDR1,INPUT);
  pinMode(LDR2,INPUT);
  pinMode(LDR3,INPUT);
  pinMode(LDR4,INPUT);
  
  pinMode(EN1,OUTPUT);
  pinMode(EN2,OUTPUT);
  
  digitalWrite(EN1,HIGH);
    digitalWrite(EN2,HIGH);
  
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
  
   if (! rtc.isrunning()) {
	   Serial.println("RTC is NOT running!");
	   // following line sets the RTC to the date & time this sketch was compiled
	   rtc.adjust(DateTime(__DATE__, __TIME__));
   }
  
  // Adjust Date and time
  #ifdef ADJUST_RTC
  rtc.adjust(DateTime(DATE, TIME));
  #endif
  
  // Initiate LCD cursor
  lcd.setCursor(0,0);
  
  // Stepper Motor speed settings
  myStepper.setSpeed(60);
   
  // Timer 1 Initialization
  Timer1.initialize(Timeus);
  Timer1.attachInterrupt(timer1Isr);
  
  pinMode(STEPPER_EN1,OUTPUT);
  pinMode(STEPPER_EN2,OUTPUT);
  
  digitalWrite(STEPPER_EN1,HIGH);
  digitalWrite(STEPPER_EN2,HIGH);
}

void loop(void)
{
float _humi,_ldr1,_ldr2,_ldr3,_ldr4,_temp;
float LDR1,LDR2;
float LDR;
int current;
unsigned char Voltage;
int   Status;
int   mm;
int   hh;
int   MM;
int   HH;

unsigned char dummy;

 DateTime now = rtc.now();
 //while(1)

//motorRotate();
//testMotor();

// Calculate next predicted interval for data logging
MM = now.minute();
HH = now.hour();
if(MM >= (60 - DATA_LOG_SMS_INTERVAL))
{
	mm = (MM + DATA_LOG_SMS_INTERVAL) - 60;
	if(HH == 23)
	hh = 0;
	else
<<<<<<< HEAD
	hh = HH+1;
=======
	hh = HH + 1;
>>>>>>> 62e6c21e79af73490bd2f57179a008eacbd7c521
}
else
{
	mm = MM + DATA_LOG_SMS_INTERVAL;
	hh = HH;
}


#ifndef DEBUG
bootTest();
#endif
lcdUpdateFlag = true;

// Infinite loop (It will run continuosly)
while(1)
{
	if(lcdUpdateFlag == true)
	{
lcd.setCursor(0,0);
lcd.print(LCDMSG12);
lcd.setCursor(0,1);
lcd.print(LCDMSG13);		
lcdUpdateFlag = false;
	}


// Read All the datas
_humi   = mySensors.getHumi();
_temp   = mySensors.getTemp(DEGC);
_ldr1   = mySensors.getLight(1);
_ldr2   = mySensors.getLight(2);
_ldr3   = mySensors.getLight(3);
_ldr4   = mySensors.getLight(4);
current = mySensors.getCurrent();
Voltage = mySensors.getVoltage();

// GET RTC Date and time
DateTime now = rtc.now();
now = rtc.now();
MM = now.minute();
HH = now.hour();

<<<<<<< HEAD
  #ifdef SENSOR_ALERT
=======
#ifdef SENSOR_ALERT
>>>>>>> 62e6c21e79af73490bd2f57179a008eacbd7c521
// Alert Condition
if(_humi > HUMID_UPPER_LIMIT || _humi < HUMID_LOWER_LIMIT || _temp > TEMPR_UPPER_LIMIT || _temp < TEMPR_LOWER_LIMIT)
{
  #if ALERT_TYPE == 0
  myModem.connectCall(USER_NO);
  ////////////////// Display LCD
  ////////////////// Add here
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(LCDMSG1);
  lcd.setCursor(0,1);
  lcd.print(LCDMSG2);
  
  #elif ALERT_TYPE == 1 || ALERT_TYPE == 2 || ALERT_TYPE == 3
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(LCDMSG1);
  
  if(_humi > HUMID_UPPER_LIMIT)
  {
    Status = 0;
  }
  else if(_humi < HUMID_LOWER_LIMIT)
  {
    Status = 1;
  }
  else if(_temp > TEMPR_UPPER_LIMIT )
  {
    Status = 2;
  }
  else if(_temp < TEMPR_LOWER_LIMIT)
  {
    Status = 3;
  }
  else
  {
     Status = 4;
  }
  
   if(Status == 0)
   {
             #if ALERT_TYPE == 1
             ////////////////// Display LCD
             ////////////////// Add here
             myModem.sendSms(USER_NO,USERMSG1);
             ////////////////// Display LCD
             ////////////////// Add here
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG4);
             #elif ALERT_TYPE == 2
             ////////////////// Display LCD
             ////////////////// Add here
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG5);
             myModem.sendSms(DAQ_SERVER_NO,USERMSG1);
             ////////////////// Display LCD
             ////////////////// Add here
             #elif ALERT_TYPE == 3
             ////////////////// Display LCD
             ////////////////// Add here
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG6);
             myModem.sendSms(USER_NO,USERMSG1);
             myModem.sendSms(DAQ_SERVER_NO,USERMSG1);
             ////////////////// Display LCD
             ////////////////// Add here
             #endif
   }
   else if(Status == 1)
   {
             #if ALERT_TYPE == 1
             ////////////////// Display LCD
             ////////////////// Add here
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG4);
             myModem.sendSms(USER_NO,USERMSG2);
             ////////////////// Display LCD
             ////////////////// Add here
             #elif ALERT_TYPE == 2
             ////////////////// Display LCD
             ////////////////// Add here
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG5);             
             myModem.sendSms(DAQ_SERVER_NO,USERMSG2);
             ////////////////// Display LCD
             ////////////////// Add here
             #elif ALERT_TYPE == 3
             ////////////////// Display LCD
             ////////////////// Add here
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG6);
             myModem.sendSms(USER_NO,USERMSG2);
             myModem.sendSms(DAQ_SERVER_NO,USERMSG2);
             ////////////////// Display LCD
             ////////////////// Add here
             #endif
   }
    else if(Status == 2)
    {
             #if   ALERT_TYPE == 1
             ////////////////// Display LCD
             ////////////////// Add here
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG4);
             myModem.sendSms(USER_NO,USERMSG3);
             ////////////////// Display LCD
             ////////////////// Add here
             #elif ALERT_TYPE == 2
             ////////////////// Display LCD
             ////////////////// Add here
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG5);   
             myModem.sendSms(DAQ_SERVER_NO,USERMSG3);
             ////////////////// Display LCD
             ////////////////// Add here 
             #elif ALERT_TYPE == 3
             ////////////////// Display LCD
             ////////////////// Add here
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG6);
             myModem.sendSms(USER_NO,USERMSG1);
             myModem.sendSms(DAQ_SERVER_NO,USERMSG3);
             ////////////////// Display LCD
             ////////////////// Add here
             #endif
    }
    else if(Status == 3)
        {
             #if   ALERT_TYPE == 1
             ////////////////// Display LCD
             ////////////////// Add here
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG4);
             myModem.sendSms(USER_NO,USERMSG4);
             ////////////////// Display LCD
             ////////////////// Add here
             #elif ALERT_TYPE == 2
             ////////////////// Display LCD
             ////////////////// Add here
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG5); 
             myModem.sendSms(DAQ_SERVER_NO,USERMSG4);
             #elif ALERT_TYPE == 3
             ////////////////// Display LCD
             ////////////////// Add here
             lcd.setCursor(0,1);
             lcd.print(LCDCLR);
             lcd.setCursor(0,1);
             lcd.print(LCDMSG6);
             myModem.sendSms(USER_NO,USERMSG4);
             myModem.sendSms(DAQ_SERVER_NO,USERMSG4);
             ////////////////// Display LCD
             ////////////////// Add here
             #endif
        }
    else
    {
      // Its invalid
      // Dummy entry
    }
  
   // Put Invalid entry value for status
   Status = 10;
  lcdUpdateFlag = true;
   // Through an error      
   #else
   #error "Invalid 'ALERT_TYPE' macro"  
   
   #endif
}
#endif

#ifdef DATA_LOGGING
// Data Logging Condition
if(MM == mm && HH == hh)
{
  // In night save mode data logging is not required (Enhancement feature)
  #ifdef NIGHT_SAVE_MODE
  if(StepperStatus == true)
  {
  #endif

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(LCDMSG7);
  
  #if DATA_LOG_MODE == 0
  // Send Data to DAQ_NO
  Serial.print("AT+CMGS=\"");
  Serial.print(DAQ_SERVER_NO);
  Serial.write('"');
  Serial.println();
  delay(100);
  /*Serial.print(now.DateTime());
  Serial.write(',');*/
  Serial.print(_temp);
  Serial.write(',');
  Serial.print(_humi);
  Serial.write(',');
  Serial.print(LDR1);
  Serial.write(',');
  Serial.print(LDR2);
  Serial.write(',');
  Serial.print(current);
  Serial.write(',');
  Serial.print(Voltage);
  Serial.write(',');
  Serial.write(0x1A);
  
  lcd.setCursor(0,1);
  lcd.print(LCDMSG9);
  
    #elif DATA_LOG_MODE == 1
  // Send to User mobile
  Serial.print("AT+CMGS=\"");
  Serial.print(USER_NO);
  Serial.write('"');
  Serial.println();
  delay(100);
  /*Serial.print(now.DateTime());
  Serial.write(',');*/
  Serial.print(_temp);
  Serial.write(',');
  Serial.print(_humi);
  Serial.write(',');
  Serial.print(LDR1);
  Serial.write(',');
  Serial.print(LDR2);
  Serial.write(',');
  Serial.print(current);
  Serial.write(',');
  Serial.print(Voltage);
  Serial.write(',');
  Serial.write(0x1A);
  Serial.write(0x1A);  
  
  lcd.setCursor(0,1);
  lcd.print(LCDMSG8);
  
  
  #elif DATA_LOG_MODE == 2
  // Send to both  
  Serial.print("AT+CMGS=\"");
  Serial.print(DAQ_SERVER_NO);
  Serial.write('"');
  Serial.println();
  delay(100);
 /* Serial.print(now.DateTime());
  Serial.write(',');*/
  Serial.print(_temp);
  Serial.write(',');
  Serial.print(_humi);
  Serial.write(',');
  Serial.print(LDR1);
  Serial.write(',');
  Serial.print(LDR2);
  Serial.write(',');
  Serial.print(current);
  Serial.write(',');
  Serial.print(Voltage);
  Serial.write(',');
  Serial.write(0x1A);

  Serial.print("AT+CMGS=\"");
  Serial.print(USER_NO);
  Serial.write('"');
  Serial.println();
  delay(100);
 /* Serial.print(now.DateTime());
  Serial.write(',');*/
  Serial.print(_temp);
  Serial.write(',');
  Serial.print(_humi);
  Serial.write(',');
  Serial.print(LDR1);
  Serial.write(',');
  Serial.print(LDR2);
  Serial.write(',');
  Serial.print(current);
  Serial.write(',');
  Serial.print(Voltage);
  Serial.write(',');
  Serial.write(0x1A);
  
  lcd.setCursor(0,1);
  lcd.print(LCDMSG10);
  
<<<<<<< HEAD
  lcdUpdateFlag = true;
  
  // Through an error
  #else
  #error "Invalid 'DATA_LOG_MODE' macro"
  #endif	
  

=======
 // Through an error
  #else
  #error "Inavlid 'DATA_LOG_MODE' macro"
  #endif
>>>>>>> 62e6c21e79af73490bd2f57179a008eacbd7c521
  
  // Calculating next interval
MM = now.minute();
HH = now.hour();
if(MM >= (60 - DATA_LOG_SMS_INTERVAL))
{
	mm = (MM + DATA_LOG_SMS_INTERVAL) - 60;
	if(HH == 23)
	hh = 0;
	else
<<<<<<< HEAD
	hh = HH+1;
=======
	hh = HH + 1;
>>>>>>> 62e6c21e79af73490bd2f57179a008eacbd7c521
}
else
{
	mm = MM + DATA_LOG_SMS_INTERVAL;
	hh = HH;
}
  

#ifdef NIGHT_SAVE_MODE
}
#endif	
}
<<<<<<< HEAD
  #endif
=======
#endif
>>>>>>> 62e6c21e79af73490bd2f57179a008eacbd7c521

  
// Solar panel rotation logic
// Add Logic here
///////////////////////////////////////////////
///////////////////////////////////////////////
LDR1 = (_ldr2 + _ldr1) / 2;
LDR2 = (_ldr4 + _ldr3) / 2;
LDR = LDR1 - LDR2;

if(LDR > LDR_THRESHOLD1)
{
	myStepper.step(-1);
        delay(STEP_DELAY__);
        angle--;
}
else if(LDR < LDR_THRESHOLD2)
{
	myStepper.step(1);
        delay(STEP_DELAY__);
        angle++;
}




#ifdef NIGHT_SAVE_MODE
// Stepper Off Time
if(now.hour() >= STEPEER_OFF_TIME)
{
StepperStatus = false;
disableMotor();
#ifdef DEBUG
Serial.println("Night mode is ON");
#endif
<<<<<<< HEAD
lcd.setCursor(0,1);
lcd.print(LSDMSG11);
lcdUpdateFlag = true;
=======
/////////////////////////////////
/////////////////////////////////
// ADD LCD Display
>>>>>>> 62e6c21e79af73490bd2f57179a008eacbd7c521
}
// Stepper On Time
else if(now.hour() >= STEPPER_ON_TIME && now.hour() < STEPEER_OFF_TIME)
{
StepperStatus = true;
enableMotor();
}
#endif

// For debugging
#ifdef DEBUG
lcd.setCursor(0,0);
lcd.print(LDR);
lcd.print("    ");
Serial.print(_humi);
Serial.write(0x09);
Serial.print(_temp);
Serial.write(0x09);
Serial.print(_ldr1);
Serial.write(0x09);
Serial.print(_ldr2);
Serial.write(0x09);
Serial.print(_ldr3);
Serial.write(0x09);
Serial.print(_ldr4);
Serial.write(0x09);
Serial.print(current);
Serial.write(0x09);
Serial.print(Voltage);
Serial.write(0x09);
Serial.print(now.hour());
Serial.write(0x09);
Serial.print(now.minute());
Serial.write(0x09);
Serial.print(now.second());
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
  if(myModem.detectModem() == 1)         
  {
  lcd.print(BOOTMSG2);
  
  #ifdef DEBUG
  Serial.println(BOOTMSG2);
  #endif
  
  ss = 0;
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
  
  ss = 0;
  Timer1.detachInterrupt();       // stop timer interrupt
  Timer1.stop();
  // Infinite loop
  while(true)
  {
    ;
  } 
  }
  
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
  lcd.clear();
}
