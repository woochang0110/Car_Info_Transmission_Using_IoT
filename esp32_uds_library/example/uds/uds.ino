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
	Serial.println("MCP2515 Library Receive Example...");	
  } else {
	Serial.println("Error Initializing ESP32 CAN..."); 
	return;	
  }
  delay(5000);
}

void loop()
{
  uint8_t ds[]={0x00};
  struct Session_t diag;
  uint16_t retval=0;

  Serial.println(F("Diag Session"));
  diag.tx_id=0x7E0;
  diag.rx_id=0x7E8;
  diag.sid=UDS_SID_DIAGNOSTIC_CONTROL;
  diag.Data=ds;
  diag.len=1;
  if(retval=uds.Session(&diag))
  {
    Serial.print(F("UDS Session Error "));
    Serial.print(retval); Serial.print(F(" NRC "));
    Serial.println(retval,HEX);
  }
  else
  {
    Serial.println(F("Established with "));
    uds.print_buffer(diag.Data,diag.len);
  }
  delay(1000);
}
