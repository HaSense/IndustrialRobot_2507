# Chapter 03. 도서 등록: INSERT

## 1. 개념 설명

`INSERT`는 새 행을 추가합니다. Java에서는 `Connection`으로 `PreparedStatement`를 만들고 `executeUpdate()`를 호출합니다.

## 2. 짧은 SQL과 Java

```sql
INSERT INTO book (title, author, price)
VALUES (?, ?, ?);
```

`?`에는 실행할 때 실제 값이 들어갑니다.

```java
String sql = "INSERT INTO book(title, author, price) VALUES (?, ?, ?)";

try (Connection con = DriverManager.getConnection(URL);
     PreparedStatement ps = con.prepareStatement(sql)) {
    ps.setString(1, "해리 포터");
    ps.setString(2, "J. K. 롤링");
    ps.setInt(3, 18000);
    int count = ps.executeUpdate();
    System.out.println(count + "권 등록");
}catch (SQLException e) {
            System.out.println("연결 실패: " + e.getMessage());
}
```

## 3. 결과 표

| book_id | title | author | price |
|---:|---|---|---:|
| 1 | 어린 왕자 | 생텍쥐페리 | 12000 |
| 2 | 해리 포터 | J. K. 롤링 | 18000 |

## 4. 실습

제목이 `데미안`, 저자가 `헤르만 헤세`, 가격이 `13500`인 도서를 등록하세요.

## 5. 정답

```java
ps.setString(1, "데미안");
ps.setString(2, "헤르만 헤세");
ps.setInt(3, 13500);
ps.executeUpdate();
```

