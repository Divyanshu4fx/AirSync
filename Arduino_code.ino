#include <Wire.h>
#include "DFRobot_RGBLCD1602.h"

DFRobot_RGBLCD1602 lcd(0x60, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.setCursor(3, 0);
  lcd.print("Starting....");
  Serial.println("Starting....");
  delay(1000);
}

void loop() {
  int smoke = analogRead(A0);
  smoke = map(smoke, 0, 1023, 40, 800.0);
  int co = analogRead(A1);
  co = map(co, 0, 1023, 0.1, 10.0);
  
  Serial.print(smoke);
  Serial.print(" ");
  Serial.println(co);
  delay(500);

  lcd.setCursor(0, 0);
  lcd.print("Air Q ");
  lcd.print(smoke);
  lcd.print(" ppm");
  
  lcd.setCursor(0, 1);
  lcd.print("CO ");
  lcd.print(co);
  lcd.print(" ppm");
}
