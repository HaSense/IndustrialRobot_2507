# Chapter 08. 도서 관리 CRUD 프로그램

## 1. 개념 설명

CRUD는 데이터 처리의 네 가지 기본 기능입니다.

| 기능 | SQL | 메서드 예 |
|---|---|---|
| Create | INSERT | `addBook()` |
| Read | SELECT | `listBooks()` |
| Update | UPDATE | `updateBookPrice()` |
| Delete | DELETE | `deleteBook()` |

데이터베이스 코드를 메서드로 나누면 메뉴 코드와 SQL 코드가 뒤섞이지 않습니다.

## 2. 짧은 Java

```java
static final String URL = "jdbc:sqlite:data/study.db";

static void addBook(String title, String author, int price) throws SQLException {
    String sql = "INSERT INTO book(title, author, price) VALUES (?, ?, ?)";
    try (Connection con = DriverManager.getConnection(URL);
         PreparedStatement ps = con.prepareStatement(sql)) {
        ps.setString(1, title);
        ps.setString(2, author);
        ps.setInt(3, price);
        ps.executeUpdate();
    }
}
```

```java
while (true) {
    System.out.println("1. 등록  2. 목록  3. 가격 수정  4. 삭제  0. 종료");
    int menu = Integer.parseInt(scanner.nextLine());
    if (menu == 0) break;

    switch (menu) {
        case 1 -> addBook("데미안", "헤르만 헤세", 13500);
        case 2 -> listBooks();
        case 3 -> updateBookPrice(1, 15000);
        case 4 -> deleteBook(1);
        default -> System.out.println("메뉴를 다시 선택하세요.");
    }
}
```

## 3. 실행 결과

```text
1. 등록  2. 목록  3. 가격 수정  4. 삭제  0. 종료
2
1 | 어린 왕자 | 생텍쥐페리 | 15000원
2 | 데미안 | 헤르만 헤세 | 13500원
```

## 4. 실습

1. 등록 정보를 키보드로 입력받도록 변경합니다.
2. 도서 번호 하나를 조회하는 `findBookById()`를 만듭니다.
3. 존재하지 않는 번호에는 안내 문구를 출력합니다.

## 5. 정답

```java
static void findBookById(int bookId) throws SQLException {
    String sql = "SELECT * FROM book WHERE book_id = ?";
    try (Connection con = DriverManager.getConnection(URL);
         PreparedStatement ps = con.prepareStatement(sql)) {
        ps.setInt(1, bookId);
        try (ResultSet rs = ps.executeQuery()) {
            if (rs.next()) {
                System.out.printf("%d | %s | %s | %d원%n",
                        rs.getInt("book_id"), rs.getString("title"),
                        rs.getString("author"), rs.getInt("price"));
            } else {
                System.out.println("도서를 찾지 못했습니다.");
            }
        }
    }
}
```

