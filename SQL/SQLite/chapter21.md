# Chapter 21. 서브쿼리(Subquery)

---

# 학습목표

본 단원을 학습한 후 다음 내용을 설명할 수 있다.

- 서브쿼리(Subquery)의 개념을 이해할 수 있다.
- 단일 행(Subquery)을 사용할 수 있다.
- 다중 행(Subquery)을 사용할 수 있다.
- IN과 EXISTS를 이용한 서브쿼리를 작성할 수 있다.
- FROM절에서 사용하는 서브쿼리를 이해할 수 있다.
- 스마트팩토리 데이터를 이용하여 다양한 서브쿼리를 작성할 수 있다.

---

# 서브쿼리(Subquery)란?

서브쿼리(Subquery)는 SQL문 안에 포함되어 있는 또 하나의 SELECT문이다.

즉,

> SELECT문 안에 또 다른 SELECT문이 들어있는 형태

이다.

형태

```sql
SELECT ...
FROM ...
WHERE 컬럼 =
(
    SELECT ...
);
```

---

# 왜 사용할까?

예를 들어 다음과 같은 질문이 있다고 하자.

- 평균 재고보다 많이 보유한 자재는?
- 가장 비싼 자재는?
- 입고 이력이 있는 자재는?
- 평균 이상 입고된 자재는?

이러한 문제는 먼저 데이터를 계산한 후 그 결과를 이용하여 다시 조회해야 한다.

이를 하나의 SQL문으로 해결하는 것이 서브쿼리이다.

---

# 실습용 테이블 생성

## material

```sql
CREATE TABLE material
(
    material_id INTEGER PRIMARY KEY,
    material_name TEXT,
    stock_qty INTEGER,
    unit_price INTEGER
);
```

데이터 입력

```sql
INSERT INTO material VALUES
(1,'Steel Plate',120,45000),
(2,'Bolt',800,500),
(3,'Nut',950,300),
(4,'Bearing',80,12000),
(5,'Sensor',60,38000),
(6,'Motor',30,150000),
(7,'Cable',400,2500),
(8,'Cylinder',45,98000);
```

---

## material_in

```sql
CREATE TABLE material_in
(
    in_id INTEGER PRIMARY KEY,
    material_name TEXT,
    in_qty INTEGER,
    supplier TEXT,
    in_date TEXT
);
```

데이터 입력

```sql
INSERT INTO material_in VALUES
(1,'Steel Plate',50,'POSCO','2026-06-21'),
(2,'Bolt',500,'ABC','2026-06-22'),
(3,'Bearing',30,'SKF','2026-06-22'),
(4,'Motor',20,'LS','2026-06-23'),
(5,'Cable',150,'LS','2026-06-23'),
(6,'Sensor',40,'Samsung','2026-06-24'),
(7,'Cylinder',15,'SMC','2026-06-24'),
(8,'Bolt',300,'ABC','2026-06-25');
```

---

# 데이터 확인

```sql
SELECT *
FROM material;

SELECT *
FROM material_in;
```

---

# 단일 행 서브쿼리

평균 재고보다 많은 자재를 조회하시오.

```sql
SELECT *
FROM material
WHERE stock_qty >
(
    SELECT AVG(stock_qty)
    FROM material
);
```

실행 순서

```
평균 재고 계산

↓

평균보다 큰 자재 조회
```

---

# 실습 1

평균 재고보다 적은 자재를 조회하시오.

---

# 가장 비싼 자재 조회

```sql
SELECT *
FROM material
WHERE unit_price =
(
    SELECT MAX(unit_price)
    FROM material
);
```

---

# 가장 저렴한 자재 조회

```sql
SELECT *
FROM material
WHERE unit_price =
(
    SELECT MIN(unit_price)
    FROM material
);
```

---

# 실습 2

가장 비싼 자재와 가장 저렴한 자재를 각각 조회하시오.

---

# 다중 행 서브쿼리

100개 이상 입고된 자재의 정보를 조회하시오.

먼저

```sql
SELECT material_name
FROM material_in
WHERE in_qty >= 100;
```

결과

```
Bolt
Cable
```

생성된 결과를 이용한다.

```sql
SELECT *
FROM material
WHERE material_name IN
(
    SELECT material_name
    FROM material_in
    WHERE in_qty >=100
);
```

---

# IN 사용

```sql
SELECT *
FROM material
WHERE material_name IN
(
    SELECT material_name
    FROM material_in
);
```

IN은 여러 개의 값을 비교할 때 사용한다.

---

# 실습 3

입고 이력이 있는 자재만 조회하시오.

---

# EXISTS

```sql
SELECT *
FROM material m
WHERE EXISTS
(
    SELECT *
    FROM material_in i
    WHERE m.material_name=i.material_name
);
```

EXISTS는 데이터의 존재 여부만 확인한다.

---

# 실습 4

EXISTS를 이용하여 입고 기록이 있는 자재를 조회하시오.

---

# FROM절의 서브쿼리

공급업체별 총 입고 수량 계산

```sql
SELECT *
FROM
(
    SELECT supplier,
           SUM(in_qty) total_qty
    FROM material_in
    GROUP BY supplier
);
```

결과

|supplier|total_qty|
|---|---|
|ABC|800|
|LS|170|
|POSCO|50|
|SKF|30|
|Samsung|40|
|SMC|15|

---

# 실습 5

공급업체별 총 입고량을 조회하시오.

---

# 중첩 서브쿼리

평균 공급량보다 많이 공급한 업체를 조회하시오.

```sql
SELECT *
FROM
(
    SELECT supplier,
           SUM(in_qty) total_qty
    FROM material_in
    GROUP BY supplier
)
WHERE total_qty >
(
    SELECT AVG(total_qty)
    FROM
    (
        SELECT SUM(in_qty) total_qty
        FROM material_in
        GROUP BY supplier
    )
);
```

---

# 실습 6

평균보다 많이 공급한 업체를 조회하시오.

---

# 스마트팩토리 실전 예제 1

가장 재고가 많은 자재 조회

```sql
SELECT *
FROM material
WHERE stock_qty =
(
    SELECT MAX(stock_qty)
    FROM material
);
```

---

# 스마트팩토리 실전 예제 2

평균 단가보다 비싼 자재 조회

```sql
SELECT *
FROM material
WHERE unit_price >
(
    SELECT AVG(unit_price)
    FROM material
);
```

---

# 스마트팩토리 실전 예제 3

200개 이상 입고된 자재 조회

```sql
SELECT *
FROM material
WHERE material_name IN
(
    SELECT material_name
    FROM material_in
    WHERE in_qty >=200
);
```

---

# 스마트팩토리 실전 예제 4

LS에서 공급한 자재의 정보를 조회하시오.

```sql
SELECT *
FROM material
WHERE material_name IN
(
    SELECT material_name
    FROM material_in
    WHERE supplier='LS'
);
```

---

# 종합 실습

다음을 모두 수행하시오.

1. 평균 재고보다 많은 자재 조회
2. 평균 재고보다 적은 자재 조회
3. 가장 비싼 자재 조회
4. 가장 저렴한 자재 조회
5. 100개 이상 입고된 자재 조회
6. 입고 기록이 있는 자재 조회
7. 평균 단가보다 비싼 자재 조회
8. LS에서 공급한 자재 조회

---

# 응용 실습 1

공급업체별 총 입고량을 계산한 후

평균 공급량보다 많이 공급한 업체를 조회하시오.

---

# 응용 실습 2

다음 조건을 만족하는 자재를 조회하시오.

- 재고는 평균 이상이다.
- 단가는 평균 이하이다.

서브쿼리를 이용하여 작성하시오.

---

# 응용 실습 3 (실무형)

다음 조건을 만족하는 자재를 조회하시오.

- 입고 이력이 존재한다.
- 단가는 평균 이상이다.
- 재고는 평균 이상이다.

서브쿼리를 이용하여 작성하시오.

---

# 핵심 정리

- 서브쿼리는 SELECT문 안에 포함된 또 하나의 SELECT문이다.
- 하나의 값을 반환하면 단일 행 서브쿼리이다.
- 여러 개의 값을 반환하면 다중 행 서브쿼리이다.
- 여러 개의 값을 비교할 때는 IN을 사용한다.
- 데이터 존재 여부를 확인할 때는 EXISTS를 사용한다.
- FROM절에서도 서브쿼리를 사용할 수 있다.
- 스마트팩토리에서는 자재 재고 분석, 공급업체 분석, 입고 이력 조회, 평균 재고 비교 등에 서브쿼리가 자주 활용된다.
