# Part 4. 데이터 수정 및 삭제

# Chapter 11. DELETE

---

# 학습목표

본 단원을 학습한 후 다음 내용을 수행할 수 있다.

* DELETE 문의 역할을 설명할 수 있다.
* 특정 데이터를 삭제할 수 있다.
* WHERE 조건을 이용하여 원하는 행만 삭제할 수 있다.
* DELETE 사용 시 주의사항을 이해할 수 있다.
* 삭제 전 SELECT로 확인하는 습관을 가질 수 있다.

---

# 1. DELETE란?

DELETE는 테이블에 저장된 데이터를 삭제하는 SQL 명령이다.

예를 들어

```text id="bz40l0"
잘못 입력된 생산실적
중복 입력된 데이터
시험용 데이터
```

등을 삭제할 때 사용한다.

---

# 2. DELETE 기본 문법

```sql id="s8vb8d"
DELETE FROM 테이블명
WHERE 조건;
```

---

예제

```sql id="mlthk4"
DELETE FROM production
WHERE id = 1;
```

---

의미

```text id="gf8e3k"
production 테이블에서

id가 1인 데이터 삭제
```

---

# 3. 삭제 전 데이터 확인

먼저 현재 데이터를 확인한다.

```sql id="m7uqqr"
SELECT * FROM production;
```

결과

```text id="guxqei"
1|Motor|100|2026-06-22
2|Sensor|50|2026-06-22
3|Bearing|80|2026-06-22
```

---

# 4. 특정 데이터 삭제

id가 1인 데이터 삭제

```sql id="it6m3c"
DELETE FROM production
WHERE id = 1;
```

---

확인

```sql id="g5nmk6"
SELECT * FROM production;
```

결과

```text id="ye3qf9"
2|Sensor|50|2026-06-22
3|Bearing|80|2026-06-22
```

---

# 5. 문자열 조건 삭제

제품명이 Sensor인 데이터 삭제

```sql id="c6mx6d"
DELETE FROM production
WHERE product_name = 'Sensor';
```

---

확인

```sql id="km5y4e"
SELECT * FROM production;
```

---

# 6. 조건 삭제

생산량이 100 미만인 데이터 삭제

```sql id="f8kmxv"
DELETE FROM production
WHERE qty < 100;
```

---

삭제 전

```text id="cglf8s"
Motor    100
Sensor    50
Bearing   80
```

---

삭제 후

```text id="pkb2qo"
Motor    100
```

---

# 7. DELETE와 UPDATE 차이

UPDATE

```sql id="nq9p0l"
UPDATE production
SET qty = 120
WHERE id = 1;
```

데이터 수정

---

DELETE

```sql id="qr5g1z"
DELETE FROM production
WHERE id = 1;
```

데이터 삭제

---

# 8. WHERE가 중요한 이유

다음 SQL을 실행하면?

```sql id="8zn2ry"
DELETE FROM production;
```

---

결과

```text id="1q42m7"
모든 데이터 삭제
```

---

삭제 전

```text id="1b0u2n"
1 Motor
2 Sensor
3 Bearing
```

---

삭제 후

```text id="hmh68w"
데이터 없음
```

---

따라서 DELETE는 반드시 WHERE와 함께 사용하는 습관을 가져야 한다.

---

# 실습용 데이터 준비

테이블 생성

```sql id="7esw8d"
CREATE TABLE production(
    id INTEGER PRIMARY KEY,
    product_name TEXT,
    qty INTEGER,
    prod_date TEXT
);
```

---

데이터 입력

```sql id="9obc7p"
INSERT INTO production
VALUES(1,'Motor',100,'2026-06-22');

INSERT INTO production
VALUES(2,'Sensor',50,'2026-06-22');

INSERT INTO production
VALUES(3,'Bearing',80,'2026-06-22');

INSERT INTO production
VALUES(4,'Pump',150,'2026-06-22');

INSERT INTO production
VALUES(5,'Valve',60,'2026-06-22');
```

---

확인

```sql id="4af9wm"
SELECT * FROM production;
```

---

# 실습 1. 특정 ID 삭제

id가 1인 데이터를 삭제하시오.

---

정답

```sql id="t85azk"
DELETE FROM production
WHERE id = 1;
```

---

확인

```sql id="a8s5gi"
SELECT * FROM production;
```

---

# 실습 2. 제품명으로 삭제

제품명이

```text id="i5o2u7"
Sensor
```

인 데이터를 삭제하시오.

---

정답

```sql id="8j0v5x"
DELETE FROM production
WHERE product_name = 'Sensor';
```

---

# 실습 3. 생산량 조건 삭제

생산량이

```text id="4wg4zq"
100 미만
```

인 데이터를 모두 삭제하시오.

---

정답

```sql id="wz0exq"
DELETE FROM production
WHERE qty < 100;
```

---

# 실습 4. 날짜 조건 삭제

생산일이

```text id="14mhya"
2026-06-22
```

인 데이터를 삭제하시오.

---

정답

```sql id="u26wry"
DELETE FROM production
WHERE prod_date = '2026-06-22';
```

---

# 실습 5. 삭제 전 조회

다음 조건에 해당하는 데이터를 먼저 조회하시오.

```text id="d2m2pv"
qty < 100
```

---

조회

```sql id="jlwmcs"
SELECT *
FROM production
WHERE qty < 100;
```

---

확인 후 삭제

```sql id="z4rzhd"
DELETE FROM production
WHERE qty < 100;
```

---

# 실습 6. 실무 시나리오

상황

```text id="mwowxg"
Motor 생산실적이
중복 입력되었다.
```

현재

```text id="sfd5l5"
1 Motor 100
6 Motor 100
```

---

중복된 id=6 데이터를 삭제하시오.

---

정답

```sql id="eyiz6j"
DELETE FROM production
WHERE id = 6;
```

---

# 실습 7. 생산량이 가장 작은 데이터 삭제

먼저 조회

```sql id="9rtuxt"
SELECT *
FROM production
ORDER BY qty ASC;
```

---

가장 작은 생산량 데이터를 찾아 삭제하시오.

---

예시

```sql id="mz6r1z"
DELETE FROM production
WHERE id = 5;
```

---

# 도전 과제

다음 데이터를 추가하시오.

```sql id="f13yyv"
INSERT INTO production
VALUES(6,'Motor-X',200,'2026-06-23');

INSERT INTO production
VALUES(7,'Motor-X',250,'2026-06-23');

INSERT INTO production
VALUES(8,'Sensor-X',90,'2026-06-23');
```

---

문제 1

```text id="9pb6ju"
Motor-X
```

만 삭제하시오.

---

문제 2

```text id="1m2v4d"
qty가 100 이하
```

인 데이터를 삭제하시오.

---

문제 3

```text id="40s5g8"
2026-06-23
```

생산 데이터를 삭제하시오.

---

# 제조AI 관점

실제 MES에서는 DELETE를 자주 사용하지 않는다.

이유

```text id="sd84gm"
생산 이력은 중요한 기록
```

이기 때문이다.

---

실무에서는

```text id="fd31wn"
삭제
```

보다

```text id="vs84vk"
사용안함
취소
비활성화
```

상태로 관리하는 경우가 많다.

---

예)

| id | status  |
| -- | ------- |
| 1  | ACTIVE  |
| 2  | DELETED |

---

# DELETE 사용 시 주의사항

삭제 전 반드시 확인

```sql id="c9zc0z"
SELECT *
FROM production
WHERE id = 1;
```

---

확인 후 삭제

```sql id="lzv8uz"
DELETE FROM production
WHERE id = 1;
```

---

좋은 습관

```text id="ifm7ks"
SELECT
↓
확인
↓
DELETE
```

---

# 핵심 정리

* DELETE는 데이터를 삭제하는 명령이다.
* WHERE 조건으로 삭제 대상을 지정한다.
* WHERE 없이 실행하면 모든 데이터가 삭제된다.
* 삭제 전 반드시 SELECT로 확인한다.
* 실제 MES에서는 삭제보다 상태값 변경을 더 많이 사용한다.

---

# 다음 시간

Chapter 12. 제품 관리

* 제품 테이블 설계
* PRIMARY KEY
* 제품 정보 관리
* 스마트팩토리 데이터 모델링
