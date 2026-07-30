# Chapter 2. SQLiteCpp 소개

## 학습 목표

- SQLiteCpp가 SQLite C API를 감싸는 C++ 래퍼임을 설명한다.
- 객체지향 래퍼와 RAII가 자원 관리에 주는 이점을 이해한다.
- SQLiteCpp가 SQL 자체를 대신 작성해 주는 ORM은 아님을 구분한다.

## 1. SQLiteCpp란?

SQLiteCpp(SQLiteC++)는 SQLite3 C API 위에 `Database`, `Statement`, `Column`, `Transaction` 같은 C++ 클래스를 제공한다. 데이터베이스 엔진은 여전히 SQLite이며 SQL 문법과 트랜잭션 규칙도 그대로 적용된다.

```cpp
#include <SQLiteCpp/SQLiteCpp.h>

SQLite::Database db("student.db",
                    SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
db.exec("CREATE TABLE IF NOT EXISTS memo(id INTEGER PRIMARY KEY, body TEXT)");
```

## 2. Wrapper와 ORM의 차이

Wrapper는 저수준 함수 호출을 클래스 인터페이스로 정리한다. SQLiteCpp에서는 개발자가 SQL을 직접 작성한다. 반면 ORM은 클래스와 테이블의 매핑, SQL 생성, 객체 변경 추적 등을 더 많이 자동화한다.

| 관점 | SQLite C API | SQLiteCpp | 전형적 ORM |
|---|---|---|---|
| SQL 작성 | 직접 | 직접 | 일부/대부분 생성 |
| 자원 관리 | 수동 | RAII | 프레임워크 관리 |
| 타입 매핑 | 함수 선택 | 오버로드/Column | 모델 규칙 |
| 추상화 수준 | 낮음 | 중간 | 높음 |

직접 SQL을 학습하면서 C++다운 자원 관리를 얻고 싶을 때 SQLiteCpp가 알맞다.

## 3. RAII

RAII는 자원을 객체 수명에 묶는 C++ 관용구다. 생성자가 자원을 획득하고 소멸자가 반환한다.

```cpp
void printCount() {
    SQLite::Database db("student.db", SQLite::OPEN_READONLY);
    SQLite::Statement query(db, "SELECT COUNT(*) FROM student");

    if (query.executeStep())
        std::cout << query.getColumn(0).getInt() << '\n';
} // query 해제 후 db 닫힘
```

정상 종료뿐 아니라 예외로 블록을 빠져나가도 지역 객체의 소멸자는 호출된다. 단, `Database`보다 이를 참조하는 `Statement`가 먼저 파괴되어야 한다. 위처럼 선언하면 역순으로 안전하게 파괴된다.

## 4. 예외 기반 오류 처리

SQLiteCpp는 SQLite 오류를 `SQLite::Exception`으로 전달한다.

```cpp
try {
    SQLite::Database db("student.db", SQLite::OPEN_READWRITE);
    db.exec("INVALID SQL");
} catch (const SQLite::Exception& e) {
    std::cerr << "DB 오류: " << e.what() << '\n';
}
```

예외는 반환 코드 검사를 흩어 놓지 않게 하지만, 오류가 사라지는 것은 아니다. 제약조건 위반, 잠금, 잘못된 SQL 등을 적절한 계층에서 잡아 사용자 메시지나 로그로 바꿔야 한다.

## 5. 핵심 클래스

- `SQLite::Database`: 연결, 즉시 실행, 영향받은 행 수, 마지막 ID
- `SQLite::Statement`: SQL 준비, 값 바인딩, 실행과 반복 조회
- `SQLite::Column`: 조회 결과 한 컬럼의 타입별 값
- `SQLite::Transaction`: 시작과 커밋, 미커밋 시 자동 롤백
- `SQLite::Exception`: SQLite 오류 정보를 담은 예외

## 실습

Chapter 1의 C API 연결/테이블 생성 코드를 SQLiteCpp로 바꾼다. 두 버전에서 명시적으로 정리해야 하는 함수의 수를 비교한다.

## 확인 문제

1. SQLiteCpp를 사용해도 SQL 지식이 필요한 이유는 무엇인가?
2. RAII가 예외 상황에서 특히 유용한 이유는 무엇인가?
3. `Statement`가 참조하는 `Database`의 수명이 더 길어야 하는 이유는 무엇인가?

