#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT11

int pinGnd = 2;
int pinDht = 3;
int pinVcc = 4;

// DHT 클래스의 객체 dht를 생성한다.
// pinDht   : DHT11의 DATA(Signal) 핀이 연결된 Arduino 핀 번호
// DHTTYPE  : 사용할 센서의 종류(DHT11)
// 이후 dht 객체를 이용하여 온도와 습도를 읽을 수 있다.
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
   // DHT11 내부의 온도 센서에서 온도를 읽어온다.
  // 기본 반환 단위는 섭씨(°C)이며 float 형태로 반환된다.
  // 예: 24.0, 25.0
  float fTemp = dht.readTemperature();
  // DHT11 내부의 습도 센서에서 상대습도를 읽어온다.
  // 반환 단위는 %(상대습도, %RH)이며 float 형태로 반환된다.
  // 예: 45.0 → 상대습도 45%
  float fHumi = dht.readHumidity();
 
  //둘 중 하나의 값을 가져올 수 없으면 에러를 출력한다.
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
