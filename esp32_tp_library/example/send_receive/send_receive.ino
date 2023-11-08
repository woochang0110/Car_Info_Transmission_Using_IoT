#include <iso-tp-esp32.h>

ESP32_CAN CAN0;
IsoTp isotp(&CAN0);
/*
{To ArbID} 0x10 0x18 0x01 0x02 0x03 0x04 0x05 0x06  – First Frame
{From ArbID} 0x30 0x00 0x00 – Flow Control Frame
{To ArbID} 0x21 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D – Consecutive Frame
{To ArbID} 0x22 0x0E 0x0F 0x10 0x11 0x12 0x13 0x14 – Consecutive Frame
{To ArbID} 0x23 0x15 0x16 0x17 0x18 0xAA 0xAA 0xAA – Consecutive Frame
*/

struct Message_t TxMsg, RxMsg;
INT8U sf_test[] = { 0x00, 0x01 };
INT8U mf_test[] = { 0x10, 0x18, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, \
                    0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18};
INT32U can_tx_id = 0x7E0;
INT32U can_rx_id = 0x7F0;

void setup()
{
  Serial.begin(115200);
  // Initialize ESP32 CAN with a baudrate of 500kb/s and tx pin 16, rx pin 17.
  if(CAN0.begin(16,17,CAN_500KBPS) == CAN_OK) Serial.println("ESP32 CAN Initialized Successfully!");
  else Serial.println("Error Initializing ESP32 CAN...");
}

void loop()
{ 
  TxMsg.len=sizeof(mf_test);
  TxMsg.Buffer=(INT8U *)calloc(MAX_MSGBUF,sizeof(INT8U));
  TxMsg.tx_id=can_tx_id;
  TxMsg.rx_id=can_rx_id;
  memcpy(TxMsg.Buffer,mf_test,sizeof(mf_test));
  Serial.println(F("Send..."));
  isotp.send(&TxMsg);
  
  RxMsg.tx_id=can_tx_id;
  RxMsg.rx_id=can_rx_id;
  RxMsg.Buffer=(INT8U *)calloc(MAX_MSGBUF,sizeof(INT8U));
  Serial.println(F("Receive..."));
  isotp.receive(&RxMsg);
  isotp.print_buffer(RxMsg.rx_id, RxMsg.Buffer, RxMsg.len);
}
