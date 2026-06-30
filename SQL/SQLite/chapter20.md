# Chapter 20. 테이블 변경(ALTER TABLE)

---

# 학습목표

본 단원을 학습한 후 다음 내용을 설명할 수 있다.

- ALTER TABLE의 목적을 이해할 수 있다.
- 테이블 이름을 변경할 수 있다.
- 컬럼을 추가할 수 있다.
- 컬럼 이름을 변경할 수 있다.
- 컬럼을 삭제할 수 있다.
- SQLite에서 ALTER TABLE의 제약사항을 이해할 수 있다.
- 기존 데이터를 유지하면서 테이블 구조를 변경할 수 있다.

---

# ALTER TABLE이란?

ALTER TABLE은 이미 생성되어 있는 테이블의 구조를 변경하는 SQL 명령이다.

프로그램을 개발하다 보면 새로운 기능이 추가되면서 기존 테이블에 컬럼을 추가하거나 이름을 변경해야 하는 경우가 자주 발생한다.

예를 들어 회원관리 프로그램을 개발하던 중 휴대폰 번호를 저장해야 하는 요구사항이 추가될 수 있다.

기존 테이블

```
Member
--------------------
id
name
email
```

기능 추가 후

```
Member
--------------------
id
name
email
phone
```

새로운 테이블을 만드는 것이 아니라 기존 테이블을 수정하는 것이 ALTER TABLE이다.

---

# SQLite에서 지원하는 ALTER TABLE

SQLite에서 지원하는 기능은 다음과 같다.

|기능|지원 여부|
|---|---|
|테이블 이름 변경|O|
|컬럼 추가|O|
|컬럼 이름 변경|O (SQLite 3.25 이상)|
|컬럼 삭제|O (SQLite 3.35 이상)|

다음 기능은 직접 지원하지 않는다.

- 컬럼 자료형 변경
- 컬럼 순서 변경
- PRIMARY KEY 변경
- UNIQUE 제약조건 변경

---

# 실습용 테이블 생성

```sql
CREATE TABLE member
(
    id INTEGER PRIMARY KEY,
    name TEXT,
    email TEXT
);
```

데이터 입력

```sql
INSERT INTO member VALUES
(1,'Kim','kim@test.com'),
(2,'Lee','lee@test.com'),
(3,'Park','park@test.com');
```

조회

```sql
SELECT *
FROM member;
```

결과

|id|name|email|
|---|---|---|
|1|Kim|kim@test.com|
|2|Lee|lee@test.com|
|3|Park|park@test.com|

---

# 테이블 이름 변경

형식

```sql
ALTER TABLE 기존테이블명
RENAME TO 새로운테이블명;
```

예제

```sql
ALTER TABLE member
RENAME TO customer;
```

확인

```sql
.tables
```

또는

```sql
SELECT *
FROM customer;
```

---

# 실습 1

다음을 수행하시오.

- customer를 client로 변경하시오.
- 전체 데이터를 조회하시오.

---

# 컬럼 추가

형식

```sql
ALTER TABLE 테이블명
ADD COLUMN 컬럼명 자료형;
```

예제

```sql
ALTER TABLE client
ADD COLUMN phone TEXT;
```

조회

```sql
SELECT *
FROM client;
```

결과

|id|name|email|phone|
|---|---|---|---|
|1|Kim|kim@test.com|NULL|
|2|Lee|lee@test.com|NULL|
|3|Park|park@test.com|NULL|

기존 데이터는 그대로 유지된다.

---

# 데이터 수정

```sql
UPDATE client
SET phone='010-1111-1111'
WHERE id=1;

UPDATE client
SET phone='010-2222-2222'
WHERE id=2;

UPDATE client
SET phone='010-3333-3333'
WHERE id=3;
```

조회

```sql
SELECT *
FROM client;
```

---

# 실습 2

address 컬럼을 추가한 후 자신의 주소를 입력하시오.

---

# 컬럼 이름 변경

형식

```sql
ALTER TABLE 테이블명
RENAME COLUMN 기존컬럼명
TO 새로운컬럼명;
```

예제

```sql
ALTER TABLE client
RENAME COLUMN name
TO customer_name;
```

조회

```sql
SELECT *
FROM client;
```

결과

|id|customer_name|email|phone|address|

---

# 실습 3

email 컬럼을 email_address로 변경하시오.

---

# 컬럼 삭제

SQLite 3.35 이상

형식

```sql
ALTER TABLE 테이블명
DROP COLUMN 컬럼명;
```

예제

```sql
ALTER TABLE client
DROP COLUMN address;
```

조회

```sql
SELECT *
FROM client;
```

---

# 실습 4

phone 컬럼을 삭제하시오.

---

# DEFAULT 값을 이용한 컬럼 추가

컬럼을 추가할 때 기본값을 지정할 수 있다.

```sql
ALTER TABLE client
ADD COLUMN grade TEXT DEFAULT 'Bronze';
```

조회

```sql
SELECT *
FROM client;
```

결과

|id|customer_name|email_address|grade|
|---|---|---|---|
|1|Kim|kim@test.com|Bronze|
|2|Lee|lee@test.com|Bronze|
|3|Park|park@test.com|Bronze|

---

# 컬럼은 항상 마지막에 추가된다.

기존

```
id
name
email
```

추가 후

```
id
name
email
phone
```

중간에 컬럼을 삽입할 수는 없다.

---

# SQLite ALTER TABLE의 한계

SQLite에서는 다음 작업을 직접 수행할 수 없다.

- 컬럼 타입 변경
- PRIMARY KEY 변경
- 컬럼 위치 변경
- UNIQUE 변경

이러한 경우에는 일반적으로 다음 순서로 작업한다.

1. 새로운 테이블 생성
2. 기존 데이터 복사
3. 기존 테이블 삭제
4. 새 테이블 이름 변경

---

# 실전 예제

도서관리 프로그램에서 출판사를 저장해야 하는 기능이 추가되었다.

기존 테이블

```sql
CREATE TABLE book
(
    id INTEGER PRIMARY KEY,
    title TEXT,
    author TEXT
);
```

컬럼 추가

```sql
ALTER TABLE book
ADD COLUMN publisher TEXT;
```

조회

```sql
SELECT *
FROM book;
```

---

# 종합 실습

다음 테이블을 생성하시오.

```sql
CREATE TABLE employee
(
    id INTEGER PRIMARY KEY,
    name TEXT,
    department TEXT
);
```

데이터

```sql
INSERT INTO employee VALUES
(1,'Kim','Sales'),
(2,'Lee','HR'),
(3,'Park','Development');
```

다음을 수행하시오.

1. employee를 staff로 변경한다.
2. phone 컬럼을 추가한다.
3. salary 컬럼을 추가한다.
4. department를 team으로 변경한다.
5. phone 컬럼을 삭제한다.
6. position 컬럼을 추가하고 기본값을 'Staff'로 설정한다.
7. 전체 데이터를 조회한다.

---

# 응용 실습 1

다음 테이블을 생성하시오.

```sql
CREATE TABLE product
(
    id INTEGER PRIMARY KEY,
    name TEXT,
    price INTEGER
);
```

다음을 수행하시오.

- stock 컬럼 추가
- name을 product_name으로 변경
- stock 삭제

---

# 응용 실습 2

다음 테이블을 생성하시오.

```sql
CREATE TABLE movie
(
    id INTEGER PRIMARY KEY,
    title TEXT,
    director TEXT
);
```

다음을 수행하시오.

- release_year 추가
- rating 추가
- director를 producer로 변경
- rating 삭제

---

# 응용 실습 3

다음 테이블을 생성하시오.

```sql
CREATE TABLE course
(
    id INTEGER PRIMARY KEY,
    subject TEXT,
    professor TEXT
);
```

다음을 수행하시오.

- classroom 추가
- credit 추가
- professor를 instructor로 변경
- classroom 삭제
- 전체 데이터를 조회한다.

---

# 핵심 정리

- ALTER TABLE은 기존 테이블의 구조를 변경하는 명령이다.
- RENAME TO는 테이블 이름을 변경한다.
- ADD COLUMN은 새로운 컬럼을 추가한다.
- RENAME COLUMN은 컬럼명을 변경한다.
- DROP COLUMN은 컬럼을 삭제한다.
- 새 컬럼은 항상 마지막에 추가된다.
- SQLite는 컬럼 타입 변경이나 컬럼 순서 변경은 직접 지원하지 않는다.
- 실제 프로젝트에서는 새로운 기능이 추가될 때 ALTER TABLE을 매우 자주 사용한다.
