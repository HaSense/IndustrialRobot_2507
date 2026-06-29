# Part 5. 스마트팩토리 데이터 모델링

# Chapter 13. 생산 이력 관리

---

# 학습목표

본 단원을 학습한 후 다음 내용을 수행할 수 있다.

* 생산 이력(Production History)의 개념을 이해할 수 있다.
* 생산 이력 테이블이 필요한 이유를 설명할 수 있다.
* 제품과 생산이력의 관계를 이해할 수 있다.
* 생산 이력 테이블을 설계할 수 있다.
* 생산 실적 데이터를 저장하고 조회할 수 있다.

---

# 1. 생산 이력이란?

생산 이력(Production History)은

```text
언제
어떤 제품을
몇 개 생산했는가
```

를 기록한 데이터이다.

---

예시

| 생산번호 | 제품     | 생산수량 | 생산일        |
| ---- | ------ | ---- | ---------- |
| 1    | Motor  | 100  | 2026-06-22 |
| 2    | Sensor | 50   | 2026-06-22 |
| 3    | Motor  | 120  | 2026-06-23 |

---

스마트팩토리에서는 생산 이력이 매우 중요하다.

이유

```text
생산량 분석
LOT 추적
품질 추적
생산성 분석
```

에 사용되기 때문이다.

---

# 2. 제품 테이블만으로는 부족하다

현재 제품 테이블

| product_id | name    | price   |
| ---------- | ------- | --------
| 1          | Motor   | 100000  |
| 2          | Sensor  | 50000   |
| 3          | Bearing | 10000   |

---

문제

```text
Motor를
언제 생산했는지 알 수 없다.
```

---

따라서 생산 이력이 필요하다.

---

# 3. 생산 이력 관리 도메인 지식

실제 MES에서는 생산이 끝날 때마다 생산 실적이 저장된다.

---

예시

```text
2026-06-22
Motor
100개 생산
```

↓

생산이력 저장

---

다음날

```text
2026-06-23
Motor
120개 생산
```

↓

새로운 생산이력 저장

---

즉

```text
제품 1개
생산이력 여러 개
```

관계가 된다.

---

# 4. 생산 이력 테이블 설계

요구사항

생산 이력은 다음 정보를 가진다.

| 항목         | 설명      |
| ---------- | ------- |
| history_id | 생산이력 번호 |
| product_id | 제품번호    |
| qty        | 생산수량    |
| prod_date  | 생산일     |

---

ERD 개념

```text
Products
----------------
product_id
name
price

        │

        ▼

Production_History
----------------
history_id
product_id
qty
prod_date
```

---

# 5. 생산 이력 테이블 생성

```sql
CREATE TABLE production_history(
    history_id INTEGER PRIMARY KEY,
    product_id INTEGER,
    qty INTEGER,
    prod_date TEXT
);
```

---

설명

```text
history_id  : 생산이력번호
product_id  : 제품번호
qty         : 생산수량
prod_date   : 생산일
```

---

# 6. 제품 데이터 준비

먼저 제품 정보를 입력한다.

```sql
INSERT INTO products
VALUES(1,'Motor',5000);
```

```sql
INSERT INTO products
VALUES(2,'Sensor',12000);
```

```sql
INSERT INTO products
VALUES(3,'Bearing',3000);
```

---

확인

```sql
SELECT * FROM products;
```

---

결과

```text
1|Motor|5000
2|Sensor|12000
3|Bearing|3000
```

---

# 7. 생산 이력 등록

Motor 생산

```sql
INSERT INTO production_history
VALUES(1,1,100,'2026-06-22');
```

---

Sensor 생산

```sql
INSERT INTO production_history
VALUES(2,2,50,'2026-06-22');
```

---

Motor 추가 생산

```sql
INSERT INTO production_history
VALUES(3,1,120,'2026-06-23');
```

---

조회

```sql
SELECT * FROM production_history;
```

---

결과

```text
1|1|100|2026-06-22
2|2|50|2026-06-22
3|1|120|2026-06-23
```
join구문
```sql
SELECT
    h.history_id,
    p.name,
    h.qty,
    h.prod_date
FROM Production_History h
INNER JOIN Products p
ON h.product_id = p.product_id;

```


---

# 8. 생산 이력 해석

```text
1|1|100|2026-06-22
```

의 의미

```text
제품번호 1번(Motor)

2026-06-22

100개 생산
```

---

# 9. 생산량 통계

전체 생산량

```sql
SELECT SUM(qty)
FROM production_history;
```

---

제품별 생산량

```sql
SELECT product_id,
       SUM(qty)
FROM production_history
GROUP BY product_id;
```

---

# 실습 1. 생산 이력 테이블 생성

다음 SQL을 실행하시오.

```sql
CREATE TABLE production_history(
    history_id INTEGER PRIMARY KEY,
    product_id INTEGER,
    qty INTEGER,
    prod_date TEXT
);
```

---

확인

```sql
.tables
```

---

# 실습 2. 생산 실적 등록

다음 데이터를 입력하시오.

```sql
INSERT INTO production_history
VALUES(1,1,100,'2026-06-22');
```

```sql
INSERT INTO production_history
VALUES(2,2,50,'2026-06-22');
```

```sql
INSERT INTO production_history
VALUES(3,1,120,'2026-06-23');
```

---

조회

```sql
SELECT * FROM production_history;
```

---

# 실습 3. 새로운 생산 실적 추가

다음 생산 정보를 추가하시오.

```text
제품번호 : 3
생산수량 : 80
생산일   : 2026-06-23
```

---

정답 예시

```sql
INSERT INTO production_history
VALUES(4,3,80,'2026-06-23');
```

---

# 실습 4. 총 생산량 계산

```sql
SELECT SUM(qty)
FROM production_history;
```

---

현재까지 생산된 총 생산량을 계산하시오.

---

# 실습 5. 평균 생산량 계산

```sql
SELECT AVG(qty)
FROM production_history;
```

---

평균 생산량을 계산하시오.

---

# 실습 6. 최대 생산량 찾기

```sql
SELECT MAX(qty)
FROM production_history;
```

---

가장 많이 생산된 생산실적을 확인하시오.

---

# 실습 7. 날짜별 생산 조회

```sql
SELECT *
FROM production_history
WHERE prod_date='2026-06-23';
```

---

2026-06-23 생산 데이터를 조회하시오.

---

# 실습 8. 제품별 생산량 계산

```sql
SELECT product_id,
       SUM(qty)
FROM production_history
GROUP BY product_id;
```

---

결과를 해석하시오.

---

# 실습 9. 생산량 순 정렬

```sql
SELECT *
FROM production_history
ORDER BY qty DESC;
```

---

생산량이 많은 순으로 정렬하시오.

---

# 도전 과제

다음 데이터를 추가하시오.

```sql
INSERT INTO production_history
VALUES(5,1,200,'2026-06-24');

INSERT INTO production_history
VALUES(6,2,150,'2026-06-24');

INSERT INTO production_history
VALUES(7,3,90,'2026-06-24');
```

---

문제 1

전체 생산량 조회

---

문제 2

제품별 총 생산량 조회

---

문제 3

가장 많이 생산된 실적 조회

---

문제 4

2026-06-24 생산량 합계 계산

---

# 제조AI 관점

MES에서 생산 이력은 핵심 데이터이다.

생산 이력을 통해

```text
생산성 분석
가동률 분석
LOT 추적
품질 추적
불량 분석
```

을 수행한다.

---

실제 MES 구조

```text
Products
      │
      ▼
Production_History
      │
      ▼
LOT_History
      │
      ▼
Quality
```

---

생산 이력은

```text
"무엇을 만들었는가?"
```

를 기록하는 가장 중요한 테이블이다.

---

# 핵심 정리

* 생산 이력은 생산 실적을 저장하는 데이터이다.
* 제품 하나는 여러 생산 이력을 가질 수 있다.
* production_history 테이블로 생산 실적을 관리한다.
* 생산량 통계 분석의 기반이 된다.
* MES의 핵심 데이터 중 하나이다.

---

# 다음 시간

Chapter 14. LOT 추적 관리

* LOT 개념
* 추적성(Traceability)
* LOT 번호 설계
* LOT 이력 테이블 설계
* 제조 품질 관리
