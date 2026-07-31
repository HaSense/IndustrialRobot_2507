# Chapter 4. SQLiteCpp 프로젝트 만들기

## 학습 목표

- 최소 SQLiteCpp 프로젝트를 구성한다.
- `Database` 객체로 `student.db`를 생성한다.
- g++ 직접 빌드와 CMake target 기반 빌드를 구분한다.

## 1. 프로젝트 구조

```text
student_app/
├── CMakeLists.txt
├── external/
│   └── SQLiteCpp/
└── src/
    └── main.cpp
```

빌드 산출물은 소스 밖의 `build/`에 둔다. 데이터베이스의 상대 경로는 소스 디렉터리가 아니라 프로그램 실행 당시의 현재 디렉터리를 기준으로 해석된다.

## 2. 첫 프로그램

```cpp
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>

int main() {
    try {
        SQLite::Database db(
            "student.db",
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
        );
        std::cout << "student.db 열기 성공" <<std::endl;
    } catch (const SQLite::Exception& e) {
        std::cerr << "데이터베이스 오류: " << e.what() << std::endl;
        return 1;
    }
}
```

`OPEN_CREATE`는 파일이 없으면 생성하도록 허용한다. 새 파일이 만들어져도 테이블은 자동 생성되지 않는다.

## 3. CMakeLists.txt

프로젝트에 SQLiteCpp 소스가 포함된 경우:

```cmake
cmake_minimum_required(VERSION 3.16)
project(student_app VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(external/SQLiteCpp)
add_executable(student_app src/main.cpp)
target_link_libraries(student_app PRIVATE SQLiteCpp)

if(MSVC)
    target_compile_options(student_app PRIVATE /W4)
else()
    target_compile_options(student_app PRIVATE -Wall -Wextra -Wpedantic)
endif()
```

```bash
cmake -S . -B build
cmake --build build
./build/student_app
```

`target_link_libraries()`는 링크 옵션뿐 아니라 target이 공개한 include 경로도 전달할 수 있어 수동 `-I`보다 안전하다.

## 4. g++ 직접 컴파일

라이브러리가 시스템에 설치되어 있다면:

```bash
g++ -std=c++17 -Wall -Wextra src/main.cpp \
    -o student_app -lSQLiteCpp -lsqlite3
./student_app
```

라이브러리 순서가 중요한 환경에서는 의존하는 대상 뒤에 라이브러리를 둔다. 실제 설치 설정에 따라 `-I`, `-L`, `-ldl`, `-pthread` 등이 필요할 수 있다.

## 5. 파일 생성 확인

```bash
ls -l student.db
sqlite3 student.db ".tables"
```

`.tables`가 비어 있는 것은 정상이다. 다음 장에서 테이블을 생성한다. `student.db`를 특정 위치에 두려면 설정값이나 `std::filesystem::path`로 경로를 명시한다.

## 실습

1. 프로그램을 다른 디렉터리에서 실행하고 DB가 어디에 생성되는지 확인한다.
2. 읽기 전용 플래그로 존재하지 않는 DB를 열어 예외 메시지를 관찰한다.
3. 빌드 경고 옵션을 켜고 경고 없이 컴파일한다.

## 확인 문제

1. `OPEN_READWRITE | OPEN_CREATE`에서 비트 OR를 사용하는 이유는 무엇인가?
2. 소스 내부 빌드보다 out-of-source 빌드가 좋은 이유는 무엇인가?
3. 상대 DB 경로의 기준은 무엇인가?

