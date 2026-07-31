# Chapter 5. Database 클래스

## 학습 목표

- `SQLite::Database`의 열기 모드와 주요 기능을 사용한다.
- 연결 수명과 예외 처리 원칙을 설명한다.

## 1. 열기 모드

```cpp
SQLite::Database readOnly("student.db", SQLite::OPEN_READONLY);
SQLite::Database writable(
    "student.db",
    SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
);
```

읽기만 하는 프로그램은 `OPEN_READONLY`로 의도를 명확히 한다. 생성 권한이 필요하지 않으면 `OPEN_CREATE`를 빼 잘못된 경로에 빈 DB가 생기는 일을 막는다.

## 2. 주요 작업

```cpp
SQLite::Database db("student.db",
    SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

db.exec("PRAGMA foreign_keys = ON");
db.exec("CREATE TABLE IF NOT EXISTS log("
        "id INTEGER PRIMARY KEY, message TEXT NOT NULL)");

int changed = db.exec("DELETE FROM log WHERE id < 0");
std::cout << "변경 행: " << changed << '\n';
std::cout << "SQLite 버전: " << db.execAndGet("SELECT sqlite_version()")
                                  .getString() << '\n';
```

`exec()`는 결과 행을 순회하지 않는 SQL에 사용하며 영향받은 행 수를 반환한다. 단일 스칼라 결과에는 `execAndGet()`이 편리하다. 여러 행 조회에는 `Statement`를 사용한다.

## 3. 연결 범위 설계

`Database` 복사는 허용되지 않는다. 애플리케이션에서는 연결의 소유자를 명확히 하고 필요한 객체에 참조로 전달한다.

```cpp
void initialize(SQLite::Database& db) {
    db.exec("PRAGMA foreign_keys = ON");
}

int main() {
    try {
        SQLite::Database db("student.db",
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        initialize(db);
    } catch (const SQLite::Exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
```

SQLite 연결 하나를 여러 스레드에서 무분별하게 공유하지 않는다. 동시성 모델과 SQLite 빌드 옵션을 확인하고, 초급 프로젝트에서는 한 스레드가 연결을 소유하게 한다.

## 4. 잠금과 busy timeout

다른 연결이 쓰기 잠금을 보유하면 `database is locked`가 발생할 수 있다. 짧은 경합을 기다리게 할 수 있다.

```cpp
db.setBusyTimeout(3000); // 밀리초
```

timeout은 교착이나 긴 트랜잭션을 해결하지 않는다. 트랜잭션을 짧게 유지하고, 사용자 입력을 기다리는 동안 트랜잭션을 열어 두지 않는다.

## 실습

읽기/쓰기 연결로 DB를 생성하고 `PRAGMA foreign_keys`, SQLite 버전, 테이블 수를 출력한다. 존재하지 않는 DB를 읽기 전용으로 열었을 때 예외도 확인한다.

## 확인 문제

1. `exec()`와 `execAndGet()`의 용도 차이는 무엇인가?
2. 쓰기 프로그램에서 busy timeout만 크게 늘리는 것이 근본 해결이 아닌 이유는?
3. `Database`보다 `Statement`의 수명이 짧아야 하는 이유는?

