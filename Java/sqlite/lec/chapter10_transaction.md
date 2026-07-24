# Chapter 10. 편의점 고객과 주문: 트랜잭션

## 1. 개념 설명

트랜잭션은 여러 SQL을 하나의 작업 단위로 묶습니다. 모든 처리가 성공하면 `commit()`, 중간에 실패하면 `rollback()`을 호출합니다.

주문 등록과 고객 포인트 적립은 함께 성공하거나 함께 취소되어야 합니다.

## 2. 짧은 SQL

```sql
CREATE TABLE customer (
    customer_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    point INTEGER NOT NULL DEFAULT 0
);

CREATE TABLE store_order (
    order_id INTEGER PRIMARY KEY AUTOINCREMENT,
    customer_id INTEGER NOT NULL,
    total_amount INTEGER NOT NULL,
    ordered_at TEXT NOT NULL,
    FOREIGN KEY (customer_id) REFERENCES customer(customer_id)
);
```

## 3. Java 트랜잭션

```java
try (Connection con = DriverManager.getConnection(URL)) {
    con.setAutoCommit(false);
    try {
        try (PreparedStatement orderPs = con.prepareStatement(
                "INSERT INTO store_order(customer_id, total_amount, ordered_at) " +
                "VALUES (?, ?, date('now'))")) {
            orderPs.setInt(1, 1);
            orderPs.setInt(2, 10000);
            orderPs.executeUpdate();
        }

        try (PreparedStatement pointPs = con.prepareStatement(
                "UPDATE customer SET point = point + ? WHERE customer_id = ?")) {
            pointPs.setInt(1, 100);
            pointPs.setInt(2, 1);
            pointPs.executeUpdate();
        }
        con.commit();
    } catch (SQLException e) {
        con.rollback();
        throw e;
    }
}
```

## 4. 결과 표

| 처리 상황 | 주문 | 포인트 |
|---|---|---|
| 두 SQL 성공 | 저장 | 적립 |
| 하나라도 실패 | 취소 | 취소 |

## 5. 실습

20000원 주문을 등록하고 결제액의 1%인 200포인트를 적립하도록 값을 변경하세요.

## 6. 정답

```java
orderPs.setInt(2, 20000);
pointPs.setInt(1, 200);
```

