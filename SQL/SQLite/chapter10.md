# Part 4. 데이터 수정 및 삭제

# Chapter 10. UPDATE

---

# 학습목표

본 단원을 학습한 후 다음 내용을 수행할 수 있다.

* UPDATE 문의 역할을 설명할 수 있다.
* 특정 데이터를 수정할 수 있다.
* WHERE 조건을 이용하여 원하는 행만 수정할 수 있다.
* 여러 컬럼을 동시에 수정할 수 있다.
* UPDATE 사용 시 주의사항을 이해할 수 있다.

---

# 1. UPDATE란?

UPDATE는 이미 저장된 데이터를 수정하는 SQL 명령이다.

예를 들어 생산량이 잘못 입력되었다면 데이터를 수정해야 한다.

수정 전

| id | product_name | qty | prod_date  |
| -- | ------------ | --- | ---------- |
| 1  | Motor        | 100 | 2026-06-22 |

실제 생산량

```text
120
```

이라면 수정이 필요하다.

---

# 2. UPDATE 기본 문법

```sql
UPDATE 테이블명
SET 컬럼명 = 값
WHERE 조건;
```

---

예제

```sql
UPDATE production
SET qty = 120
WHERE id = 1;
```

---

의미

```text
production 테이블에서

id가 1인 행의

qty를 120으로 수정
```

---

# 3. 수정 전 데이터 확인

현재 데이터 조회

```sql
SELECT * FROM production;
```

결과

```text
1|Motor|100|2026-06-22
2|Sensor|50|2026-06-22
3|Bearing|80|2026-06-22
```

---

# 4. 생산량 수정

수정 명령

```sql
UPDATE production
SET qty = 120
WHERE id = 1;
```

---

수정 결과 확인

```sql
SELECT * FROM production;
```

결과

```text
1|Motor|120|2026-06-22
2|Sensor|50|2026-06-22
3|Bearing|80|2026-06-22
```

---

# 5. 문자열 수정

제품명 변경

수정 전

```text
Motor
```

수정 후

```text
Motor-X
```

---

SQL

```sql
UPDATE production
SET product_name = 'Motor-X'
WHERE id = 1;
```

---

확인

```sql
SELECT * FROM production;
```

---

# 6. 날짜 수정

생산일 변경

```sql
UPDATE production
SET prod_date = '2026-06-25'
WHERE id = 1;
```

---

결과 확인

```sql
SELECT * FROM production;
```

---

# 7. 여러 컬럼 동시 수정

한 번에 여러 컬럼 수정 가능

```sql
UPDATE production
SET
    product_name = 'Motor-Pro',
    qty = 150
WHERE id = 1;
```

---

결과

```text
1|Motor-Pro|150|2026-06-22
```

---

# 8. WHERE가 중요한 이유

다음 SQL을 실행하면?

```sql
UPDATE production
SET qty = 999;
```

---

결과

```text
모든 행 수정
```

---

수정 전

```text
1 Motor 100
2 Sensor 50
3 Bearing 80
```

---

수정 후

```text
1 Motor 999
2 Sensor 999
3 Bearing 999
```

---

따라서 UPDATE는 반드시 WHERE와 함께 사용하는 습관을 가져야 한다.

---

# 실습용 데이터 준비

다음 데이터를 입력하시오.

```sql
INSERT INTO production
VALUES(1,'Motor',100,'2026-06-22');

INSERT INTO production
VALUES(2,'Sensor',50,'2026-06-22');

INSERT INTO production
VALUES(3,'Bearing',80,'2026-06-22');
```

---

데이터 확인

```sql
SELECT * FROM production;
```

---

결과

```text
1|Motor|100|2026-06-22
2|Sensor|50|2026-06-22
3|Bearing|80|2026-06-22
```

---

# 실습 1. 생산량 수정

id가 1인 제품의 생산량을

```text
120
```

으로 수정하시오.

---

정답

```sql
UPDATE production
SET qty = 120
WHERE id = 1;
```

---

확인

```sql
SELECT * FROM production;
```

---

# 실습 2. 제품명 수정

id가 2인 제품명을

```text
TemperatureSensor
```

로 수정하시오.

---

정답

```sql
UPDATE production
SET product_name = 'TemperatureSensor'
WHERE id = 2;
```

---

# 실습 3. 생산일 수정

id가 3인 데이터의 생산일을

```text
2026-06-25
```

로 변경하시오.

---

정답

```sql
UPDATE production
SET prod_date = '2026-06-25'
WHERE id = 3;
```

---

# 실습 4. 여러 컬럼 수정

id가 1인 데이터를 다음과 같이 수정하시오.

```text
제품명 : Motor-Pro
생산량 : 200
```

---

정답

```sql
UPDATE production
SET
    product_name = 'Motor-Pro',
    qty = 200
WHERE id = 1;
```

---

# 실습 5. 조건 검색 후 수정

먼저 조회

```sql
SELECT *
FROM production
WHERE qty < 100;
```

---

조회된 데이터 중

qty를

```text
100
```

으로 수정하시오.

---

예시

```sql
UPDATE production
SET qty = 100
WHERE qty < 100;
```

---

# 실습 6. 실무 시나리오

상황

```text
LOT 생산 실적 입력 오류 발생
```

입력된 데이터

```text
Motor 생산량 : 100
```

실제 생산량

```text
130
```

---

수정 SQL을 작성하시오.

---

예시 답안

```sql
UPDATE production
SET qty = 130
WHERE id = 1;
```

---

# 도전 과제

다음 데이터를 입력하시오.

```sql
INSERT INTO production
VALUES(4,'Pump',70,'2026-06-22');

INSERT INTO production
VALUES(5,'Valve',90,'2026-06-22');
```

---

문제 1

```text
qty가 100 미만인 데이터
```

를 모두

```text
100
```

으로 수정하시오.

---

문제 2

```text
Pump
```

를

```text
Pump-X
```

로 수정하시오.

---

문제 3

모든 생산일을

```text
2026-07-01
```

로 변경하시오.

---

# 제조AI 관점

실제 MES에서는 다음과 같은 상황에서 UPDATE가 사용된다.

* 생산량 수정
* 품질 검사 결과 수정
* LOT 정보 수정
* 작업 지시 상태 변경
* 설비 상태 변경

예)

```text
대기
```

↓

```text
생산중
```

↓

```text
완료
```

---

# 핵심 정리

* UPDATE는 기존 데이터를 수정하는 명령이다.
* SET으로 변경할 값을 지정한다.
* WHERE로 수정 대상을 지정한다.
* 여러 컬럼을 동시에 수정할 수 있다.
* WHERE 없이 UPDATE하면 모든 행이 수정된다.
* UPDATE 후 반드시 SELECT로 결과를 확인해야 한다.

---

# 다음 시간

Chapter 11. DELETE

* DELETE 문
* 특정 데이터 삭제
* 조건 삭제
* 전체 삭제
* 데이터 복구 주의사항
