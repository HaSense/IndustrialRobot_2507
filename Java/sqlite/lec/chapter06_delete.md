# Chapter 06. 도서 삭제: DELETE

## 1. 개념 설명

`DELETE`는 행을 삭제합니다. 삭제는 되돌리기 어려우므로 기본키로 대상을 조회한 다음 삭제하는 습관이 중요합니다.

## 2. 짧은 SQL과 Java

```sql
DELETE FROM book WHERE book_id = ?;
```

```java
String sql = "DELETE FROM book WHERE book_id = ?";

try (Connection con = DriverManager.getConnection(URL);
     PreparedStatement ps = con.prepareStatement(sql)) {
    ps.setInt(1, 2);
    int count = ps.executeUpdate();
    System.out.println(count == 1 ? "삭제 완료" : "해당 도서 없음");
}
```

## 3. 결과 표

| 처리 | 영향받은 행 |
|---|---:|
| 존재하는 ID 삭제 | 1 |
| 존재하지 않는 ID 삭제 | 0 |

## 4. 실습

사용자에게 도서 번호를 입력받아 삭제하고, 삭제 여부를 출력하세요.

## 5. 정답

```java
Scanner scanner = new Scanner(System.in);
System.out.print("삭제할 도서 번호: ");
int bookId = scanner.nextInt();

try (PreparedStatement ps =
         con.prepareStatement("DELETE FROM book WHERE book_id = ?")) {
    ps.setInt(1, bookId);
    int count = ps.executeUpdate();
    System.out.println(count > 0 ? "삭제 완료" : "도서를 찾지 못했습니다.");
}
```

