# Chapter 12. 종합 실습: 영화 예매 CRUD

## 1. 목표

지금까지 배운 JDBC 연결, `PreparedStatement`, CRUD, JOIN, 예외 처리를 이용해 콘솔 영화 예매 프로그램을 만듭니다.

## 2. 기능

```text
1. 영화 등록
2. 영화 목록
3. 고객 등록
4. 예매 등록
5. 예매 목록
6. 예매 좌석 수정
7. 예매 취소
0. 종료
```

## 3. 핵심 SQL

```sql
-- 예매 등록
INSERT INTO booking(movie_id, customer_id, seat_no, booked_at)
VALUES (?, ?, ?, datetime('now', 'localtime'));

-- 예매 좌석 수정
UPDATE booking SET seat_no = ? WHERE booking_id = ?;

-- 예매 취소
DELETE FROM booking WHERE booking_id = ?;
```

## 4. 결과 표

| 예매번호 | 영화 | 고객 | 좌석 |
|---:|---|---|---|
| 1 | 인사이드 아웃 | 김하늘 | A-1 |
| 2 | 코코 | 이바다 | B-3 |

## 5. 실습

다음 조건을 만족하는 프로그램을 작성하세요.

1. 모든 입력값은 `Scanner`로 받습니다.
2. SQL에는 문자열 연결 대신 `PreparedStatement`를 사용합니다.
3. 예매 목록은 세 테이블을 JOIN하여 출력합니다.
4. 중복 좌석 예매가 발생하면 프로그램을 종료하지 않고 안내합니다.
5. 삭제된 예매 번호를 입력하면 `예매를 찾지 못했습니다`를 출력합니다.

## 6. 정답 예시

```java
static void addBooking(int movieId, int customerId, String seatNo) {
    String sql = """
        INSERT INTO booking(movie_id, customer_id, seat_no, booked_at)
        VALUES (?, ?, ?, datetime('now', 'localtime'))
        """;

    try (Connection con = DriverManager.getConnection(URL);
         PreparedStatement ps = con.prepareStatement(sql)) {
        ps.setInt(1, movieId);
        ps.setInt(2, customerId);
        ps.setString(3, seatNo);
        ps.executeUpdate();
        System.out.println("예매 완료");
    } catch (SQLException e) {
        if (e.getMessage().contains("UNIQUE")) {
            System.out.println("이미 예매된 좌석입니다.");
        } else {
            System.out.println("예매 실패: " + e.getMessage());
        }
    }
}
```

```java
static void listBookings() throws SQLException {
    String sql = """
        SELECT b.booking_id, m.title, c.name, b.seat_no
        FROM booking b
        JOIN movie m ON b.movie_id = m.movie_id
        JOIN movie_customer c ON b.customer_id = c.customer_id
        ORDER BY b.booking_id
        """;

    try (Connection con = DriverManager.getConnection(URL);
         PreparedStatement ps = con.prepareStatement(sql);
         ResultSet rs = ps.executeQuery()) {
        while (rs.next()) {
            System.out.printf("%d | %s | %s | %s%n",
                    rs.getInt("booking_id"),
                    rs.getString("title"),
                    rs.getString("name"),
                    rs.getString("seat_no"));
        }
    }
}
```

## 7. 확장 과제

- 영화 제목 검색
- 고객별 예매 조회
- 예매 수가 많은 영화 순위
- 메뉴 입력 오류 처리

