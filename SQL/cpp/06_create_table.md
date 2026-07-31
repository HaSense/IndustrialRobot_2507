# Chapter 6. CREATE TABLE

## 학습 목표

- `db.exec()`로 재실행 가능한 테이블 초기화를 구현한다.
- SQLite의 핵심 제약조건을 스키마에 적용한다.

## 1. 테이블 생성

```cpp
void createSchema(SQLite::Database& db) {
    db.exec(R"sql(
        CREATE TABLE IF NOT EXISTS student (
            id    INTEGER PRIMARY KEY AUTOINCREMENT,
            name  TEXT NOT NULL,
            age   INTEGER NOT NULL CHECK (age >= 0),
            email TEXT UNIQUE
        )
    )sql");
}
```

raw string literal은 여러 줄 SQL의 따옴표 이스케이프를 줄인다. `IF NOT EXISTS`는 같은 초기화 코드를 반복 실행할 수 있게 하지만 기존 테이블의 구조를 갱신하지는 않는다.

## 2. 제약조건

- `PRIMARY KEY`: 행을 식별한다.
- `NOT NULL`: 필수 값을 강제한다.
- `UNIQUE`: 중복을 막는다. SQLite에서는 여러 `NULL`이 허용된다.
- `CHECK`: 값의 도메인을 검사한다.
- `FOREIGN KEY`: 테이블 사이 참조 무결성을 보장한다.

`AUTOINCREMENT`는 삭제된 rowid 재사용까지 방지해야 할 때 쓴다. 단순 자동 번호만 필요하면 `INTEGER PRIMARY KEY`로도 충분하며 보통 더 가볍다.

## 3. 스키마 확인

```cpp
SQLite::Statement query(db, "PRAGMA table_info(student)");
while (query.executeStep()) {
    std::cout << query.getColumn("name").getString()
              << " / " << query.getColumn("type").getString()
              << '\n';
}
```

CLI에서는 `.schema student`와 `PRAGMA table_info(student);`를 사용할 수 있다.

## 4. 스키마 변경

배포 후에는 `IF NOT EXISTS`만으로 부족하다. `PRAGMA user_version` 같은 버전 값과 순차 migration을 관리한다.

```cpp
int version = db.execAndGet("PRAGMA user_version").getInt();
if (version == 0) {
    SQLite::Transaction tx(db);
    createSchema(db);
    db.exec("PRAGMA user_version = 1");
    tx.commit();
}
```

## 실습

1. 스키마를 두 번 초기화해 오류가 없는지 확인한다.
2. 음수 나이와 중복 이메일을 넣어 제약조건 오류를 관찰한다.
3. `AUTOINCREMENT`를 뺀 스키마와 차이를 조사한다.

## 확인 문제

1. `IF NOT EXISTS`가 기존 컬럼을 추가하지 않는 이유는 무엇인가?
2. 데이터 유효성을 C++뿐 아니라 DB 제약조건으로도 표현해야 하는 이유는?
3. `TEXT UNIQUE` 컬럼에서 `NULL`은 어떤 의미인가?

