# Chapter 09. 도서와 대출: 두 테이블과 JOIN

## 1. 개념 설명

대출 정보에 도서 제목을 반복 저장하지 않고 `book_id`를 저장합니다. `JOIN`은 두 테이블의 관련 행을 연결해 한 결과로 보여 줍니다.

## 2. 짧은 SQL

```sql
CREATE TABLE loan (
    loan_id INTEGER PRIMARY KEY AUTOINCREMENT,
    book_id INTEGER NOT NULL,
    borrower_name TEXT NOT NULL,
    loan_date TEXT NOT NULL,
    returned INTEGER NOT NULL DEFAULT 0,
    FOREIGN KEY (book_id) REFERENCES book(book_id)
);
```

```sql
SELECT l.loan_id, b.title, l.borrower_name, l.loan_date, l.returned
FROM loan AS l
JOIN book AS b ON l.book_id = b.book_id
ORDER BY l.loan_id;
```

## 3. 결과 표

| loan_id | title | borrower_name | loan_date | returned |
|---:|---|---|---|---:|
| 1 | 어린 왕자 | 김하늘 | 2026-07-24 | 0 |
| 2 | 데미안 | 이바다 | 2026-07-23 | 1 |

## 4. Java 조회

```java
String sql = """
    SELECT l.loan_id, b.title, l.borrower_name, l.loan_date
    FROM loan l
    JOIN book b ON l.book_id = b.book_id
    WHERE l.returned = 0
    ORDER BY l.loan_date
    """;
```

## 5. 실습

1. 반납하지 않은 대출만 조회하세요.
2. 대출 번호 1을 반납 처리하세요.

## 6. 정답

```sql
SELECT l.loan_id, b.title, l.borrower_name
FROM loan l
JOIN book b ON l.book_id = b.book_id
WHERE l.returned = 0;

UPDATE loan SET returned = 1 WHERE loan_id = 1;
```

