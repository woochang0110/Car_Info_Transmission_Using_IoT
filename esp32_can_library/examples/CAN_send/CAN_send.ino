// CAN Send Example
//

#include <esp32_can.h>

ESP32_CAN CAN0;  

void setup()
{
  Serial.begin(115200);

  // Initialize ESP32 CAN with a baudrate of 500kb/s and tx pin 16, rx pin 17.
  if(CAN0.begin(16,17,CAN_500KBPS) == CAN_OK) Serial.println("ESP32 CAN Initialized Successfully!");
  else Serial.println("Error Initializing ESP32 CAN...");

}

byte data[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

void loop()
{
  // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
  byte sndStat = CAN0.sendMsgBuf(0x100, CAN_STD, 8, data);
  if(sndStat == CAN_OK){
    Serial.println("Message Sent Successfully!");
  } else {
    Serial.println("Error Sending Message...");
  }
  delay(100);   // send data per 100ms
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/