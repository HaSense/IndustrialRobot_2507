# Chapter 9. 그룹화 (GROUP BY / HAVING)

---

# 학습 목표

이번 장에서는 다음 내용을 학습한다.

- GROUP BY의 개념 이해
- 같은 데이터를 하나의 그룹으로 묶기
- COUNT, SUM, AVG와 함께 사용하기
- HAVING을 이용한 그룹 조건 검색
- 생산 데이터를 분석하는 SQL 작성하기

---

# 1. GROUP BY란?

GROUP BY는

**같은 값을 가지는 행(Row)들을 하나의 그룹(Group)으로 묶는 기능**이다.

예를 들어

| 제품명 | 생산량 |
|--------|--------|
| Motor | 100 |
| Motor | 120 |
| Sensor | 80 |
| Motor | 150 |
| Sensor | 90 |

GROUP BY product_name 을 수행하면

| 제품명 | 총 생산량 |
|--------|-----------|
| Motor | 370 |
| Sensor | 170 |

처럼 하나의 행으로 묶인다.

---

# GROUP BY 문법

```sql
SELECT 그룹컬럼,
       집계함수
FROM 테이블
GROUP BY 그룹컬럼;
```

예제

```sql
SELECT product_name,
       SUM(qty)
FROM product_1
GROUP BY product_name;
```

---

# GROUP BY에서 자주 사용하는 함수

| 함수 | 의미 |
|------|------|
| COUNT() | 개수 |
| SUM() | 합계 |
| AVG() | 평균 |
| MAX() | 최대값 |
| MIN() | 최소값 |

GROUP BY와 거의 항상 같이 사용된다.

---

# 실습을 위한 테이블 생성

```sql
CREATE TABLE product_1 (
    id INTEGER PRIMARY KEY,
    product_name TEXT,
    worker TEXT,
    qty INTEGER,
    prod_date TEXT
);
```

---

# 실습 데이터 입력

```sql
INSERT INTO product_1 VALUES
(1,'Motor','Kim',120,'2026-06-20'),
(2,'Sensor','Lee',80,'2026-06-20'),
(3,'Motor','Kim',100,'2026-06-21'),
(4,'Valve','Park',90,'2026-06-21'),
(5,'Sensor','Lee',150,'2026-06-22'),
(6,'Motor','Choi',130,'2026-06-22'),
(7,'Valve','Park',110,'2026-06-23'),
(8,'Battery','Kim',200,'2026-06-23'),
(9,'Battery','Lee',180,'2026-06-24'),
(10,'Motor','Kim',140,'2026-06-24'),
(11,'Sensor','Park',90,'2026-06-24'),
(12,'Battery','Choi',170,'2026-06-25');
```

---

# 데이터 확인

```sql
SELECT *
FROM product_1;
```

예상 결과

|제품|작업자|수량|
|----|------|----|
|Motor|Kim|120|
|Sensor|Lee|80|
|Motor|Kim|100|
|...|...|...|

---

# GROUP BY 예제 1

## 제품별 생산 횟수

```sql
SELECT product_name,
       COUNT(*)
FROM product_1
GROUP BY product_name;
```

결과 예

|제품|생산횟수|
|----|---------|
|Battery|3|
|Motor|4|
|Sensor|3|
|Valve|2|

---

# GROUP BY 예제 2

## 제품별 총 생산량

```sql
SELECT product_name,
       SUM(qty)
FROM product_1
GROUP BY product_name;
```

결과

|제품|총 생산량|
|----|----------|
|Battery|550|
|Motor|490|
|Sensor|320|
|Valve|200|

---

# GROUP BY 예제 3

## 제품별 평균 생산량

```sql
SELECT product_name,
       AVG(qty)
FROM product_1
GROUP BY product_name;
```

---

# GROUP BY 예제 4

## 작업자별 생산량

```sql
SELECT worker,
       SUM(qty)
FROM product_1
GROUP BY worker;
```

예상 결과

|작업자|총 생산량|
|------|----------|
|Kim|560|
|Lee|410|
|Park|290|
|Choi|300|

---

# GROUP BY 예제 5

## 작업자별 평균 생산량

```sql
SELECT worker,
       AVG(qty)
FROM product_1
GROUP BY worker;
```

---

# GROUP BY 예제 6

## 작업자별 최대 생산량

```sql
SELECT worker,
       MAX(qty)
FROM product_1
GROUP BY worker;
```

---

# GROUP BY 여러 컬럼 사용

제품과 작업자를 함께 그룹화

```sql
SELECT product_name,
       worker,
       SUM(qty)
FROM product_1
GROUP BY product_name,
         worker;
```

결과

|제품|작업자|합계|
|----|------|----|
|Motor|Kim|360|
|Motor|Choi|130|
|Battery|Kim|200|
|Battery|Lee|180|

---

# HAVING

WHERE는

**행(Row)을 먼저 선택한다.**

HAVING은

**GROUP BY 이후 그룹을 선택한다.**

순서

```
FROM

↓

WHERE

↓

GROUP BY

↓

HAVING

↓

ORDER BY
```

---

# HAVING 문법

```sql
SELECT 그룹컬럼,
       집계함수
FROM 테이블
GROUP BY 그룹컬럼
HAVING 조건;
```

---

# HAVING 예제 1

총 생산량이 400 이상인 제품

```sql
SELECT product_name,
       SUM(qty)
FROM product_1
GROUP BY product_name
HAVING SUM(qty) >= 400;
```

결과

|제품|합계|
|----|----|
|Battery|550|
|Motor|490|

---

# HAVING 예제 2

평균 생산량이 150 이상인 제품

```sql
SELECT product_name,
       AVG(qty)
FROM product_1
GROUP BY product_name
HAVING AVG(qty) >= 150;
```

---

# HAVING 예제 3

생산 횟수가 3회 이상인 제품

```sql
SELECT product_name,
       COUNT(*)
FROM product_1
GROUP BY product_name
HAVING COUNT(*) >= 3;
```

---

# WHERE와 HAVING 비교

잘못된 예

```sql
SELECT product_name,
       SUM(qty)
FROM product_1
WHERE SUM(qty) > 300;
```

오류 발생

집계함수는 WHERE에서 사용할 수 없다.

---

올바른 방법

```sql
SELECT product_name,
       SUM(qty)
FROM product_1
GROUP BY product_name
HAVING SUM(qty) > 300;
```

---

# ORDER BY 함께 사용

총 생산량이 큰 순서

```sql
SELECT product_name,
       SUM(qty) AS total_qty
FROM product_1
GROUP BY product_name
ORDER BY total_qty DESC;
```

---

# ORDER BY + HAVING

```sql
SELECT product_name,
       SUM(qty) AS total_qty
FROM product_1
GROUP BY product_name
HAVING SUM(qty)>=300
ORDER BY total_qty DESC;
```

---

# 실습 1. 제품별 생산량 구하기

문제

각 제품의 총 생산량을 출력하시오.

출력 컬럼

- 제품명
- 총 생산량

힌트

```
SUM()

GROUP BY
```

---

# 실습 2. 작업자별 생산량 구하기

문제

작업자별 총 생산량을 출력하시오.

출력

- 작업자
- 총 생산량

추가

생산량이 많은 순으로 정렬한다.

힌트

```
GROUP BY

ORDER BY DESC
```

---

# 실습 3. 평균 생산량 이상인 제품 찾기

문제

평균 생산량이 150 이상인 제품을 출력하시오.

출력

- 제품명
- 평균 생산량

힌트

```
AVG()

HAVING
```

---

# 실습 4. 생산 횟수가 많은 제품

문제

생산된 횟수가 3회 이상인 제품을 출력하시오.

출력

- 제품명
- 생산횟수

힌트

```
COUNT(*)

HAVING
```

---

# 실습 5. 작업자별 최대 생산량

문제

작업자별 가장 많이 생산한 수량을 출력하시오.

출력

- 작업자
- 최대 생산량

힌트

```
MAX()
```

---

# 실습 6. 제품별 최소 생산량

문제

제품별 최소 생산량을 출력하시오.

출력

- 제품명
- 최소 생산량

힌트

```
MIN()
```

---

# 종합 실습

다음 조건을 만족하는 SQL을 작성하시오.

1. 제품별 총 생산량 계산
2. 총 생산량이 300 이상만 출력
3. 생산량이 많은 순으로 정렬

예상 키워드

```sql
GROUP BY

SUM()

HAVING

ORDER BY DESC
```

---

# 핵심 정리

|구문|설명|
|----|----|
|GROUP BY|같은 값을 그룹으로 묶는다.|
|COUNT()|개수를 계산한다.|
|SUM()|합계를 계산한다.|
|AVG()|평균을 계산한다.|
|MAX()|최대값을 계산한다.|
|MIN()|최소값을 계산한다.|
|HAVING|그룹에 조건을 지정한다.|
|WHERE|행(Row)에 조건을 지정한다.|
|ORDER BY|결과를 정렬한다.|

---

# 이번 장에서 반드시 기억할 내용

- GROUP BY는 **같은 값을 하나의 그룹으로 묶는 기능**이다.
- GROUP BY와 함께 **COUNT(), SUM(), AVG(), MAX(), MIN()**을 자주 사용한다.
- **WHERE는 그룹화 이전**, **HAVING은 그룹화 이후**에 조건을 적용한다.
- GROUP BY → HAVING → ORDER BY 순서를 익혀두면 실무에서도 가장 많이 사용하는 집계 SQL을 작성할 수 있다.
