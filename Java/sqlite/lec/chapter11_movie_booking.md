# Chapter 11. 영화와 예매: 세 테이블 JOIN

## 1. 개념 설명

영화, 고객, 예매를 각각 저장합니다. 예매 테이블은 영화와 고객의 기본키를 외래키로 가집니다.

## 2. 짧은 SQL

```sql
CREATE TABLE movie (
    movie_id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL,
    running_minutes INTEGER NOT NULL
);

CREATE TABLE movie_customer (
    customer_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL
);

CREATE TABLE booking (
    booking_id INTEGER PRIMARY KEY AUTOINCREMENT,
    movie_id INTEGER NOT NULL,
    customer_id INTEGER NOT NULL,
    seat_no TEXT NOT NULL,
    booked_at TEXT NOT NULL,
    FOREIGN KEY (movie_id) REFERENCES movie(movie_id),
    FOREIGN KEY (customer_id) REFERENCES movie_customer(customer_id),
    UNIQUE (movie_id, seat_no)
);
```

```sql
SELECT bk.booking_id, m.title, c.name, bk.seat_no
FROM booking bk
JOIN movie m ON bk.movie_id = m.movie_id
JOIN movie_customer c ON bk.customer_id = c.customer_id
ORDER BY bk.booking_id;
```

## 3. 결과 표

| booking_id | title | name | seat_no |
|---:|---|---|---|
| 1 | 인사이드 아웃 | 김하늘 | A-1 |
| 2 | 인사이드 아웃 | 이바다 | A-2 |

## 4. 실습

1. 특정 영화의 예매 목록만 조회하세요.
2. `A-1` 좌석을 다시 예매할 때 어떤 일이 생기는지 확인하세요.

## 5. 정답

```sql
SELECT bk.booking_id, m.title, c.name, bk.seat_no
FROM booking bk
JOIN movie m ON bk.movie_id = m.movie_id
JOIN movie_customer c ON bk.customer_id = c.customer_id
WHERE m.movie_id = 1;
```

`UNIQUE (movie_id, seat_no)` 제약 때문에 같은 영화의 같은 좌석은 두 번 저장되지 않습니다.

