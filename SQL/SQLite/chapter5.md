# Chapter 5. 데이터 추가 (INSERT)

## 학습목표

* INSERT 문의 역할을 이해할 수 있다.
* 테이블에 데이터를 추가할 수 있다.
* 여러 개의 생산 데이터를 입력할 수 있다.
* 입력된 데이터를 확인할 수 있다.

---

# 5.1 데이터 추가란?

테이블을 만들었다면 이제 데이터를 저장해야 한다.

예를 들어 생산 현장에서 다음과 같은 생산 실적이 발생했다고 가정하자.

| 생산번호 | 제품명    | 생산수량 | 생산일        |
| ---- | ------ | ---- | ---------- |
| 1    | Motor  | 100  | 2026-06-22 |
| 2    | Sensor | 200  | 2026-06-22 |
| 3    | Pump   | 150  | 2026-06-23 |

이 데이터를 테이블에 저장하는 작업을 **INSERT** 라고 한다.

---

# 5.2 INSERT 기본 문법

## 문법

```sql
INSERT INTO 테이블명
VALUES(값1, 값2, 값3);
```

---

## 예제

```sql
INSERT INTO production
VALUES(1,'Motor',100,'2026-06-22');
```

실행 결과

```text
sqlite>
```

오류가 없으면 정상적으로 저장된 것이다.

---

# 5.3 컬럼명을 지정하여 입력

컬럼명을 직접 지정할 수도 있다.

## 문법

```sql
INSERT INTO 테이블명(
    컬럼명1,
    컬럼명2
)
VALUES(
    값1,
    값2
);
```

---

## 예제

```sql
INSERT INTO production(
    id,
    product_name,
    quantity,
    production_date
)
VALUES(
    2,
    'Sensor',
    200,
    '2026-06-22'
);
```

---

# 5.4 문자열 입력

문자열(TEXT)은 작은따옴표를 사용한다.

## 올바른 예

```sql
INSERT INTO production
VALUES(3,'Pump',150,'2026-06-23');
```

---

## 잘못된 예

```sql
INSERT INTO production
VALUES(3,Pump,150,'2026-06-23');
```

오류 발생

```text
no such column: Pump
```

---

# 5.5 숫자 입력

정수(INTEGER)는 따옴표 없이 입력한다.

```sql
100
200
300
```

예제

```sql
INSERT INTO production
VALUES(4,'Valve',300,'2026-06-23');
```

---

# 5.6 PRIMARY KEY 중복 오류

ID는 PRIMARY KEY 이므로 중복될 수 없다.

이미 존재하는 ID를 다시 입력하면 오류가 발생한다.

```sql
INSERT INTO production
VALUES(1,'Robot',500,'2026-06-25');
```

결과

```text
UNIQUE constraint failed
```

---

# 실습 1. production 테이블 확인

먼저 현재 테이블이 존재하는지 확인한다.

```sql
.tables
```

결과

```text
production
```

---

# 실습 2. 첫 번째 생산 데이터 입력

```sql
INSERT INTO production
VALUES(
    1,
    'Motor',
    100,
    '2026-06-22'
);
```

---

# 실습 3. 두 번째 생산 데이터 입력

```sql
INSERT INTO production
VALUES(
    2,
    'Sensor',
    200,
    '2026-06-22'
);
```

---

# 실습 4. 세 번째 생산 데이터 입력

```sql
INSERT INTO production
VALUES(
    3,
    'Pump',
    150,
    '2026-06-23'
);
```

---

# 실습 5. 네 번째 생산 데이터 입력

```sql
INSERT INTO production
VALUES(
    4,
    'Valve',
    300,
    '2026-06-23'
);
```

---

# 실습 6. 다섯 번째 생산 데이터 입력

```sql
INSERT INTO production
VALUES(
    5,
    'Bearing',
    250,
    '2026-06-24'
);
```

---

# 실습 7. 입력 결과 확인

현재까지 입력한 데이터를 확인한다.

```sql
SELECT * FROM production;
```

결과 예

```text
1|Motor|100|2026-06-22
2|Sensor|200|2026-06-22
3|Pump|150|2026-06-23
4|Valve|300|2026-06-23
5|Bearing|250|2026-06-24
```

---

# 실습 8. 표 형태로 보기

```sql
.mode table
```

조회

```sql
SELECT * FROM production;
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

# 실습 9. 중복 키 오류 확인

다음 명령을 실행해보자.

```sql
INSERT INTO production
VALUES(
    1,
    'Robot',
    500,
    '2026-06-25'
);
```

결과

```text
UNIQUE constraint failed
```

ID가 중복되어 저장되지 않는다.

---

# 제조AI 응용 실습

다음 생산 데이터를 직접 입력하시오.

| ID | 제품명     | 수량  | 생산일 |
| -- | ------- | --- | --- |
| 6  | Battery | 120 |     |
| 7  | Module  | 180 |     |
| 8  | Frame   | 220 |     |

생산일은 오늘 날짜를 사용한다.

입력 후 조회하시오.

```sql
SELECT * FROM production;
```

---

# 핵심 정리

| 명령어           | 설명       |
| ------------- | -------- |
| INSERT INTO   | 데이터 추가   |
| VALUES        | 저장할 값 지정 |
| SELECT * FROM | 전체 조회    |
| .mode table   | 표 형태 출력  |

---

# 확인 문제

### 문제 1

데이터를 추가하는 SQL 명령어는?

### 문제 2

문자열 데이터는 어떤 기호로 감싸야 하는가?

### 문제 3

PRIMARY KEY가 중복되면 어떻게 되는가?

### 문제 4

production 테이블에 다음 데이터를 추가하는 SQL을 작성하시오.

| id | product_name | qty | prod_date  |
| -- | ------------ | --- | ---------- |
| 10 | Robot        | 500 | 2026-06-25 |

### 문제 5

production 테이블의 모든 데이터를 조회하는 SQL을 작성하시오.
