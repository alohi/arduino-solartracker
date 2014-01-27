#include "gsmModem.h"
#include "sensors.h"
#include "config.h"
#include "OneWire.h"
#include "TimerOne.h"
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
bootTest();
while(1)
{
_humi = mySensors.getHumi();
_temp = mySensors.getTemp(DEGC);
_ldr1 = mySensors.getLight(1);
_ldr2 = mySensors.getLight(2);
_ldr3 = mySensors.getLight(3);
_ldr4 = mySensors.getLight(4);
}
}

void bootTest(void)
{
  unsigned char i = 0;
  lcd.setCursor(0,0);
  lcd.print("Wait initiating..");
  lcd.setCursor(0,1);
  for(i = 0;i<16;i++)
  {
    lcd.write('.');
    delay(300);
  }
  delay(StartUpDelay);
  lcd.clear();
  // GSM Modem test
  // If detected
  if(myModem.detectModem() == 1)
  {
  lcd.print("Modem Detcted");
  ss = 0;
  }
  // If not detected
  else
  {
  lcd.print("Modem Not detected");
  lcd.setCursor(0,1);
  lcd.print("Device is ideal");
  ss = 0;
  Timer1.detachInterrupt();       // Release timer interrupt
  Timer1.stop();
  // Infinite loop
  while(true);
  }
  delay(StartUpDelay);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Device initiated");
   lcd.setCursor(0,1);
   lcd.print("Successfully");
  delay(StartUpDelay);
  lcd.clear();
}
