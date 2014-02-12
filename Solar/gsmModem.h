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
**   File       : gsmModem.h
**   Author     : Sriharsha
**   Website    : www.zuna.in
**   Email      : helpzuna@gmail.com
**   Description: This is the GSM Modem (SIM300) driver include file for arduino
***/

#ifndef __gsmModem_h__
#define __gsmModem_h__

// Timeout for GSM Modem detect in seconds
#define GSM_DETECT_TIMEOUT 10

#define ConnectCallCmd        "ATD"
#define disConnectCallCmd     "ATH"
#define GsmTestCmd            "AT"
#define GsmAckChar1           'O'
#define GsmAckChar2           'K'
#define GsmAckIndex1           2
#define GsmAckIndex2           3
#define GsmAckLength           5
#define gsmSendSmsCmd1        "AT+CMGF=1"
#define gsmSendSmsCmd2        "AT+CMGS="

#define ASCII_SUB             0x1A

class gsmModem
{
  public:
  unsigned char detectModem(void);
  void          sendSms(char*, char*);         // Mob No,Sms Text
  void          connectCall(char*);            // Mob No
  void          disconnectCall(void);          // Disconnect Ongoing call
};

#endif
