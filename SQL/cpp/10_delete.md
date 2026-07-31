# Chapter 10. DELETE

## 학습 목표

- 조건을 바인딩하여 특정 행을 삭제한다.
- 물리 삭제와 소프트 삭제의 차이를 설명한다.

## 1. ID로 삭제

```cpp
SQLite::Statement remove(db, "DELETE FROM student WHERE id = ?");
remove.bind(1, 3);

int changed = remove.exec();
std::cout << (changed == 1 ? "삭제 완료" : "대상 없음") << '\n';
```

반환된 행 수를 검사해 사용자가 존재하지 않는 ID를 입력한 경우를 알려 준다.

## 2. 삭제 전 확인

UI 흐름은 다음처럼 구성한다.

1. ID로 학생을 조회한다.
2. 이름과 이메일을 보여 준다.
3. 사용자에게 삭제 여부를 확인한다.
4. 확인한 경우에만 DELETE를 실행한다.

확인과 삭제 사이에 다른 연결이 데이터를 바꿀 수 있다. 높은 일관성이 필요하면 트랜잭션과 조건을 강화한다.

## 3. 참조 무결성

다른 테이블이 학생을 참조한다면 외래 키 동작을 설계해야 한다.

```sql
CREATE TABLE attendance (
    id INTEGER PRIMARY KEY,
    student_id INTEGER NOT NULL,
    attended_on TEXT NOT NULL,
    FOREIGN KEY(student_id) REFERENCES student(id) ON DELETE RESTRICT
);
```

`RESTRICT`는 참조 중인 학생 삭제를 막고, `CASCADE`는 자식 행도 지운다. 편의가 아니라 업무 의미에 따라 선택한다. 연결에서 `PRAGMA foreign_keys = ON`도 필요하다.

## 4. 소프트 삭제

감사나 복구가 필요하면 `deleted_at` 컬럼을 두고 UPDATE한다.

```sql
UPDATE student
SET deleted_at = CURRENT_TIMESTAMP
WHERE id = ? AND deleted_at IS NULL;
```

이후 모든 일반 조회에 `WHERE deleted_at IS NULL` 규칙이 필요하다. 소프트 삭제는 단순 DELETE보다 모델 복잡도가 높다.

## 실습

학생 삭제 메뉴를 만들고 대상이 없을 때와 삭제가 완료됐을 때 메시지를 다르게 출력한다. `attendance` 테이블을 추가해 `RESTRICT` 동작도 확인한다.

## 확인 문제

1. DELETE에 `WHERE`를 빠뜨리면 어떻게 되는가?
2. `ON DELETE CASCADE`를 신중히 선택해야 하는 이유는?
3. 소프트 삭제의 장점과 추가 비용은 무엇인가?

