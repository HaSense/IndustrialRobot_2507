//적외서 송신기는 리모컨으로 하세요.
//수신기는 Sketch 라이브러리에서 IRremote by shirriff, z30t, ArminJo를 선택하세요.

#include <IRremote.hpp>

//디지털에 연결
int pinGnd = 2;
int pinVcc = 3;
int pinRecv = 4;

void setup() {

  Serial.begin(115200);

  pinMode(pinGnd, OUTPUT);
  pinMode(pinVcc, OUTPUT);

  digitalWrite(pinGnd, LOW);
  digitalWrite(pinVcc, HIGH);

  // IR 수신 시작
  IrReceiver.begin(pinRecv);

}

void loop() {

  // IR 신호가 수신되었는지 확인
  if (IrReceiver.decode()) {

    // 수신된 IR 데이터를 HEX로 출력
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);

    // 다음 IR 신호 수신 준비
    IrReceiver.resume();
  }

}
