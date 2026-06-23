# Part 6. JOIN

# Chapter 15. JOIN 기초

---

# 학습목표

본 단원을 학습한 후 다음 내용을 수행할 수 있다.

* JOIN이 필요한 이유를 설명할 수 있다.
* 데이터 정규화의 장점을 이해할 수 있다.
* INNER JOIN의 개념을 설명할 수 있다.
* 제품 정보와 생산 이력을 연결할 수 있다.
* 스마트팩토리 데이터 조회 SQL을 작성할 수 있다.

---

# 이번 장이 중요한 이유

지금까지 우리는 다음 테이블을 만들었다.

```text id="j1"
products
```

```text id="j2"
production_history
```

```text id="j3"
lot_history
```

---

그런데 문제가 있다.

생산이력 조회

```sql id="j4"
SELECT *
FROM production_history;
```

결과

```text id="j5"
1|1|100|2026-06-22
2|2|50|2026-06-22
3|1|120|2026-06-23
```

---

질문

```text id="j6"
제품번호 1번이 무엇인가?
```

---

질문

```text id="j7"
제품번호 2번은 무엇인가?
```

---

사람은 알 수 없다.

그래서 JOIN이 필요하다.

---

# 1. JOIN이란?

JOIN은

```text id="j8"
두 개 이상의 테이블을 연결하여
하나의 결과로 조회하는 기능
```

이다.

---

예시

제품 테이블

| product_id | name    |
| ---------- | ------- |
| 1          | Motor   |
| 2          | Sensor  |
| 3          | Bearing |

---

생산이력 테이블

| history_id | product_id | qty |
| ---------- | ---------- | --- |
| 1          | 1          | 100 |
| 2          | 2          | 50  |
| 3          | 1          | 120 |

---

JOIN 결과

| 제품명    | 생산량 |
| ------ | --- |
| Motor  | 100 |
| Sensor | 50  |
| Motor  | 120 |

---

# 2. 왜 JOIN을 사용하는가?

잘못된 설계

```text id="j9"
생산이력 테이블에

Motor
Motor
Motor
Sensor
Sensor
```

를 계속 저장

---

문제

```text id="j10"
데이터 중복
```

---

```text id="j11"
오타 발생
```

---

```text id="j12"
저장 공간 증가
```

---

좋은 설계

```text id="j13"
제품번호만 저장
```

---

필요할 때 JOIN

```text id="j14"
product_id
↓
제품명 조회
```

---

# 3. INNER JOIN

가장 많이 사용하는 JOIN

---

문법

```sql id="j15"
SELECT
    컬럼
FROM 테이블A
JOIN 테이블B
ON 조건;
```

---

# 4. 제품과 생산이력 연결

제품 테이블

```sql id="j16"
SELECT *
FROM products;
```

결과

```text id="j17"
1|Motor|5000
2|Sensor|12000
3|Bearing|3000
```

---

생산이력

```sql id="j18"
SELECT *
FROM production_history;
```

결과

```text id="j19"
1|1|100|2026-06-22
2|2|50|2026-06-22
3|1|120|2026-06-23
```

---

JOIN

```sql id="j20"
SELECT
    products.name,
    production_history.qty
FROM products
JOIN production_history
ON products.product_id =
   production_history.product_id;
```

---

결과

```text id="j21"
Motor|100
Sensor|50
Motor|120
```

---

# 5. 별칭(Alias)

실무에서는 테이블명이 길다.

---

예제

```sql id="j22"
SELECT
    p.name,
    h.qty
FROM products p
JOIN production_history h
ON p.product_id = h.product_id;
```

---

의미

```text id="j23"
products → p

production_history → h
```

---

실무에서 가장 많이 사용하는 방식이다.

---

# JOIN 동작 원리

```text id="j24"
products

1 Motor
2 Sensor
3 Bearing
```

---

```text id="j25"
production_history

1 1 100
2 2 50
3 1 120
```

---

비교

```text id="j26"
1 = 1
2 = 2
1 = 1
```

---

결과 생성

```text id="j27"
Motor 100
Sensor 50
Motor 120
```

---

# 실습 준비

products 생성

```sql id="j28"
CREATE TABLE products(
    product_id INTEGER PRIMARY KEY,
    name TEXT,
    price INTEGER
);
```

---

데이터 입력

```sql id="j29"
INSERT INTO products VALUES(1,'Motor',5000);
INSERT INTO products VALUES(2,'Sensor',12000);
INSERT INTO products VALUES(3,'Bearing',3000);
```

---

production_history 생성

```sql id="j30"
CREATE TABLE production_history(
    history_id INTEGER PRIMARY KEY,
    product_id INTEGER,
    qty INTEGER,
    prod_date TEXT
);
```

---

데이터 입력

```sql id="j31"
INSERT INTO production_history VALUES(1,1,100,'2026-06-22');
INSERT INTO production_history VALUES(2,2,50,'2026-06-22');
INSERT INTO production_history VALUES(3,1,120,'2026-06-23');
INSERT INTO production_history VALUES(4,3,80,'2026-06-23');
```

---

# 실습 1. 제품 조회

```sql id="j32"
SELECT *
FROM products;
```

---

결과 확인

---

# 실습 2. 생산이력 조회

```sql id="j33"
SELECT *
FROM production_history;
```

---

결과 확인

---

# 실습 3. 첫 번째 JOIN

```sql id="j34"
SELECT
    p.name,
    h.qty
FROM products p
JOIN production_history h
ON p.product_id = h.product_id;
```

---

결과

```text id="j35"
Motor
Sensor
Motor
Bearing
```

확인

---

# 실습 4. 생산일 추가 조회

```sql id="j36"
SELECT
    p.name,
    h.qty,
    h.prod_date
FROM products p
JOIN production_history h
ON p.product_id = h.product_id;
```

---

결과 분석

```text id="j37"
제품명
생산량
생산일
```

확인

---

# 실습 5. 생산량 내림차순

```sql id="j38"
SELECT
    p.name,
    h.qty
FROM products p
JOIN production_history h
ON p.product_id = h.product_id
ORDER BY h.qty DESC;
```

---

가장 많이 생산된 제품 확인

---

# 실습 6. Motor만 조회

```sql id="j39"
SELECT
    p.name,
    h.qty
FROM products p
JOIN production_history h
ON p.product_id = h.product_id
WHERE p.name='Motor';
```

---

Motor 생산 실적 확인

---

# 실습 7. 생산량 100개 이상 조회

```sql id="j40"
SELECT
    p.name,
    h.qty
FROM products p
JOIN production_history h
ON p.product_id = h.product_id
WHERE h.qty >= 100;
```

---

결과 분석

---

# 실습 8. LOT와 연결 준비

lot_history 조회

```sql id="j41"
SELECT *
FROM lot_history;
```

---

다음 장에서

```text id="j42"
제품
+
LOT
```

JOIN을 수행할 준비를 한다.

---

# 도전 과제

문제 1

제품명과 생산일 조회

---

문제 2

생산량이 80 이상인 데이터 조회

---

문제 3

제품명을 기준으로 정렬

---

문제 4

생산일 기준으로 정렬

---

# 제조AI 관점

실제 MES는 JOIN의 집합이다.

예시

```text id="j43"
제품
+
생산이력
+
LOT
+
품질
+
설비
```

---

JOIN 결과

```text id="j44"
Motor

LOT001

100개 생산

설비 3호기

검사 OK
```

---

실제 MES 화면은 대부분

```text id="j45"
여러 테이블 JOIN 결과
```

를 보여주는 것이다.

---

# 핵심 정리

* JOIN은 여러 테이블을 연결하는 기능이다.
* 데이터 중복을 줄이기 위해 사용한다.
* INNER JOIN은 가장 많이 사용하는 JOIN이다.
* ON 조건으로 연결 기준을 지정한다.
* 실무 MES는 대부분 JOIN 기반 조회로 동작한다.

---

# 다음 시간

Chapter 16. 생산현황 조회

* GROUP BY
* 제품별 생산량
* 생산현황 분석
* 실무 조회문 작성
