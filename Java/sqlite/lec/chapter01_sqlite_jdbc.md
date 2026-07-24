# Chapter 01. SQLite와 JDBC의 역할

## 1. 개념 설명

SQLite는 데이터베이스를 하나의 파일에 저장하는 소형 관계형 데이터베이스입니다. 별도의 데이터베이스 서버가 없어도 실행할 수 있어 학습용 프로그램에 적합합니다.

JDBC(Java Database Connectivity)는 Java 프로그램이 데이터베이스에 SQL을 전달하고 결과를 받는 표준 방법입니다.

```text
Java 프로그램 → JDBC 드라이버 → SQLite → study.db
```

## 2. 짧은 SQL

```sql
CREATE TABLE book (
    book_id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL,
    author TEXT NOT NULL,
    price INTEGER NOT NULL
);

INSERT INTO book (title, author, price)
VALUES ('어린 왕자', '생텍쥐페리', 12000);
```

## 3. 결과 표

| book_id | title | author | price |
|---:|---|---|---:|
| 1 | 어린 왕자 | 생텍쥐페리 | 12000 |

## 4. 실습

1. `study.db` 파일을 만듭니다.
2. `book` 테이블을 생성합니다.
3. 도서 한 권을 추가합니다.
4. `SELECT * FROM book;`으로 확인합니다.

## 5. 정답

```bash
sqlite3 study.db
```

```sql
CREATE TABLE book (
    book_id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL,
    author TEXT NOT NULL,
    price INTEGER NOT NULL
);

INSERT INTO book (title, author, price)
VALUES ('자바의 정석', '남궁성', 30000);

SELECT * FROM book;
```

