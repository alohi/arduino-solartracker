//Library for reading 8-bit ADC PCF8591 w/4 inputs

/*
* Based on the MCP23009 Library by Adafruit
* Version:  86a8bbf from Liquid Crystal libary (Modified by Adafruit)
*/

#ifndef PCF8591_H
#define PCF8591_H

extern void pcf8591begin(void);
extern int pcf8591analogRead(uint8_t);


#define PCF8591_ADDRESS 0x90 >> 1

/*
* REGISTERS
* These will be bit OR'd in the begin
*/

//analog output enable                    B76543210
#define PCF8591_ANALOG_OUT_ENABLE         0b01000000

//analog input style
#define PCF8591_ANALOG_IN_SINGLE          0b0000000
#define PCF8591_ANALOG_IN_3_DIFFERENTIAL  0b0100000
#define PCF8591_ANALOG_IN_SINGLE_AND_DIFF 0b1000000
#define PCF8591_ANALOG_IN_2_DIFFERENTIAL  0b1100000

#define PCF8591_AUTO_INCREMENT            0b0000100

#endif //PCF8591_H

