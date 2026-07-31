# Chapter 8. SELECT

## 학습 목표

- `executeStep()`으로 여러 행을 반복 조회한다.
- `getColumn()`으로 타입과 NULL을 안전하게 읽는다.
- 단건 조회와 다건 조회를 구분한다.

## 1. 전체 조회

```cpp
SQLite::Statement query(db,
    "SELECT id, name, age, email FROM student ORDER BY id");

while (query.executeStep()) {
    int id = query.getColumn(0).getInt();
    std::string name = query.getColumn(1).getString();
    int age = query.getColumn(2).getInt();
    const auto emailColumn = query.getColumn(3);

    std::cout << id << " | " << name << " | " << age << " | "
              << (emailColumn.isNull() ? "(없음)"
                                       : emailColumn.getString())
              << '\n';
}
```

컬럼 인덱스는 0부터다. SQL에서 필요한 컬럼을 명시하고 그 순서와 C++ 읽기 순서를 맞춘다. `SELECT *`는 스키마 변경에 취약하다.

## 2. ID로 한 명 조회

```cpp
SQLite::Statement query(db,
    "SELECT id, name, age, email FROM student WHERE id = ?");
query.bind(1, 1);

if (!query.executeStep()) {
    std::cout << "학생을 찾을 수 없습니다.\n";
} else {
    std::cout << query.getColumn("name").getString() << '\n';
}
```

결과 없음은 DB 오류가 아니라 정상적인 조회 결과다. Repository에서는 이를 `std::optional<Student>`로 표현할 수 있다.

## 3. 조건, 검색, 페이징

```cpp
SQLite::Statement query(db,
    "SELECT id, name, age, email "
    "FROM student WHERE name LIKE ? ESCAPE '\\' "
    "ORDER BY id LIMIT ? OFFSET ?");
query.bind(1, "%민%");
query.bind(2, 10);
query.bind(3, 0);
```

검색어 자체에 `%`와 `_`가 들어올 때 리터럴로 취급하려면 escape 처리가 필요하다. 정렬 컬럼명은 값 바인딩으로 대체할 수 없으므로 허용 목록에서 선택한다.

## 4. 집계

```cpp
int count = db.execAndGet("SELECT COUNT(*) FROM student").getInt();
double average = db.execAndGet(
    "SELECT COALESCE(AVG(age), 0) FROM student").getDouble();
```

빈 집합의 `AVG()`는 `NULL`이므로 `COALESCE`로 기본값을 정했다.

## 실습

전체 학생을 ID순으로 출력하고, 최소/최대 나이 조건과 이름 검색을 추가한다. 이메일이 NULL인 행도 오류 없이 표시한다.

## 확인 문제

1. `executeStep()`이 `false`인 것은 항상 오류인가?
2. 컬럼 이름 조회와 인덱스 조회의 장단점은?
3. 정렬 방향이나 컬럼명을 `bind()`할 수 없는 이유는?

