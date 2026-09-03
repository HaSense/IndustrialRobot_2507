// Sketch에서 LiquidCrystal I2C by frank de Braander 모듈 받아서 삽입하기


#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Hello Arduino LCD Test!");

  lcd.setCursor(0, 1);
  lcd.print("R4 Minima");
}

void loop() {
}
