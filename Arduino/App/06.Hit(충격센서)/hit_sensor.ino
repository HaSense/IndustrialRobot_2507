// 4번 핀을 GND 역할로 사용
const int pinGnd = 4;
// 3번 핀을 VCC 역할로 사용
const int pinVcc = 3;
// SW-520D 충격/기울기 센서의 신호(S) 핀
// 인터럽트를 사용하기 위해 2번 핀에 연결
const int pinHit = 2;

// 충격이 감지된 횟수를 저장하는 변수
// 인터럽트 함수(ISR)에서 값이 변경되므로 volatile 사용
volatile int count = 0;

void setup() {
  Serial.begin(115200);
  pinMode(pinGnd, OUTPUT);
  pinMode(pinVcc, OUTPUT);

  // 4번 핀을 LOW로 만들어 GND 역할
  digitalWrite(pinGnd, LOW);
  // 3번 핀을 HIGH로 만들어 VCC 역할
  digitalWrite(pinVcc, HIGH);

  // 센서 신호 핀을 입력으로 설정
  // INPUT_PULLUP을 사용하면 내부 풀업 저항이 활성화되어
  // 센서가 동작하지 않을 때 입력값이 안정적으로 HIGH 상태를 유지
  pinMode(pinHit, INPUT_PULLUP);

  // 외부 인터럽트 설정
  //
  // digitalPinToInterrupt(pinHit)
  //   → 2번 핀에 해당하는 인터럽트 번호를 자동으로 가져옴
  //
  // hitISR
  //   → 인터럽트가 발생했을 때 실행할 함수
  //
  // FALLING
  //   → 입력 신호가 HIGH에서 LOW로 변하는 순간 인터럽트 발생
  attachInterrupt(
    digitalPinToInterrupt(pinHit),
    hitISR,
    FALLING
  );
}

void loop() {

  // count 값을 읽는 동안 인터럽트를 잠시 중지
  // ISR에서 count가 변경되는 것을 방지
  noInterrupts();

  // 현재 충격 횟수를 복사
  int currentCount = count;

  // 인터럽트를 다시 활성화
  interrupts();

  // 현재까지 감지된 충격 횟수를 시리얼 모니터에 출력
  Serial.print("Hit : ");
  Serial.println(currentCount);

  // 0.5초마다 결과 출력
  delay(500);
}


// --------------------------------------------------
// Interrupt Service Routine (ISR)
// --------------------------------------------------
// SW-520D에서 충격이 감지되어
// 2번 핀의 신호가 HIGH → LOW로 변하면
// 이 함수가 즉시 호출됨
void hitISR() {

  // 충격 횟수 1 증가
  count++;
}
