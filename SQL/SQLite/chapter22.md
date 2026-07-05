# Chapter 21. 제약조건(Constraint)

## 학습 목표

이번 장에서는 SQLite에서 사용하는 다양한 **제약조건(Constraint)** 을 학습한다.

학습이 끝나면 다음 내용을 이해할 수 있다.

- PRIMARY KEY
- FOREIGN KEY
- UNIQUE
- NOT NULL
- CHECK
- DEFAULT
- 제약조건을 사용하는 이유
- 실무에서 사용하는 모델링 방법

---

# 21-1. 제약조건(Constraint)이란?

제약조건(Constraint)은 **잘못된 데이터가 저장되지 않도록 데이터베이스가 강제로 검사하는 규칙**이다.

예를 들어

- 같은 회원 ID가 두 번 저장되면 안 된다.
- 가격은 음수가 될 수 없다.
- 주문 수량은 1개 이상이어야 한다.
- 상품명은 반드시 입력되어야 한다.

이러한 규칙을 **DBMS가 직접 검사**하도록 만드는 것이 제약조건이다.

---

# 쇼핑몰 예제

이번 장에서는 다음과 같은 쇼핑몰을 만든다.

```
Customers
──────────────
customer_id
name
email

        │

        │

Orders
──────────────
order_id
customer_id
order_date

        │

        │

Order_Items
──────────────
item_id
order_id
product_name
quantity
price
```

---

# 실습 준비

SQLite에서는 외래키 기능이 기본적으로 꺼져 있다.

먼저 활성화한다.

```sql
PRAGMA foreign_keys = ON;
```

확인

```sql
PRAGMA foreign_keys;
```

결과

```
1
```

---

# 기존 테이블 삭제

```sql
DROP TABLE IF EXISTS Order_Items;
DROP TABLE IF EXISTS Orders;
DROP TABLE IF EXISTS Customers;
```

삭제 순서는 **자식 → 부모** 순서이다.

---

# Customers 테이블 생성

```sql
CREATE TABLE Customers
(
    customer_id INTEGER PRIMARY KEY,
    name        TEXT NOT NULL,
    email       TEXT UNIQUE
);
```

---

# Orders 테이블 생성

```sql
CREATE TABLE Orders
(
    order_id INTEGER PRIMARY KEY,
    customer_id INTEGER NOT NULL,
    order_date TEXT DEFAULT CURRENT_DATE,

    FOREIGN KEY(customer_id)
        REFERENCES Customers(customer_id)
);
```

---

# Order_Items 생성

```sql
CREATE TABLE Order_Items
(
    item_id INTEGER PRIMARY KEY,

    order_id INTEGER NOT NULL,

    product_name TEXT NOT NULL,

    quantity INTEGER
        CHECK(quantity > 0),

    price INTEGER
        CHECK(price >= 0),

    FOREIGN KEY(order_id)
        REFERENCES Orders(order_id)
);
```

---

# 스키마 확인

```sql
.schema
```

또는

```sql
.schema Customers
```

---

# 데이터 입력

Customers

```sql
INSERT INTO Customers VALUES
(1,'Kim','kim@test.com');

INSERT INTO Customers VALUES
(2,'Lee','lee@test.com');

INSERT INTO Customers VALUES
(3,'Park','park@test.com');
```

Orders

```sql
INSERT INTO Orders(order_id, customer_id)
VALUES(1,1);

INSERT INTO Orders(order_id, customer_id)
VALUES(2,2);

INSERT INTO Orders(order_id, customer_id)
VALUES(3,3);
```

Order_Items

```sql
INSERT INTO Order_Items
VALUES
(1,1,'Keyboard',2,50000);

INSERT INTO Order_Items
VALUES
(2,1,'Mouse',1,30000);

INSERT INTO Order_Items
VALUES
(3,2,'Monitor',1,250000);
```

---

# 전체 구조 확인

```sql
SELECT *
FROM Customers;

SELECT *
FROM Orders;

SELECT *
FROM Order_Items;
```

---

# 21-2 PRIMARY KEY

Primary Key는

**테이블에서 하나의 행(Row)을 유일하게 식별하는 값**이다.

특징

- 중복 불가
- NULL 불가
- 테이블당 하나만 존재

예

```sql
customer_id INTEGER PRIMARY KEY
```

---

## 실습

이미 존재하는 번호를 입력한다.

```sql
INSERT INTO Customers
VALUES(1,'Hong','hong@test.com');
```

결과

```
UNIQUE constraint failed
```

왜 실패했는가?

> Primary Key는 중복을 허용하지 않는다.

---

# 21-3 NOT NULL

NULL 값을 허용하지 않는다.

```sql
name TEXT NOT NULL
```

---

## 실습

```sql
INSERT INTO Customers
VALUES
(10,NULL,'aaa@test.com');
```

결과

```
NOT NULL constraint failed
```

---

# 21-4 UNIQUE

중복을 허용하지 않는다.

Primary Key와 차이점

|PRIMARY KEY|UNIQUE|
|------------|------|
|NULL 불가|NULL 가능|
|테이블당 하나|여러 개 가능|

---

## 실습

이미 존재하는 이메일 입력

```sql
INSERT INTO Customers
VALUES
(10,'Hong','kim@test.com');
```

결과

```
UNIQUE constraint failed
```

---

# 21-5 CHECK

조건을 검사한다.

예

```sql
CHECK(quantity > 0)
```

---

## 실습

```sql
INSERT INTO Order_Items
VALUES
(10,1,'SSD',0,100000);
```

결과

```
CHECK constraint failed
```

---

또는

```sql
INSERT INTO Order_Items
VALUES
(11,1,'SSD',-1,100000);
```

---

가격 검사

```sql
INSERT INTO Order_Items
VALUES
(12,1,'SSD',1,-100);
```

---

# 21-6 DEFAULT

값을 입력하지 않으면 기본값을 저장한다.

```sql
order_date TEXT DEFAULT CURRENT_DATE
```

---

## 실습

```sql
INSERT INTO Orders(order_id,customer_id)
VALUES(10,1);
```

조회

```sql
SELECT *
FROM Orders;
```

오늘 날짜가 자동 입력된다.

---

# 제약조건 요약

|제약조건|설명|
|---------|----|
|PRIMARY KEY|기본키|
|FOREIGN KEY|외래키|
|NOT NULL|NULL 금지|
|UNIQUE|중복 금지|
|CHECK|조건 검사|
|DEFAULT|기본값|

---

# 실습 문제

### 문제 1

상품 가격이 0원 이상만 저장되도록 Product 테이블을 만들어라.

---

### 문제 2

회원 이름은 반드시 입력하도록 수정하라.

---

### 문제 3

이메일이 중복되지 않도록 만들어라.

---

### 문제 4

주문 수량은 1개 이상만 저장되도록 만들어라.

---

### 문제 5

가입일을 현재 날짜로 자동 저장되도록 만들어라.

---

# Chapter 22. 외래키(Foreign Key)

# 학습 목표

이번 장에서는

- 외래키란?
- 참조 무결성
- 부모 테이블
- 자식 테이블
- 삭제 순서

를 학습한다.

---

# 22-1 외래키란?

외래키(Foreign Key)는

**다른 테이블의 Primary Key를 참조하는 컬럼**이다.

예를 들어

```
Customers

customer_id
```

↓

```
Orders

customer_id
```

Orders는 Customers를 참조한다.

---

# 왜 필요한가?

외래키가 없다면

존재하지 않는 회원이 주문할 수도 있다.

예

```
회원

1 Kim

2 Lee
```

그런데

```
주문

customer_id = 100
```

100번 회원은 존재하지 않는다.

이러한 잘못된 데이터를 막기 위해 외래키를 사용한다.

---

# 참조 무결성(Referential Integrity)

참조 무결성이란

**참조하는 데이터는 반드시 존재해야 한다.**

이다.

즉

주문이 있다면

회원도 반드시 있어야 한다.

---

# FOREIGN KEY 작성법

```sql
FOREIGN KEY(customer_id)
REFERENCES Customers(customer_id)
```

의미

```
Orders.customer_id

↓

Customers.customer_id
```

를 참조한다.

---

# 실습

존재하지 않는 회원

```sql
INSERT INTO Orders
VALUES
(100,999,'2026-07-01');
```

결과

```
FOREIGN KEY constraint failed
```

왜 실패했는가?

회원이 존재하지 않는다.

---

# 올바른 입력

```sql
INSERT INTO Orders
VALUES
(100,1,'2026-07-01');
```

성공한다.

---

# 삭제 순서

다음과 같은 구조라면

```
Customers

↓

Orders

↓

Order_Items
```

삭제는

```
① Order_Items

↓

② Orders

↓

③ Customers
```

순서대로 해야 한다.

---

# 삭제 실습

다음 SQL을 실행해보자.

```sql
DELETE FROM Customers
WHERE customer_id=1;
```

결과

```
FOREIGN KEY constraint failed
```

왜 실패했는가?

회원을 참조하는 주문이 존재하기 때문이다.

---

# 올바른 삭제

```sql
DELETE FROM Order_Items
WHERE order_id=1;

DELETE FROM Orders
WHERE order_id=1;

DELETE FROM Customers
WHERE customer_id=1;
```

성공한다.

---

# 실습 문제

### 문제 1

존재하지 않는 customer_id로 주문을 입력해보라.

---

### 문제 2

회원을 먼저 삭제해보고 오류를 확인하라.

---

### 문제 3

올바른 삭제 순서대로 삭제해보라.

---

### 문제 4

새로운 회원을 추가한 후

주문

주문상품

까지 입력해보라.

---

### 문제 5

현재 테이블 관계를 그림으로 그려보라.

```
Customers

↓

Orders

↓

Order_Items
```

학생들이 직접 손으로 그려보면
외래키 관계를 이해하는 데 큰 도움이 된다.

---

# 이번 장 핵심 정리

- Primary Key는 행을 식별한다.
- Foreign Key는 다른 테이블을 참조한다.
- UNIQUE는 중복을 막는다.
- NOT NULL은 필수 입력이다.
- CHECK는 조건을 검사한다.
- DEFAULT는 기본값을 저장한다.
- 외래키는 참조 무결성을 유지한다.
- 부모보다 자식을 먼저 삭제해야 한다.
