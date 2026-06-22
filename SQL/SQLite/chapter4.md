# Chapter 4. 테이블 생성

## 학습목표

* SQLite의 주요 데이터 타입을 이해할 수 있다.
* CREATE TABLE 문법을 사용할 수 있다.
* 생산량 관리 테이블을 생성할 수 있다.
* 생성된 테이블 구조를 확인할 수 있다.

---

# 4.1 테이블(Table)이란?

테이블은 데이터를 저장하는 가장 기본적인 구조이다.

엑셀의 시트와 비슷한 형태로 구성된다.

예를 들어 생산 데이터를 저장한다면 다음과 같이 표현할 수 있다.

| ID | 제품명    | 생산량 | 생산일        |
| -- | ------ | --- | ---------- |
| 1  | Motor  | 100 | 2026-06-22 |
| 2  | Sensor | 200 | 2026-06-23 |

* 행(Row) : 하나의 데이터
* 열(Column) : 데이터의 속성

---

# 4.2 SQLite 데이터 타입

SQLite는 비교적 단순한 데이터 타입을 사용한다.

| 타입      | 설명       | 예     |
| ------- | -------- | ----- |
| INTEGER | 정수       | 100   |
| REAL    | 실수       | 10.5  |
| TEXT    | 문자열      | Motor |
| BLOB    | 바이너리 데이터 | 이미지   |
| NULL    | 값 없음     | NULL  |

---

## 예제

```sql
CREATE TABLE test(
    id INTEGER,
    name TEXT,
    score REAL
);
```

---

# 4.3 PRIMARY KEY

기본키(Primary Key)는 각 데이터를 구분하는 고유 번호이다.

예를 들어 다음 데이터가 있다고 가정하자.

| ID | 제품명    |
| -- | ------ |
| 1  | Motor  |
| 2  | Sensor |
| 3  | Pump   |

ID 값은 중복될 수 없다.

SQLite에서는 다음과 같이 지정한다.

```sql
id INTEGER PRIMARY KEY
```

---

# 4.4 CREATE TABLE 문법

## 기본 문법

```sql
CREATE TABLE 테이블명(
    컬럼명 데이터타입,
    컬럼명 데이터타입
);
```

---

## 예제

```sql
CREATE TABLE product(
    id INTEGER PRIMARY KEY,
    name TEXT,
    price INTEGER
);
```

---

# 4.5 생산량 테이블 설계

제조 현장에서 자주 사용하는 생산량 관리 테이블을 만들어보자.

| 컬럼명          | 타입      | 설명   |
| ------------ | ------- | ---- |
| id           | INTEGER | 생산번호 |
| product_name | TEXT    | 제품명  |
| qty          | INTEGER | 생산수량 |
| prod_date    | TEXT    | 생산일  |

---

# 4.6 생산량 테이블 생성

## SQL 작성

```sql
CREATE TABLE production(
    id INTEGER PRIMARY KEY,
    product_name TEXT,
    qty INTEGER,
    prod_date TEXT
);
```

실행 결과

```text
sqlite>
```

오류 메시지가 없으면 성공적으로 생성된 것이다.

---

# 4.7 테이블 확인

현재 데이터베이스에 존재하는 테이블을 확인한다.

```sql
.tables
```

결과 예

```text
production
```

---

# 4.8 테이블 구조 확인

생성된 테이블의 컬럼 정보를 확인한다.

```sql
.schema production
```

결과 예

```sql
CREATE TABLE production(
    id INTEGER PRIMARY KEY,
    product_name TEXT,
    qty INTEGER,
    prod_date TEXT
);
```

---

# 실습 1. SQLite 실행

```bash
sqlite3 factory.db
```

---

# 실습 2. 현재 테이블 확인

```sql
.tables
```

아무것도 출력되지 않으면 아직 테이블이 없는 상태이다.

---

# 실습 3. 생산량 테이블 생성

다음 SQL을 입력하시오.

```sql
CREATE TABLE production(
    id INTEGER PRIMARY KEY,
    product_name TEXT,
    qty INTEGER,
    prod_date TEXT
);
```

---

# 실습 4. 생성 여부 확인

```sql
.tables
```

결과

```text
production
```

---

# 실습 5. 테이블 구조 확인

```sql
.schema production
```

결과

```sql
CREATE TABLE production(
    id INTEGER PRIMARY KEY,
    product_name TEXT,
    qty INTEGER,
    prod_date TEXT
);
```

---

# 실습 6. 오류 확인 실습

동일한 테이블을 다시 생성해보자.

```sql
CREATE TABLE production(
    id INTEGER PRIMARY KEY,
    product_name TEXT,
    qty INTEGER,
    prod_date TEXT
);
```

결과

```text
Error: table production already exists
```

이미 존재하는 테이블은 다시 생성할 수 없다.

---

# 실습 7. 테이블 삭제 후 재생성

테이블 삭제

```sql
DROP TABLE production;
```

확인

```sql
.tables
```

다시 생성

```sql
CREATE TABLE production(
    id INTEGER PRIMARY KEY,
    product_name TEXT,
    qty INTEGER,
    prod_date TEXT
);
```

---

# 핵심 정리

| 명령어          | 설명        |
| ------------ | --------- |
| CREATE TABLE | 테이블 생성    |
| PRIMARY KEY  | 기본키 지정    |
| .tables      | 테이블 목록 확인 |
| .schema      | 테이블 구조 확인 |
| DROP TABLE   | 테이블 삭제    |

---

# 확인 문제

### 문제 1

정수를 저장하는 SQLite 데이터 타입은?

### 문제 2

문자열을 저장하는 SQLite 데이터 타입은?

### 문제 3

테이블을 생성하는 SQL 명령은?

### 문제 4

현재 데이터베이스의 테이블 목록을 확인하는 명령은?

### 문제 5

production 테이블을 삭제하는 SQL 문을 작성하시오.

### 문제 6

PRIMARY KEY의 역할은 무엇인가?

### 문제 7

다음 구조의 employee 테이블을 생성하는 SQL을 작성하시오.

| 컬럼명    | 타입      |
| ------ | ------- |
| id     | INTEGER |
| name   | TEXT    |
| salary | INTEGER |
