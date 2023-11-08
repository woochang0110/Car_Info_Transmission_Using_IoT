// CAN Send Example
//

#include <esp32_can.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
#define TX_CYCLE_TIME 1000
unsigned long txCycleTime;

ESP32_CAN CAN0;  

void setup()
{
  Serial.begin(115200);

  // Initialize ESP32 CAN with a baudrate of 500kb/s and tx pin 16, rx pin 17.
  if(CAN0.begin(16,17,CAN_500KBPS) == CAN_OK) Serial.println("ESP32 CAN Initialized Successfully!");
  else Serial.println("Error Initializing ESP32 CAN...");

  // Initialize Tx cycle time with current system time.
  txCycleTime = millis();
}

byte data[8] = {0x02, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00};

void loop()
{
  // read current system time
  unsigned long systemTime = millis();
  
  if((systemTime - txCycleTime) >= TX_CYCLE_TIME)
  {
    // Save system time for next CAN Tx
    txCycleTime = systemTime;
    
    // send data:  ID = 0x7DF, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
    byte sndStat = CAN0.sendMsgBuf(0x7DF, CAN_STD, 8, data);
    if(sndStat == CAN_OK){
      Serial.println("Message Sent Successfully!");
    } else {
      Serial.println("Error Sending Message...");
    }
  }

  // receive data 
  if(CAN0.checkReceive() == CAN_MSGAVAIL) 
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);              // Read data: len = data length, buf = data byte(s)

    Serial.print("ID: ");                     // Print the message ID.
    Serial.print(rxId, HEX);

    
    Serial.print(" len: ");                     // Print the message length.
    Serial.print(len);

    Serial.print("  Data: ");
    for(int i = 0; i<len; i++)                // Print each byte of the data.
    {
      if(rxBuf[i] < 0x10)                     // If data byte is less than 0x10, add a leading zero.
      {
        Serial.print("0");
      }
      Serial.print(rxBuf[i], HEX);
      Serial.print(" ");
    }

    Serial.println();
  }  
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
