// 빛의 강도를 측정하는 센서 cds 아날로그 핀에 연결해 주세요.

int pinGnd = A2;
int pinVcc = A1;
int pinCds = A0;

void setup() {
  Serial.begin(115200);

  pinMode(pinGnd, OUTPUT);
  pinMode(pinVcc, OUTPUT);
  pinMode(pinCds, INPUT);

  digitalWrite(pinGnd, LOW);
  digitalWrite(pinVcc, HIGH);
}

void loop() {
  int adcData = analogRead(pinCds);

  Serial.println(adcData);

  delay(5000);
}
