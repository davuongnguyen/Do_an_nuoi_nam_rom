#include <Arduino.h>
#include "DHT.h"
#include "Adafruit_Sensor.h"
#include "DHT_U.h"
#include "Wire.h" 
#include "LiquidCrystal_I2C.h"

#pragma region Định nghĩa từ khóa
#define DHTTYPE DHT11

#define Soil_Sensor A1
#define DHTPIN 2

#define Button_mode 4
#define Button_light 5
#define Button_fan 6
#define Button_pump 7

#define Light 8
#define Fan 9
#define Pump 10

#pragma endregion

#pragma region Khởi tạo
// Khởi tạo màn hình LCD 16x2 và giao tiếp i2c ở địa chỉ 0x3F
LiquidCrystal_I2C lcd(0x27,16,2); 

// Khởi tạo cảm biến DHT
DHT dht(DHTPIN, DHTTYPE);

const byte INPUT_Digital[] = {Button_mode, Button_light, Button_fan, Button_pump};
const byte OUTPUT_PIN[] = {Light, Fan, Pump};


unsigned long time_Reconnect = 0, time_sensor = 0, time_button = 0;

// Khởi tạo biến lưu trữ nhiệt độ, độ ẩm ngoài trời
float hum = 0, temp = 0;

// Khởi tạo biến lưu độ ẩm đất và % độ ẩm đất
int value = 0, soil_moisture = 0;

// Lưu trạng thái đầu ra: đèn, quạt, máy bơm, mode
// 0 - bật
// 1 - tắt
// Với mode: 0 - manual; 1 - auto
byte state[] = {0, 0, 0, 0};

#pragma endregion

#pragma region Nguyên mẫu các hàm
// Đọc dữ liệu từ cảm biến
void ReadSensor();

// Đọc dữ liệu từ các nút
void ReadButton();

// Xử lý dữ liệu đã nhận
void Process();

// Hiển thị lên màn hình LCD
void Display();

#pragma endregion

void setup() 
{
  // put your setup code here, to run once:
  for (byte i = 0; i < 4; i++)
  {
    pinMode(INPUT_Digital[i], INPUT_PULLUP);
  }

  for (byte k = 0; k < 3; k++)
  {
    pinMode(OUTPUT_PIN[k], OUTPUT);
    digitalWrite(OUTPUT_PIN[k], state[k]);
  }

  //Thiết lập tốc độ baud rate cho serial monitor
  Serial.begin(115200);

  // Khởi tạo LCD
  lcd.init();  
  lcd.backlight(); 

  // Bắt đầu thuật toán giải mã dữ liệu từ cảm biến dth
  dht.begin();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  ReadSensor();
  
  ReadButton();

  Process();
  
  Display();

}

void ReadSensor()
{
  // Đợi một khoảng thời gian giữa các lần đọc dữ liệu
  if ((unsigned long)(millis() - time_sensor) > 1000)
  {
    time_sensor = millis();

    // Đọc dữ liệu từ cảm biến nhiệt độ độ ẩm DHT11
    hum = dht.readHumidity();
    temp = dht.readTemperature();

    // Nếu nhiệt độ hoặc độ ẩm là nan (Not a number)
    if (isnan(hum) || isnan(temp))
    {
      Serial.println(F("Đọc dữ liệu từ cảm biến thất bại!"));
      return;
    }

    // Đọc dữ liệu từ cảm biến độ ẩm đất
    value = analogRead(Soil_Sensor);     // Ta sẽ đọc giá trị hiệu điện thế của cảm biến
    soil_moisture = map(value, 0, 1023, 0, 100);
  }
}

void ReadButton()
{
  // Đọc lần lượt trạng thái 4 nút bấm
  // Chuyển trạng thái biến lưu state nếu có sự thay đổi về nút bấm
  // sau đó sẽ delay tạm thời 500ms để chống dội
  for (int i = 0; i < 4; i++)
  {
    if (digitalRead(INPUT_Digital[i]) == LOW)
    {
      Serial.println("Đã nhấn ...");
      state[i] = !state[i];
      delay(500);
    }
  }
}

void Process()
{
  if(state[3] == 0)
  {
    for (byte i = 0; i < 3; i++)
      digitalWrite(OUTPUT_PIN[i], state[i]);
  }
  else
  {
    // Nếu nhiệt độ dưới 25 độ C, bật đèn đến khi nhiệt độ lên trên 30 độ thì tắt
    if(temp < 25)
      digitalWrite(Light, HIGH);
    if(temp > 30)
      digitalWrite(Light, LOW);

    // Nếu nhiệt độ trên 40 độ C, bật quạt đến khi nhiệt độ dưới 30 độ thì tắt
    if(temp > 40)
      digitalWrite(Fan, HIGH);
    if(temp <= 30)
      digitalWrite(Fan, LOW);
    
    // Nếu độ ẩm đất dưới 60%, bơm nước đến khi độ ẩm lên trên 75% thì tắt
    if(soil_moisture < 60)
      digitalWrite(Fan, HIGH);
    if(soil_moisture > 75)
      digitalWrite(Fan, LOW);
  }

}

void Display()
{
  lcd.setCursor(0,0);
  if(state[3] == 0)
    lcd.print("Mode: manual");
  else
    lcd.print("Mode:   auto");

  lcd.setCursor(0,1);
  lcd.print(int(temp));
  lcd.setCursor(2,1);
  lcd.print("C");
  lcd.setCursor(4,1);
  lcd.print(int(hum));
  lcd.setCursor(6,1);
  lcd.print("%");
  lcd.setCursor(8,1);
  lcd.print("Dat:");
  lcd.setCursor(12,1);
  lcd.print(int(soil_moisture));
  lcd.setCursor(15,1);
  lcd.print("%");
}