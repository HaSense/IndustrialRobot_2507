# Chapter 23. ON DELETE 옵션과 참조 무결성

# 학습 목표

이번 장에서는 외래키(Foreign Key)의 삭제 동작을 제어하는 **ON DELETE 옵션**을 학습한다.

학습이 끝나면 다음 내용을 이해할 수 있다.

- 기본 삭제 동작(NO ACTION)
- 삭제 실패 원인
- 올바른 삭제 순서
- ON DELETE CASCADE
- ON DELETE SET NULL
- ON DELETE RESTRICT
- 언제 어떤 옵션을 사용하는가

---

# 이번 장에서 사용할 쇼핑몰 모델

```
Customers
──────────────
customer_id
name
email

        │

        ▼

Orders
──────────────
order_id
customer_id
order_date

        │

        ▼

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

SQLite는 외래키 기능을 활성화해야 한다.

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

---

# 테이블 생성 (기본 삭제 방식)

SQLite에서 ON DELETE를 지정하지 않으면

기본적으로 **NO ACTION(RESTRICT와 유사한 동작)** 이 적용된다.

Customers

```sql
CREATE TABLE Customers
(
    customer_id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    email TEXT UNIQUE
);
```

Orders

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

Order_Items

```sql
CREATE TABLE Order_Items
(
    item_id INTEGER PRIMARY KEY,
    order_id INTEGER NOT NULL,
    product_name TEXT,
    quantity INTEGER,
    price INTEGER,

    FOREIGN KEY(order_id)
        REFERENCES Orders(order_id)
);
```

---

# 데이터 입력

```sql
INSERT INTO Customers VALUES
(1,'Kim','kim@test.com'),
(2,'Lee','lee@test.com'),
(3,'Park','park@test.com');
```

```sql
INSERT INTO Orders VALUES
(1,1,'2026-07-01'),
(2,1,'2026-07-02'),
(3,2,'2026-07-03');
```

```sql
INSERT INTO Order_Items VALUES
(1,1,'Keyboard',2,50000),
(2,1,'Mouse',1,30000),
(3,2,'Monitor',1,250000),
(4,3,'USB',5,10000);
```

---

# 현재 데이터

Customers

|customer_id|name|
|-----------|------|
|1|Kim|
|2|Lee|
|3|Park|

Orders

|order_id|customer_id|
|---------|-----------|
|1|1|
|2|1|
|3|2|

Order_Items

|item_id|order_id|
|--------|---------|
|1|1|
|2|1|
|3|2|
|4|3|

---

# 23-1 기본 삭제

회원 Kim을 삭제해보자.

```sql
DELETE FROM Customers
WHERE customer_id=1;
```

결과

```
FOREIGN KEY constraint failed
```

---

# 왜 삭제되지 않는가?

```
Customers

1 Kim

↓

Orders

1
2
```

Kim 회원을 참조하는 주문이 존재하기 때문이다.

외래키는

**부모가 먼저 삭제되는 것을 허용하지 않는다.**

---

# 23-2 삭제 실패

실패 원인

```
Customer

↓

Orders
```

부모를 삭제하려고 했기 때문이다.

데이터베이스는

참조 무결성을 유지하기 위해 삭제를 막는다.

---

# 23-3 삭제 순서

올바른 삭제 순서는

```
Order_Items

↓

Orders

↓

Customers
```

이다.

실습

```sql
DELETE FROM Order_Items
WHERE order_id=1;

DELETE FROM Order_Items
WHERE order_id=2;
```

다음

```sql
DELETE FROM Orders
WHERE customer_id=1;
```

마지막

```sql
DELETE FROM Customers
WHERE customer_id=1;
```

삭제 성공

---

# 삭제 순서를 그림으로 이해하기

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

항상 자식부터 삭제한다.

---

# 23-4 ON DELETE CASCADE

이번에는 자동 삭제 기능을 사용한다.

기존 테이블 삭제

```sql
DROP TABLE IF EXISTS Order_Items;
DROP TABLE IF EXISTS Orders;
DROP TABLE IF EXISTS Customers;
```

---

# CASCADE 테이블 생성

Customers

```sql
CREATE TABLE Customers
(
    customer_id INTEGER PRIMARY KEY,
    name TEXT
);
```

Orders

```sql
CREATE TABLE Orders
(
    order_id INTEGER PRIMARY KEY,
    customer_id INTEGER,

    FOREIGN KEY(customer_id)
    REFERENCES Customers(customer_id)
    ON DELETE CASCADE
);
```

Order_Items

```sql
CREATE TABLE Order_Items
(
    item_id INTEGER PRIMARY KEY,
    order_id INTEGER,
    product_name TEXT,

    FOREIGN KEY(order_id)
    REFERENCES Orders(order_id)
    ON DELETE CASCADE
);
```

---

# 데이터 입력

```sql
INSERT INTO Customers VALUES
(1,'Kim'),
(2,'Lee');
```

```sql
INSERT INTO Orders VALUES
(1,1),
(2,1),
(3,2);
```

```sql
INSERT INTO Order_Items VALUES
(1,1,'Keyboard'),
(2,2,'Mouse'),
(3,3,'Monitor');
```

---

# CASCADE 실습

회원 삭제

```sql
DELETE FROM Customers
WHERE customer_id=1;
```

조회

```sql
SELECT * FROM Customers;

SELECT * FROM Orders;

SELECT * FROM Order_Items;
```

---

# 결과

Kim 삭제

↓

Orders 삭제

↓

Order_Items 삭제

자동으로 함께 삭제된다.

```
Customers

Kim 삭제

↓

Orders

삭제

↓

Order_Items

삭제
```

---

# CASCADE 장점

- 삭제가 편하다.
- 프로그램 코드가 단순하다.
- 부모만 삭제하면 된다.

---

# CASCADE 단점

잘못 삭제하면

관련 데이터가 모두 삭제된다.

예

```
회원 삭제

↓

주문 삭제

↓

주문상품 삭제
```

실수로 삭제하면 복구하기 어렵다.

---

# 23-5 ON DELETE SET NULL

이번에는

회원이 삭제되더라도

주문은 남겨두고 싶다고 가정하자.

예

퇴사한 영업사원

탈퇴한 회원

담당자는 없어도 기록은 남겨야 한다.

---

테이블 생성

```sql
CREATE TABLE Customers
(
    customer_id INTEGER PRIMARY KEY,
    name TEXT
);
```

```sql
CREATE TABLE Orders
(
    order_id INTEGER PRIMARY KEY,

    customer_id INTEGER,

    FOREIGN KEY(customer_id)
    REFERENCES Customers(customer_id)
    ON DELETE SET NULL
);
```

주의

SET NULL은

컬럼이 NULL을 허용해야 한다.

---

실습

```sql
DELETE FROM Customers
WHERE customer_id=1;
```

조회

```sql
SELECT *
FROM Orders;
```

결과

|order_id|customer_id|
|---------|-----------|
|1|NULL|
|2|NULL|
|3|2|

---

# 언제 사용하는가?

대표 사례

- 탈퇴 회원
- 퇴사 직원
- 삭제된 공급업체

기록은 유지하고

참조만 제거한다.

---

# 23-6 ON DELETE RESTRICT

삭제 자체를 막는다.

```sql
FOREIGN KEY(customer_id)
REFERENCES Customers(customer_id)
ON DELETE RESTRICT
```

실습

```sql
DELETE FROM Customers
WHERE customer_id=2;
```

결과

```
FOREIGN KEY constraint failed
```

삭제 불가

---

# RESTRICT는 언제 사용하는가?

은행

```
고객

↓

계좌
```

계좌가 존재하는데

고객을 삭제하면 안 된다.

병원

```
환자

↓

진료기록
```

환자가 삭제되면 안 된다.

---

# 쇼핑몰에서 어떤 옵션을 사용할까?

|관계|추천|
|------|------|
|회원 → 주문|RESTRICT|
|주문 → 주문상품|CASCADE|
|담당직원 → 주문|SET NULL|

---

# 스마트팩토리 실습

이번에는 새로운 제조 데이터를 사용한다.

```
Recipe
──────────────
recipe_id
recipe_name

        │

        ▼

Batch_Run
──────────────
batch_id
recipe_id
operator

        │

        ▼

Sensor_Log
──────────────
log_id
batch_id
temperature
record_time
```

---

# 테이블 생성

```sql
DROP TABLE IF EXISTS Sensor_Log;
DROP TABLE IF EXISTS Batch_Run;
DROP TABLE IF EXISTS Recipe;
```

Recipe

```sql
CREATE TABLE Recipe
(
    recipe_id INTEGER PRIMARY KEY,
    recipe_name TEXT
);
```

Batch_Run

```sql
CREATE TABLE Batch_Run
(
    batch_id INTEGER PRIMARY KEY,

    recipe_id INTEGER,

    operator TEXT,

    FOREIGN KEY(recipe_id)
    REFERENCES Recipe(recipe_id)
    ON DELETE RESTRICT
);
```

Sensor_Log

```sql
CREATE TABLE Sensor_Log
(
    log_id INTEGER PRIMARY KEY,

    batch_id INTEGER,

    temperature REAL,

    record_time TEXT,

    FOREIGN KEY(batch_id)
    REFERENCES Batch_Run(batch_id)
    ON DELETE CASCADE
);
```

---

# 데이터 입력

```sql
INSERT INTO Recipe VALUES
(1,'Battery Mix'),
(2,'Coating');
```

```sql
INSERT INTO Batch_Run VALUES
(100,1,'Kim'),
(101,2,'Lee');
```

```sql
INSERT INTO Sensor_Log VALUES
(1,100,82.5,'2026-07-06 09:00'),
(2,100,83.2,'2026-07-06 09:05'),
(3,101,71.8,'2026-07-06 10:00');
```

---

# 실습 1

Batch 삭제

```sql
DELETE FROM Batch_Run
WHERE batch_id=100;
```

Sensor_Log도 함께 삭제되는지 확인하라.

---

# 실습 2

Recipe 삭제

```sql
DELETE FROM Recipe
WHERE recipe_id=1;
```

왜 실패하는지 확인하라.

---

# 실습 3

Sensor_Log를 먼저 삭제한 후

Batch_Run을 삭제해보자.

---

# 실습 4

Recipe를 삭제할 수 있는 순서를 작성하라.

---

# 실습 5

현재 관계를 그림으로 그려보자.

```
Recipe

↓

Batch_Run

↓

Sensor_Log
```

---

# 이번 장 핵심 정리

- ON DELETE를 지정하지 않으면 부모 삭제가 제한된다.
- 자식 데이터를 먼저 삭제해야 부모를 삭제할 수 있다.
- **CASCADE**는 부모 삭제 시 자식도 자동 삭제된다.
- **SET NULL**은 자식 데이터는 유지하고 참조만 제거한다.
- **RESTRICT**는 참조 중인 부모의 삭제를 허용하지 않는다.
- 실무에서는 데이터의 의미를 고려하여 ON DELETE 옵션을 선택해야 한다.
- 쇼핑몰과 스마트팩토리 모두 데이터의 성격에 따라 서로 다른 ON DELETE 전략을 적용한다.
