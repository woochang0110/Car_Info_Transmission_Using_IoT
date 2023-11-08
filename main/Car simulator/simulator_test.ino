//가변저항
const int engine_temperature_pin = 34;
const int mileage_pin = 35;
const int rpm_pin = 32;
const int car_speed_pin = 33;
const int fuel_remaining_pin = 25;

//스위치
const int error_code1_pin = 5;
const int error_code2_pin = 22;
const int error_code3_pin = 18;
const int IGN_ON_pin = 21;
const int auto_simulator_pin = 19;

void setup() {
    Serial.begin(115200);
    
    pinMode(5, INPUT); // 고장코드 1
    pinMode(22, INPUT); // 고장코드 2
    pinMode(18, INPUT); // 고장코드 3
    pinMode(21, INPUT); // IGN ON
    pinMode(19, INPUT); //auto simulator
}

void loop() {
    int reading_engine_temperature = analogRead(engine_temperature_pin);
    int engine_temperature = map(reading_engine_temperature, 0,4095,0,100);
    Serial.println(String("engine_temperature_pin : ")+engine_temperature); // 0~100

    int reading_mileage = analogRead(mileage_pin);
    int mileage = map(reading_mileage, 0,4095,0,200000);
    Serial.println(String("mileage : ")+mileage);//0~20000

    int reading_rpm = analogRead(rpm_pin);
    int rpm = map(reading_rpm, 0,4095,0,8000);
    Serial.println(String("rpm_pin : ")+rpm); //0~8000

    int reading_car_speed = analogRead(car_speed_pin);
    int car_speed = map(reading_car_speed, 0,4095,0,240);
    Serial.println(String("car_speed : ")+car_speed); //0~240

    int reading_fuel_remaining = analogRead(fuel_remaining_pin);
    int fuel_remaining = map(reading_fuel_remaining, 0,4095,0,60);
    Serial.println(String("fuel_remaining : ")+fuel_remaining);

    int reaing_error_code1 = digitalRead(error_code1_pin);
    Serial.println(String("error_code1 : ")+reaing_error_code1);

    int rading_error_code2 = digitalRead(error_code2_pin);
    Serial.println(String("error_code2 : ")+rading_error_code2);

    int reading_error_code3 = digitalRead(error_code3_pin);
    Serial.println(String("error_code3 : ")+reading_error_code3);

    int reading_IGN_ON = digitalRead(IGN_ON_pin);
    Serial.println(String("IGN_ON : ")+reading_IGN_ON);

    int reaing_auto_simulator = digitalRead(auto_simulator_pin);
    Serial.println(String("auto_simulator_pin : ")+reaing_auto_simulator);
    
    delay(5000);


}
