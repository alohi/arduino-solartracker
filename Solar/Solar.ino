#include "gsmModem.h"
#include "sensors.h"
#include "config.h"
#include "OneWire.h"
#include "TimerOne.h"
#include <LiquidCrystal.h>

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

void setup(void)
{ 
  lcd.begin(16,2);
  Serial.begin(9600);
  pinMode(LDR1,INPUT);
  pinMode(LDR2,INPUT);
  pinMode(LDR3,INPUT);
  pinMode(LDR4,INPUT);
  pinMode(TEMP,INPUT);
  pinMode(HUMI,INPUT);
  
  mySensors.beginTemp();
  
   Timer1.initialize(1000);
   Timer1.attachInterrupt(timer1Isr);
}

void loop(void)
{
/*Serial.print(mySensors.getLight(1));
Serial.write(',');
Serial.print(mySensors.getLight(2));
Serial.write(',');
Serial.print(mySensors.getLight(3));
Serial.write(',');
Serial.print(mySensors.getLight(4));
Serial.write(',');
Serial.print(mySensors.getHumi());
Serial.write(',');
Serial.print(mySensors.getTemp(0));

lcd.setCursor(0,0);
lcd.print(mySensors.getLight(1));
lcd.write(',');
lcd.print(mySensors.getLight(2));
lcd.write(',');
lcd.print(mySensors.getLight(3));
lcd.write(',');
lcd.setCursor(0,1);
lcd.print(mySensors.getLight(4));
lcd.write(',');
lcd.print(mySensors.getHumi());
lcd.write(',');
lcd.print(mySensors.getTemp(0));
delay(200);*/
float _humi,_ldr1,_ldr2,_ldr3,_ldr4,_temp;
_humi = mySensors.getHumi();
_temp = mySensors.getTemp(DEGC);
_ldr1 = mySensors.getLight(1);
_ldr2 = mySensors.getLight(2);
_ldr3 = mySensors.getLight(3);
_ldr4 = mySensors.getLight(4);

Serial.print(_humi);
Serial.write(',');
Serial.print(_temp);
Serial.write(',');
Serial.print(_ldr1);
Serial.write(',');
Serial.print(_ldr2);
Serial.write(',');
Serial.print(_ldr3);
Serial.write(',');
Serial.print(_ldr4);
Serial.println();
delay(200);
}

