# Chapter 7. 조건 검색 (WHERE)

## 학습목표

* WHERE 절의 역할을 이해할 수 있다.
* 특정 조건에 맞는 데이터만 조회할 수 있다.
* 비교 연산자를 사용할 수 있다.
* 제조 현장의 생산 데이터를 조건별로 검색할 수 있다.

---

# 7.1 조건 검색이란?

지금까지는 테이블의 모든 데이터를 조회하였다.

```sql
SELECT *
FROM production;
```

하지만 실제 현장에서는 특정 데이터만 보고 싶은 경우가 많다.

예를 들어

* 생산량이 200개 이상인 제품
* Motor 제품만 조회
* 특정 날짜의 생산 기록 조회

이럴 때 사용하는 것이 **WHERE** 절이다.

---

# 7.2 WHERE 기본 문법

## 문법

```sql
SELECT 컬럼명
FROM 테이블명
WHERE 조건;
```

---

## 예제

```sql
SELECT *
FROM production
WHERE qty > 200;
```

---

# 7.3 비교 연산자

WHERE 절에서는 비교 연산자를 사용한다.

| 연산자 | 의미     |
| --- | ------ |
| =   | 같다     |
| !=  | 같지 않다  |
| <>  | 같지 않다  |
| >   | 크다     |
| <   | 작다     |
| >=  | 크거나 같다 |
| <=  | 작거나 같다 |

---

# 7.4 숫자 조건 검색

생산량이 200보다 큰 데이터 조회

```sql
SELECT *
FROM production
WHERE qty > 200;
```

결과 예

```text
4|Valve|300|2026-06-23
5|Bearing|250|2026-06-24
8|Frame|220|2026-06-25
```

---

# 7.5 문자열 조건 검색

제품명이 Motor인 데이터 조회

```sql
SELECT *
FROM production
WHERE product_name = 'Motor';
```

결과

```text
1|Motor|100|2026-06-22
```

문자열은 반드시 작은따옴표를 사용한다.

---

# 7.6 날짜 조건 검색

특정 날짜 생산 데이터 조회

```sql
SELECT *
FROM production
WHERE prod_date = '2026-06-25';
```

---

# 7.7 특정 컬럼만 조회

조건 검색과 특정 컬럼 조회를 함께 사용할 수 있다.

```sql
SELECT product_name, qty
FROM production
WHERE qty >= 200;
```

결과 예

```text
Valve|300
Bearing|250
Frame|220
```

---

# 7.8 제조 현장 활용 사례

## 생산량 200개 이상 제품 조회

```sql
SELECT *
FROM production
WHERE qty >= 200;
```

---

## Motor 제품 조회

```sql
SELECT *
FROM production
WHERE product_name = 'Motor';
```

---

## 특정 날짜 생산실적 조회

```sql
SELECT *
FROM production
WHERE prod_date = '2026-06-25';
```

---

# 실습 1. 데이터 확인

먼저 데이터를 확인한다.

```sql
.mode table
.headers on

SELECT *
FROM production;
```

예상 데이터

```text
+----+--------------+-----+------------+
| id | product_name | qty | prod_date  |
+----+--------------+-----+------------+
| 1  | Motor        | 100 | 2026-06-22 |
| 2  | Sensor       | 200 | 2026-06-22 |
| 3  | Pump         | 150 | 2026-06-23 |
| 4  | Valve        | 300 | 2026-06-23 |
| 5  | Bearing      | 250 | 2026-06-24 |
| 6  | Battery      | 120 | 2026-06-25 |
| 7  | Module       | 180 | 2026-06-25 |
| 8  | Frame        | 220 | 2026-06-25 |
+----+--------------+-----+------------+
```

---

# 실습 2. 생산량이 200보다 큰 데이터 조회

```sql
SELECT *
FROM production
WHERE qty > 200;
```

몇 개의 제품이 조회되는지 확인하시오.

---

# 실습 3. 생산량이 200 이상인 데이터 조회

```sql
SELECT *
FROM production
WHERE qty >= 200;
```

실습 2와 결과를 비교하시오.

---

# 실습 4. 생산량이 150 미만인 제품 조회

```sql
SELECT *
FROM production
WHERE qty < 150;
```

---

# 실습 5. Motor 제품 조회

```sql
SELECT *
FROM production
WHERE product_name = 'Motor';
```

---

# 실습 6. Sensor 제품 조회

```sql
SELECT *
FROM production
WHERE product_name = 'Sensor';
```

---

# 실습 7. 생산일이 2026-06-25인 데이터 조회

```sql
SELECT *
FROM production
WHERE prod_date = '2026-06-25';
```

---

# 실습 8. 생산일이 2026-06-23인 데이터 조회

```sql
SELECT *
FROM production
WHERE prod_date = '2026-06-23';
```

---

# 실습 9. 생산량이 200 이상인 제품명만 조회

```sql
SELECT product_name
FROM production
WHERE qty >= 200;
```

---

# 실습 10. 생산량이 200 이상인 제품명과 생산량 조회

```sql
SELECT product_name, qty
FROM production
WHERE qty >= 200;
```

---

# 실습 11. 같지 않은 데이터 조회

Motor가 아닌 제품 조회

```sql
SELECT *
FROM production
WHERE product_name != 'Motor';
```

---

# 실습 12. 제조AI 생산관리 실습

다음 요구사항에 맞는 SQL을 작성하시오.

### 문제 1

생산량이 250 이상인 제품 조회

### 문제 2

Battery 제품만 조회

### 문제 3

2026-06-22 생산 데이터 조회

### 문제 4

생산량이 150 이하인 제품 조회

### 문제 5

Motor 제품의 생산량만 조회

---

# 실습 13. 품질관리 실습

품질팀에서 다음 정보를 요청하였다.

### 요구사항

생산량이 200개 이상인 제품의

* 제품명
* 생산량
* 생산일

만 출력하시오.

정답 예시

```sql
SELECT product_name, qty, prod_date
FROM production
WHERE qty >= 200;
```

---

# 실습 14. SQL 작성 연습

다음 결과가 나오도록 SQL을 작성하시오.

```text
Valve|300
Bearing|250
Frame|220
```

---

# 실습 15. 데이터 분석 실습

다음 질문에 SQL로 답하시오.

### 문제 1

생산량이 가장 많은 제품 후보는?

### 문제 2

생산량이 200개 이상인 제품은 몇 개인가?

### 문제 3

2026-06-25 생산 제품은 몇 개인가?

### 문제 4

Motor 제품의 생산량은 얼마인가?

---

# 핵심 정리

| 문법    | 설명     |
| ----- | ------ |
| WHERE | 조건 검색  |
| =     | 같다     |
| !=    | 같지 않다  |
| >     | 크다     |
| <     | 작다     |
| >=    | 크거나 같다 |
| <=    | 작거나 같다 |

---

# 확인 문제

### 문제 1

생산량이 300인 데이터를 조회하는 SQL은?

### 문제 2

제품명이 Frame인 데이터를 조회하는 SQL은?

### 문제 3

생산일이 2026-06-24인 데이터를 조회하는 SQL은?

### 문제 4

생산량이 200 이상인 제품명만 조회하는 SQL은?

### 문제 5

Motor가 아닌 모든 데이터를 조회하는 SQL은?
