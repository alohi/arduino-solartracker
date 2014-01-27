#include "gsmModem.h"
#include "sensors.h"
#include "config.h"
#include "OneWire.h"
#include "TimerOne.h"
#include "appmsg.h"
#include <LiquidCrystal.h>
#include <Servo.h>

// Globals for Timer Calculation
volatile unsigned int ms = 0;
volatile unsigned int ss = 0;


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
// An instance for servo motor
Servo myServo;

void setup(void)
{ 
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  Serial.begin(9600);
  pinMode(LDR1,INPUT);
  pinMode(LDR2,INPUT);
  pinMode(LDR3,INPUT);
  pinMode(LDR4,INPUT);
  pinMode(TEMP,INPUT);
  pinMode(HUMI,INPUT);
  // Initiate temperature sensor (It will search address of onewire device and store it in a global)
  mySensors.beginTemp();
  // Attach servo motor pin
  myServo.attach(SERVO);
  
  Timer1.initialize(Timeus);
  Timer1.attachInterrupt(timer1Isr);
}

void loop(void)
{
float _humi,_ldr1,_ldr2,_ldr3,_ldr4,_temp;
int Status;
bootTest();
while(1)
{
_humi = mySensors.getHumi();
_temp = mySensors.getTemp(DEGC);
_ldr1 = mySensors.getLight(1);
_ldr2 = mySensors.getLight(2);
_ldr3 = mySensors.getLight(3);
_ldr4 = mySensors.getLight(4);

if(_humi > HUMID_UPPER_LIMIT || _humi < HUMID_LOWER_LIMIT || _temp > TEMPR_UPPER_LIMIT || _temp < TEMPR_LOWER_LIMIT)
{
  #if ALERT_TYPE == 0
  myModem.connectCall(USER_NO);
  
  #elif ALERT_TYPE == 1 || ALERT_TYPE == 2 || ALERT_TYPE == 3
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
             myModem.sendSms(USER_NO,USERMSG1);
             #elif ALERT_TYPE == 2
             myModem.sendSms(DAQ_SERVER_NO,USERMSG1);
             #elif ALERT_TYPE == 3
             myModem.sendSms(USER_NO,USERMSG1);
             myModem.sendSms(DAQ_SERVER_NO,USERMSG1);
             #endif
   }
   else if(Status == 1)
   {
             #if ALERT_TYPE == 1
             myModem.sendSms(USER_NO,USERMSG2);
             #elif ALERT_TYPE == 2
             myModem.sendSms(DAQ_SERVER_NO,USERMSG2);
             #elif ALERT_TYPE == 3
             myModem.sendSms(USER_NO,USERMSG2);
             myModem.sendSms(DAQ_SERVER_NO,USERMSG2);
             #endif
   }
    else if(Status == 2)
    {
             #if   ALERT_TYPE == 1
             myModem.sendSms(USER_NO,USERMSG3);
             #elif ALERT_TYPE == 2
             myModem.sendSms(DAQ_SERVER_NO,USERMSG3);
             #elif ALERT_TYPE == 3
             myModem.sendSms(USER_NO,USERMSG1);
             myModem.sendSms(DAQ_SERVER_NO,USERMSG3);
             #endif
    }
    else if(Status == 4)
        {
             #if   ALERT_TYPE == 1
             myModem.sendSms(USER_NO,USERMSG4);
             #elif ALERT_TYPE == 2
             myModem.sendSms(DAQ_SERVER_NO,USERMSG4);
             #elif ALERT_TYPE == 3
             myModem.sendSms(USER_NO,USERMSG4);
             myModem.sendSms(DAQ_SERVER_NO,USERMSG4);
             #endif
        }
        
   #else
   #error "Invalid 'ALERT_TYPE' macro"   
   #endif
}
if(ss >= DATA_LOG_SMS_INTERVAL)
{
  // temporary disable interrupt and stop the timer
  Timer1.detachInterrupt();       // stop timer interrupt
  Timer1.stop();
  // Clear Counts
  ss = 0;
  ms = 0;
  #if DATA_LOG_MODE == 0
  // Send Data to DAQ_NO
  #elif DATA_LOG_MODE == 1
  // Send to User mobile
  #elif DATA_LOG_MODE == 2
  // Send to both
  #else
  #error "Inavlid 'DATA_LOG_MODE' macro"
  #endif
}
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
  
  // GSM Modem test
  // If detected
  if(myModem.detectModem() == 1)
  {
  lcd.print(BOOTMSG2);
  ss = 0;
  #ifdef STARTUP_ALERT_SMS == 1
  myModem.sendSms(DAQ_SERVER_NO,BOOTMSG3);
  #elif  STARTUP_ALERT_SMS == 2
  myModem.sendSms(USER_NO,BOOTMSG3);
  #elif  STARTUP_ALERT_SMS == 3
  myModem.sendSms(DAQ_SERVER_NO,BOOTMSG3);
  myModem.sendSms(USER_NO,BOOTMSG3);
  #else
  #error "Invalid value for 'STARTUP_ALERT_SMS' macro"
  #endif
  }
  // If not detected
  else
  {
  lcd.print(BOOTMSG4);
  lcd.setCursor(0,1);
  lcd.print(BOOTMSG5);
  ss = 0;
  Timer1.detachInterrupt();       // stop timer interrupt
  Timer1.stop();
  // Infinite loop
  while(true);
  }
  delay(StartUpDelay);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(BOOTMSG6);
  lcd.setCursor(0,1);
  lcd.print(BOOTMSG7);
  delay(StartUpDelay);
  lcd.clear();
}
