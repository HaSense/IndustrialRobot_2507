# Chapter 8. 정렬 (ORDER BY)

## 학습목표

* ORDER BY의 역할을 이해할 수 있다.
* 데이터를 오름차순으로 정렬할 수 있다.
* 데이터를 내림차순으로 정렬할 수 있다.
* 생산량 데이터를 다양한 기준으로 정렬할 수 있다.
* 생산 현장의 순위 데이터를 조회할 수 있다.

---

# 8.1 정렬이란?

데이터를 조회할 때 원하는 기준으로 순서를 변경하는 것을 **정렬(Sort)** 이라고 한다.

예를 들어 생산량 데이터를 조회할 때 다음과 같은 요구가 있을 수 있다.

* 생산량이 많은 순서대로 보고 싶다.
* 생산량이 적은 순서대로 보고 싶다.
* 생산일이 빠른 순서대로 보고 싶다.
* 생산일이 최근 순서대로 보고 싶다.

이때 사용하는 것이 `ORDER BY` 이다.

---

# 8.2 ORDER BY 기본 문법

## 문법

```sql id="5z1n7h"
SELECT *
FROM 테이블명
ORDER BY 컬럼명;
```

---

## 예제

```sql id="r77wkn"
SELECT *
FROM production
ORDER BY qty;
```

생산량 기준으로 정렬된다.

---

# 8.3 ASC (오름차순)

ASC는 작은 값에서 큰 값 순서로 정렬한다.

ASC는 생략 가능하다.

## 문법

```sql id="u4m5yv"
SELECT *
FROM production
ORDER BY qty ASC;
```

---

## 결과 예

```text id="lg22g9"
100
120
150
180
200
220
250
300
```

---

# 8.4 DESC (내림차순)

DESC는 큰 값에서 작은 값 순서로 정렬한다.

## 문법

```sql id="9vc56f"
SELECT *
FROM production
ORDER BY qty DESC;
```

---

## 결과 예

```text id="03hnc2"
300
250
220
200
180
150
120
100
```

---

# 8.5 문자열 정렬

제품명을 기준으로 정렬할 수도 있다.

```sql id="t4u2p2"
SELECT *
FROM production
ORDER BY product_name ASC;
```

결과 예

```text id="c4md5u"
Battery
Bearing
Frame
Module
Motor
Pump
Sensor
Valve
```

---

# 8.6 날짜 정렬

생산일 기준으로 정렬할 수 있다.

```sql id="q2j2fi"
SELECT *
FROM production
ORDER BY prod_date ASC;
```

---

최근 생산일 순 조회

```sql id="y3s7cg"
SELECT *
FROM production
ORDER BY prod_date DESC;
```

---

# 8.7 특정 컬럼만 정렬 조회

```sql id="x08mq8"
SELECT product_name, qty
FROM production
ORDER BY qty DESC;
```

결과 예

```text id="1qf22x"
Valve|300
Bearing|250
Frame|220
Sensor|200
Module|180
Pump|150
Battery|120
Motor|100
```

---

# 제조 현장 활용 예제

## 생산량 순위 조회

```sql id="buxmkj"
SELECT *
FROM production
ORDER BY qty DESC;
```

---

## 최신 생산 데이터 조회

```sql id="8r5tso"
SELECT *
FROM production
ORDER BY prod_date DESC;
```

---

## 제품명 순 정렬

```sql id="5jzrra"
SELECT *
FROM production
ORDER BY product_name ASC;
```

---

# 실습 1. 데이터 확인

```sql id="mj2e6v"
.mode table
.headers on

SELECT *
FROM production;
```

현재 저장된 데이터를 확인한다.

---

# 실습 2. 생산량 오름차순 정렬

```sql id="wccmlc"
SELECT *
FROM production
ORDER BY qty ASC;
```

가장 적게 생산된 제품이 맨 위에 표시되는지 확인하시오.

---

# 실습 3. 생산량 내림차순 정렬

```sql id="r6i2je"
SELECT *
FROM production
ORDER BY qty DESC;
```

가장 많이 생산된 제품이 맨 위에 표시되는지 확인하시오.

---

# 실습 4. 제품명 오름차순 정렬

```sql id="4mwr5e"
SELECT *
FROM production
ORDER BY product_name ASC;
```

알파벳 순으로 정렬되는지 확인하시오.

---

# 실습 5. 제품명 내림차순 정렬

```sql id="8i9rrn"
SELECT *
FROM production
ORDER BY product_name DESC;
```

---

# 실습 6. 생산일 오름차순 정렬

```sql id="sxxzk5"
SELECT *
FROM production
ORDER BY prod_date ASC;
```

가장 오래된 생산일이 위에 표시되는지 확인하시오.

---

# 실습 7. 생산일 내림차순 정렬

```sql id="jokf5p"
SELECT *
FROM production
ORDER BY prod_date DESC;
```

최근 생산 데이터가 위에 표시되는지 확인하시오.

---

# 실습 8. 생산량 순위표 만들기

다음 SQL을 실행하시오.

```sql id="gb3kh6"
SELECT product_name, qty
FROM production
ORDER BY qty DESC;
```

결과 예

```text id="yc1bzk"
Valve      300
Bearing    250
Frame      220
Sensor     200
Module     180
Pump       150
Battery    120
Motor      100
```

생산량 1등 제품을 확인하시오.

---

# 실습 9. 제조AI 생산관리 실습

생산팀에서 요청하였다.

### 요구사항

제품명과 생산량만 표시하고

생산량이 높은 순서대로 출력하시오.

정답

```sql id="yag0cm"
SELECT product_name, qty
FROM production
ORDER BY qty DESC;
```

---

# 실습 10. 품질관리 실습

품질팀에서 최근 생산 데이터를 확인하려고 한다.

### 요구사항

생산일이 최근인 순서대로 출력하시오.

```sql id="0d8vk0"
SELECT *
FROM production
ORDER BY prod_date DESC;
```

---

# 실습 11. 직접 SQL 작성

다음 요구사항을 만족하는 SQL을 작성하시오.

### 문제 1

제품명을 가나다(알파벳) 순으로 정렬

### 문제 2

제품명을 역순으로 정렬

### 문제 3

생산량이 적은 순으로 정렬

### 문제 4

생산량이 많은 순으로 정렬

### 문제 5

생산일이 최신인 순으로 정렬

---

# 실습 12. 생산 순위 분석

다음 질문에 답하시오.

### 문제 1

생산량 1위 제품은?

### 문제 2

생산량 2위 제품은?

### 문제 3

가장 최근 생산된 날짜는?

### 문제 4

가장 적게 생산된 제품은?

힌트

```sql id="qktlx8"
ORDER BY
```

---

# 실습 13. 추가 데이터 입력 후 정렬

다음 데이터를 추가하시오.

```sql id="7w7mwu"
INSERT INTO production
VALUES(9,'Robot',500,'2026-06-26');

INSERT INTO production
VALUES(10,'Panel',90,'2026-06-26');
```

전체 조회

```sql id="ggm53d"
SELECT *
FROM production;
```

---

# 실습 14. 생산량 순위 재조회

```sql id="hf1ekn"
SELECT product_name, qty
FROM production
ORDER BY qty DESC;
```

질문

### 생산량 1위 제품은?

### 생산량 최하위 제품은?

---

# 실습 15. 종합 실습

다음 결과가 나오도록 SQL을 작성하시오.

```text id="lgr3up"
Robot     500
Valve     300
Bearing   250
Frame     220
Sensor    200
...
```

정답 조건

* 제품명 출력
* 생산량 출력
* 생산량 높은 순 정렬

---

# 핵심 정리

| 문법                | 설명       |
| ----------------- | -------- |
| ORDER BY          | 정렬       |
| ASC               | 오름차순     |
| DESC              | 내림차순     |
| ORDER BY qty ASC  | 생산량 오름차순 |
| ORDER BY qty DESC | 생산량 내림차순 |

---

# 확인 문제

### 문제 1

생산량을 높은 순으로 정렬하는 SQL은?

### 문제 2

제품명을 오름차순으로 정렬하는 SQL은?

### 문제 3

생산일을 최신 순으로 정렬하는 SQL은?

### 문제 4

ASC와 DESC의 차이는 무엇인가?

### 문제 5

다음 결과가 출력되도록 SQL을 작성하시오.

```text id="5xj83q"
Robot     500
Valve     300
Bearing   250
```
