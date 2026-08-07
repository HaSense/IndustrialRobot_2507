# 9장 데이터 추가·수정·삭제

테이블 구조만으로는 업무를 처리할 수 없습니다. 이제 행을 추가하고, 조건에 맞는 행을 수정하며, 정말 필요할 때 삭제합니다. 데이터 변경 언어(Data Manipulation Language, DML)는 간단해 보이지만 조건을 빠뜨리면 많은 행을 한 번에 바꿀 수 있습니다. 이 장에서는 결과를 즉시 확인하고 되돌릴 수 있는 습관까지 함께 익힙니다.

> **데이터 기준**  
> 9~12장은 `examples/09_load_sample_data.sql`이 만든 고정 데이터를 사용합니다. 이 초기화 파일은 기존 실습 데이터를 모두 지우므로 개인용 `task_management` 데이터베이스에서만 실행합니다.

## 이 장에서 배울 내용

- 한 행과 여러 행을 `INSERT`하고 생성 결과를 받는다.
- `UPDATE`와 `DELETE`에 안전한 조건을 작성한다.
- `RETURNING`으로 변경된 행을 즉시 확인한다.
- `ON CONFLICT`로 중복 입력을 처리한다.
- `DELETE`와 `TRUNCATE`의 차이를 설명한다.
- 변경 전 조회, 트랜잭션과 행 수 확인으로 대량 실수를 예방한다.

## 선행 지식

- 8장의 7개 테이블이 `task_management`에 생성되어 있어야 합니다.
- `task_admin`으로 접속하고 현재 대상을 확인합니다.

```bash
psql -X -h localhost -U task_admin -d task_management -W
```

```sql
SELECT current_database(), current_user;
```

## 1. 공통 샘플 데이터 준비

> **위험: 기존 데이터 삭제**  
> 다음 파일은 7개 테이블을 `TRUNCATE ... RESTART IDENTITY`한 뒤 고정 샘플을 다시 입력합니다. 보존할 데이터가 있으면 실행하지 마십시오.

저장소 루트의 **Ubuntu Bash**에서 실행합니다.

```bash
psql -X -v ON_ERROR_STOP=1 -h localhost -U task_admin \
  -d task_management -W -f examples/09_load_sample_data.sql
```

마지막 결과의 예상 행 수는 다음과 같습니다.

| 테이블 | 행 수 |
|---|---:|
| `department` | 4 |
| `app_user` | 6 |
| `project` | 3 |
| `project_member` | 8 |
| `task` | 12 |
| `task_comment` | 4 |
| `task_history` | 12 |

스크립트는 자식부터 모든 테이블을 한 `TRUNCATE` 목록에 넣고 Identity를 초기화합니다. 이후 고정 날짜, 가상 이름과 `example.test` 이메일만 입력합니다. 중간 오류가 나면 트랜잭션 전체가 커밋되지 않습니다.

## 2. INSERT로 한 행 추가

열 목록과 값의 순서를 명시합니다.

```sql
INSERT INTO department (department_code, name)
VALUES ('LAB', '기술연구팀');
```

성공 태그는 `INSERT 0 1`입니다. Identity와 기본 시각은 생략했으므로 PostgreSQL이 값을 만듭니다.

모든 열의 물리적 순서에 의존하는 다음 방식은 사용하지 않습니다.

```sql
-- 나쁜 예: 스키마 변경에 취약합니다.
INSERT INTO department
VALUES (100, 'LAB', '기술연구팀', CURRENT_TIMESTAMP);
```

열 목록은 어떤 값을 의도적으로 입력하고 무엇을 기본값에 맡기는지 보여 줍니다.

## 3. RETURNING으로 결과 받기

`INSERT` 뒤 별도 조회를 하지 않고 생성된 키와 기본값을 받을 수 있습니다.

```sql
INSERT INTO department (department_code, name)
VALUES ('DESIGN', '서비스디자인팀')
RETURNING department_id, department_code, name, created_at;
```

`RETURNING *`도 가능하지만 애플리케이션에서는 필요한 열을 명시하는 편이 결과 계약을 안정적으로 유지합니다. `RETURNING`은 `INSERT`, `UPDATE`, `DELETE`에서 모두 사용할 수 있습니다.

## 4. 여러 행 입력

한 문장에 여러 값 묶음을 넣습니다.

```sql
INSERT INTO department (department_code, name) VALUES
    ('SEC', '정보보안팀'),
    ('DATA', '데이터분석팀')
RETURNING department_id, department_code;
```

모든 행이 한 SQL 문장으로 처리됩니다. 한 행이 고유성이나 `CHECK`를 위반하면 문장 전체가 실패합니다.

다른 테이블의 키가 필요하면 `INSERT ... SELECT`를 사용할 수 있습니다.

```sql
INSERT INTO task_comment (task_id, author_id, content)
SELECT t.task_id, u.user_id, '화면 구성을 확인했습니다.'
FROM task t
CROSS JOIN app_user u
WHERE t.title = '화면 설계'
  AND u.email = 'haneul.kim@example.test'
RETURNING comment_id, content;
```

실무에서는 이름이 중복될 수 있으므로 프로젝트 코드나 이메일 같은 고유 조건도 함께 사용합니다.

## 5. UPDATE로 행 수정

`UPDATE`는 조건에 맞는 기존 행의 열 값을 바꿉니다.

```sql
UPDATE task
SET priority = 'urgent',
    updated_at = TIMESTAMPTZ '2026-08-04 15:00:00+09'
WHERE title = '화면 설계'
  AND project_id = (
      SELECT project_id
      FROM project
      WHERE project_code = 'PORTAL'
  )
RETURNING task_id, title, priority, updated_at;
```

예상 변경 행은 1개입니다. 제목은 중복될 수 있으므로 프로젝트 조건까지 사용합니다.

### 5.1 기존 값을 이용한 수정

숫자 열은 기존 값에 계산식을 적용할 수 있습니다. 현재 모델에는 수량 열이 없으므로 임시 테이블로 확인합니다.

```sql
CREATE TEMP TABLE counter_practice (
    counter_id integer PRIMARY KEY,
    value integer NOT NULL
);

INSERT INTO counter_practice VALUES (1, 10);

UPDATE counter_practice
SET value = value + 1
WHERE counter_id = 1
RETURNING value;
```

결과는 `11`입니다.

### 5.2 함께 지켜야 하는 열

`task_completed_time_ck` 때문에 상태만 `done`으로 바꾸면 실패합니다. 완료 시각도 같은 문장에서 설정합니다.

```sql
UPDATE task
SET status = 'done',
    completed_at = TIMESTAMPTZ '2026-09-09 17:00:00+09',
    updated_at = TIMESTAMPTZ '2026-09-09 17:00:00+09'
WHERE title = '기술 검토'
RETURNING title, status, completed_at;
```

## 6. 안전한 UPDATE 절차

조건을 빠뜨린 다음 문장은 모든 업무를 수정합니다.

```sql
-- 위험: 실행하지 않습니다.
UPDATE task SET status = 'cancelled';
```

다음 순서를 습관으로 만듭니다.

1. 같은 `WHERE`로 먼저 `SELECT`한다.
2. 예상 기본 키와 행 수를 확인한다.
3. 중요한 변경은 `BEGIN`으로 시작한다.
4. `UPDATE ... RETURNING`으로 실제 변경 행을 본다.
5. 맞으면 `COMMIT`, 다르면 `ROLLBACK`한다.

```sql
BEGIN;

SELECT task_id, title, priority
FROM task
WHERE status = 'blocked';

UPDATE task
SET priority = 'urgent',
    updated_at = CURRENT_TIMESTAMP
WHERE status = 'blocked'
RETURNING task_id, title, priority;

ROLLBACK;
```

트랜잭션은 18장에서 자세히 배우지만 지금부터 안전 장치로 사용합니다.

## 7. DELETE로 행 삭제

```sql
DELETE FROM task_comment
WHERE content = '화면 구성을 확인했습니다.'
RETURNING comment_id, content;
```

`DELETE`도 `WHERE`가 없으면 모든 행을 대상으로 합니다. 외래 키가 참조하는 행은 `ON DELETE RESTRICT` 때문에 삭제가 거부될 수 있습니다.

```sql
-- 참조 중이라면 오류가 나는 확인용 예제입니다.
DELETE FROM department
WHERE department_code = 'DEV';
```

부서를 지우기보다 사용 여부를 관리하는 별도 상태 정책을 적용해야 합니다. 현재 모델의 사용자는 `is_active`로 비활성화합니다.

## 8. DELETE와 TRUNCATE

| 구분 | `DELETE` | `TRUNCATE` |
|---|---|---|
| 행 선택 | `WHERE` 가능 | 테이블 전체 |
| `RETURNING` | 가능 | 불가 |
| 행별 삭제 트리거 | 실행 | 실행하지 않음 |
| Identity 초기화 | 별도 조치 | `RESTART IDENTITY` 가능 |
| 주 용도 | 업무 행 선택 삭제 | 실습·적재 테이블 전체 초기화 |

`TRUNCATE`는 빠른 전체 초기화 명령이며 강한 잠금을 얻습니다. 공통 데이터 초기화 파일처럼 의도가 명확할 때만 사용합니다.

임시 테이블에서 안전하게 연습합니다.

```sql
CREATE TEMP TABLE truncate_practice (
    practice_id integer GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    note text NOT NULL
);

INSERT INTO truncate_practice (note) VALUES ('첫 행'), ('둘째 행');
TRUNCATE TABLE truncate_practice RESTART IDENTITY;
INSERT INTO truncate_practice (note) VALUES ('초기화 뒤 첫 행');
SELECT * FROM truncate_practice;
```

새 `practice_id`는 1입니다.

## 9. ON CONFLICT로 UPSERT

**UPSERT**는 없으면 입력하고 고유 키가 충돌하면 다른 동작을 수행하는 패턴입니다.

중복을 무시하려면 다음과 같이 씁니다.

```sql
INSERT INTO department (department_code, name)
VALUES ('DEV', '개발팀')
ON CONFLICT (department_code) DO NOTHING;
```

기존 값을 갱신하려면 `EXCLUDED`로 입력하려던 행을 참조합니다.

```sql
INSERT INTO department (department_code, name)
VALUES ('LAB', '디지털연구팀')
ON CONFLICT (department_code) DO UPDATE
SET name = EXCLUDED.name
RETURNING department_id, department_code, name;
```

충돌 대상은 기본 키, `UNIQUE` 제약 또는 이에 대응하는 열이어야 합니다. 모든 오류를 무시하는 기능이 아니며 `CHECK`와 외래 키 위반은 그대로 실패합니다.

## 10. 샘플 데이터 스크립트의 원리

초기화 파일은 다음 순서로 실행됩니다.

```text
BEGIN
  → 모든 테이블 TRUNCATE + Identity 초기화
  → 부모 테이블 입력
  → 연결·자식 테이블 입력
  → 행 수 조회
COMMIT
```

외래 키에는 사람이 기억하기 어려운 숫자를 직접 적지 않고 코드와 이메일을 조회하는 스칼라 서브쿼리를 사용합니다. 서브쿼리는 14장에서 자세히 다룹니다. 현재는 고유한 코드가 대응하는 ID를 찾아 준다고 이해하면 충분합니다.

고정된 날짜를 사용하므로 10~12장의 조회 결과가 실행 날짜에 따라 달라지지 않습니다.

## 11. 따라 하기: 변경 후 되돌리기

공통 데이터를 훼손하지 않는 예제 파일을 실행합니다.

```bash
psql -X -v ON_ERROR_STOP=1 -h localhost -U task_admin \
  -d task_management -W -f examples/09_crud_practice.sql
```

파일은 `BEGIN`으로 시작해 `INSERT`, `UPDATE`, UPSERT, `DELETE`와 임시 테이블 `TRUNCATE`를 수행하고 마지막에 `ROLLBACK`합니다. 따라서 여러 번 실행할 수 있습니다.

롤백 뒤 다음 조회는 0행이어야 합니다.

```sql
SELECT department_code, name
FROM department
WHERE department_code = 'LAB';
```

Identity 시퀀스의 번호 증가는 트랜잭션 롤백으로 되돌아가지 않을 수 있습니다. 따라서 다음 생성 번호에 공백이 생겨도 정상이며 업무 로직이 연속 번호에 의존하면 안 됩니다.

## 12. 원리 이해: 문장 원자성과 트랜잭션

다중 행 `INSERT` 하나에서 마지막 행이 제약조건을 위반하면 앞 행만 저장되지 않습니다. SQL 문장은 하나의 단위로 성공하거나 실패합니다. 여러 SQL 문장을 하나의 업무 단위로 묶으려면 명시적 트랜잭션을 사용합니다.

```text
SQL 문장 하나: 문장 단위 원자성
BEGIN ... COMMIT: 여러 문장을 하나의 업무 단위로 묶음
```

`psql`의 기본 자동 커밋 상태에서는 성공한 각 문장이 즉시 커밋됩니다. 대량 변경 전에 `BEGIN`을 직접 실행해야 `ROLLBACK`할 수 있습니다.

## 13. 주의 및 오류 해결

### `duplicate key value violates unique constraint`

이메일이나 코드가 이미 존재합니다. 기존 행을 조회하고 중복 입력인지 갱신 의도인지 판단합니다. 갱신이라면 충돌 대상이 명확한 `ON CONFLICT`를 사용합니다.

### `violates check constraint task_completed_time_ck`

`done` 상태와 `completed_at`을 함께 설정하지 않았습니다. 두 열을 같은 문장에서 일관되게 바꿉니다.

### `UPDATE 0` 또는 `DELETE 0`

오류가 아니라 조건과 일치하는 행이 없다는 뜻입니다. 조건값, 대소문자와 대상 데이터베이스를 확인합니다. 조건을 무작정 넓히지 않습니다.

### 예상보다 많은 행이 변경되었다

아직 커밋하지 않았다면 즉시 `ROLLBACK`합니다. 이미 자동 커밋됐다면 같은 세션에서 단순히 되돌릴 수 없습니다. 백업 또는 변경 이력으로 복구해야 하므로 변경 전 트랜잭션이 중요합니다.

### `cannot truncate a table referenced in a foreign key constraint`

참조 관계의 테이블을 일부만 비우려 했습니다. 관련 테이블을 같은 `TRUNCATE` 문장에 명시하거나 행 단위 삭제 순서를 설계합니다. 원인을 모른 채 `CASCADE`를 붙이지 않습니다.

## 14. 실습 문제

1. `RETURNING`을 사용해 `DOC` 부서를 추가하고 생성된 키를 확인하세요.
2. `PORTAL` 프로젝트의 `사용자 교육 자료` 업무 담당자를 김하늘로 지정하세요.
3. `blocked` 업무의 우선순위를 `urgent`로 바꾸기 전에 대상 행을 조회하세요.
4. `DOC` 부서 이름을 UPSERT로 `문서관리팀`으로 바꾸세요.
5. 위 변경을 모두 트랜잭션에서 수행하고 되돌리세요.

### 응용 문제

1. `done` 업무를 `in_progress`로 되돌릴 때 함께 바꿔야 할 열을 설명하고 SQL을 작성하세요.
2. `DELETE`와 `TRUNCATE` 중 공통 샘플 데이터 초기화에 적합한 명령과 이유를 설명하세요.

## 15. 실습 문제 정답

1. 다음 문장을 `BEGIN` 뒤에서 실행합니다.

   ```sql
   INSERT INTO department (department_code, name)
   VALUES ('DOC', '문서지원팀')
   RETURNING department_id, department_code;
   ```

2. 고유 이메일로 담당자 ID를 찾습니다.

   ```sql
   UPDATE task
   SET assignee_id = (
           SELECT user_id FROM app_user
           WHERE email = 'haneul.kim@example.test'
       ),
       updated_at = CURRENT_TIMESTAMP
   WHERE title = '사용자 교육 자료'
     AND project_id = (
         SELECT project_id FROM project WHERE project_code = 'PORTAL'
     )
   RETURNING task_id, title, assignee_id;
   ```

3. 변경 전 조회입니다.

   ```sql
   SELECT task_id, title, priority
   FROM task
   WHERE status = 'blocked';
   ```

4. 정답입니다.

   ```sql
   INSERT INTO department (department_code, name)
   VALUES ('DOC', '문서관리팀')
   ON CONFLICT (department_code) DO UPDATE
   SET name = EXCLUDED.name
   RETURNING department_id, department_code, name;
   ```

5. 처음에 `BEGIN;`, 확인 뒤 `ROLLBACK;`을 실행합니다.

응용 1의 정답입니다.

```sql
UPDATE task
SET status = 'in_progress',
    completed_at = NULL,
    updated_at = CURRENT_TIMESTAMP
WHERE title = '요구사항 정리'
RETURNING title, status, completed_at;
```

`done`이 아니면 `completed_at`이 NULL이어야 한다는 제약을 함께 만족시킵니다.

응용 2: 모든 실습 행과 Identity를 한 번에 초기화하므로 `TRUNCATE ... RESTART IDENTITY`가 적합합니다. 부분 삭제, 행별 반환 또는 일반 업무 삭제에는 `DELETE`를 사용합니다.

## 16. 핵심 정리

- `INSERT`에는 열 목록을 명시합니다.
- 여러 행 입력 중 하나가 실패하면 문장 전체가 실패합니다.
- `RETURNING`은 변경된 행의 키와 값을 즉시 돌려줍니다.
- `UPDATE`와 `DELETE` 전에 같은 조건의 `SELECT`로 대상과 행 수를 확인합니다.
- 중요한 변경은 `BEGIN` 안에서 실행하고 검토 후 커밋합니다.
- `DELETE`는 행을 선택하고 `TRUNCATE`는 테이블 전체를 빠르게 초기화합니다.
- UPSERT는 명확한 고유 충돌 대상을 지정합니다.
- Identity 번호의 공백은 정상이며 행 수나 업무 번호로 해석하지 않습니다.

## 17. 확인 문제

1. `INSERT`에서 열 목록을 쓰는 이유는 무엇인가요?
2. `RETURNING`은 어떤 세 DML에서 사용할 수 있나요?
3. 조건 없는 `UPDATE`가 위험한 이유는 무엇인가요?
4. `ON CONFLICT DO UPDATE`의 `EXCLUDED`는 무엇을 뜻하나요?
5. `DELETE`와 `TRUNCATE`의 가장 중요한 차이는 무엇인가요?
6. 롤백 뒤에도 Identity 번호가 비어 있을 수 있는 이유는 무엇인가요?

## 참고한 공식 문서

- [INSERT](https://www.postgresql.org/docs/18/sql-insert.html)
- [UPDATE](https://www.postgresql.org/docs/18/sql-update.html)
- [DELETE](https://www.postgresql.org/docs/18/sql-delete.html)
- [TRUNCATE](https://www.postgresql.org/docs/18/sql-truncate.html)
- [변경 행 반환](https://www.postgresql.org/docs/18/dml-returning.html)

## 18. 다음 장 안내

공통 데이터가 준비되었습니다. 10장에서는 데이터를 바꾸지 않고 필요한 열과 행을 찾습니다. 별칭, 계산식, `DISTINCT`, `WHERE`, NULL 검사, 패턴 검색, 정렬과 조회 건수 제한을 익힙니다.
