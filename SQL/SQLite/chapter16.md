# Part 6. JOIN

# Chapter 16. 생산현황 조회

---

# 학습목표

본 단원을 학습한 후 다음 내용을 수행할 수 있다.

* 생산현황 조회의 목적을 설명할 수 있다.
* JOIN과 GROUP BY를 함께 사용할 수 있다.
* 제품별 생산량을 집계할 수 있다.
* 생산 통계 조회문을 작성할 수 있다.
* MES에서 사용하는 생산현황 화면의 원리를 이해할 수 있다.

---

# 이번 장의 의미

지금까지 학습한 내용

```text id="p1"
CREATE TABLE
INSERT
SELECT
WHERE
ORDER BY
SUM
AVG
MAX
MIN
JOIN
```

---

이번 장에서는

```text id="p2"
실무 조회문
```

을 작성한다.

---

실제 MES 화면

```text id="p3"
제품별 생산량
일별 생산량
LOT별 생산량
월별 생산량
```

을 모두 SQL로 계산한다.

---

# 1. 생산현황이란?

생산현황

```text id="p4"
현재까지
얼마나 생산되었는가?
```

를 보여주는 정보이다.

---

예시

| 제품      | 생산량 |
| ------- | --- |
| Motor   | 220 |
| Sensor  | 50  |
| Bearing | 80  |

---

관리자는 이를 보고

```text id="p5"
생산 계획
설비 운영
납기 관리
```

를 수행한다.

---

# 2. 생산현황 조회를 위한 데이터

products

| product_id | name    |
| ---------- | ------- |
| 1          | Motor   |
| 2          | Sensor  |
| 3          | Bearing |

---

production_history

| history_id | product_id | qty |
| ---------- | ---------- | --- |
| 1          | 1          | 100 |
| 2          | 2          | 50  |
| 3          | 1          | 120 |
| 4          | 3          | 80  |

---

# 문제

현재 총 생산량

```text id="p6"
Motor = ?
```

---

데이터가 여러 건 존재한다.

```text id="p7"
100
120
```

---

합계를 구해야 한다.

---

# 3. GROUP BY

GROUP BY

```text id="p8"
같은 값을 그룹으로 묶는다.
```

---

예제

```sql id="p9"
SELECT
    product_id,
    SUM(qty)
FROM production_history
GROUP BY product_id;
```

---

결과

```text id="p10"
1|220
2|50
3|80
```

---

문제

```text id="p11"
1
2
3
```

이 무엇인지 모른다.

---

JOIN 필요

---

# 4. JOIN + GROUP BY

실무 조회문

```sql id="p12"
SELECT
    p.name,
    SUM(h.qty)
FROM products p
JOIN production_history h
ON p.product_id = h.product_id
GROUP BY p.name;
```

---

결과

```text id="p13"
Motor|220
Sensor|50
Bearing|80
```

---

이것이 가장 기본적인 MES 생산현황 화면이다.

---

# 5. 생산량 내림차순

```sql id="p14"
SELECT
    p.name,
    SUM(h.qty)
FROM products p
JOIN production_history h
ON p.product_id = h.product_id
GROUP BY p.name
ORDER BY SUM(h.qty) DESC;
```

---

결과

```text id="p15"
Motor|220
Bearing|80
Sensor|50
```

---

# 6. 평균 생산량 조회

```sql id="p16"
SELECT
    p.name,
    AVG(h.qty)
FROM products p
JOIN production_history h
ON p.product_id = h.product_id
GROUP BY p.name;
```

---

결과

```text id="p17"
Motor|110
Sensor|50
Bearing|80
```

---

# 7. 최대 생산량 조회

```sql id="p18"
SELECT
    p.name,
    MAX(h.qty)
FROM products p
JOIN production_history h
ON p.product_id = h.product_id
GROUP BY p.name;
```

---

결과

```text id="p19"
Motor|120
Sensor|50
Bearing|80
```

---

# 8. 생산횟수 조회

COUNT 사용

```sql id="p20"
SELECT
    p.name,
    COUNT(*)
FROM products p
JOIN production_history h
ON p.product_id = h.product_id
GROUP BY p.name;
```

---

결과

```text id="p21"
Motor|2
Sensor|1
Bearing|1
```

---

의미

```text id="p22"
Motor 생산 기록 2건
```

---

# 실습 준비

products

```sql id="p23"
INSERT INTO products VALUES(1,'Motor',5000);
INSERT INTO products VALUES(2,'Sensor',12000);
INSERT INTO products VALUES(3,'Bearing',3000);
```

---

production_history

```sql id="p24"
INSERT INTO production_history VALUES(1,1,100,'2026-06-22');
INSERT INTO production_history VALUES(2,2,50,'2026-06-22');
INSERT INTO production_history VALUES(3,1,120,'2026-06-23');
INSERT INTO production_history VALUES(4,3,80,'2026-06-23');
```

---

# 실습 1. 제품별 총 생산량 조회

```sql id="p25"
SELECT
    p.name,
    SUM(h.qty)
FROM products p
JOIN production_history h
ON p.product_id = h.product_id
GROUP BY p.name;
```

---

결과 해석

```text id="p26"
Motor 총 생산량
Sensor 총 생산량
Bearing 총 생산량
```

---

# 실습 2. 생산량 순 정렬

```sql id="p27"
SELECT
    p.name,
    SUM(h.qty)
FROM products p
JOIN production_history h
ON p.product_id = h.product_id
GROUP BY p.name
ORDER BY SUM(h.qty) DESC;
```

---

가장 많이 생산된 제품 확인

---

# 실습 3. 평균 생산량 조회

```sql id="p28"
SELECT
    p.name,
    AVG(h.qty)
FROM products p
JOIN production_history h
ON p.product_id = h.product_id
GROUP BY p.name;
```

---

# 실습 4. 생산횟수 조회

```sql id="p29"
SELECT
    p.name,
    COUNT(*)
FROM products p
JOIN production_history h
ON p.product_id = h.product_id
GROUP BY p.name;
```

---

생산 횟수를 확인하시오.

---

# 실습 5. 최대 생산량 조회

```sql id="p30"
SELECT
    p.name,
    MAX(h.qty)
FROM products p
JOIN production_history h
ON p.product_id = h.product_id
GROUP BY p.name;
```

---

가장 많이 생산한 기록 확인

---

# 실습 6. 특정 제품 생산현황

Motor 조회

```sql id="p31"
SELECT
    p.name,
    SUM(h.qty)
FROM products p
JOIN production_history h
ON p.product_id = h.product_id
WHERE p.name='Motor'
GROUP BY p.name;
```

---

Motor 총 생산량 확인

---

# 실습 7. 날짜별 생산량

```sql id="p32"
SELECT
    prod_date,
    SUM(qty)
FROM production_history
GROUP BY prod_date;
```

---

결과 분석

```text id="p33"
2026-06-22
2026-06-23
```

별 생산량

---

# 실습 8. MES 생산현황 화면 만들기

조회문 작성

```sql id="p34"
SELECT
    p.name,
    SUM(h.qty) AS total_qty,
    AVG(h.qty) AS avg_qty,
    MAX(h.qty) AS max_qty
FROM products p
JOIN production_history h
ON p.product_id = h.product_id
GROUP BY p.name;
```

---

결과 예

```text id="p35"
Motor   220 110 120
Sensor   50  50  50
Bearing  80  80  80
```

---

실제 MES 화면과 유사하다.

---

# 도전 과제

다음 데이터를 추가하시오.

```sql id="p36"
INSERT INTO production_history
VALUES(5,1,200,'2026-06-24');

INSERT INTO production_history
VALUES(6,2,150,'2026-06-24');

INSERT INTO production_history
VALUES(7,3,90,'2026-06-24');
```

---

문제 1

제품별 총 생산량

---

문제 2

제품별 평균 생산량

---

문제 3

제품별 최대 생산량

---

문제 4

생산량 순 정렬

---

문제 5

가장 많이 생산된 제품 찾기

---

# 제조AI 관점

실제 MES 대시보드

```text id="p37"
오늘 생산량
제품별 생산량
라인별 생산량
LOT별 생산량
월별 생산량
```

은 대부분

```text id="p38"
JOIN
GROUP BY
SUM
AVG
COUNT
```

조합으로 만들어진다.

---

실무 조회 예

```text id="p39"
Motor

총 생산량 : 15,000 EA

생산 횟수 : 120회

평균 생산량 : 125 EA
```

---

# MES 개발자의 관점

실제 개발 업무의 상당 부분은

```text id="p40"
데이터 입력
```

보다

```text id="p41"
데이터 조회
```

에 사용된다.

---

따라서

```text id="p42"
JOIN
GROUP BY
SUM
```

은 반드시 익숙해져야 한다.

---

# 핵심 정리

* 생산현황 조회는 MES의 핵심 기능이다.
* GROUP BY는 같은 값을 묶는다.
* JOIN과 GROUP BY를 함께 사용하면 제품별 통계가 가능하다.
* SUM, AVG, MAX, COUNT를 함께 사용할 수 있다.
* 실제 MES 화면은 대부분 집계 SQL로 구성된다.

---

# 다음 시간

Chapter 17. SQLite API

* sqlite3_open()
* sqlite3_exec()
* sqlite3_close()
* C 언어와 SQLite 연동 시작
