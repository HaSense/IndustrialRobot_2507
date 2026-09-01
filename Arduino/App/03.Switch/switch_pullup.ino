/* 풀업 저항 형태로 처리 */

int pinSwitch = 12;

void setup() {
  Serial.begin(115200);
  pinMode(pinSwitch, INPUT_PULLUP);
}

void loop() {
  if(digitalRead(pinSwitch) == LOW) {
    Serial.println("Pushed");
    delay(300);
  }
}
