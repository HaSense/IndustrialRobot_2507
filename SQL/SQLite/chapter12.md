# Part 5. 스마트팩토리 데이터 모델링

# Chapter 12. 제품 관리

---

# 학습목표

본 단원을 학습한 후 다음 내용을 설명할 수 있다.

* 데이터 모델링의 개념을 이해할 수 있다.
* 제품(Product) 테이블이 필요한 이유를 설명할 수 있다.
* 제품 정보를 데이터베이스로 관리할 수 있다.
* PRIMARY KEY의 역할을 이해할 수 있다.
* 제품 테이블을 설계하고 생성할 수 있다.

---

# 1. 데이터 모델링이란?

데이터 모델링(Data Modeling)은

```text id="1c1d01"
현실 세계의 정보를
데이터베이스 구조로 설계하는 과정
```

이다.

---

예를 들어 제조공장에는 다음과 같은 정보가 존재한다.

```text id="56bb1e"
제품
생산실적
LOT
설비
작업자
품질검사
```

이러한 정보를 테이블로 설계하는 과정이 데이터 모델링이다.

---

# 2. 스마트팩토리에서 제품이란?

제품(Product)은 공장에서 생산되는 대상이다.

예)

| 제품코드 | 제품명     |
| ---- | ------- |
| P001 | Motor   |
| P002 | Sensor  |
| P003 | Bearing |

---

실제 MES에서는 제품 정보가 매우 중요하다.

생산실적도

```text id="4c41a8"
어떤 제품을
몇 개 생산했는가?
```

를 기록하기 때문이다.

---

# 3. 제품 테이블이 필요한 이유

잘못된 설계 예시

생산실적 테이블

| id | product_name | qty |
| -- | ------------ | --- |
| 1  | Motor        | 100 |
| 2  | Motor        | 120 |
| 3  | Motor        | 80  |

---

문제점

```text id="ea7c49"
Motor
```

가 계속 반복된다.

---

오타 발생 가능

```text id="f0c909"
Motor
motor
MOTOR
Motro
```

---

좋은 설계

제품 테이블

| product_id | name   |
| ---------- | ------ |
| 1          | Motor  |
| 2          | Sensor |

생산실적 테이블

| history_id | product_id | qty |
| ---------- | ---------- | --- |
| 1          | 1          | 100 |
| 2          | 1          | 120 |

---

장점

* 데이터 중복 감소
* 오타 방지
* 저장 공간 절약
* 관리 용이

---

# 4. 제품 관리 도메인 지식

실제 제조업에서는 제품마다 다음 정보를 가진다.

---

## 제품 코드

```text id="a0df22"
P001
P002
P003
```

---

## 제품명

```text id="d9174a"
Motor
Sensor
Bearing
```

---

## 단가

```text id="ea51bb"
5000
12000
3000
```

---

## 규격

```text id="c6a6a7"
M-100
S-200
B-300
```

---

## 사용 여부

```text id="ec13f8"
사용중
단종
시험품
```

---

실제 ERP/MES에서는 수십 개 이상의 항목이 존재한다.

---

# 5. 제품 테이블 설계

이번 수업에서는 단순화하여 설계한다.

---

## 요구사항

제품은 다음 정보를 가진다.

| 항목         | 설명   |
| ---------- | ---- |
| product_id | 제품번호 |
| name       | 제품명  |
| price      | 단가   |

---

## ERD 개념

```text id="d04c76"
Products
------------------
product_id
name
price
```

---

# 6. 제품 테이블 생성

```sql id="0fdb73"
CREATE TABLE products(
    product_id INTEGER PRIMARY KEY,
    name TEXT,
    price INTEGER
);
```

---

설명

```text id="07d4a4"
product_id : 제품번호
name       : 제품명
price      : 단가
```

---

# 7. 테이블 확인

```sql id="af4b67"
.tables
```

---

결과

```text id="f73e0d"
products
production
```

---

구조 확인

```sql id="dc97f4"
.schema products
```

---

# 8. 제품 등록

제품 추가

```sql id="a44a45"
INSERT INTO products
VALUES(1,'Motor',5000);
```

---

```sql id="879a96"
INSERT INTO products
VALUES(2,'Sensor',12000);
```

---

```sql id="dfc561"
INSERT INTO products
VALUES(3,'Bearing',3000);
```

---

조회

```sql id="15f36b"
SELECT * FROM products;
```

---

결과

```text id="eb648c"
1|Motor|5000
2|Sensor|12000
3|Bearing|3000
```

---

# 실습 1. 제품 테이블 생성

다음 SQL을 실행하시오.

```sql id="5e727f"
CREATE TABLE products(
    product_id INTEGER PRIMARY KEY,
    name TEXT,
    price INTEGER
);
```

---

테이블 생성 확인

```sql id="fe5fcb"
.tables
```

---

# 실습 2. 제품 3개 등록

다음 데이터를 입력하시오.

```sql id="b72db8"
INSERT INTO products
VALUES(1,'Motor',5000);
```

```sql id="7a01a6"
INSERT INTO products
VALUES(2,'Sensor',12000);
```

```sql id="52ea3c"
INSERT INTO products
VALUES(3,'Bearing',3000);
```

---

조회

```sql id="5291ae"
SELECT * FROM products;
```

---

# 실습 3. 새로운 제품 추가

다음 제품을 추가하시오.

| 제품번호 | 제품명  | 단가   |
| ---- | ---- | ---- |
| 4    | Pump | 8000 |

---

정답 예시

```sql id="f2c495"
INSERT INTO products
VALUES(4,'Pump',8000);
```

---

# 실습 4. 가격이 가장 비싼 제품 찾기

조회

```sql id="f01c97"
SELECT *
FROM products
ORDER BY price DESC;
```

---

가장 위에 표시되는 제품을 확인하시오.

---

# 실습 5. 가격순 정렬

오름차순

```sql id="ff8172"
SELECT *
FROM products
ORDER BY price ASC;
```

---

내림차순

```sql id="1f05c9"
SELECT *
FROM products
ORDER BY price DESC;
```

---

# 실습 6. 제품 수 확인

```sql id="d7594d"
SELECT COUNT(*)
FROM products;
```

---

현재 등록된 제품 수를 확인하시오.

---

# 실습 7. 평균 단가 계산

```sql id="69af70"
SELECT AVG(price)
FROM products;
```

---

현재 등록된 제품들의 평균 단가를 계산하시오.

---

# 도전 과제

다음 제품을 추가하시오.

| 제품번호 | 제품명   | 단가    |
| ---- | ----- | ----- |
| 5    | Valve | 7000  |
| 6    | Gear  | 9000  |
| 7    | Shaft | 15000 |

---

문제 1

가장 비싼 제품 조회

---

문제 2

가장 저렴한 제품 조회

---

문제 3

전체 제품 수 조회

---

문제 4

평균 단가 조회

---

# 제조AI 관점

실제 MES 구조

```text id="ab93a3"
Products
     ↓
Production History
     ↓
LOT History
     ↓
Quality
```

---

제품은 모든 데이터의 기준이 된다.

예)

```text id="3152c7"
Motor 생산량
Motor LOT
Motor 품질정보
```

모두 제품 테이블과 연결된다.

---

# 핵심 정리

* 데이터 모델링은 현실 세계를 테이블로 설계하는 과정이다.
* 제품은 제조 데이터의 기준 정보(Master Data)이다.
* 제품 테이블은 데이터 중복을 줄여준다.
* product_id는 PRIMARY KEY로 사용한다.
* 제품 정보는 생산실적, LOT, 품질 데이터와 연결된다.

---

# 다음 시간

Chapter 13. 생산 이력 관리

* 생산 이력 테이블 설계
* 생산실적 저장
* 제품과 생산이력 관계
* 제조 데이터 추적
