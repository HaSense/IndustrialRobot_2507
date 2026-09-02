 int pinGnd = A1;
 int pinVcc = A2;
 int pinTouchA = A0;
 int pinTouchD = A3;

 long timer1 = 0, timer2 = 0;
 int touchFlag = 1;

void setup() {
 Serial.begin(115200);

 pinMode(pinGnd, OUTPUT);
 pinMode(pinVcc, OUTPUT);
 pinMode(pinTouchD, INPUT);
 digitalWrite(pinGnd, LOW);
 digitalWrite(pinVcc, HIGH);
}

void loop() {

  // 터치 센서의 디지털 출력값을 읽는다.
  // HIGH이면 터치가 감지되고 있는 상태
  if (digitalRead(pinTouchD)) {

    // 현재 시간을 밀리초(ms) 단위로 저장
    timer1 = millis();

    // 이전 HIGH 감지 시간(timer2)과 현재 시간(timer1)의
    // 차이가 18ms보다 작은지 확인한다.
    // 센서가 약 16.6ms 간격으로 HIGH 신호를 발생시키는 경우
    // 연속적인 터치 신호인지 판단하기 위한 조건이다.
    if ((timer1 - timer2) < 18) {

      // 아직 Touch On 상태로 처리하지 않았다면
      if (touchFlag == 0) {

        // 터치가 시작되었음을 출력
        Serial.println("Touch On");

        // 현재 터치 상태임을 저장
        // 이후 같은 터치에 대해 Touch On이 반복 출력되는 것을 방지
        touchFlag = 1;
      }
    }

    // 현재 HIGH 감지 시간을 다음 비교를 위해 저장
    timer2 = timer1;
  }

  else {

    // 현재 센서값이 LOW인 경우,
    // 마지막 HIGH 신호가 발생한 후 18ms 이상 지났는지 확인한다.
    // 18ms 이상 HIGH가 발생하지 않았다면
    // 터치가 끝난 것으로 판단한다.
    if ((millis() - timer1) > 18) {

      // 이전 상태가 터치 중이었다면
      if (touchFlag) {

        // 터치가 끝났음을 출력
        Serial.println("Touch Off");

        // 터치 상태를 OFF로 변경
        touchFlag = 0;
      }
    }
  }
}
