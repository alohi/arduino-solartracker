#ifndef PCF8591_H
#define PCF8591_H

extern void           pcf8591begin(void);
extern void           pcf8591Read(unsigned char*);

#define PCF8591_ADDRESS 0x92 >> 1 


#endif 

