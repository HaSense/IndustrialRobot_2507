// D0를 2번 +를 3번 GBD를 4번 연결하세요.

int pinGnd = 4;
int pinVcc = 3;
int pinReed = 2;

void setup() {
  Serial.begin(115200);
  pinMode(pinGnd, OUTPUT);
  pinMode(pinVcc, OUTPUT);
  pinMode(pinReed, INPUT);
  digitalWrite(pinGnd, LOW);
  digitalWrite(pinVcc, HIGH);
}

void loop() {
  if(digitalRead(pinReed)) { // Reed read
    Serial.println("Mini Reed ON"); // HIGH
  }
  else {
    Serial.println("Mini Reed OFF"); // LOW
  }
  delay(500);


}
