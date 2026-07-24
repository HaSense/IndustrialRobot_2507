# Chapter 07. PreparedStatement와 안전한 입력

## 1. 개념 설명

문자열을 이어 붙여 SQL을 만들면 따옴표 처리 오류와 SQL 인젝션 위험이 생깁니다. `PreparedStatement`는 SQL 구조와 입력값을 분리합니다.

## 2. 비교 코드

피해야 할 코드:

```java
String sql = "SELECT * FROM book WHERE title = '" + title + "'";
```

권장 코드:

```java
String sql = "SELECT * FROM book WHERE title = ?";
try (PreparedStatement ps = con.prepareStatement(sql)) {
    ps.setString(1, title);
    try (ResultSet rs = ps.executeQuery()) {
        while (rs.next()) {
            System.out.println(rs.getString("title"));
        }
    }
}
```

## 3. 결과 표

| 입력값 | 문자열 연결 | PreparedStatement |
|---|---|---|
| `어린 왕자` | 조회 가능 | 조회 가능 |
| `It's Java` | 따옴표 오류 가능 | 정상 처리 |
| 악의적인 SQL 조각 | 위험 | 일반 문자열로 처리 |

## 4. 실습

저자 이름을 입력받아 그 저자의 도서를 조회하세요.

## 5. 정답

```java
String sql = "SELECT book_id, title, price FROM book WHERE author = ?";
try (PreparedStatement ps = con.prepareStatement(sql)) {
    ps.setString(1, scanner.nextLine());
    try (ResultSet rs = ps.executeQuery()) {
        while (rs.next()) {
            System.out.printf("%d %s %d원%n",
                    rs.getInt("book_id"),
                    rs.getString("title"),
                    rs.getInt("price"));
        }
    }
}
```

