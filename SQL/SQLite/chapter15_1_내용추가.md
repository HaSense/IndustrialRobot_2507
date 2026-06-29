# Chapter 15. JOIN

---

# 학습목표

본 단원을 학습한 후 다음 내용을 설명할 수 있다.

* JOIN의 개념을 이해할 수 있다.
* 여러 테이블을 연결하는 이유를 설명할 수 있다.
* INNER JOIN을 사용할 수 있다.
* LEFT JOIN을 사용할 수 있다.
* RIGHT JOIN의 개념을 설명할 수 있다.
* CROSS JOIN을 사용할 수 있다.
* 생산관리 데이터베이스에서 JOIN을 활용할 수 있다.

---

# JOIN이란?

JOIN은 **여러 개의 테이블을 하나처럼 연결하여 조회하는 기능**이다.

실제 데이터베이스에서는 하나의 테이블에 모든 정보를 저장하지 않는다.

예를 들어

제품 정보

|제품번호|제품명|
|--------|------|
|1|Motor|
|2|Sensor|

생산 정보

|생산번호|제품번호|생산량|
|--------|--------|------|
|1|1|100|
|2|2|200|

생산 테이블에는 제품명이 없다.

대신 제품번호만 저장한다.

조회할 때

생산정보 + 제품정보

를 연결하여 원하는 결과를 얻는다.

---

# 왜 JOIN을 사용하는가?

예를 들어

생산 테이블

|id|product_id|qty|
|--|----------|---|
|1|1|100|

제품 테이블

|product_id|product_name|
|----------|------------|
|1|Motor|

JOIN을 수행하면

|id|product_name|qty|
|--|------------|---|
|1|Motor|100|

처럼 조회할 수 있다.

---

# JOIN 종류

SQLite에서 자주 사용하는 JOIN은 다음과 같다.

* INNER JOIN
* LEFT JOIN
* CROSS JOIN

그리고

* RIGHT JOIN (개념)

을 이해하면 대부분의 DBMS를 사용할 수 있다.

---

# 실습용 테이블 생성

## Product

```sql
CREATE TABLE product(
    product_id INTEGER PRIMARY KEY,
    product_name TEXT,
    price INTEGER
);
```

---

## Worker

```sql
CREATE TABLE worker(
    worker_id INTEGER PRIMARY KEY,
    worker_name TEXT,
    department TEXT
);
```

---

## Production

```sql
CREATE TABLE production(
    production_id INTEGER PRIMARY KEY,
    product_id INTEGER,
    worker_id INTEGER,
    qty INTEGER,
    prod_date TEXT
);
```

---

# 데이터 입력

## Product

```sql
INSERT INTO product VALUES(1,'Motor',50000);
INSERT INTO product VALUES(2,'Sensor',12000);
INSERT INTO product VALUES(3,'Pump',80000);
INSERT INTO product VALUES(4,'Valve',30000);
```

---

## Worker

```sql
INSERT INTO worker VALUES(1,'Kim','Assembly');
INSERT INTO worker VALUES(2,'Lee','Assembly');
INSERT INTO worker VALUES(3,'Park','QC');
INSERT INTO worker VALUES(4,'Choi','Packing');
```

---

## Production

```sql
INSERT INTO production VALUES(1,1,1,100,'2026-06-22');
INSERT INTO production VALUES(2,2,2,150,'2026-06-22');
INSERT INTO production VALUES(3,3,1,120,'2026-06-23');
INSERT INTO production VALUES(4,1,3,180,'2026-06-23');
INSERT INTO production VALUES(5,4,4,90,'2026-06-24');
```

---

# INNER JOIN

가장 많이 사용하는 JOIN이다.

양쪽 테이블에 모두 존재하는 데이터만 조회한다.

형식

```sql
SELECT 컬럼
FROM 테이블1
INNER JOIN 테이블2
ON 연결조건;
```

---

## 예제

```sql
SELECT *
FROM production
INNER JOIN product
ON production.product_id = product.product_id;
```

결과

|생산번호|제품번호|작업자|생산량|제품명|
|---------|--------|------|------|------|
|1|1|1|100|Motor|
|2|2|2|150|Sensor|
|3|3|1|120|Pump|

---

# 컬럼 선택하기

실무에서는 필요한 컬럼만 조회한다.

```sql
SELECT
    production.production_id,
    product.product_name,
    production.qty
FROM production
INNER JOIN product
ON production.product_id = product.product_id;
```

---

# 테이블 별칭(Alias)

실무에서는 별칭을 많이 사용한다.

```sql
SELECT
    p.production_id,
    pr.product_name,
    p.qty
FROM production p
INNER JOIN product pr
ON p.product_id = pr.product_id;
```

코드가 훨씬 짧아진다.

---

# LEFT JOIN

왼쪽 테이블의 모든 데이터를 조회한다.

오른쪽 데이터가 없으면 NULL이 출력된다.

형식

```sql
SELECT *
FROM 테이블1
LEFT JOIN 테이블2
ON 조건;
```

---

예제

```sql
SELECT *
FROM product
LEFT JOIN production
ON product.product_id = production.product_id;
```

생산되지 않은 제품도 모두 출력된다.

---

# LEFT JOIN 결과 예

제품

|Motor|
|Sensor|
|Pump|
|Valve|
|Battery|

생산

Motor

Sensor

Pump

Valve

Battery는 생산기록이 없다.

결과

|제품|생산량|
|----|------|
|Motor|100|
|Sensor|200|
|Pump|150|
|Valve|300|
|Battery|NULL|

---

# RIGHT JOIN (개념)

SQLite는 RIGHT JOIN을 지원하지 않는다.

다른 DBMS에서는

오른쪽 테이블의 모든 데이터를 출력하는 JOIN이다.

즉

LEFT JOIN과 반대 개념이다.

예를 들어

```sql
A LEFT JOIN B
```

는

```sql
B RIGHT JOIN A
```

와 동일한 의미이다.

---

# CROSS JOIN

모든 경우의 수를 만든다.

형식

```sql
SELECT *
FROM table1
CROSS JOIN table2;
```

예

제품 4개

작업자 4명

결과

4 × 4 = 16개

---

예제

```sql
SELECT
product_name,
worker_name
FROM product
CROSS JOIN worker;
```

---

# 실습 1. 생산정보 + 제품정보

제품명을 함께 출력하시오.

출력

|생산번호|제품명|생산량|
|---------|------|------|

힌트

```sql
INNER JOIN
```

---

정답

```sql
SELECT
p.production_id,
pr.product_name,
p.qty
FROM production p
INNER JOIN product pr
ON p.product_id = pr.product_id;
```

---

# 실습 2. 생산정보 + 작업자

작업자 이름을 함께 출력하시오.

출력

|생산번호|작업자|생산량|

정답

```sql
SELECT
p.production_id,
w.worker_name,
p.qty
FROM production p
INNER JOIN worker w
ON p.worker_id = w.worker_id;
```

---

# 실습 3. 생산정보 + 제품정보 + 작업자

세 개의 테이블을 JOIN하여 출력하시오.

출력

|생산번호|제품명|작업자|생산량|

정답

```sql
SELECT
p.production_id,
pr.product_name,
w.worker_name,
p.qty
FROM production p
INNER JOIN product pr
ON p.product_id = pr.product_id
INNER JOIN worker w
ON p.worker_id = w.worker_id;
```

---

# 실습 4

생산량이 120개 이상인 데이터만 조회하시오.

출력

|제품명|작업자|생산량|

힌트

```sql
WHERE
```

---

정답

```sql
SELECT
pr.product_name,
w.worker_name,
p.qty
FROM production p
INNER JOIN product pr
ON p.product_id = pr.product_id
INNER JOIN worker w
ON p.worker_id = w.worker_id
WHERE p.qty >= 120;
```

---

# 실습 5

생산일이 2026-06-23인 데이터만 조회하시오.

출력

|제품명|작업자|생산량|생산일|

---

정답

```sql
SELECT
pr.product_name,
w.worker_name,
p.qty,
p.prod_date
FROM production p
INNER JOIN product pr
ON p.product_id = pr.product_id
INNER JOIN worker w
ON p.worker_id = w.worker_id
WHERE p.prod_date='2026-06-23';
```

---

# 실습 6

제품명 기준으로 오름차순 정렬하시오.

```sql
SELECT
pr.product_name,
w.worker_name,
p.qty
FROM production p
INNER JOIN product pr
ON p.product_id = pr.product_id
INNER JOIN worker w
ON p.worker_id = w.worker_id
ORDER BY pr.product_name;
```

---

# 핵심정리

* JOIN은 여러 테이블을 연결하여 조회하는 기능이다.
* 실무에서는 대부분 INNER JOIN을 사용한다.
* LEFT JOIN은 왼쪽 테이블의 모든 데이터를 조회한다.
* SQLite는 RIGHT JOIN을 지원하지 않는다.
* CROSS JOIN은 가능한 모든 조합을 생성한다.
* 생산관리 시스템에서는 제품, 작업자, 생산이력을 JOIN하여 조회하는 경우가 매우 많다.
