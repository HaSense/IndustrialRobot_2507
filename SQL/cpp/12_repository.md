# Chapter 12. StudentRepository 클래스

## 학습 목표

- Repository 패턴으로 SQL을 UI와 모델에서 분리한다.
- CRUD 결과를 C++ 타입으로 명확히 표현한다.
- 조회 행을 `Student` 객체로 변환한다.

## 1. 인터페이스

```cpp
// include/StudentRepository.h
#pragma once

#include "Student.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <optional>
#include <vector>

class StudentRepository {
public:
    explicit StudentRepository(SQLite::Database& db) : db_(db) {}

    int insert(const Student& student);
    std::vector<Student> findAll();
    std::optional<Student> findById(int id);
    bool update(const Student& student);
    bool remove(int id);

private:
    SQLite::Database& db_; // 소유하지 않는 참조
    static Student mapRow(SQLite::Statement& query);
};
```

Repository보다 `Database`가 오래 살아 있어야 한다. `delete`는 C++ 예약어이므로 메서드 이름은 `remove`를 사용한다.

## 2. INSERT와 행 매핑

```cpp
int StudentRepository::insert(const Student& student) {
    SQLite::Statement stmt(db_,
        "INSERT INTO student(name, age, email) VALUES (?, ?, ?)");
    stmt.bind(1, student.name());
    stmt.bind(2, student.age());
    if (student.email())
        stmt.bind(3, *student.email());
    else
        stmt.bind(3);
    stmt.exec();
    return static_cast<int>(db_.getLastInsertRowid());
}

Student StudentRepository::mapRow(SQLite::Statement& q) {
    std::optional<std::string> email;
    if (!q.getColumn("email").isNull())
        email = q.getColumn("email").getString();

    return Student(q.getColumn("id").getInt(),
                   q.getColumn("name").getString(),
                   q.getColumn("age").getInt(),
                   std::move(email));
}
```

## 3. 조회

```cpp
std::vector<Student> StudentRepository::findAll() {
    SQLite::Statement q(db_,
        "SELECT id, name, age, email FROM student ORDER BY id");
    std::vector<Student> result;
    while (q.executeStep())
        result.push_back(mapRow(q));
    return result;
}

std::optional<Student> StudentRepository::findById(int id) {
    SQLite::Statement q(db_,
        "SELECT id, name, age, email FROM student WHERE id = ?");
    q.bind(1, id);
    if (!q.executeStep())
        return std::nullopt;
    return mapRow(q);
}
```

## 4. 수정과 삭제

```cpp
bool StudentRepository::update(const Student& student) {
    SQLite::Statement stmt(db_,
        "UPDATE student SET name=?, age=?, email=? WHERE id=?");
    stmt.bind(1, student.name());
    stmt.bind(2, student.age());
    if (student.email()) stmt.bind(3, *student.email());
    else stmt.bind(3);
    stmt.bind(4, student.id());
    return stmt.exec() == 1;
}

bool StudentRepository::remove(int id) {
    SQLite::Statement stmt(db_, "DELETE FROM student WHERE id=?");
    stmt.bind(1, id);
    return stmt.exec() == 1;
}
```

## 5. 패턴의 효과와 한계

UI는 SQL 컬럼명을 몰라도 되고 테스트에서 저장소 동작을 한곳에서 검증할 수 있다. 하지만 모든 쿼리를 무조건 Repository 하나에 몰아넣지는 않는다. 복잡한 통계나 다른 aggregate는 별도 query/service 객체가 더 명확할 수 있다.

## 실습

메모리 DB `":memory:"`를 열어 스키마를 생성한 뒤 insert → findById → update → findAll → remove 순서의 통합 테스트를 작성한다.

## 확인 문제

1. `findById()`가 포인터 대신 `optional`을 반환하는 이점은?
2. Repository가 `Database&`를 보관할 때 지켜야 할 수명 규칙은?
3. 행-객체 변환을 한 함수에 모으는 이유는?

