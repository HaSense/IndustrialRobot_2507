# Chapter 1. SQLite C API 빠른 복습

## 학습 목표

- SQLite가 서버 없이 파일을 직접 읽고 쓰는 내장형 데이터베이스임을 설명한다.
- C API에서 연결, 준비, 바인딩, 실행, 해제의 흐름을 구현한다.
- C API의 자원 관리 부담이 C++ 래퍼에서 어떻게 개선되는지 예측한다.

## 1. SQLite의 동작 구조

애플리케이션은 별도 DB 서버와 통신하지 않는다. 프로세스에 링크된 SQLite 라이브러리가 데이터베이스 파일을 직접 관리한다. `sqlite3*`는 연결 핸들이고 `sqlite3_stmt*`는 컴파일된 SQL 문 핸들이다.

```text
C++ 프로그램 → SQLite 라이브러리 → student.db
              sqlite3*          파일/페이지/트랜잭션
```

SQLite는 동적 타입을 사용하지만 컬럼에는 타입 선호도(type affinity)가 있다. `INTEGER PRIMARY KEY`는 내부 `rowid`의 별칭이 된다. 외래 키를 사용할 때는 연결마다 `PRAGMA foreign_keys = ON`을 실행한다.

## 2. 연결과 종료

```cpp
#include <sqlite3.h>
#include <iostream>

int main() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open("student.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << sqlite3_errmsg(db) << '\n';
        sqlite3_close(db);
        return 1;
    }

    std::cout << "연결 성공\n";
    sqlite3_close(db);
}
```

`sqlite3_open()`은 실패하더라도 오류 정보를 가진 핸들을 돌려줄 수 있다. 따라서 핸들이 있다면 오류 메시지를 읽고 닫는다. 종료 시 미해제 statement가 있으면 `sqlite3_close()`가 `SQLITE_BUSY`를 반환할 수 있다.

## 3. DDL과 단순 SQL: sqlite3_exec()

```cpp
#include <sqlite3.h>
#include <iostream>

int main()
{
    sqlite3* db = nullptr;
    char* error = nullptr;

    // 1. 데이터베이스 열기
    int rc = sqlite3_open("student.db", &db);

    if (rc != SQLITE_OK)
    {
        std::cerr << "데이터베이스 연결 실패: "
                  << sqlite3_errmsg(db) << '\n';

        sqlite3_close(db);
        return 1;
    }

    std::cout << "데이터베이스 연결 성공\n";

    // 2. 테이블 생성 SQL
    const char* ddl = R"(
        CREATE TABLE IF NOT EXISTS student (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            age INTEGER NOT NULL CHECK(age >= 0),
            email TEXT UNIQUE
        );
    )";

    // 3. SQL 실행
    rc = sqlite3_exec(db, ddl, nullptr, nullptr, &error);

    if (rc != SQLITE_OK)
    {
        std::cerr << "테이블 생성 실패: ";

        if (error != nullptr)
        {
            std::cerr << error << '\n';
            sqlite3_free(error);
        }

        sqlite3_close(db);
        return 1;
    }

    std::cout << "student 테이블 생성 성공\n";

    // 4. 데이터베이스 닫기
    sqlite3_close(db);

    return 0;
}
```

`sqlite3_exec()`는 준비·실행·해제를 한 번에 수행하므로 DDL에 편리하다. 사용자 값을 문자열 결합으로 넣으면 SQL 삽입 공격과 따옴표 오류가 생기므로 CRUD에는 prepared statement를 사용한다.

## 4. INSERT: 준비, 바인딩, 실행, 해제

```cpp
const char* sql = "INSERT INTO student(name, age, email) VALUES (?, ?, ?)";
sqlite3_stmt* stmt = nullptr;

if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << sqlite3_errmsg(db) << '\n';
    return 1;
}

sqlite3_bind_text(stmt, 1, "김민수", -1, SQLITE_TRANSIENT);
sqlite3_bind_int(stmt, 2, 20);
sqlite3_bind_text(stmt, 3, "minsu@example.com", -1, SQLITE_TRANSIENT);

if (sqlite3_step(stmt) != SQLITE_DONE)
    std::cerr << sqlite3_errmsg(db) << '\n';

sqlite3_finalize(stmt);
```

바인딩 인덱스는 0이 아니라 1부터 시작한다. `SQLITE_TRANSIENT`는 SQLite가 문자열을 복사하게 한다. 쓰기 SQL의 성공 결과는 `SQLITE_DONE`이다.

## 5. SELECT와 반복 조회

```cpp
sqlite3_prepare_v2(db,
    "SELECT id, name, age, email FROM student ORDER BY id",
    -1, &stmt, nullptr);

while (sqlite3_step(stmt) == SQLITE_ROW) {
    int id = sqlite3_column_int(stmt, 0);
    const auto* name = sqlite3_column_text(stmt, 1);
    int age = sqlite3_column_int(stmt, 2);
    bool emailNull = sqlite3_column_type(stmt, 3) == SQLITE_NULL;

    std::cout << id << ", " << name << ", " << age << ", "
              << (emailNull ? "(없음)" :
                  reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)))
              << '\n';
}
sqlite3_finalize(stmt);
```

조회 컬럼 인덱스는 0부터 시작한다. `sqlite3_step()`은 행마다 `SQLITE_ROW`, 끝에서 `SQLITE_DONE`을 반환한다.

## 6. C API와 SQLiteCpp의 연결

| C API | SQLiteCpp | 개선점 |
|---|---|---|
| `sqlite3*` | `SQLite::Database` | 소멸자가 연결을 닫음 |
| `sqlite3_stmt*` | `SQLite::Statement` | 소멸자가 statement를 해제 |
| 반환 코드 | `SQLite::Exception` | 예외로 오류 흐름 통합 |
| `sqlite3_bind_*` | `bind()` | C++ 오버로드 |
| `sqlite3_column_*` | `getColumn()` | 읽기 코드 단순화 |

## 실습

`student` 테이블을 만들고 학생 2명을 INSERT한 뒤 전체 조회, 한 명의 나이 UPDATE, 한 명 DELETE를 C API로 구현한다. 모든 반환 코드를 확인하고 statement가 모든 경로에서 해제되는지 점검한다.

## 확인 문제

1. 바인딩 인덱스와 조회 컬럼 인덱스의 시작값은 각각 무엇인가?
2. `sqlite3_step()`의 `SQLITE_ROW`와 `SQLITE_DONE`은 언제 반환되는가?
3. 사용자 입력을 SQL 문자열에 직접 연결하면 안 되는 이유는 무엇인가?

