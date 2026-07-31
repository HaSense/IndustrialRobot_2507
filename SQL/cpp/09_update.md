# Chapter 9. UPDATE

## 학습 목표

- 바인딩된 UPDATE를 실행한다.
- 영향받은 행 수로 성공, 미발견, 동일 값 상황을 해석한다.

## 1. 학생 정보 수정

```cpp
SQLite::Statement update(db,
    "UPDATE student "
    "SET name = ?, age = ?, email = ? "
    "WHERE id = ?");

update.bind(1, "김민준");
update.bind(2, 22);
update.bind(3, "minjun@example.com");
update.bind(4, 1);

int changed = update.exec();
if (changed == 0)
    std::cout << "수정할 학생이 없습니다.\n";
else
    std::cout << "수정 완료\n";
```

`WHERE`를 빠뜨리면 모든 행이 바뀐다. 실행 전에 대상 조건을 같은 `WHERE`의 SELECT로 확인하는 습관이 유용하다.

## 2. 일부 컬럼 수정

고정된 변경 종류마다 명시적 SQL을 두는 것이 안전하다.

```cpp
SQLite::Statement updateAge(db,
    "UPDATE student SET age = :age WHERE id = :id");
updateAge.bind(":age", 23);
updateAge.bind(":id", 1);
updateAge.exec();
```

컬럼명을 사용자 입력에서 직접 이어 붙이지 않는다. 동적 UPDATE가 꼭 필요하면 허용된 필드명을 코드에서 매핑한다.

## 3. 동시 수정 고려

마지막으로 읽은 상태가 유지됐을 때만 수정하는 낙관적 검사를 만들 수 있다.

```sql
UPDATE student
SET name = :new_name, age = :new_age
WHERE id = :id AND name = :old_name AND age = :old_age;
```

영향받은 행이 0이면 삭제되었거나 다른 작업이 먼저 수정했을 수 있다. 더 큰 프로젝트에서는 `updated_at`이나 버전 컬럼을 둔다.

## 실습

ID를 입력받아 현재 정보를 먼저 보여주고 새 이름/나이/이메일로 수정한다. 존재하지 않는 ID, 중복 이메일, 음수 나이의 결과를 구분해 출력한다.

## 확인 문제

1. UPDATE에서 영향받은 행 수가 0인 경우는 무엇인가?
2. 동적 컬럼명을 사용자 문자열로 조립하면 어떤 위험이 있는가?
3. 낙관적 잠금의 기본 아이디어는 무엇인가?

