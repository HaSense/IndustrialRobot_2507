int pinSwitch = 12;

void setup() {
  Serial.begin(115200);
  pinMode(pinSwitch, INPUT);
}

void loop() {
  int value = digitalRead(pinSwitch);

  Serial.println(value);

  delay(300);
}
