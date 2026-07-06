# 고객 삭제 시 주문 이력을 유지하는 데이터베이스 설계

## 왜 주문을 삭제하면 안 될까?

쇼핑몰이나 스마트팩토리에서는 **고객 정보(Customer)**와 **주문 정보(Order)**의 성격이 다릅니다.

- 고객 정보는 변경되거나 삭제될 수 있다.
- 주문 정보는 회사의 거래 이력이므로 보존해야 한다.
- 생산 이력, 출하 이력, 품질 이력도 동일하게 삭제하지 않는다.

따라서 고객이 탈퇴하거나 삭제되더라도 **주문 이력은 그대로 유지**하는 것이 일반적인 모델링이다.

---

# 잘못된 모델링

다음과 같이 `ON DELETE CASCADE`를 사용하면 문제가 발생할 수 있다.

```sql
FOREIGN KEY(customer_id)
REFERENCES Customers(customer_id)
ON DELETE CASCADE;
```

예를 들어

```
Customers
----------
1 Kim
2 Lee

Orders
----------
100  customer_id=1
101  customer_id=2
```

고객 1을 삭제하면

```sql
DELETE FROM Customers
WHERE customer_id = 1;
```

결과

```
Customers
----------
2 Lee

Orders
----------
101 customer_id=2
```

주문까지 모두 삭제된다.

이는 대부분의 쇼핑몰이나 제조 시스템에서는 바람직하지 않다.

---

# 권장 모델링

주문에는 주문 당시 고객 정보를 함께 저장한다.

```
Customers
──────────────
customer_id
name
email

        │

        ▼

Orders
──────────────────────────────
order_id
customer_id
customer_name_snapshot
customer_email_snapshot
order_date

        │

        ▼

Order_Items
──────────────────────
item_id
order_id
product_name
quantity
price
```

---

# Customers 테이블

```sql
CREATE TABLE Customers
(
    customer_id INTEGER PRIMARY KEY,

    name TEXT NOT NULL,

    email TEXT UNIQUE,

    deleted_at TEXT
);
```

---

# Orders 테이블

```sql
CREATE TABLE Orders
(
    order_id INTEGER PRIMARY KEY,

    customer_id INTEGER,

    customer_name_snapshot TEXT NOT NULL,

    customer_email_snapshot TEXT,

    order_date TEXT DEFAULT CURRENT_DATE,

    FOREIGN KEY(customer_id)
        REFERENCES Customers(customer_id)
        ON DELETE SET NULL
);
```

핵심은 다음 부분이다.

```sql
ON DELETE SET NULL
```

고객이 삭제되면

```
customer_id → NULL
```

이 된다.

하지만 주문 당시의 고객 이름과 이메일은 그대로 남는다.

---

# Order_Items 테이블

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
        ON DELETE CASCADE
);
```

주문이 삭제되면

주문 상세도 함께 삭제되는 것이 자연스럽다.

---

# 데이터 입력 예제

## 고객 등록

```sql
INSERT INTO Customers
VALUES
(1,'Kim Factory','kim@factory.com',NULL);

INSERT INTO Customers
VALUES
(2,'Lee Motors','lee@factory.com',NULL);
```

---

## 주문 등록

주문 당시 고객 정보를 함께 저장한다.

```sql
INSERT INTO Orders
(
    order_id,
    customer_id,
    customer_name_snapshot,
    customer_email_snapshot
)
SELECT
    100,
    customer_id,
    name,
    email
FROM Customers
WHERE customer_id=1;
```

---

## 주문 상품 등록

```sql
INSERT INTO Order_Items
VALUES
(1,100,'Bearing',10,5000);

INSERT INTO Order_Items
VALUES
(2,100,'Sensor',2,30000);
```

---

# 고객 삭제

```sql
DELETE FROM Customers
WHERE customer_id=1;
```

삭제 후

Customers

```
2 Lee Motors
```

Orders

```
100
customer_id = NULL
customer_name_snapshot = Kim Factory
customer_email_snapshot = kim@factory.com
```

주문은 그대로 유지된다.

---

# 조회

```sql
SELECT
    o.order_id,
    o.customer_id,
    o.customer_name_snapshot,
    oi.product_name,
    oi.quantity,
    oi.price
FROM Orders o
JOIN Order_Items oi
ON o.order_id = oi.order_id;
```

결과

|order_id|customer_id|customer_name_snapshot|product_name|
|---------|-----------|----------------------|------------|
|100|NULL|Kim Factory|Bearing|
|100|NULL|Kim Factory|Sensor|

---

# 실무에서는 더 많이 사용하는 방법

실제 ERP, MES, 스마트팩토리에서는 고객을 삭제하지 않는 경우가 많다.

삭제 대신 **논리 삭제(Soft Delete)** 를 사용한다.

예를 들어

```sql
UPDATE Customers
SET deleted_at = DATETIME('now')
WHERE customer_id = 1;
```

삭제된 고객은

```sql
SELECT *
FROM Customers
WHERE deleted_at IS NULL;
```

처럼 조회에서 제외한다.

---

# ON DELETE 방식 비교

|방식|결과|실무 사용|
|----|----|---------|
|기본(RESTRICT)|삭제 불가|★★★★★|
|SET NULL|외래키만 NULL|★★★★☆|
|CASCADE|관련 데이터 모두 삭제|★★☆☆☆|
|논리 삭제(Soft Delete)|데이터는 그대로 유지|★★★★★|

---

# 스마트팩토리에서는 어떻게 사용할까?

|데이터|권장 방법|
|-------|---------|
|고객(Customer)|논리 삭제 또는 SET NULL|
|주문(Order)|삭제하지 않음|
|생산이력(Production)|삭제하지 않음|
|품질이력(QC)|삭제하지 않음|
|LOT 이력|삭제하지 않음|
|출하이력|삭제하지 않음|

---

# 정리

- 고객은 삭제될 수 있다.
- 주문은 회사의 자산이므로 삭제하지 않는다.
- 주문 당시 고객 정보를 Snapshot으로 저장한다.
- 외래키는 `ON DELETE SET NULL`을 사용하는 경우가 많다.
- 실제 ERP, MES, 스마트팩토리에서는 논리 삭제(Soft Delete)를 가장 많이 사용한다.
