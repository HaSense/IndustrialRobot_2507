int pinGnd = 5;
int pinEcho = 4;
int pinTrigger = 3;
int pinVcc = 2;

void setup() {

  Serial.begin(115200);

  pinMode(pinGnd, OUTPUT);
  pinMode(pinVcc, OUTPUT);
  pinMode(pinTrigger, OUTPUT);
  pinMode(pinEcho, INPUT);

  digitalWrite(pinGnd, LOW);
  digitalWrite(pinVcc, HIGH);

}

void loop() {

  float fDuration, fDistance;

  digitalWrite(pinTrigger, HIGH);

  /*
    초음파 센서에 측정을 시작하라는 Trigger 신호를
    약 10마이크로초(10us) 동안 HIGH 상태로 유지한다.

    delay(10)을 사용하면 10밀리초(ms)를 기다리게 되는데,
    10ms = 10,000us 이므로 너무 긴 신호가 된다.

    초음파 센서의 Trigger 입력은 매우 짧은 펄스가 필요하므로
    밀리초 단위의 delay()가 아니라
    마이크로초 단위의 delayMicroseconds()를 사용한다.

    1초 = 1,000밀리초(ms)
    1밀리초 = 1,000마이크로초(us)

    따라서 10us = 0.00001초이다.
  */
  delayMicroseconds(10);

  digitalWrite(pinTrigger, LOW);

  fDuration = pulseIn(pinEcho, HIGH);

  Serial.println(fDuration);

  /*
    pulseIn()으로 측정한 fDuration은
    초음파가 물체까지 갔다가 다시 센서로 돌아오는 데 걸린 시간이다.

    pulseIn()의 반환 단위는 마이크로초(us)이다.

    음속은 약 340m/s이다.

    따라서 기본 공식은

        거리 = 속도 × 시간

    이지만 fDuration은 마이크로초이고,
    최종 결과는 cm 단위로 구해야 하므로 단위 변환이 필요하다.

    음속:
        340 m/s

    m를 cm로 변환하면:
        340 × 100 = 34,000 cm/s

    1초는 1,000,000us이므로:

        34,000 / 1,000,000
        = 0.034 cm/us

    따라서 초음파가 이동한 전체 거리는:

        fDuration × 0.034

    그런데 fDuration은

        센서 → 물체 → 센서

    즉 왕복 시간이다.

    우리가 원하는 것은 센서에서 물체까지의 편도 거리이므로
    마지막에 2로 나눈다.

        거리(cm)
        = fDuration × 0.034 / 2

    현재 코드의 식

        (340 * fDuration) / 10000 / 2

    도 같은 계산이다.

    340 / 10000 = 0.034

    따라서 결과적으로

        fDistance = fDuration × 0.034 / 2

    가 된다.

    예:
        fDuration = 1000us 라면

        1000 × 0.034 / 2
        = 17cm

    즉 센서와 물체 사이의 거리는 약 17cm이다.
  */
  fDistance = ((float)(340 * fDuration)) / 10000 / 2;

  Serial.print(fDistance);
  Serial.println("cm");

  delay(500);

}
