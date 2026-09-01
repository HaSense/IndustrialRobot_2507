const char* DEVICE_ID = "arduino-r4-01";

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(A0));
}

void loop() {
  float temperatureC = random(2000, 3100) / 100.0;

  Serial.print("{\"device_id\":\"");
  Serial.print(DEVICE_ID);
  Serial.print("\",\"temperature_c\":");
  Serial.print(temperatureC, 2);
  Serial.println("}");

  delay(5000);
}
