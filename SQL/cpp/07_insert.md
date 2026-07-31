# Chapter 7. INSERT

## 학습 목표

- `SQLite::Statement`와 `bind()`로 안전하게 데이터를 등록한다.
- 영향받은 행 수와 생성된 ID를 확인한다.

## 1. Prepared statement

SQL 구조와 데이터를 분리하면 따옴표 처리 오류와 SQL 삽입을 예방하고 같은 SQL을 반복 실행하기 쉽다.

```cpp
SQLite::Statement insert(
    db,
    "INSERT INTO student(name, age, email) VALUES (?, ?, ?)"
);

insert.bind(1, "김민수");
insert.bind(2, 20);
insert.bind(3, "minsu@example.com");
int changed = insert.exec();

std::cout << changed << "행 등록\n";
std::cout << "새 ID: " << db.getLastInsertRowid() << '\n';
```

바인딩 번호는 1부터다. 입력값을 작은따옴표로 감싸 SQL 문자열에 결합하지 않는다.

## 2. 이름 있는 매개변수

```cpp
SQLite::Statement insert(db,
    "INSERT INTO student(name, age, email) "
    "VALUES (:name, :age, :email)");

insert.bind(":name", "이서연");
insert.bind(":age", 21);
insert.bind(":email", "seoyeon@example.com");
insert.exec();
```

이름 있는 매개변수는 컬럼이 많을 때 가독성이 좋다. SQL에 쓴 접두사(`:`, `@`, `$`)를 포함한 정확한 이름을 사용한다.

## 3. NULL 입력

```cpp
SQLite::Statement insert(db,
    "INSERT INTO student(name, age, email) VALUES (?, ?, ?)");
insert.bind(1, "박지훈");
insert.bind(2, 19);
insert.bind(3); // NULL
insert.exec();
```

빈 문자열과 `NULL`은 다르다. 값이 없음을 표현할지 빈 값 자체를 표현할지 도메인 규칙을 정한다.

## 4. Statement 재사용

```cpp
for (const auto& name : {"A", "B", "C"}) {
    insert.reset();
    insert.clearBindings();
    insert.bind(1, name);
    insert.bind(2, 20);
    insert.bind(3); // NULL
    insert.exec();
}
```

여러 INSERT는 Chapter 13처럼 트랜잭션으로 묶어야 성능과 원자성이 좋아진다.

## 실습

이름, 나이, 이메일을 입력받아 학생을 등록한다. 이름이 비었거나 나이가 음수이면 실행 전에 거부하고, DB 제약조건 예외도 별도로 출력한다.

## 확인 문제

1. `bind()`가 문자열 결합보다 안전한 이유는?
2. `getLastInsertRowid()`는 언제 읽어야 하는가?
3. statement를 재사용할 때 `reset()`과 `clearBindings()`의 역할은?

