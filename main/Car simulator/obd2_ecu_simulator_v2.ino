#include <esp32_can.h>
#include <iso-tp-esp32.h>
#include <uds-esp32.h>

ESP32_CAN CAN0;
IsoTp isotp(&CAN0);
UDS uds(&isotp);

//가변저항
const int CoolantTemp_pin = 34;
const int Odometer_pin = 35;
const int EngineRpm_pin = 32;
const int VehicleSpeed_pin = 33;
const int EngineFuelRate_pin = 25;

//스위치
const int Error_Code1_pin = 5;
const int Error_Code2_pin = 22;
const int Error_Code3_pin = 18;
const int IGN_ON_pin = 21;
const int Auto_Simulator_pin = 19;

struct Session_t session;
//8비트로 잘라서 배열에 넣어주는 함수
void hex_converter_8(int num, uint8_t bitarray[], int bitnum); //들어온 값 , 값을 분해해서 넣을 배열, 배열이 8bit로 몇개 구성되어 있는지


uint8_t pidSupport0x01To0x20[] = {0x08, 0x18, 0x00, 0x00};
uint8_t pidSupport0x81To0xA0[] = {0x00, 0x00, 0x00, 0x08};
uint8_t pidSupport0xA1To0xC0[] = {0x04, 0x00, 0x00, 0x00};
uint8_t pid0x05CoolantTemp[] = {0x78}; // 0x78 = 120 => 120 - 40 = 80 C
uint8_t pid0x0CEngineRpm[] = {0x1F, 0x40}; // 0x1F40 = 8000 => 8000/ 4 = 2000 rpm
uint8_t pid0x0DVehicleSpeed[] = {0x64}; // 0x64 = 100 Km/h
uint8_t pid0x9DEngineFuelRate[] = {0x3C}; //0x3C=60L
uint8_t pid0xA6Odometer[] = {0x01, 0x86, 0xA0}; //0x186A0=100000km

//ign on 체크 변수 일단 스위치라 이렇게 작성 나중에 똑딱이 달면 코드 변경필요함
int Ign_index = 0;
boolean Ign = false;

#define NUM_PIDS 8
struct {
  uint8_t pid;
  uint8_t* Data;
} responseData[NUM_PIDS] = {{0x00, pidSupport0x01To0x20},
							{0x05, pid0x05CoolantTemp},
							{0x0C, pid0x0CEngineRpm},
							{0x0D, pid0x0DVehicleSpeed},
							{0x80, pidSupport0x81To0xA0},
							{0x9D, pid0x9DEngineFuelRate},
							{0xA0, pidSupport0xA1To0xC0},
							{0xA6, pid0xA6Odometer}
							};

void setup()
{
  Serial.begin(115200);
  // Initialize ESP32 CAN with a baudrate of 500kb/s and tx pin 16, rx pin 17.
  if (CAN0.begin(16, 17, CAN_500KBPS) == CAN_OK) {
    Serial.println("ESP32 CAN Initialized Successfully!");
  } else {
    Serial.println("Error Initializing ESP32 CAN...");
    return;
  }
  pinMode(5, INPUT); // 고장코드 1
  pinMode(22, INPUT); // 고장코드 2
  pinMode(18, INPUT); // 고장코드 3
  pinMode(21, INPUT); // IGN ON
  pinMode(19, INPUT); //auto simulator
  delay(5000);
}

#define LEN_DATA 8

void loop()
{
  uint8_t rxData[LEN_DATA];
  uint8_t txData[LEN_DATA];
  struct Session_t diag;
  uint16_t retval = 0;
  uint8_t pid;


  Serial.println(F("OBD2 ECU server: "));
  diag.tx_id = 0x7E8;
  diag.rx_id = 0x7E0;
  diag.Data = rxData;

  //IGN ON 체크
  //일단 스위치라 이렇게 작성 나중에 똑딱이 달면 코드 변경필요함
  // TODO : 스위치가 변경되었을 때만 Ign_index 증가시키려면 스위치 이전상태를 저장해서 현재값과 비교한 후 이전값이 0이고 현재값이 1일때만 증가시킴. 
  int reading_IGN_ON = digitalRead(IGN_ON_pin);
  if (reading_IGN_ON == 1) { //ign버튼 눌렸을 때
    Ign_index++;
  }
  //버튼을 스위치로 바꿔주는 과정
  if (Ign_index % 2 == 0) { //안켜졌을 때
    return;
  }
  else if (Ign_index % 2 == 1) { //켜졌을 때
    Ign = true;
    Serial.println(String("IGN_ON "));

  }

  //여기까지 ign on 체크 루틴



  //IGN이 ON됬으므로 자동차 정보 값들 배열에 저장
  int reading_Odometer = analogRead(Odometer_pin);
  int Odometer = map(reading_Odometer, 0, 4095, 0, 200000);
  Serial.println("Odometer");
  hex_converter_8(Odometer,pid0xA6Odometer , 6);
  Serial.println();
  
  // TODO : Need to consider fomular, min, max of PDI tabale.
  int reading_CoolantTemp = analogRead(CoolantTemp_pin);
  int CoolantTemp = map(reading_CoolantTemp, 0, 4095, 0, 100);
  Serial.println("Engine Temperature");
  hex_converter_8(CoolantTemp,pid0x05CoolantTemp , 2);
  Serial.println();

  
  int reading_EngineRpm = analogRead(EngineRpm_pin);
  int EngineRpm = map(reading_EngineRpm, 0, 4095, 0, 8000);
  Serial.println("Engine RPM");
  hex_converter_8(EngineRpm,pid0x0CEngineRpm , 4);
  Serial.println();

  int reading_VehicleSpeed = analogRead(VehicleSpeed_pin);
  int VehicleSpeed = map(reading_VehicleSpeed, 0, 4095, 0, 240);
  Serial.println("Vehicle Speed");
  hex_converter_8(VehicleSpeed,pid0x0DVehicleSpeed , 2);
  Serial.println();

  
  int reading_EngineFuelRate = analogRead(EngineFuelRate_pin);
  int EngineFuelRate = map(reading_EngineFuelRate, 0, 4095, 0, 60);
  Serial.println("Engine Fuel Rate");
  hex_converter_8(EngineFuelRate,pid0x9DEngineFuelRate , 2);
  Serial.println();

  int reading_Error_Code1 = digitalRead(Error_Code1_pin);
  int reading_Error_Code2 = digitalRead(Error_Code2_pin);
  int reading_Error_Code3 = digitalRead(Error_Code3_pin);
  int reading_Auto_Simulator = digitalRead(Auto_Simulator_pin);

  // responseData[NUM_PIDS] = {{0x00, pidSupport0x01To0x20}, {0x05, pid0x05CoolantTemp}, {0x0C, pid0x0CEngineRpm}, {0x0D, pid0x0DVehicleSpeed}, {0x9D, pid0x9DEngineFuelRate}, {0xA6, pid0xA6Odometer}};

  // hex_converter_8(int num, uint8_t bitarray[], int bitnum);





  if (retval = uds.SessionServer(&diag))
  {
    Serial.println(F("No OBD request from tool."));
  }
  else
  {
    Serial.print(F("request SID: ")); Serial.println(diag.sid);
    pid = diag.Data[0];
    Serial.print(F("request PID: ")); Serial.println(pid);
    Serial.print(F("request data: "));
    for (uint8_t i = 1; i < diag.len; i++)
    {
      Serial.print(diag.Data[i]); Serial.print(F(" "));
    }
    Serial.println();

    memset(txData, 0, LEN_DATA);
    for (uint8_t i = 0; i < NUM_PIDS; i++)
    {
      if (responseData[i].pid == pid) // Find the required pid data
      {
        diag.sid = diag.sid + 0x40; // Add 0x40 to received sid
        txData[0] = pid;
        uint8_t lenPidValue = sizeof(responseData[i].Data);
        memcpy(txData + 1, responseData[i].Data, lenPidValue);
        diag.Data = txData;
        diag.len = lenPidValue + 1;
        retval = uds.serverResponse(&diag);
      }
    }
  }
  delay(1000);
}



void hex_converter_8(int num, uint8_t bitarray[], int bitnum) { //들어온 값 , 값을 분해해서 넣을 배열, 배열이 8bit로 몇개 구성되어 있는지
  int pos = bitnum;
  int temp[bitnum] = { 0 };    // 16진수로 된 문자열을 임시저장할 배열 (순서 제대로임)
  int decimal = num;
  
  for(int i=0; i<pos;i++){
    if(decimal<16){
      temp[pos-i-1]=decimal;
    }
    temp[pos-i-1]=decimal%16;
    decimal=decimal/16;
  }
  for (int j = 0; j < pos/ 2; j++) {
      bitarray[j] = temp[2 * j] * 16 + temp[2 * j + 1]; //int로 잘려져있는 값들을 8비트 배열에 차곡차곡 넣어줌
        Serial.print(" 0x ");Serial.print(bitarray[j],HEX);
  }
}
