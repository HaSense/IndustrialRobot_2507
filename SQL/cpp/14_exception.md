# Chapter 14. 예외 처리

## 학습 목표

- `SQLite::Exception`과 일반 입력 오류를 구분한다.
- 오류를 적절한 계층에서 기록하고 사용자 메시지로 변환한다.
- 예외 안전성과 RAII의 관계를 이해한다.

## 1. 기본 처리

```cpp
int main() {
    try {
        SQLite::Database db("student.db",
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        runApplication(db);
        return 0;
    } catch (const SQLite::Exception& e) {
        std::cerr << "데이터베이스 오류: " << e.what() << '\n';
    } catch (const std::exception& e) {
        std::cerr << "오류: " << e.what() << '\n';
    }
    return 1;
}
```

구체적인 예외를 먼저 잡고 기반 예외를 나중에 잡는다. `catch (...)`로 무조건 삼키면 원인 분석이 어렵다.

## 2. 오류 분류

- 사용자 입력 오류: 숫자 변환 실패, 빈 이름
- 정상적인 미발견: `findById()`의 `nullopt`
- DB 규칙 위반: 중복 이메일, CHECK/NOT NULL 위반
- 운영 오류: 읽기 전용 파일, 디스크 공간, 잠금
- 개발 오류: SQL 문법, 컬럼명 불일치

정상적인 미발견을 예외로 만들 필요는 없다. 반대로 복구할 수 없는 DB 오류를 단순 “등록 실패”로만 출력하면 진단 정보가 부족하다.

## 3. 좁은 catch

메뉴 하나의 실패가 프로그램 전체 종료로 이어질 필요가 없다.

```cpp
void registerStudent(StudentRepository& repo) {
    try {
        Student student = readStudent();
        int id = repo.insert(student);
        std::cout << "등록 ID: " << id << '\n';
    } catch (const std::invalid_argument& e) {
        std::cout << "입력 오류: " << e.what() << '\n';
    } catch (const SQLite::Exception& e) {
        std::cerr << "등록 DB 오류: " << e.what() << '\n';
    }
}
```

처리할 수 없는 예외는 다시 던지거나 최상위에서 종료한다.

## 4. 입력 스트림 오류

```cpp
int readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line))
            throw std::runtime_error("입력이 종료되었습니다.");
        try {
            std::size_t used = 0;
            int value = std::stoi(line, &used);
            if (used == line.size()) return value;
        } catch (const std::exception&) {}
        std::cout << "정수를 입력하세요.\n";
    }
}
```

`operator>>`와 `getline`을 섞을 때 남은 개행 문제가 생기므로 콘솔 앱에서는 한 줄을 읽고 변환하는 방식이 단순하다.

## 5. 로그 보안

오류 로그에 SQL과 바인딩 값을 모두 기록하면 개인정보가 노출될 수 있다. 사용자에게는 이해 가능한 메시지를, 개발 로그에는 필요한 진단 정보만 남긴다. 비밀번호나 민감 데이터는 출력하지 않는다.

## 실습

잘못된 DB 경로, 중복 이메일, 음수 나이, 숫자가 아닌 메뉴 입력을 각각 발생시키고 프로그램이 종료되어야 하는 오류와 메뉴로 돌아가야 하는 오류를 분류한다.

## 확인 문제

1. 예외를 너무 이른 계층에서 잡고 무시하면 어떤 문제가 생기는가?
2. “조회 결과 없음”을 예외와 구분하는 이유는?
3. RAII가 예외 안전성에 기여하는 방식은?

