#include <esp32_can.h>
#include <iso-tp-esp32.h>
#include <uds-esp32.h>

ESP32_CAN CAN0;
IsoTp isotp(&CAN0);
UDS uds(&isotp);

struct Session_t session;

uint8_t pidSupport0x01To0x20[]={0x08,0x18,0x00,0x00};
uint8_t pid0x05CoolantTemp[]={0x78}; // 0x78 = 120 => 120 - 40 = 80 C 
uint8_t pid0x0CEngineRpm[]={0x1F,0x40}; // 0x1F40 = 8000 => 8000/ 4 = 2000 rpm
uint8_t pid0x0DVehicleSpeed[]={0x64}; // 0x64 = 100 Km/h

#define NUM_PIDS 4
struct {
  uint8_t pid;
  uint8_t* Data;  
} responseData[NUM_PIDS]= {{0x00,pidSupport0x01To0x20},{0x05,pid0x05CoolantTemp},{0x0C,pid0x0CEngineRpm},{0x0D,pid0x0DVehicleSpeed}};

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

#define LEN_DATA 8

void loop()
{
  uint8_t rxData[LEN_DATA];
  uint8_t txData[LEN_DATA];
  struct Session_t diag;
  uint16_t retval=0;
  uint8_t pid;
  

  Serial.println(F("OBD2 ECU server: "));
  diag.tx_id=0x7E8;
  diag.rx_id=0x7E0;
  diag.Data=rxData;
  if(retval=uds.SessionServer(&diag))
  {
    Serial.println(F("No OBD request from tool."));
  }
  else
  {
    Serial.print(F("request SID: "));Serial.println(diag.sid); 
    pid = diag.Data[0];
    Serial.print(F("request PID: "));Serial.println(pid); 
    Serial.print(F("request data: "));    
    for(uint8_t i=1; i<diag.len;i++) 
    { 
      Serial.print(diag.Data[i]);Serial.print(F(" ")); 
    }
    Serial.println();    

    memset(txData,0,LEN_DATA);
    for(uint8_t i=0; i<NUM_PIDS; i++)
    {
      if(responseData[i].pid == pid) // Find the required pid data
      {
        diag.sid = diag.sid + 0x40; // Add 0x40 to received sid 
        txData[0] = pid;
        uint8_t lenPidValue = sizeof(responseData[i].Data);
        memcpy(txData+1,responseData[i].Data,lenPidValue);
        diag.Data=txData;
        diag.len = lenPidValue + 1;
        retval = uds.serverResponse(&diag);
      }
    }    
  }
  delay(1000);
}
