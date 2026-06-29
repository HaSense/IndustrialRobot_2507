# Part 5. 스마트팩토리 데이터 모델링

# Chapter 14. 라면공장을 이용한 LOT 추적 관리

---

# 학습목표

본 단원을 학습한 후 다음 내용을 설명할 수 있다.

* LOT의 개념을 이해할 수 있다.
* 추적성(Traceability)의 의미를 설명할 수 있다.
* 제조업에서 LOT가 필요한 이유를 설명할 수 있다.
* 라면공장의 LOT를 SQLite로 관리할 수 있다.
* LOT 정보를 조회할 수 있다.

---

# 라면공장 이야기

우리 회사는 매운라면을 생산하는 공장이다.

하루에 여러 번 생산하지만,

생산한 날짜마다 LOT를 부여한다.

예를 들어

```text
2026년 6월 22일

매운라면

100봉 생산
```

↓

LOT 생성

```text
RAMEN_20260622
```

---

다음날

```text
2026년 6월 23일

매운라면

120봉 생산
```

↓

새로운 LOT

```text
RAMEN_20260623
```

---

결과

| LOT            | 제품   | 생산수량 |
| -------------- | ---- | ---- |
| RAMEN_20260622 | 매운라면 | 100  |
| RAMEN_20260623 | 매운라면 | 120  |

---

# 1. LOT란?

LOT란

```text
같은 조건에서 생산된 제품의 묶음
```

이다.

제품 하나를 의미하는 것이 아니라

생산 그룹을 의미한다.

---

# 2. 왜 LOT가 필요한가?

며칠 후 고객센터에서 전화가 왔다.

```text
매운라면에서

이물질 발견
```

어떤 제품을 회수해야 할까?

---

## LOT가 없다면

```text
모든 매운라면 회수
```

↓

엄청난 손실

---

## LOT가 있다면

```text
RAMEN_20260622

만 회수
```

↓

피해 최소화

---

# 3. 추적성(Traceability)

Traceability란

```text
제품의 생산 이력을 추적할 수 있는 능력
```

을 의미한다.

예를 들어

```text
이 제품은

언제 생산되었는가?
```

---

```text
얼마나 생산되었는가?
```

---

```text
어떤 LOT인가?
```

---

이 질문에 모두 답할 수 있어야 한다.

---

# SQLite 테이블 설계

이번 장에서는 가장 간단한 형태의 LOT 테이블을 만든다.

```sql
CREATE TABLE ramen_lot(
    lot_no TEXT,
    product_name TEXT,
    qty INTEGER,
    prod_date TEXT
);
```

---

# 테이블 구조

| 컬럼           | 설명     |
| ------------ | ------ |
| lot_no       | LOT 번호 |
| product_name | 제품명    |
| qty          | 생산수량   |
| prod_date    | 생산일    |

---

# 실습 1. 테이블 생성

```sql
CREATE TABLE ramen_lot(
    lot_no TEXT,
    product_name TEXT,
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

# 실습 2. 데이터 등록

```sql
INSERT INTO ramen_lot
VALUES(
'RAMEN_20260622',
'매운라면',
100,
'2026-06-22'
);
```

```sql
INSERT INTO ramen_lot
VALUES(
'RAMEN_20260623',
'매운라면',
120,
'2026-06-23'
);
```

```sql
INSERT INTO ramen_lot
VALUES(
'SEAFOOD_20260624',
'해물라면',
80,
'2026-06-24'
);
```

---

# 실습 3. 전체 조회

```sql
SELECT *
FROM ramen_lot;
```

---

예상 결과

```text
RAMEN_20260622|매운라면|100|2026-06-22
RAMEN_20260623|매운라면|120|2026-06-23
SEAFOOD_20260624|해물라면|80|2026-06-24
```

---

# 실습 4. 특정 LOT 조회

```sql
SELECT *
FROM ramen_lot
WHERE lot_no='RAMEN_20260622';
```

---

# 실습 5. 특정 날짜 생산 조회

```sql
SELECT *
FROM ramen_lot
WHERE prod_date='2026-06-22';
```

---

# 실습 6. 생산량이 100봉 이상인 LOT 조회

```sql
SELECT *
FROM ramen_lot
WHERE qty>=100;
```

---

# 실습 7. 전체 생산량 계산

```sql
SELECT SUM(qty)
FROM ramen_lot;
```

---

# 실습 8. 평균 생산량 계산

```sql
SELECT AVG(qty)
FROM ramen_lot;
```

---

# 실습 9. 가장 많이 생산한 LOT 찾기

```sql
SELECT *
FROM ramen_lot
ORDER BY qty DESC;
```

---

# 실습 10. LOT 개수 확인

```sql
SELECT COUNT(*)
FROM ramen_lot;
```

---

# 도전 과제

다음 데이터를 추가하시오.

```text
치즈라면
2026-06-24
150봉
```

LOT 번호는 직접 작성한다.

---

문제 1

전체 데이터를 조회하시오.

---

문제 2

2026-06-24 생산 데이터를 조회하시오.

---

문제 3

생산량이 100봉 이상인 데이터를 조회하시오.

---

문제 4

총 생산량을 계산하시오.

---

# 제조AI 관점

이번 장에서는

```text
제품

↓

LOT
```

만 관리하였다.

실제 MES에서는

```text
제품

↓

LOT

↓

원재료

↓

설비

↓

작업자

↓

품질검사

↓

출하
```

까지 연결된다.

다음 장에서는

제품 테이블과 LOT 테이블을 연결하기 위해

**JOIN**을 학습한다.

---

# 핵심 정리

* LOT는 같은 조건으로 생산된 제품 그룹이다.
* LOT는 생산 이력을 관리하기 위한 핵심 정보이다.
* 문제가 발생하면 LOT를 이용하여 회수 범위를 최소화할 수 있다.
* SQLite를 이용하여 LOT 정보를 저장하고 조회할 수 있다.
* 실제 MES에서는 LOT를 중심으로 모든 제조 데이터를 연결한다.

---

# 다음 시간

## Chapter 15. JOIN 기초

* JOIN이 필요한 이유
* 제품 테이블 생성
* 제품과 LOT 연결
* INNER JOIN 실습
