#include "arduino_secrets.h"
#include "thingProperties.h"
#include "DHT.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define DHTPIN 13
#define DHTTYPE DHT11
DHT sensor(DHTPIN, DHTTYPE);
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
bool button = true;
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
String data;
void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  pinMode(12, INPUT_PULLUP);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  sensor.begin();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  initProperties();
  display.setCursor(0, 0);
  display.print("Connecting...");
  display.display();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  delay(2000);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Connected");
  display.display();
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  temperature = sensor.readTemperature();
  humidity = sensor.readHumidity();
  float temperaturef = sensor.readTemperature();
  float humidityf = sensor.readHumidity();
  heat_Index = sensor.computeHeatIndex(temperaturef, humidityf, false);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("Data Strings");
  display.setCursor(50, 20);
  if (ArduinoCloud.connected() == 0) {
    display.print("Disconnected");
  } else {
    display.print("Connected");
  }
  display.setCursor(0, 20);
  bool button_value = digitalRead(12);
  Serial.print(button_value);
  Serial.print(" ");
  Serial.println(button);
  if (button_value == 0 )
  {
    button = !button;
    delay(1000);
  }
  if (button == true)
  { data = Serial.readString();
    if (data.isEmpty() != 1) {
      display.println(data);
      display.display();
      int spaceIndex = data.indexOf(' ');
      if (spaceIndex != -1) {
        String firstNumberString = data.substring(0, spaceIndex);
        String secondNumberString = data.substring(spaceIndex + 1);
        smoke = firstNumberString.toInt();
        co = secondNumberString.toInt();
      }
    } else {
      display.println("##");
      display.display();
    }
  }
  else{
    data="";
    data+=temperaturef;
    data+=" ";
    data+=int(humidityf);
    display.print(data);
    display.display();
  }
}
