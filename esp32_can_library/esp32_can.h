/*
  esp32_can.h
  2019 Copyright (c) Musuk Choi  All Rights Reserved.

  Author:Musuk Choi

  2019-3-22
  This library base on MCP_CAN library and ESP32 CAN driver
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-
  1301  USA
*/
#ifndef _ESP32_CAN_H_
#define _ESP32_CAN_H_

#include <Arduino.h>
// Include ESP32 CAN C library
#include "driver/can.h"	

#ifndef INT32U
#define INT32U unsigned long
#endif

#ifndef INT8U
#define INT8U byte
#endif

#define MAX_CHAR_IN_MESSAGE 8

#define CAN_25KBPS		0
#define CAN_50KBPS		1
#define CAN_100KBPS		2
#define CAN_125KBPS		3
#define CAN_250KBPS		4
#define CAN_500KBPS		5
#define CAN_800KBPS		6
#define CAN_1000KBPS	7

#define CAN_OK             (0)
#define CAN_FAILINIT       (1)
#define CAN_FAILTX         (2)
#define CAN_MSGAVAIL       (3)
#define CAN_NOMSG          (4)
#define CAN_CTRLERROR      (5)
#define CAN_GETTXBFTIMEOUT (6)
#define CAN_SENDMSGTIMEOUT (7)
#define CAN_FAIL       (0xff)

#define CAN_STD      0                                                  /* Standard IDs ONLY            */
#define CAN_EXT      1   

class ESP32_CAN
{

/*
*  esp32 CAN driver function 
*/
public:
    ESP32_CAN(void);
    INT8U begin(INT8U txpin,INT8U rxpin, INT8U speedset);       // Initilize controller prameters
    INT8U setMode(INT8U opMode);                                        // Set operational mode
    INT8U sendMsgBuf(INT32U id, INT8U ext, INT8U len, INT8U *buf);      // Send message to transmit buffer
    INT8U sendMsgBuf(INT32U id, INT8U len, INT8U *buf);                 // Send message to transmit buffer
    INT8U readMsgBuf(INT32U *id, INT8U *ext, INT8U *len, INT8U *buf);   // Read message from receive buffer
    INT8U readMsgBuf(INT32U *id, INT8U *len, INT8U *buf);               // Read message from receive buffer
    INT8U checkReceive(void);                                           // Check for received data
    INT8U checkError(void);                                             // Check for errors
    INT32U getError(void);                                               // Check for errors
    INT8U errorCountRX(void);                                           // Get error count
    INT8U errorCountTX(void);                                           // Get error count
};

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
