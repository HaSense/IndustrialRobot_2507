# Chapter 9. 데이터 개수 및 통계

## 학습목표

* COUNT 함수를 사용할 수 있다.
* SUM 함수를 사용할 수 있다.
* AVG 함수를 사용할 수 있다.
* MIN 함수를 사용할 수 있다.
* MAX 함수를 사용할 수 있다.
* 생산 데이터를 이용한 기초 통계를 계산할 수 있다.

---

# 9.1 통계 함수란?

실제 제조 현장에서는 단순 조회보다 통계 정보가 더 중요하다.

예를 들어 다음과 같은 질문이 자주 발생한다.

* 오늘 몇 개의 제품이 생산되었는가?
* 총 생산량은 얼마인가?
* 평균 생산량은 얼마인가?
* 가장 많이 생산된 제품은?
* 가장 적게 생산된 제품은?

이러한 계산을 SQL에서는 **집계 함수(Aggregate Function)** 로 수행한다.

---

# 9.2 실습 데이터 준비

현재 production 테이블 데이터

| id | product_name | qty | prod_date  |
| -- | ------------ | --- | ---------- |
| 1  | Motor        | 100 | 2026-06-22 |
| 2  | Sensor       | 200 | 2026-06-22 |
| 3  | Pump         | 150 | 2026-06-23 |
| 4  | Valve        | 300 | 2026-06-23 |
| 5  | Bearing      | 250 | 2026-06-24 |
| 6  | Battery      | 120 | 2026-06-25 |
| 7  | Module       | 180 | 2026-06-25 |
| 8  | Frame        | 220 | 2026-06-25 |
| 9  | Robot        | 500 | 2026-06-26 |
| 10 | Panel        | 90  | 2026-06-26 |

---

# 9.3 COUNT()

COUNT는 데이터 개수를 계산한다.

## 문법

```sql id="t61s0j"
SELECT COUNT(*)
FROM production;
```

---

## 결과 예

```text id="m9kym4"
10
```

현재 저장된 제품 수는 10개이다.

---

# 9.4 SUM()

SUM은 총합을 계산한다.

## 문법

```sql id="t7e7pq"
SELECT SUM(qty)
FROM production;
```

---

## 결과 예

```text id="bjlwmr"
2110
```

전체 생산량 합계

```text id="vl76u7"
100 + 200 + 150 + 300 + 250
+ 120 + 180 + 220 + 500 + 90
= 2110
```

---

# 9.5 AVG()

AVG는 평균값을 계산한다.

## 문법

```sql id="of6y2s"
SELECT AVG(qty)
FROM production;
```

---

## 결과 예

```text id="8nk69n"
211.0
```

평균 생산량

```text id="w50g2f"
총 생산량 ÷ 제품 수
2110 ÷ 10
=
211
```

---

# 9.6 MIN()

MIN은 가장 작은 값을 찾는다.

## 문법

```sql id="rqg1q0"
SELECT MIN(qty)
FROM production;
```

---

## 결과 예

```text id="tkg9cq"
90
```

가장 적게 생산된 수량

---

# 9.7 MAX()

MAX는 가장 큰 값을 찾는다.

## 문법

```sql id="l8kvxt"
SELECT MAX(qty)
FROM production;
```

---

## 결과 예

```text id="q57z2j"
500
```

가장 많이 생산된 수량

---

# 9.8 WHERE와 함께 사용하기

특정 조건의 통계를 구할 수 있다.

## 생산량 200 이상 제품 개수

```sql id="kj5e8m"
SELECT COUNT(*)
FROM production
WHERE qty >= 200;
```

---

## 생산량 200 이상 총합

```sql id="kn8dnh"
SELECT SUM(qty)
FROM production
WHERE qty >= 200;
```

---

## 2026-06-25 생산량 합계

```sql id="7m5qv4"
SELECT SUM(qty)
FROM production
WHERE prod_date='2026-06-25';
```

---

# 실습 1. 전체 데이터 확인

```sql id="2h4t2n"
.mode table
.headers on

SELECT *
FROM production;
```

---

# 실습 2. 제품 개수 구하기

현재 등록된 제품 개수를 조회하시오.

```sql id="0lclvv"
SELECT COUNT(*)
FROM production;
```

질문

현재 몇 개의 제품이 저장되어 있는가?

---

# 실습 3. 총 생산량 구하기

```sql id="0h8uv8"
SELECT SUM(qty)
FROM production;
```

질문

전체 생산량은 얼마인가?

---

# 실습 4. 평균 생산량 구하기

```sql id="s7e3ks"
SELECT AVG(qty)
FROM production;
```

질문

평균 생산량은 얼마인가?

---

# 실습 5. 최소 생산량 구하기

```sql id="7u9u6t"
SELECT MIN(qty)
FROM production;
```

질문

가장 적게 생산된 수량은?

---

# 실습 6. 최대 생산량 구하기

```sql id="s87c06"
SELECT MAX(qty)
FROM production;
```

질문

가장 많이 생산된 수량은?

---

# 실습 7. 생산량 200 이상 제품 개수

```sql id="zowj5j"
SELECT COUNT(*)
FROM production
WHERE qty >= 200;
```

---

# 실습 8. 생산량 200 이상 총 생산량

```sql id="d94llr"
SELECT SUM(qty)
FROM production
WHERE qty >= 200;
```

---

# 실습 9. 특정 날짜 생산량 합계

2026-06-25 생산량 총합

```sql id="ubg7p5"
SELECT SUM(qty)
FROM production
WHERE prod_date='2026-06-25';
```

직접 계산 결과와 비교하시오.

---

# 실습 10. 생산량 150 이하 제품 개수

```sql id="3a2gdu"
SELECT COUNT(*)
FROM production
WHERE qty <= 150;
```

---

# 실습 11. 제조AI 생산관리 실습

생산관리팀에서 다음 정보를 요청하였다.

### 요구사항

* 총 생산량
* 평균 생산량
* 최대 생산량
* 최소 생산량

조회하시오.

정답

```sql id="s5j7it"
SELECT SUM(qty)
FROM production;

SELECT AVG(qty)
FROM production;

SELECT MAX(qty)
FROM production;

SELECT MIN(qty)
FROM production;
```

---

# 실습 12. 품질관리 실습

품질팀에서 생산량 200 이상 제품만 분석하려고 한다.

### 구할 항목

* 제품 개수
* 총 생산량
* 평균 생산량

작성하시오.

힌트

```sql id="2m0qz1"
COUNT()
SUM()
AVG()
WHERE qty >= 200
```

---

# 실습 13. 데이터 입력 후 재계산

다음 데이터를 추가하시오.

```sql id="zt7q1n"
INSERT INTO production
VALUES(11,'Camera',350,'2026-06-27');

INSERT INTO production
VALUES(12,'Controller',400,'2026-06-27');
```

다음 항목을 다시 계산하시오.

```sql id="hprzti"
SELECT COUNT(*) FROM production;

SELECT SUM(qty) FROM production;

SELECT AVG(qty) FROM production;

SELECT MIN(qty) FROM production;

SELECT MAX(qty) FROM production;
```

---

# 실습 14. 생산 보고서 만들기

다음 결과를 SQL로 계산하시오.

| 항목     | 값 |
| ------ | - |
| 총 제품 수 | ? |
| 총 생산량  | ? |
| 평균 생산량 | ? |
| 최대 생산량 | ? |
| 최소 생산량 | ? |

---

# 실습 15. 종합 문제

다음 질문에 SQL로 답하시오.

### 문제 1

생산량이 200 이상인 제품은 몇 개인가?

### 문제 2

생산량이 200 이상인 제품의 총 생산량은?

### 문제 3

생산량이 150 이하인 제품 수는?

### 문제 4

현재 최고 생산량은?

### 문제 5

현재 최저 생산량은?

---

# 핵심 정리

| 함수      | 설명  |
| ------- | --- |
| COUNT() | 개수  |
| SUM()   | 합계  |
| AVG()   | 평균  |
| MIN()   | 최소값 |
| MAX()   | 최대값 |

---

# 확인 문제

### 문제 1

전체 제품 수를 구하는 SQL은?

### 문제 2

전체 생산량 합계를 구하는 SQL은?

### 문제 3

평균 생산량을 구하는 SQL은?

### 문제 4

가장 큰 생산량을 구하는 SQL은?

### 문제 5

생산량이 200 이상인 제품 개수를 구하는 SQL은?

### 문제 6

2026-06-25 생산량 총합을 구하는 SQL은?

### 문제 7

COUNT, SUM, AVG, MIN, MAX 각각의 의미를 설명하시오.
