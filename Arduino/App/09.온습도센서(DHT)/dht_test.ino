//핀배치에 신경 쓰고 라이브러리 추가가 되어야 합니다.

#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT11

int pinGnd = 2;
int pinDht = 3;
int pinVcc = 4;

DHT dht(pinDht, DHTTYPE);

void setup() {
  Serial.begin(115200);
  pinMode(pinVcc, OUTPUT);
  pinMode(pinGnd, OUTPUT);
  digitalWrite(pinVcc, HIGH);
  digitalWrite(pinGnd, LOW);

  dht.begin();
}

void loop() {
  delay(2000);
  float fTemp = dht.readTemperature();
  float fHumi = dht.readHumidity();

  if(isnan(fTemp) || isnan(fHumi)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Temperature: ");
  Serial.print(fTemp);
  Serial.print("[C]\t ");
  Serial.print("Humidity: ");
  Serial.print(fHumi);
  Serial.println("[%]");

}
