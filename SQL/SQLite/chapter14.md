# Part 5. 스마트팩토리 데이터 모델링

# Chapter 14. LOT 추적 관리

---

# 학습목표

본 단원을 학습한 후 다음 내용을 설명할 수 있다.

* LOT의 개념을 이해할 수 있다.
* 추적성(Traceability)의 의미를 설명할 수 있다.
* 제조업에서 LOT가 중요한 이유를 설명할 수 있다.
* LOT 번호를 설계할 수 있다.
* LOT 이력 테이블을 설계할 수 있다.
* SQLite를 이용하여 LOT 데이터를 저장하고 조회할 수 있다.

---

# 왜 LOT가 중요한가?

이번 장은 스마트팩토리와 MES에서 가장 중요한 개념 중 하나를 다룬다.

실제 제조 현장에서

```text id="q1"
누가 만들었는가?
```

보다

```text id="q2"
언제 생산했는가?
어떤 원자재를 사용했는가?
어떤 설비에서 생산했는가?
어떤 제품들이 영향을 받는가?
```

를 더 중요하게 생각한다.

---

실제 기업에서는

```text id="q3"
LOT 추적이 불가능하면
MES 구축 실패
```

라고 이야기할 정도로 중요하다.

---

# 1. LOT란?

LOT는 생산 단위를 의미한다.

쉽게 말하면

```text id="q4"
같은 조건으로 생산된 제품 그룹
```

이다.

---

예시

2026년 6월 22일

```text id="q5"
Motor 100개 생산
```

↓

LOT 생성

```text id="q6"
LOT001
```

---

다음날

```text id="q7"
Motor 120개 생산
```

↓

새로운 LOT

```text id="q8"
LOT002
```

---

결과

| LOT    | 제품    | 수량  |
| ------ | ----- | --- |
| LOT001 | Motor | 100 |
| LOT002 | Motor | 120 |

---

# 2. LOT가 필요한 이유

생산 중 문제가 발생했다고 가정하자.

---

상황

```text id="q9"
6월 22일 생산 제품 불량 발생
```

---

LOT가 없다면?

```text id="q10"
언제 생산된 제품인지 모름
```

↓

```text id="q11"
전체 제품 회수
```

↓

엄청난 손실

---

LOT가 있다면?

```text id="q12"
LOT001만 문제 발생
```

↓

```text id="q13"
LOT001만 회수
```

↓

피해 최소화

---

# 실제 사례

자동차

```text id="q14"
브레이크 부품 불량
```

---

식품

```text id="q15"
세균 검출
```

---

반도체

```text id="q16"
특정 웨이퍼 공정 이상
```

---

모두 LOT 추적을 수행한다.

---

# 3. 추적성(Traceability)

Traceability

```text id="q17"
추적 가능성
```

---

질문

```text id="q18"
이 제품은 언제 생산되었는가?
```

---

질문

```text id="q19"
어떤 원자재를 사용했는가?
```

---

질문

```text id="q20"
어떤 설비에서 생산되었는가?
```

---

질문

```text id="q21"
어떤 작업자가 생산했는가?
```

---

이 모든 질문에 답할 수 있어야 한다.

---

# LOT 추적 흐름

```text id="q22"
원자재
   ↓
LOT
   ↓
생산
   ↓
검사
   ↓
출하
```

---

MES의 핵심 목적

```text id="q23"
모든 생산 이력을 추적 가능하게 만드는 것
```

이다.

---

# 4. LOT 번호 설계

LOT 번호는 회사마다 다르다.

---

예시 1

```text id="q24"
LOT001
```

---

예시 2

```text id="q25"
20260622-001
```

---

예시 3

```text id="q26"
MOTOR-20260622-001
```

---

예시 4

```text id="q27"
MTR-20260622-A01
```

---

# 교육용 LOT 번호 설계

이번 수업에서는 다음 형식을 사용한다.

```text id="q28"
제품명_생산일
```

---

예시

```text id="q29"
Motor_20260622
```

---

```text id="q30"
Sensor_20260622
```

---

# LOT 번호 설계 원칙

좋은 LOT 번호는

```text id="q31"
중복되지 않아야 한다.
```

---

```text id="q32"
사람이 읽을 수 있어야 한다.
```

---

```text id="q33"
생산일 추적 가능해야 한다.
```

---

```text id="q34"
제품 구분 가능해야 한다.
```

---

# 5. LOT 이력 테이블 설계

요구사항

| 항목         | 설명     |
| ---------- | ------ |
| lot_no     | LOT 번호 |
| product_id | 제품번호   |
| qty        | 생산수량   |
| prod_date  | 생산일    |

---

테이블 설계

```sql id="q35"
CREATE TABLE lot_history(
    lot_no TEXT,
    product_id INTEGER,
    qty INTEGER,
    prod_date TEXT
);
```

---

# ERD 구조

```text id="q36"
Products
-------------------
product_id
name

       │

       ▼

Lot_History
-------------------
lot_no
product_id
qty
prod_date
```

---

# 실습 1. LOT 테이블 생성

```sql id="q37"
CREATE TABLE lot_history(
    lot_no TEXT,
    product_id INTEGER,
    qty INTEGER,
    prod_date TEXT
);
```

---

확인

```sql id="q38"
.tables
```

---

# 실습 2. LOT 데이터 등록

```sql id="q39"
INSERT INTO lot_history
VALUES(
'Motor_20260622',
1,
100,
'2026-06-22'
);
```

---

```sql id="q40"
INSERT INTO lot_history
VALUES(
'Sensor_20260622',
2,
50,
'2026-06-22'
);
```

---

```sql id="q41"
INSERT INTO lot_history
VALUES(
'Motor_20260623',
1,
120,
'2026-06-23'
);
```

---

조회

```sql id="q42"
SELECT *
FROM lot_history;
```

---

결과

```text id="q43"
Motor_20260622|1|100|2026-06-22
Sensor_20260622|2|50|2026-06-22
Motor_20260623|1|120|2026-06-23
```

---

# 실습 3. 특정 LOT 조회

```sql id="q44"
SELECT *
FROM lot_history
WHERE lot_no='Motor_20260622';
```

---

결과

```text id="q45"
Motor_20260622
```

관련 생산정보 확인

---

# 실습 4. 특정 날짜 생산 LOT 조회

```sql id="q46"
SELECT *
FROM lot_history
WHERE prod_date='2026-06-22';
```

---

# 실습 5. 생산량이 100개 이상인 LOT 조회

```sql id="q47"
SELECT *
FROM lot_history
WHERE qty >= 100;
```

---

# 실습 6. LOT 개수 확인

```sql id="q48"
SELECT COUNT(*)
FROM lot_history;
```

---

현재 생산된 LOT 수를 확인하시오.

---

# 실습 7. 가장 큰 생산 LOT 찾기

```sql id="q49"
SELECT *
FROM lot_history
ORDER BY qty DESC;
```

---

가장 많이 생산된 LOT를 확인하시오.

---

# 실습 8. LOT 번호 직접 설계

다음 정보를 이용하여 LOT 번호를 작성하시오.

```text id="q50"
제품명 : Bearing
생산일 : 2026-06-24
```

---

예상 결과

```text id="q51"
Bearing_20260624
```

---

SQLite에 저장하시오.

---

# 실습 9. 불량 LOT 추적

상황

```text id="q52"
Motor_20260622
불량 발생
```

---

다음 SQL을 작성하시오.

```sql id="q53"
SELECT *
FROM lot_history
WHERE lot_no='Motor_20260622';
```

---

문제의 LOT 정보를 찾아보시오.

---

# 도전 과제

다음 LOT를 추가하시오.

```text id="q54"
Valve_20260624
Gear_20260624
Pump_20260624
```

---

문제 1

전체 LOT 수 조회

---

문제 2

2026-06-24 생산 LOT 조회

---

문제 3

생산량 합계 계산

---

문제 4

평균 생산량 계산

---

# 제조AI 관점

실제 MES에서는 LOT가 핵심 데이터이다.

LOT를 기준으로

```text id="q55"
원자재
설비
작업자
품질
출하
```

를 연결한다.

---

실제 MES 구조

```text id="q56"
Product
    ↓
LOT
    ↓
Production
    ↓
Quality
    ↓
Shipment
```

---

LOT가 없다면

```text id="q57"
스마트팩토리 구축 불가능
```

에 가깝다.

---

# 핵심 정리

* LOT는 같은 조건으로 생산된 제품 그룹이다.
* LOT는 추적성(Traceability)의 핵심이다.
* 불량 발생 시 LOT를 이용하여 문제 제품을 추적한다.
* LOT 번호는 중복되지 않게 설계해야 한다.
* MES의 가장 중요한 데이터 중 하나가 LOT이다.
* 생산, 품질, 출하 데이터는 모두 LOT를 기준으로 연결된다.

---

# 다음 시간

Chapter 15. JOIN 기초

* JOIN 필요성
* 제품과 생산이력 연결
* 제품과 LOT 연결
* INNER JOIN 실습
