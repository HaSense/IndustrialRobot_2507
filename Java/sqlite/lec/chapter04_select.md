# Chapter 04. 도서 조회: SELECT

## 1. 개념 설명

`SELECT`는 데이터를 조회합니다. JDBC의 `executeQuery()`는 조회 결과인 `ResultSet`을 반환합니다. `resultSet.next()`는 다음 행으로 이동하며, 더 이상 행이 없으면 `false`가 됩니다.

## 2. 짧은 SQL과 Java

```sql
SELECT book_id, title, author, price
FROM book
ORDER BY book_id;
```

```java
String sql = "SELECT book_id, title, author, price FROM book ORDER BY book_id";

try (Connection con = DriverManager.getConnection(URL);
     PreparedStatement ps = con.prepareStatement(sql);
     ResultSet rs = ps.executeQuery()) {

    while (rs.next()) {
        int id = rs.getInt("book_id");
        String title = rs.getString("title");
        String author = rs.getString("author");
        int price = rs.getInt("price");
        System.out.printf("%d | %s | %s | %d원%n", id, title, author, price);
    }
}
```

## 3. 실행 결과

```text
1 | 어린 왕자 | 생텍쥐페리 | 12000원
2 | 해리 포터 | J. K. 롤링 | 18000원
```

## 4. 실습

가격이 15000원 이상인 도서만 가격이 높은 순서로 조회하세요.

## 5. 정답

```java
String sql = """
    SELECT book_id, title, author, price
    FROM book
    WHERE price >= ?
    ORDER BY price DESC
    """;

try (PreparedStatement ps = con.prepareStatement(sql)) {
    ps.setInt(1, 15000);
    try (ResultSet rs = ps.executeQuery()) {
        while (rs.next()) {
            System.out.println(rs.getString("title"));
        }
    }
}
```

