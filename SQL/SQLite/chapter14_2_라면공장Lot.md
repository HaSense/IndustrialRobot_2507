---

# LOT와 원재료 추적

지금까지는

```text
LOT 번호

↓

생산정보
```

만 관리하였다.

하지만 실제 공장에서는

```text
어떤 원재료를 사용했는가?
```

를 반드시 기록한다.

---

예를 들어

```text
RAMEN_20260622
```

라는 LOT는

다음 재료를 사용했다고 가정한다.

|재료|사용량|
|------|------|
|면|100kg|
|스프|20kg|
|건더기|10kg|

---

# 가장 쉬운 LOT 추적 구조

이번에는 테이블을 2개만 사용한다.

```text
LOT 테이블

↓

LOT 재료 테이블
```

---

## LOT 테이블

```sql
CREATE TABLE ramen_lot(
    lot_no TEXT PRIMARY KEY,
    product_name TEXT,
    qty INTEGER,
    prod_date TEXT
);
```

---

## LOT 재료 테이블

```sql
CREATE TABLE lot_material(
    lot_no TEXT,
    material_name TEXT,
    material_qty INTEGER
);
```

---

# 데이터 입력

## LOT 등록

```sql
INSERT INTO ramen_lot
VALUES
('RAMEN_20260622','매운라면',100,'2026-06-22');

INSERT INTO ramen_lot
VALUES
('RAMEN_20260623','매운라면',120,'2026-06-23');
```

---

## 원재료 등록

```sql
INSERT INTO lot_material
VALUES
('RAMEN_20260622','면',100);

INSERT INTO lot_material
VALUES
('RAMEN_20260622','스프',20);

INSERT INTO lot_material
VALUES
('RAMEN_20260622','건더기',10);

INSERT INTO lot_material
VALUES
('RAMEN_20260623','면',120);

INSERT INTO lot_material
VALUES
('RAMEN_20260623','스프',24);

INSERT INTO lot_material
VALUES
('RAMEN_20260623','건더기',12);
```

---

# 두 테이블 조회

LOT

```sql
SELECT *
FROM ramen_lot;
```

LOT 재료

```sql
SELECT *
FROM lot_material;
```

---

# JOIN으로 추적하기

이제 LOT 하나를 선택하면

사용한 재료를 확인할 수 있다.

```sql
SELECT
    l.lot_no,
    l.product_name,
    m.material_name,
    m.material_qty
FROM ramen_lot l
JOIN lot_material m
ON l.lot_no = m.lot_no
WHERE l.lot_no='RAMEN_20260622';
```

---

## 결과

|LOT|제품|재료|사용량|
|------|------|------|------|
|RAMEN_20260622|매운라면|면|100|
|RAMEN_20260622|매운라면|스프|20|
|RAMEN_20260622|매운라면|건더기|10|

---

# 왜 이렇게 관리할까?

며칠 후

```text
스프에서 문제가 발생하였다.
```

어떤 제품을 회수해야 할까?

JOIN을 이용하면

```text
스프를 사용한 LOT
```

를 쉽게 찾을 수 있다.

이것이

```text
Traceability
```

이다.

---

# 실제 MES에서는?

실제 공장은 이보다 훨씬 많은 정보를 관리한다.

대표적인 LOT 추적 구조는 다음과 같다.

```text
제품(Product)

↓

생산 LOT(Lot)

↓

원재료(Material)

↓

설비(Equipment)

↓

작업자(Operator)

↓

품질검사(QC)

↓

출하(Shipment)
```

---

# 실제 MES 테이블 예

```sql
product
-------------------
product_id
product_name
spec

lot
-------------------
lot_no
product_id
qty
prod_date

material
-------------------
material_id
material_name

lot_material
-------------------
lot_no
material_id
qty

equipment
-------------------
equipment_id
equipment_name

lot_equipment
-------------------
lot_no
equipment_id

employee
-------------------
emp_id
emp_name

lot_operator
-------------------
lot_no
emp_id

inspection
-------------------
inspect_id
lot_no
result
inspect_date
```

---

# 실제 추적 예

어느 날

```text
LOT

RAMEN_20260622
```

에 문제가 발생하였다.

MES에서는 다음 정보를 모두 확인할 수 있다.

```text
제품

↓

사용한 원재료

↓

사용 설비

↓

작업자

↓

품질검사 결과

↓

출하된 거래처
```

이 모든 정보가 하나의 LOT 번호를 중심으로 연결된다.

---

# 실습

## 문제 1

모든 LOT와 재료를 JOIN하여 조회하시오.

---

## 문제 2

RAMEN_20260623의 재료만 조회하시오.

---

## 문제 3

스프를 사용한 LOT를 조회하시오.

---

## 문제 4

각 LOT별 사용한 재료 개수를 계산하시오.

힌트

```sql
COUNT()
GROUP BY
```

---

# 핵심 정리

* LOT는 생산 단위를 관리하는 핵심 정보이다.
* LOT와 원재료를 연결하면 어떤 재료가 사용되었는지 추적할 수 있다.
* JOIN을 이용하면 LOT별 재료 정보를 쉽게 조회할 수 있다.
* 실제 MES는 LOT를 중심으로 원재료, 설비, 작업자, 품질검사, 출하 정보를 모두 연결하여 관리한다.
