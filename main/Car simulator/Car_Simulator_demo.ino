#include <esp32_can.h>
#include <iso-tp-esp32.h>
#include <uds-esp32.h>

ESP32_CAN CAN0;
IsoTp isotp(&CAN0);
UDS uds(&isotp);
struct Session_t session;

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
const int IGN_pin = 21;
const int Auto_Simulator_pin = 19;

//함수
void AutoSimulator_OBD2(uint32_t Max, uint8_t bitarray[], int digit );
void AutoSimulator_UDS();
void hex_converter_8(uint32_t num, uint8_t bitarray[], int digit);

//PID Data Array
uint8_t pidSupport0x01To0x20[] = {0x08, 0x18, 0x00, 0x00};
uint8_t pidSupport0x81To0xA0[] = {0x00, 0x00, 0x00, 0x08};
uint8_t pidSupport0xA1To0xC0[] = {0x04, 0x00, 0x00, 0x00};
uint8_t pid0x05CoolantTemp[] = {0x78}; // 0x78 = 120 => 120 - 40 = 80 C
uint8_t pid0x0CEngineRpm[] = {0x1F, 0x40}; // 0x1F 40 = 8000 => 8000/ 4 = 2000 rpm
uint8_t pid0x0DVehicleSpeed[] = {0x64}; // 0x64 = 100 Km/h
uint8_t pid0x9DEngineFuelRate[] = {0x3C}; //0x3C=60L
uint8_t pid0xA6Odometer[] = {0x00, 0x01, 0x86, 0xA0}; //0x00 01 86 A0=100000  =>100000/10=10000km

//DTC Data Array
uint8_t P00BBFuelInjectorInsufficientFlow[] = {0x00, 0xBB};
uint8_t B0020LeftSideAirbagDeploymentControl[] = {0x80, 0x20};
uint8_t C0281BrakeSwitchCircuit[] = {0x42, 0x81};

//PID Struct
#define NUM_PIDS 8
struct {
  uint8_t pid;
  uint8_t* Data;
} responseData[NUM_PIDS] = { {0, pidSupport0x01To0x20},
  {0x05, pid0x05CoolantTemp},
  {0x0C, pid0x0CEngineRpm},
  {0x0D, pid0x0DVehicleSpeed},
  {0x80, pidSupport0x81To0xA0},
  {0x9D, pid0x9DEngineFuelRate},
  {0xA0, pidSupport0xA1To0xC0},
  {0xA6, pid0xA6Odometer}
};

//DTC Struct
#define NUM_DTCS 3
struct {
  int value;
  uint8_t* Data;
} responseDTC[NUM_PIDS] = { {0, P00BBFuelInjectorInsufficientFlow},
  {0, B0020LeftSideAirbagDeploymentControl},
  {0, C0281BrakeSwitchCircuit}
};



void setup() {
  // put your setup code here, to run once:
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
void loop() {
  uint8_t rxData[LEN_DATA];
  uint8_t txData[LEN_DATA];
  struct Session_t diag;
  uint16_t retval = 0;
  uint8_t pid;

  int IGN = digitalRead(IGN_pin);
  if (!IGN) {//IGN OFF
    Serial.println("IGN OFF");

  }
  else {//IGN ON
    Serial.println("IGN ON");
    int AutoSimul = digitalRead(Auto_Simulator_pin);

    if (!AutoSimul) {//Auto Simulator OFF
      Serial.println("Auto Simulator OFF");
      Serial.println("Odometer ");
      int reading_Odometer = analogRead(Odometer_pin);
      Serial.println(reading_Odometer);
      uint32_t Odometer = reading_Odometer * 488; //실질적인 값을 표현하기 위해
      Serial.println(Odometer);
      hex_converter_8(Odometer, pid0xA6Odometer , 8);
      Serial.println();

      // TODO : Need to consider fomular, min, max of PDI tabale.
      //temperature=A-40
      int reading_CoolantTemp = analogRead(CoolantTemp_pin);
      int CoolantTemp = map(reading_CoolantTemp, 0, 4095, 0, 255);
      Serial.println("Engine Temperature");
      hex_converter_8(CoolantTemp, pid0x05CoolantTemp , 2);
      Serial.println();

      //RPM =A-40
      int reading_EngineRpm = analogRead(EngineRpm_pin);
      uint16_t EngineRpm = map(reading_EngineRpm, 0, 4095, 0, 65535);
      Serial.println("Engine RPM");
      hex_converter_8(EngineRpm, pid0x0CEngineRpm , 4);
      Serial.println();

      int reading_VehicleSpeed = analogRead(VehicleSpeed_pin);
      int VehicleSpeed = map(reading_VehicleSpeed, 0, 4095, 0, 255);
      Serial.println("Vehicle Speed");
      hex_converter_8(VehicleSpeed, pid0x0DVehicleSpeed , 2);
      Serial.println();


      int reading_EngineFuelRate = analogRead(EngineFuelRate_pin);
      int EngineFuelRate = map(reading_EngineFuelRate, 0, 4095, 0, 60);
      Serial.println("Engine Fuel Rate");
      hex_converter_8(EngineFuelRate, pid0x9DEngineFuelRate , 2);
      Serial.println();


      responseDTC[1].value = digitalRead(Error_Code1_pin);
      responseDTC[2].value = digitalRead(Error_Code2_pin);
      responseDTC[3].value = digitalRead(Error_Code3_pin);
    }
    else {//Auto Simulator ON
      Serial.println("Auto Simulator ON");
      AutoSimulator_OBD2(2000000, pid0xA6Odometer, 8 );
      AutoSimulator_OBD2(255, pid0x05CoolantTemp, 2 );
      AutoSimulator_OBD2(65535, pid0x0CEngineRpm, 4 );
      AutoSimulator_OBD2(255, pid0x0DVehicleSpeed, 2 );
      AutoSimulator_OBD2(60, pid0x9DEngineFuelRate, 2 );
      AutoSimulator_UDS();
    }

    //Array에 data는 다 들어감
    //-> DATA 전송

    if (retval = uds.SessionServer(&diag))
    {
      Serial.println(F("No OBD request from tool."));
    }
    else
    {
      Serial.print(F("request SID: ")); Serial.println(diag.sid);

      //OBD2 DATA 전송
      if (diag.sid == OBD_MODE_SHOW_CURRENT_DATA) {

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

         //UDS 고장진단코드 전송
    if (diag.sid == OBD_MODE_READ_DTC) {
      Serial.println("UDS start...");
      int count = 0;// DTC 갯수

      for (int i = 0; i < NUM_DTCS; i++) {
        if (responseDTC[i].value) {
          count++;
        }
      }
      memset(txData, 0, 2 * count);
      if (count == 0) {//DTC가 하나도 없을 때
        Serial.println("No DTC");
        diag.Data = 0;
        diag.len  = 4;
        retval = uds.serverResponse(&diag);
      }
      else { //DTC 1개라도 있을떄
        diag.sid = diag.sid + 0x40; // Add 0x40 to received sid
        if (count == 1) {//DTC 1개
          Serial.println(" 1 DTC");
          for (int i = 0; i < NUM_DTCS; i++) {//on인 DTC 1개 찾아서 전송
            if (responseDTC[i].value) {
              uint8_t lenDTCValue = sizeof(responseDTC[i].Data);
              memcpy(txData, responseDTC[i].Data, lenDTCValue);
              diag.Data = txData;
              diag.len = 2 * count;
              retval = uds.serverResponse(&diag);
            }
          }
        }//DTC 1개 close






        else if (count == 2) {//DTC 2개
          Serial.println(" 2 DTC ");
          for (int i = 0; i < NUM_DTCS; i++) {
            if (responseDTC[i].value) {
              uint8_t lenDTCValue = sizeof(responseDTC[i].Data);
              memcpy(txData + 2 * i, responseDTC[i].Data, lenDTCValue);

            }
          }
          diag.Data = txData;
          diag.len = 4;
          retval = uds.serverResponse(&diag);
        }//DTC 2개 close


        else if (count == 3) {//DTC 3개
          Serial.println(" 3 DTC ");
          for (int i = 0; i < NUM_DTCS; i++) {
            uint8_t lenDTCValue = sizeof(responseDTC[i].Data);
            memcpy(txData + 2 * i, responseDTC[i].Data, lenDTCValue);

          }
          diag.Data = txData;
          diag.len = 6;
          retval = uds.serverResponse(&diag);

        }//DTC 3개 close

      }//DTC 1개라도 있을때 close
    }//sid=dtc close
    }
    delay(1000);



  }

}

void AutoSimulator_OBD2(uint32_t Max, uint8_t bitarray[], int digit ) {
  uint32_t value = random(0, Max);
  hex_converter_8(value, bitarray, digit);
}
void AutoSimulator_UDS() {
  for (int i = 0; i < NUM_DTCS; i++) {
    responseDTC[i].value = random(0, 2);
  }
}

void hex_converter_8(uint32_t num, uint8_t bitarray[], int digit) { //들어온 값 , 값을 분해해서 넣을 배열, 배열이 8bit로 몇개 구성되어 있는지
  int pos = digit;
  int temp[digit] = { 0 };    // 16진수로 된 문자열을 임시저장할 배열 (순서 제대로임)
  uint32_t decimal = num;
  Serial.println();
  for (int i = 0; i < pos; i++) {
    if (decimal < 16) {
      temp[pos - i - 1] = decimal;
    }
    temp[pos - i - 1] = decimal % 16;
    decimal = decimal / 16;
    Serial.print(temp[pos - i - 1]); Serial.print(" ");
  }

  Serial.print(" 0x");
  for (int j = 0; j < pos / 2; j++) {
    bitarray[j] = temp[2 * j] * 16 + temp[2 * j + 1]; //int로 잘려져있는 값들을 8비트 배열에 차곡차곡 넣어줌
    if (bitarray[j] < 16) {
      Serial.print("0");
    }
    Serial.print(bitarray[j], HEX); Serial.print(" ");
  }
}
