# include <esp32_can.h>
# include <iso-tp-esp32.h>
# include <uds-esp32.h>
# include <WiFi.h>
# include <PubSubClient.h>

# define MSG_BUFFER_SIZE (50)
# define BUILTIN_LED 25

ESP32_CAN CAN0;
IsoTp isotp(&CAN0);
UDS uds(&isotp);

struct Session_t session;

char msg[MSG_BUFFER_SIZE];
const char* ssid = "Hong";
const char* password = "123456789q";
const char* mqtt_server = "180.71.2.76";
uint8_t tmp[] = {};
String packet;
long lastMsg = 0;

// 웹에 나타나는 정보 변수 초기화
int IGN = 0;
int RPM = 0;
int D = 0;
int V = 0;
int Eng_temp = 0;
int Fuel_level = 0;

WiFiClient esp_client;
PubSubClient client(esp_client);

// 사용되는 함수 초기화
void displayWifistatus(void);
void OBD2_receiver(uint8_t pid);
void UDS_receiver(void);
long data_process(int count, uint8_t *array_pointer);
void mqtt_trans(int IGN, int (RPM), double D, int V, float Eng_temp, int Fuel_level);
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  //1. wifi연결
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    displayWifistatus();
    delay(500);
  }
  displayWifistatus();
  Serial.println();
  Serial.print("Conncected, IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());

  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());

  //2. Mqtt 연결
  pinMode(BUILTIN_LED, OUTPUT);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  //3. CAN 연결
  if (CAN0.begin(16, 17, CAN_500KBPS) == CAN_OK) {        //25,50,100,125,250,500,800,1000
    Serial.println("ESP32 CAN Initialized Successfully!");
  } else {
    Serial.println("Error Initializing ESP32 CAN...");
    return;
  }
  delay(5000);
}

void loop() {
  uint8_t pid[] = {0x1F, 0x05, 0x0C, 0x0D, 0x9D, 0xA6};
  uint8_t uds_id[] = {};
  //1. OBD2로 Data receive
  for (int i = 0; i < sizeof(pid) / sizeof(uint8_t) ; i++)
    OBD2_receiver(pid[i]);
  //2. UDS로 Data receive
  UDS_receiver();
  if (RPM != 0)   //엔진이 켜져있다면
    IGN = 1;      //IGN을 True로
  else            //엔진이 꺼져있다면
    IGN = 0;      //IGN을 False로
  if (IGN == 1);    //3. 시동이 켜져있다면
    mqtt_trans(IGN);   //4. MQTT를 이용해 서버로 전송
}

void displayWifistatus(void) {
  Serial.print("ESP WiFi connection status: ");

  switch (WiFi.status()) {
    case 0:
      Serial.println("WL_IDLE_STATUS");
      break;

    case 1:
      Serial.println("WL_NO_SSID_AVAIL");
      break;

    case 3:
      Serial.println("WL_CONNECTED");
      break;

    case 4:
      Serial.println("WL_CONNECT_FAILED");
      break;

    case 6:
      Serial.println("WL_DISCONNECTED");
      break;

    default:
      Serial.println("Unknown status");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("start", "Connected Mqtt...");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void OBD2_receiver(uint8_t pid) {
  int num = 0;
  int count = 0;
  long f_result = 0;
  struct Session_t diag;
  uint16_t retval = 0;
  Serial.print(F("OBD2 request PIDs: ")); Serial.println(pid);
  for (int i = 0; i < 1; i++) {
    uint16_t rx_id_array[] = {0x7E9, 0x7E9};
    diag.tx_id = 0x7DF; //0x7E0, 0x7DF  -> 7DF였으면 원래 응답이 와야하는대 안됐음.
    //                 예제 하나 줄테니 실제 차량에서 진행해보자.
    //
    diag.rx_id = rx_id_array[i];
    Serial.print("tx_id : "); Serial.println(diag.tx_id);
    Serial.print("rx_id : "); Serial.println(diag.rx_id);
    //diag.rx_id = 0x7E8;
    diag.sid = OBD_MODE_SHOW_CURRENT_DATA;
    diag.Data = &pid;
    diag.len = 1;

    if (retval = uds.Session(&diag))
    {
      Serial.print(F("OBD2 Error "));
      Serial.print(retval);
      Serial.print(F(" NRC "));
      Serial.println(retval, HEX);
    }
    else
    {
      //0x1F, 0x05, 0x0C, 0x0D, 0x9D, 0xA6
      //pid별 최대 바이트로 자르는 코드 시작
      //1.각 pid별로 몇바이트인지 count변수에 저장
      switch (pid) {
        case 0x05:
          count = 1;
          //2. count만큼 데이터를 자르고 저장 tmp배열에 임시 저장
          for (int i = 0; i < count; i++) {
            tmp[i] = diag.Data[i];
          }
          //3. 데이터 후처리
          Eng_temp = data_process(count, tmp) - 40;
          Serial.println("엔진 온도");
          Serial.println(Eng_temp);
          break;
        case 0x0C:
          count = 2;
          //2. count만큼 데이터를 자르고 저장 tmp배열에 임시 저장
          for (int i = 0; i < count; i++) {
            tmp[i] = diag.Data[i];
          }
          //3. 데이터 후처리
          RPM = data_process(count, tmp) / 4;
          Serial.println("RPM");
          Serial.println(RPM);
          break;
        case 0x0D:
          count = 1;
          //2. count만큼 데이터를 자르고 저장 tmp배열에 임시 저장
          for (int i = 0; i < count; i++) {
            tmp[i] = diag.Data[i];
          }
          //3. 데이터 후처리
          V = data_process(count, tmp);
          Serial.println("차량 속력");
          Serial.println(V);
          break;
        case 0x9D:
          count = 1;
          //2. count만큼 데이터를 자르고 저장 tmp배열에 임시 저장
          for (int i = 0; i < count; i++) {
            tmp[i] = diag.Data[i];
          }
          //3. 데이터 후처리
          Fuel_level = data_process(count, tmp);
          Serial.println("연료 잔량");
          Serial.println(Fuel_level);
          break;
        case 0xA6:
          count = 4;
          //2. count만큼 데이터를 자르고 저장 tmp배열에 임시 저장
          for (int i = 0; i < count; i++) {
            tmp[i] = diag.Data[i];
          }
          //3. 데이터 후처리
          D = data_process(count, tmp) / 10;
          Serial.println("총 주행거리");
          Serial.println(D);
          break;
        default:
          Serial.println("미구현");
          break;
      }
    }
    Serial.print("OBD code : "); Serial.print(pid); Serial.println(" end");
    Serial.println("");
    delay(1000);
  }
}

void UDS_receiver(void) {
  int num = 0;
  uint8_t pid = 0x00;  //pid 의미없음
  struct Session_t diag;
  uint16_t retval = 0;

  Serial.println(F("Diag Session"));
  for (int i = 0; i < 8; i++) {
    uint16_t rx_id_array[] = {0x7E8, 0x7E9, 0x7EA, 0x7EB, 0x7EC, 0x7ED, 0x7EE, 0x7EF};
    diag.tx_id = 0x7DF; //0x7E0, 0x7DF
    diag.rx_id = rx_id_array[i];
    Serial.print("tx_id : "); Serial.println(diag.tx_id);
    Serial.print("rx_id : "); Serial.println(diag.rx_id);
    //diag.tx_id = 0x7E0;
    //diag.rx_id = 0x7E8;
    diag.sid = OBD_MODE_READ_DTC;
    diag.Data = &pid;         //진단 데이터 수신
    diag.len = 0;             // before 1
    if (retval = uds.Session(&diag))
    {
      Serial.print(F("UDS Session Error "));
      Serial.print(retval); Serial.print(F(" NRC "));
      Serial.println(retval, HEX);
    }
    else
    {
      Serial.println(F("Established with "));
      uds.print_buffer(diag.Data, diag.len);
      Serial.print("Length : "); Serial.println(diag.len);
    }
    Serial.print("UDS code"); Serial.print(num); Serial.println(" end...");
    delay(1000);
  }
}

long data_process(int count, uint8_t *array_pointer) {
  long result = 0;
  //3. 데이터 후처리 시작
  if (count != 1) //0,2,4,8씩 제곱
  {
    if (count == 2) //0~2
    {
      result = tmp[0] * pow(16, 2) + tmp[1];
      //Serial.print("count is 2 : "); Serial.println(result);
    }
    else if (count == 3) //0~4
    {
      result = tmp[0] * pow(16, 4) + tmp[1] * pow(16, 2) + tmp[2];
      //Serial.print("count is 3 : "); Serial.println(result);
    }
    else if (count == 4) //0~8
    {
      result = tmp[0] * pow(16, 8) + tmp[1] * pow(16, 4) + tmp[2] * pow(16, 2) + tmp[3];
      //Serial.print("count is 4 : "); Serial.println(result);
    }
  }
  else {
    result = tmp[0];
    //Serial.print("count is 1 : "); Serial.println(tmp[0]);
  }
  return result;
}

void mqtt_trans(int IGN, int RPM, int D, int V, int Eng_temp, int Fuel_level) {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  unsigned long now = millis();
  if (now - lastMsg > 500) {
    lastMsg = now;
    packet = String(IGN)+" "+String(RPM)+" "+String(D)+" "+String(V)+" "+String(Eng_temp)+" "+String(Fuel_level);
    packet.toCharArray(msg, 50);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("Env_car", msg);
  }
}
