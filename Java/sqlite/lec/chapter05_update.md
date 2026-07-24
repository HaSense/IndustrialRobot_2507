# Chapter 05. 도서 수정: UPDATE

## 1. 개념 설명

`UPDATE`는 기존 행을 수정합니다. `WHERE`가 없으면 모든 행이 바뀌므로 먼저 수정 대상의 기본키를 확인해야 합니다.

## 2. 짧은 SQL과 Java

```sql
UPDATE book
SET price = ?
WHERE book_id = ?;
```

```java
String sql = "UPDATE book SET price = ? WHERE book_id = ?";

try (Connection con = DriverManager.getConnection(URL);
     PreparedStatement ps = con.prepareStatement(sql)) {
    ps.setInt(1, 15000);
    ps.setInt(2, 1);
    int count = ps.executeUpdate();
    System.out.println(count + "권 수정");
}
```

## 3. 결과 표

| book_id | title | 수정 전 | 수정 후 |
|---:|---|---:|---:|
| 1 | 어린 왕자 | 12000 | 15000 |

## 4. 실습

`book_id`가 2인 도서의 제목을 `해리 포터와 마법사의 돌`로 수정하세요.

## 5. 정답

```java
String sql = "UPDATE book SET title = ? WHERE book_id = ?";
try (PreparedStatement ps = con.prepareStatement(sql)) {
    ps.setString(1, "해리 포터와 마법사의 돌");
    ps.setInt(2, 2);
    System.out.println(ps.executeUpdate() + "권 수정");
}
```

