# Chapter 10. SQLite 일반 함수

## 학습 목표

이번 장에서는 SQLite에서 제공하는 일반 함수(Function)를 이용하여 데이터를 보다 효율적으로 처리하는 방법을 학습한다.

학습 내용

- 문자열 함수
- 숫자 함수
- 날짜 함수
- NULL 처리 함수
- 계산식 작성
- 함수를 활용한 실무 데이터 분석

---

# 함수(Function)란?

함수(Function)는 데이터를 가공하거나 계산하는 기능을 제공하는 명령이다.

예를 들어

제품명이

```
Motor
```

일 때

대문자로 변경하면

```
MOTOR
```

문자열 길이를 구하면

```
5
```

현재 날짜를 출력하면

```
2026-06-25
```

처럼 새로운 결과를 만들어 낸다.

---

# 함수를 사용하는 이유

데이터베이스에는 수천~수백만 건의 데이터가 저장될 수 있다.

만약 제품명의 길이를 모두 계산하거나

모든 생산금액을 계산하거나

오늘 날짜와 비교하려면

프로그램에서 계산하는 것보다 SQL 함수가 훨씬 빠르고 간단하다.

예)

```
제품명 길이

↓

LENGTH()

↓

SQL이 자동 계산
```

---

# 함수의 기본 문법

함수는 다음과 같이 사용한다.

```sql
함수명(컬럼명)
```

예)

```sql
SELECT LENGTH(product_name)
FROM production;
```

여러 개의 함수도 사용할 수 있다.

```sql
SELECT
UPPER(product_name),
LENGTH(product_name)
FROM production;
```

---

# SQLite에서 자주 사용하는 함수

## 1. 문자열 함수

|함수|설명|
|----|----|
|LENGTH()|문자열 길이|
|UPPER()|대문자|
|LOWER()|소문자|
|SUBSTR()|문자열 일부 추출|
|REPLACE()|문자열 변경|
|TRIM()|공백 제거|

---

## 2. 숫자 함수

|함수|설명|
|----|----|
|ROUND()|반올림|
|ABS()|절댓값|
|MIN()|최소값|
|MAX()|최대값|

---

## 3. 날짜 함수

|함수|설명|
|----|----|
|DATE()|날짜|
|TIME()|시간|
|DATETIME()|날짜와 시간|
|STRFTIME()|날짜 형식 변경|

---

## 4. NULL 처리 함수

|함수|설명|
|----|----|
|IFNULL()|NULL이면 다른 값 출력|
|COALESCE()|NULL이 아닌 첫 번째 값 반환|

---

# 함수는 SELECT와 함께 사용한다.

가장 많이 사용하는 형태

```sql
SELECT 함수명(컬럼명)
FROM 테이블명;
```

예)

```sql
SELECT
UPPER(product_name)
FROM production;
```

---

# 여러 함수를 함께 사용할 수도 있다.

```sql
SELECT
UPPER(product_name),
LENGTH(product_name),
ROUND(price*1.1,0)
FROM production;
```

실행 결과

|제품명|글자수|부가세 포함 가격|
|------|------|--------------|
|MOTOR|5|5500|
|SENSOR|6|13200|

---

# 제조업에서 함수 활용 사례

|업무|사용 함수|
|----|---------|
|제품명 출력|UPPER()|
|LOT 길이 확인|LENGTH()|
|총 생산금액|quantity * price|
|부가세 계산|ROUND()|
|현재 날짜 출력|DATE()|
|NULL 데이터 처리|IFNULL()|
