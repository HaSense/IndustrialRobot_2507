# Chapter 3. SQLiteCpp 설치

## 학습 목표

- SQLiteCpp 소스를 내려받아 CMake로 빌드하고 설치한다.
- 헤더와 라이브러리의 위치를 확인한다.
- 시스템 설치와 프로젝트 내 포함 방식의 차이를 이해한다.

## 1. 사전 준비

Ubuntu/Debian 예:

```bash
sudo apt update
sudo apt install build-essential cmake git libsqlite3-dev
g++ --version
cmake --version
```

배포판에 따라 패키지 이름이 다르다. Windows는 Visual Studio의 C++ 워크로드와 CMake, macOS는 Xcode Command Line Tools와 CMake가 필요하다.

## 2. 방법 A: 소스 빌드 후 시스템 설치

```bash
git clone --recurse-submodules https://github.com/SRombauts/SQLiteCpp.git
cmake -S SQLiteCpp -B SQLiteCpp/build \
  -DSQLITECPP_BUILD_TESTS=OFF \
  -DSQLITECPP_BUILD_EXAMPLES=OFF
cmake --build SQLiteCpp/build
sudo cmake --install SQLiteCpp/build
```

관리자 권한 없이 설치하려면 구성 시 `-DCMAKE_INSTALL_PREFIX="$PWD/local"`을 추가하고 프로젝트가 그 prefix를 찾게 한다. 라이브러리 옵션은 버전에 따라 바뀔 수 있으므로 저장소의 `CMakeLists.txt`와 README도 확인한다.

## 3. 방법 B: 프로젝트에 포함

재현 가능한 수업 프로젝트에는 Git submodule과 `add_subdirectory()` 방식이 편리하다.

```bash
mkdir student_app
cd student_app
git init
git submodule add https://github.com/SRombauts/SQLiteCpp.git external/SQLiteCpp
```

```cmake
cmake_minimum_required(VERSION 3.16)
project(student_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/SQLiteCpp)
add_executable(student_app src/main.cpp)
target_link_libraries(student_app PRIVATE SQLiteCpp)
```

이 방식은 전역 설치를 요구하지 않으며 프로젝트가 사용할 커밋을 고정할 수 있다. 저장소를 ZIP으로 복사했다면 라이선스와 업데이트 방식을 함께 관리한다.

## 4. 설치 확인

```cpp
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>

int main() {
    std::cout << SQLite::VERSION << '\n';
}
```

시스템 설치가 CMake package 설정을 제공하면 다음처럼 연결할 수 있다.

```cmake
find_package(SQLiteCpp REQUIRED)
add_executable(check main.cpp)
target_link_libraries(check PRIVATE SQLiteCpp)
```

환경별 target 이름이 다르면 설치된 CMake 설정 또는 공식 예제를 확인한다. 직접 g++로 확인하는 전형적인 명령은 다음과 같지만 설치 경로와 정적/동적 구성에 따라 추가 옵션이 필요할 수 있다.

```bash
g++ -std=c++17 main.cpp -o check -lSQLiteCpp -lsqlite3
./check
```

## 5. 파일 확인과 문제 해결

```bash
find /usr/local/include -path '*SQLiteCpp*' -maxdepth 3
find /usr/local/lib -iname '*sqlitecpp*'
```

- `SQLiteCpp/SQLiteCpp.h: No such file`: include prefix 또는 CMake target 연결 확인
- `cannot find -lSQLiteCpp`: library 경로와 실제 파일명 확인
- `undefined reference to sqlite3_*`: SQLite3 개발 라이브러리 연결 확인
- 실행 시 shared library 오류: 설치 prefix의 런타임 검색 경로 또는 `ldconfig` 확인

## 실습

두 설치 방법 중 하나를 선택해 버전 출력 프로그램을 빌드한다. `cmake -S . -B build`, `cmake --build build`, `./build/check`의 세 단계가 재현되어야 한다.

## 확인 문제

1. `--recurse-submodules`가 필요한 경우는 언제인가?
2. 시스템 설치와 `add_subdirectory()` 방식의 장단점은 무엇인가?
3. 헤더 오류와 링커 오류는 각각 어느 빌드 단계에서 발생하는가?

## 참고

- [SQLiteCpp 공식 저장소](https://github.com/SRombauts/SQLiteCpp)
- [SQLiteCpp 공식 예제 프로젝트](https://github.com/SRombauts/SQLiteCpp_Example)

