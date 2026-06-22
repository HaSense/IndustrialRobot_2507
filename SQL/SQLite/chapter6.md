# Chapter 6. 데이터 조회 (SELECT)

## 학습목표

* SELECT 문의 역할을 이해할 수 있다.
* 테이블의 모든 데이터를 조회할 수 있다.
* 특정 컬럼만 조회할 수 있다.
* 생산량 데이터를 조회하고 분석할 수 있다.
* 조회 결과를 표 형태로 출력할 수 있다.

---

# 6.1 데이터 조회란?

데이터베이스에 저장된 정보를 확인하는 작업을 **조회(Query)** 라고 한다.

SQLite에서는 `SELECT` 명령을 사용한다.

예를 들어 생산량 테이블에 다음과 같은 데이터가 저장되어 있다고 가정하자.

| ID | 제품명     | 생산량 | 생산일        |
| -- | ------- | --- | ---------- |
| 1  | Motor   | 100 | 2026-06-22 |
| 2  | Sensor  | 200 | 2026-06-22 |
| 3  | Pump    | 150 | 2026-06-23 |
| 4  | Valve   | 300 | 2026-06-23 |
| 5  | Bearing | 250 | 2026-06-24 |

---

# 6.2 SELECT 기본 문법

## 문법

```sql
SELECT 컬럼명
FROM 테이블명;
```

---

## 예제

```sql
SELECT product_name
FROM production;
```

결과

```text
Motor
Sensor
Pump
Valve
Bearing
```

---

# 6.3 전체 데이터 조회

모든 컬럼을 조회하려면 `*` 를 사용한다.

## 문법

```sql
SELECT *
FROM 테이블명;
```

---

## 예제

```sql
SELECT *
FROM production;
```

결과

```text
1|Motor|100|2026-06-22
2|Sensor|200|2026-06-22
3|Pump|150|2026-06-23
4|Valve|300|2026-06-23
5|Bearing|250|2026-06-24
```

---

# 6.4 특정 컬럼 조회

필요한 컬럼만 조회할 수 있다.

## 문법

```sql
SELECT 컬럼1, 컬럼2
FROM 테이블명;
```

---

## 예제

```sql
SELECT product_name, qty
FROM production;
```

결과

```text
Motor|100
Sensor|200
Pump|150
Valve|300
Bearing|250
```

---

# 6.5 여러 컬럼 조회

여러 개의 컬럼을 동시에 조회할 수 있다.

```sql
SELECT id, product_name, qty
FROM production;
```

---

# 6.6 보기 좋은 표 형태 출력

SQLite는 표 형태 출력을 지원한다.

## 설정

```sql
.mode table
```

---

## 조회

```sql
SELECT *
FROM production;
```

결과

```text
+----+--------------+-----+------------+
| id | product_name | qty | prod_date  |
+----+--------------+-----+------------+
| 1  | Motor        | 100 | 2026-06-22 |
| 2  | Sensor       | 200 | 2026-06-22 |
| 3  | Pump         | 150 | 2026-06-23 |
| 4  | Valve        | 300 | 2026-06-23 |
| 5  | Bearing      | 250 | 2026-06-24 |
+----+--------------+-----+------------+
```

---

# 6.7 컬럼 이름 표시

컬럼 이름을 항상 출력하도록 설정할 수 있다.

```sql
.headers on
```

조회

```sql
SELECT *
FROM production;
```

---

# 실습 1. 현재 데이터 확인

먼저 표 형태로 출력한다.

```sql
.mode table
.headers on
```

---

# 실습 2. 전체 데이터 조회

```sql
SELECT *
FROM production;
```

결과 예

```text
+----+--------------+-----+------------+
| id | product_name | qty | prod_date  |
+----+--------------+-----+------------+
| 1  | Motor        | 100 | 2026-06-22 |
| 2  | Sensor       | 200 | 2026-06-22 |
| 3  | Pump         | 150 | 2026-06-23 |
| 4  | Valve        | 300 | 2026-06-23 |
| 5  | Bearing      | 250 | 2026-06-24 |
+----+--------------+-----+------------+
```

---

# 실습 3. 제품명만 조회

```sql
SELECT product_name
FROM production;
```

---

# 실습 4. 생산량만 조회

```sql
SELECT qty
FROM production;
```

---

# 실습 5. 제품명과 생산량 조회

```sql
SELECT product_name, qty
FROM production;
```

결과 예

```text
+--------------+-----+
| product_name | qty |
+--------------+-----+
| Motor        | 100 |
| Sensor       | 200 |
| Pump         | 150 |
| Valve        | 300 |
| Bearing      | 250 |
+--------------+-----+
```

---

# 실습 6. 생산일과 제품명 조회

```sql
SELECT prod_date, product_name
FROM production;
```

---

# 실습 7. ID와 제품명 조회

```sql
SELECT id, product_name
FROM production;
```

---

# 실습 8. 제조AI 데이터 분석 실습

생산관리자가 다음 정보를 보고 싶다고 가정하자.

"제품명과 생산수량만 보고 싶다."

SQL을 작성하시오.

정답

```sql
SELECT product_name, qty
FROM production;
```

---

# 실습 9. 품질관리 실습

품질 담당자가 다음 정보를 요청하였다.

"생산번호와 생산일만 보고 싶다."

SQL을 작성하시오.

정답

```sql
SELECT id, prod_date
FROM production;
```

---

# 실습 10. 직접 입력 실습

다음 요구사항에 맞는 SQL을 작성하시오.

### 문제 1

제품명만 조회

### 문제 2

생산일만 조회

### 문제 3

제품명과 생산일 조회

### 문제 4

ID와 생산량 조회

### 문제 5

전체 데이터 조회

---

# 실습 11. 추가 데이터 입력 후 조회

다음 데이터를 추가하시오.

```sql
INSERT INTO production
VALUES(6,'Battery',120,'2026-06-25');

INSERT INTO production
VALUES(7,'Module',180,'2026-06-25');

INSERT INTO production
VALUES(8,'Frame',220,'2026-06-25');
```

전체 조회

```sql
SELECT *
FROM production;
```

---

# 실습 12. 조회 결과 해석하기

다음 질문에 답하시오.

### 질문 1

현재 저장된 제품은 몇 개인가?

### 질문 2

가장 마지막에 입력한 제품명은 무엇인가?

### 질문 3

생산일 컬럼의 이름은 무엇인가?

### 질문 4

생산량 컬럼의 이름은 무엇인가?

---

# 핵심 정리

| 명령어         | 설명       |
| ----------- | -------- |
| SELECT      | 데이터 조회   |
| SELECT *    | 전체 컬럼 조회 |
| SELECT 컬럼명  | 특정 컬럼 조회 |
| .mode table | 표 형태 출력  |
| .headers on | 컬럼명 표시   |

---

# 확인 문제

### 문제 1

전체 데이터를 조회하는 SQL은?

### 문제 2

제품명만 조회하는 SQL은?

### 문제 3

제품명과 생산량을 조회하는 SQL은?

### 문제 4

SELECT * 의 의미는 무엇인가?

### 문제 5

다음 결과를 출력하는 SQL을 작성하시오.

```text
Motor|100
Sensor|200
Pump|150
```

힌트

```sql
SELECT ?, ?
FROM production;
```
