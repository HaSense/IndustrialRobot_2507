# Chapter 11. Student 클래스

## 학습 목표

- 테이블의 한 행을 표현하는 데이터 모델 클래스를 설계한다.
- 불변식, 생성자, getter/setter의 역할을 이해한다.
- DB의 NULL을 C++ 타입으로 표현한다.

## 1. 모델 설계

`Student`는 SQL 실행을 담당하지 않고 학생 데이터와 그 규칙을 표현한다.

```cpp
// include/Student.h
#pragma once

#include <optional>
#include <string>

class Student {
public:
    Student(int id, std::string name, int age,
            std::optional<std::string> email = std::nullopt);
    Student(std::string name, int age,
            std::optional<std::string> email = std::nullopt);

    int id() const noexcept;
    const std::string& name() const noexcept;
    int age() const noexcept;
    const std::optional<std::string>& email() const noexcept;

    void setName(std::string name);
    void setAge(int age);
    void setEmail(std::optional<std::string> email);

private:
    int id_{0}; // 아직 저장되지 않은 객체
    std::string name_;
    int age_;
    std::optional<std::string> email_;

    static void validateName(const std::string& name);
    static void validateAge(int age);
};
```

`std::optional<std::string>`은 이메일 없음(NULL)과 빈 문자열을 구분한다. ID 0은 아직 DB에 저장되지 않았다는 프로젝트 규칙으로 사용한다.

## 2. 구현

```cpp
// src/Student.cpp
#include "Student.h"
#include <stdexcept>
#include <utility>

Student::Student(int id, std::string name, int age,
                 std::optional<std::string> email)
    : id_(id), name_(std::move(name)), age_(age),
      email_(std::move(email)) {
    validateName(name_);
    validateAge(age_);
}

Student::Student(std::string name, int age,
                 std::optional<std::string> email)
    : Student(0, std::move(name), age, std::move(email)) {}

int Student::id() const noexcept { return id_; }
const std::string& Student::name() const noexcept { return name_; }
int Student::age() const noexcept { return age_; }
const std::optional<std::string>& Student::email() const noexcept {
    return email_;
}

void Student::setName(std::string name) {
    validateName(name);
    name_ = std::move(name);
}
void Student::setAge(int age) {
    validateAge(age);
    age_ = age;
}
void Student::setEmail(std::optional<std::string> email) {
    email_ = std::move(email);
}

void Student::validateName(const std::string& name) {
    if (name.empty()) throw std::invalid_argument("이름은 필수입니다.");
}
void Student::validateAge(int age) {
    if (age < 0) throw std::invalid_argument("나이는 0 이상이어야 합니다.");
}
```

## 3. 책임의 경계

- `Student`: 학생 값과 즉시 검증 가능한 규칙
- `StudentRepository`: SQL과 행-객체 변환
- 서비스/UI: 입력, 출력, 여러 작업을 아우르는 업무 규칙
- 데이터베이스: `NOT NULL`, `UNIQUE`, `CHECK` 등 최종 무결성

모든 setter가 반드시 필요한 것은 아니다. 변경을 제한해야 하는 값은 setter를 제공하지 않거나 의미 있는 메서드로 표현한다.

## 실습

유효한/유효하지 않은 학생 객체를 생성하는 단위 테스트를 작성한다. 이메일 없음과 빈 이메일도 별도 테스트한다.

## 확인 문제

1. DB의 NULL 이메일을 빈 문자열로 바꾸지 않은 이유는?
2. 모델 검증과 DB 제약조건을 함께 두는 이유는?
3. 단순 setter 대신 의미 있는 메서드가 유리한 경우는?

