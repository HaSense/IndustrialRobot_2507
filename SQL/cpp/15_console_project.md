# Chapter 15. 콘솔 CRUD 프로젝트

## 프로젝트 목표

SQLiteCpp를 사용해 등록, 조회, 수정, 삭제가 가능한 학생 관리 프로그램을 완성한다. 모델·저장소·UI의 책임을 분리하고 입력 및 DB 오류 뒤에도 가능한 한 메뉴를 계속 제공한다.

## 1. 완성 구조

```text
student_manager/
├── CMakeLists.txt
├── include/
│   ├── Student.h
│   └── StudentRepository.h
├── src/
│   ├── main.cpp
│   ├── Student.cpp
│   └── StudentRepository.cpp
└── external/SQLiteCpp/
```

## 2. CMake 설정

```cmake
cmake_minimum_required(VERSION 3.16)
project(student_manager LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(external/SQLiteCpp)
add_executable(student_manager
    src/main.cpp src/Student.cpp src/StudentRepository.cpp)
target_include_directories(student_manager PRIVATE include)
target_link_libraries(student_manager PRIVATE SQLiteCpp)
```

## 3. 스키마 초기화

```cpp
void initialize(SQLite::Database& db) {
    db.exec("PRAGMA foreign_keys = ON");
    db.setBusyTimeout(3000);
    db.exec(R"(
        CREATE TABLE IF NOT EXISTS student(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            age INTEGER NOT NULL CHECK(age >= 0),
            email TEXT UNIQUE
        )
    )");
}
```

## 4. 메뉴 루프

```cpp
void printMenu() {
    std::cout << "\n[학생 관리]\n"
              << "1. 등록\n2. 전체 조회\n3. 수정\n"
              << "4. 삭제\n0. 종료\n";
}

void run(StudentRepository& repository) {
    while (true) {
        printMenu();
        int command = readInt("선택: ");
        try {
            switch (command) {
            case 1: registerStudent(repository); break;
            case 2: listStudents(repository); break;
            case 3: editStudent(repository); break;
            case 4: deleteStudent(repository); break;
            case 0: return;
            default: std::cout << "메뉴 번호를 확인하세요.\n";
            }
        } catch (const SQLite::Exception& e) {
            std::cerr << "DB 작업 실패: " << e.what() << '\n';
        } catch (const std::invalid_argument& e) {
            std::cout << "입력 오류: " << e.what() << '\n';
        }
    }
}
```

## 5. 전체 조회 예

```cpp
void listStudents(StudentRepository& repository) {
    const auto students = repository.findAll();
    if (students.empty()) {
        std::cout << "등록된 학생이 없습니다.\n";
        return;
    }
    for (const auto& s : students) {
        std::cout << s.id() << " | " << s.name()
                  << " | " << s.age() << " | "
                  << s.email().value_or("(없음)") << '\n';
    }
}
```

수정은 ID 조회 → 현재 값 표시 → 새 값 입력 → `update()` 순서로, 삭제는 ID 조회 → 확인 → `remove()` 순서로 구현한다.

## 6. main

```cpp
int main() {
    try {
        SQLite::Database db("student.db",
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        initialize(db);
        StudentRepository repository(db);
        run(repository);
        std::cout << "프로그램을 종료합니다.\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "시작 실패: " << e.what() << '\n';
        return 1;
    }
}
```

선언 역순으로 `repository`가 먼저 파괴되고 `db`가 나중에 닫힌다.

## 7. 완료 기준

- SQL 값은 모두 bind한다.
- 없는 ID를 수정/삭제해도 프로그램이 종료되지 않는다.
- NULL 이메일을 올바르게 표시하고 저장한다.
- 음수 나이와 중복 이메일이 거부된다.
- 종료 후 다시 실행해도 데이터가 유지된다.
- 빌드 경고가 없고 DB 오류가 사용자 입력 오류와 구분된다.

## 종합 실습

기본 프로젝트를 완성한 뒤 이름 검색, 페이지 조회, CSV 일괄 등록(트랜잭션), 학생 수/평균 나이 통계 중 두 가지를 추가한다.

## 회고 문제

1. UI에서 SQL을 직접 실행하지 않게 한 이유는?
2. DB 연결, Repository, Statement의 수명 순서는?
3. 이 프로젝트에 자동화 테스트를 추가한다면 어떤 경계를 먼저 검증하겠는가?

