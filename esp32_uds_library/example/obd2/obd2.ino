#include <esp32_can.h>
#include <iso-tp-esp32.h>
#include <uds-esp32.h>


ESP32_CAN CAN0;
IsoTp isotp(&CAN0);
UDS uds(&isotp);

struct Session_t session;

void setup()
{
  Serial.begin(115200);
  // Initialize ESP32 CAN with a baudrate of 500kb/s and tx pin 16, rx pin 17.
  if(CAN0.begin(16,17,CAN_500KBPS) == CAN_OK){
	Serial.println("ESP32 CAN Initialized Successfully!");
  } else {
	Serial.println("Error Initializing ESP32 CAN..."); 
	return;	
  }
  delay(5000);
}

void loop()
{
  uint8_t pid=0x00;
  struct Session_t diag;
  uint16_t retval=0;

  Serial.println(F("OBD2 request PIDs: "));
  diag.tx_id=0x7E0;
  diag.rx_id=0x7E8;
  diag.sid=OBD_MODE_SHOW_CURRENT_DATA;
  diag.Data=&pid;
  diag.len=1;
  if(retval=uds.Session(&diag))
  {
    Serial.print(F("OBD2 Error "));
    Serial.print(retval); Serial.print(F(" NRC "));
    Serial.println(retval,HEX);
  }
  else
  {
    uint32_t pids=(uint32_t)diag.Data[0]<<24 | (uint32_t)diag.Data[1]<<16 | (uint32_t)diag.Data[2]<<8 | (uint32_t)diag.Data[3];

    for(uint8_t i=0; i<32;i++) //4 Bytes PIDs as bitfield
    { 
      Serial.print(F("PID ")); Serial.print(32-i);
      if(pids & ((uint32_t)1<<i)) Serial.println(F(" yes")); else Serial.println(F(" no"));
    }
  }
  delay(1000);
}
