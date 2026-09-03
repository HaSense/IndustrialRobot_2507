// Sketch에서 LiquidCrystal I2C by frank de Braander 모듈 받아서 삽입하기


// I2C 통신을 위한 라이브러리
#include <Wire.h>
// I2C 방식 LCD 제어 라이브러리
#include <LiquidCrystal_I2C.h>

// LCD 객체 생성
// 0x27 : LCD의 I2C 주소
// 16, 2 : 16글자 × 2줄 LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

  // LCD 초기화
  lcd.init();

  // LCD 백라이트 켜기
  lcd.backlight();

  // 첫 번째 줄의 첫 번째 위치로 커서 이동
  lcd.setCursor(0, 0);
  lcd.print("Hello Arduino LCD Test!");

  // 두 번째 줄의 첫 번째 위치로 커서 이동
  lcd.setCursor(0, 1);
  lcd.print("R4 Minima");
}

void loop() {
  // 반복해서 수행할 작업 없음
}
