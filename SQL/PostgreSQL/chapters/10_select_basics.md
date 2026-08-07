# 10장 SELECT 기본

데이터를 저장한 목적은 필요한 순간에 정확히 찾아 쓰기 위해서입니다. `SELECT`는 원본 행을 바꾸지 않고 원하는 열을 선택하고, 조건으로 행을 거르고, 결과 순서를 정합니다. 이 장에서는 한 테이블을 확실하게 조회하는 방법에 집중합니다. 여러 테이블을 연결하는 조인은 13장에서 배웁니다.

## 이 장에서 배울 내용

- 필요한 열을 선택하고 별칭과 계산식을 만든다.
- `DISTINCT`로 중복 결과를 제거한다.
- 비교·논리 연산자로 조회 조건을 조합한다.
- 범위, 목록과 NULL을 올바르게 검사한다.
- `LIKE`와 `ILIKE`로 문자열 패턴을 검색한다.
- 안정적인 `ORDER BY`와 `LIMIT`, `OFFSET`을 사용한다.

## 선행 지식

- 8장의 공통 스키마와 9장의 샘플 데이터가 필요합니다.
- 샘플 상태가 다르면 9장 초기화 파일을 다시 실행합니다. 이 파일은 기존 데이터를 삭제한다는 경고를 먼저 확인하십시오.

```bash
psql -X -v ON_ERROR_STOP=1 -h localhost -U task_admin \
  -d task_management -W -f examples/09_load_sample_data.sql
```

## 1. SELECT의 기본 구조

```sql
SELECT task_id, title, status, priority
FROM task;
```

- `SELECT`: 결과에 표시할 열 또는 표현식
- `FROM`: 행을 읽을 원본 테이블

`SELECT *`는 탐색할 때 편리하지만 열 추가에 따라 결과 구조와 데이터 전송량이 달라집니다. 교재의 완성된 조회문에서는 필요한 열을 명시합니다.

```sql
SELECT * FROM task;
```

SQL 결과의 행 순서는 `ORDER BY`가 없으면 보장되지 않습니다. 화면에서 현재 순서가 반복되어도 저장 순서라고 가정하지 않습니다.

## 2. 별칭과 계산식

**별칭(alias)**은 결과 열에 읽기 쉬운 이름을 붙입니다.

```sql
SELECT title AS task_title,
       due_date - start_date AS planned_days
FROM task
WHERE start_date IS NOT NULL
  AND due_date IS NOT NULL;
```

`date - date` 결과는 날짜 간 일수입니다. 계산식은 조회 결과에만 나타나며 테이블 원본을 수정하지 않습니다.

공백이나 대문자가 포함된 별칭은 큰따옴표가 필요하지만 프로그램에서 다루기 불편하므로 `snake_case`를 기본으로 사용합니다.

```sql
SELECT title AS "업무 제목" FROM task;
```

문자열 값은 작은따옴표, 식별자와 표시용 별칭은 큰따옴표라는 차이를 기억합니다.

## 3. DISTINCT

업무에 실제로 사용된 상태만 한 번씩 봅니다.

```sql
SELECT DISTINCT status
FROM task
ORDER BY status;
```

고정 샘플에서는 `blocked`, `cancelled`, `done`, `in_progress`, `todo`의 5행입니다.

여러 열을 선택하면 열 조합 전체의 중복을 제거합니다.

```sql
SELECT DISTINCT status, priority
FROM task
ORDER BY status, priority;
```

`DISTINCT`는 결과 중복을 숨기는 도구이지 잘못된 테이블 중복을 고치는 도구가 아닙니다. 이메일과 코드 중복은 8장의 `UNIQUE`로 막습니다.

## 4. WHERE와 비교 연산자

`WHERE`는 조건 결과가 참인 행만 통과시킵니다.

```sql
SELECT task_id, title, status
FROM task
WHERE status = 'blocked';
```

대표 비교 연산자는 `=`, `<>`, `<`, `<=`, `>`, `>=`입니다.

```sql
SELECT project_code, name, start_date
FROM project
WHERE start_date >= DATE '2026-07-01';
```

날짜는 7장의 명시적 타입 리터럴을 사용합니다. 숫자처럼 보이게 날짜의 따옴표를 제거하지 않습니다.

## 5. 논리 연산자

- `AND`: 모든 조건이 참
- `OR`: 하나 이상 참
- `NOT`: 조건 결과를 반대로

```sql
SELECT title, status, priority
FROM task
WHERE status = 'todo'
  AND priority = 'high';
```

`AND`가 `OR`보다 먼저 계산됩니다. 의도가 섞이면 괄호로 명확히 합니다.

```sql
SELECT title, status, priority
FROM task
WHERE status = 'blocked'
   OR (status = 'todo' AND priority = 'high');
```

괄호가 없으면 독자가 우선순위를 기억해야 하므로 복합 조건에는 괄호를 권장합니다.

## 6. BETWEEN과 IN

`BETWEEN`은 양쪽 끝값을 모두 포함합니다.

```sql
SELECT title, due_date
FROM task
WHERE due_date BETWEEN DATE '2026-08-01' AND DATE '2026-08-31'
ORDER BY due_date;
```

고정 샘플에서는 `화면 설계`, `검색 API 구현`, `통합 테스트 작성` 3행입니다.

여러 후보 중 하나인지 검사할 때 `IN`을 사용합니다.

```sql
SELECT title, status
FROM task
WHERE status IN ('todo', 'in_progress')
ORDER BY status, task_id;
```

다음 두 조건은 같은 의미지만 `IN`이 허용 목록을 읽기 쉽습니다.

```sql
WHERE status = 'todo' OR status = 'in_progress'
```

`NOT BETWEEN`, `NOT IN`도 사용할 수 있습니다. NULL이 섞인 `NOT IN`은 14장의 서브쿼리에서 특히 주의합니다.

## 7. NULL 검사

NULL은 일반 값이 아니므로 `= NULL`로 비교하지 않습니다.

```sql
-- 잘못된 예: 결과가 나오지 않습니다.
SELECT title FROM task WHERE assignee_id = NULL;
```

`IS NULL` 또는 `IS NOT NULL`을 사용합니다.

```sql
SELECT task_id, title
FROM task
WHERE assignee_id IS NULL
ORDER BY task_id;
```

고정 샘플에서는 `사용자 교육 자료`, `로그인 화면 시안` 2행입니다.

SQL 조건은 참·거짓뿐 아니라 알 수 없음(UNKNOWN)을 포함하는 3값 논리를 사용합니다. `WHERE`는 참만 통과시키므로 NULL과의 일반 비교 결과는 제외됩니다.

## 8. 문자열 패턴 검색

`LIKE`의 와일드카드는 다음과 같습니다.

| 기호 | 의미 |
|---|---|
| `%` | 문자 0개 이상 |
| `_` | 정확히 문자 1개 |

```sql
SELECT task_id, title
FROM task
WHERE title LIKE '%설계%';
```

`LIKE`는 대소문자를 구분합니다. PostgreSQL의 `ILIKE`는 현재 로캘 규칙에 따라 대소문자를 구분하지 않는 패턴 비교를 제공합니다.

```sql
SELECT project_code, name
FROM project
WHERE project_code ILIKE 'port%';
```

문자 자체의 `%`와 `_`를 검색하려면 이스케이프 문자를 정합니다.

```sql
SELECT '진행률_100%' LIKE '%!_100!%%' ESCAPE '!' AS matched;
```

결과는 `t`입니다. 사용자 입력을 그대로 패턴에 이어 붙이는 문제는 29장의 매개변수화된 SQL에서 다룹니다.

## 9. ORDER BY

```sql
SELECT task_id, title, priority, due_date
FROM task
ORDER BY priority ASC, due_date ASC NULLS LAST, task_id ASC;
```

왼쪽 정렬 키부터 비교하고 같은 값이면 다음 키를 사용합니다.

- `ASC`: 오름차순, 기본값
- `DESC`: 내림차순
- `NULLS FIRST`: NULL을 먼저
- `NULLS LAST`: NULL을 나중에

PostgreSQL 기본 정렬에서 NULL 위치는 방향에 따라 달라질 수 있으므로 보고서 의도가 중요하면 명시합니다. 마지막에 고유한 `task_id`를 넣으면 같은 우선순위와 날짜에서도 결과 순서가 안정적입니다.

문자열 정렬은 데이터베이스의 콜레이션(collation)에 영향을 받습니다. 한글·영문 정렬 결과가 운영체제와 로캘에 따라 달라질 수 있으므로 예제의 고정 순서는 코드나 ID도 함께 사용합니다.

## 10. LIMIT과 OFFSET

상위 5행만 조회합니다.

```sql
SELECT task_id, title, due_date
FROM task
ORDER BY due_date ASC NULLS LAST, task_id
LIMIT 5;
```

앞의 5행을 건너뛰고 다음 5행을 조회합니다.

```sql
SELECT task_id, title, due_date
FROM task
ORDER BY due_date ASC NULLS LAST, task_id
LIMIT 5 OFFSET 5;
```

`LIMIT`만 쓰고 `ORDER BY`를 생략하면 어떤 행이 선택될지 보장되지 않습니다. 페이지 조회에서는 모든 페이지에 동일하고 고유한 정렬 기준을 적용합니다.

큰 `OFFSET`은 앞 행도 찾아 건너뛰어야 하므로 깊은 페이지에서 느려질 수 있습니다. 인덱스와 키 기반 페이지 이동은 17장 이후에 다룹니다.

## 11. SQL의 논리적 처리 흐름

작성 순서와 개념적인 처리 순서는 다릅니다.

```text
작성: SELECT → FROM → WHERE → ORDER BY → LIMIT
개념: FROM → WHERE → SELECT → DISTINCT → ORDER BY → LIMIT
```

이 때문에 같은 조회 단계의 별칭을 `WHERE`에서 바로 사용할 수 없습니다.

```sql
-- 잘못된 예
SELECT due_date - start_date AS planned_days
FROM task
WHERE planned_days >= 10;
```

`WHERE`에는 원래 표현식을 반복합니다.

```sql
SELECT title, due_date - start_date AS planned_days
FROM task
WHERE due_date - start_date >= 10;
```

## 12. 따라 하기: 기본 조회 모음

```bash
psql -X -v ON_ERROR_STOP=1 -h localhost -U task_admin \
  -d task_management -W -f examples/10_select_basics.sql
```

이 파일은 조회만 수행하므로 반복 실행해도 데이터가 변하지 않습니다. 주요 확인값은 다음과 같습니다.

- 전체 업무 12행
- 상태 종류 5행
- 2026년 8월 마감 업무 3행
- 담당자 미정 업무 2행
- 제목에 `설계`가 포함된 업무 1행

## 13. 원리 이해: 결과는 새로운 관계다

`SELECT` 결과도 열과 행으로 된 임시 관계로 생각할 수 있습니다. 원본 열을 고르고 계산 열을 만들며 조건으로 행을 줄입니다.

```text
task 12행
  → WHERE로 3행 선택
    → SELECT로 3개 열 투영
      → ORDER BY로 표시 순서 결정
        → LIMIT으로 일부 반환
```

조회는 원본을 바꾸지 않지만 함수 호출, 잠금 조회와 매우 큰 결과는 서버 자원을 사용할 수 있습니다. 필요한 열과 행만 요청하는 습관이 중요합니다.

## 14. 주의 및 오류 해결

### `column ... does not exist`

열 이름의 철자나 별칭 사용 위치를 확인합니다. `\d task`로 실제 열을 봅니다. 작은따옴표로 열 이름을 감싸면 문자열 값이 되므로 구분합니다.

### `operator does not exist`

서로 호환되지 않는 타입을 비교했습니다. `pg_typeof`와 `\d`로 타입을 확인하고 7장의 명시적 변환을 사용합니다.

### NULL 행이 조건에서 빠진다

`= NULL`, `<> NULL`을 쓰지 않았는지 확인합니다. NULL 포함 여부를 `IS NULL` 또는 별도 `OR` 조건으로 명시합니다.

### LIMIT 결과가 실행할 때마다 달라진다

`ORDER BY`가 없거나 동률을 해소할 고유 열이 없습니다. 마지막 정렬 키에 기본 키를 추가합니다.

### LIKE가 너무 많은 행을 찾는다

앞뒤 `%`의 범위를 확인합니다. `%단어%`는 어디에든 포함된 값을 찾습니다. 패턴의 `%`와 `_`가 실제 문자인지도 확인합니다.

## 15. 실습 문제

1. `project`에서 코드, 이름과 상태만 조회하세요.
2. 업무 상태 종류를 중복 없이 정렬하세요.
3. 상태가 `todo`이고 우선순위가 `high`인 업무를 찾으세요.
4. 마감일이 없는 업무를 찾으세요.
5. 제목에 `검`이 포함된 업무를 찾으세요.
6. 생성 시각이 최신인 업무 3개를 안정적으로 조회하세요.

### 응용 문제

1. 시작일과 마감일이 모두 있는 업무 중 계획 기간이 10일 이상인 업무를 긴 순서로 조회하세요.
2. `done`과 `cancelled`가 아닌 업무 중 담당자가 정해진 행만 조회하세요.

## 16. 실습 문제 정답

```sql
-- 1
SELECT project_code, name, status FROM project;

-- 2
SELECT DISTINCT status FROM task ORDER BY status;

-- 3
SELECT task_id, title
FROM task
WHERE status = 'todo' AND priority = 'high';

-- 4
SELECT task_id, title
FROM task
WHERE due_date IS NULL;

-- 5
SELECT task_id, title
FROM task
WHERE title LIKE '%검%';

-- 6
SELECT task_id, title, created_at
FROM task
ORDER BY created_at DESC, task_id DESC
LIMIT 3;
```

응용 문제 정답입니다.

```sql
-- 응용 1
SELECT task_id, title, due_date - start_date AS planned_days
FROM task
WHERE start_date IS NOT NULL
  AND due_date IS NOT NULL
  AND due_date - start_date >= 10
ORDER BY planned_days DESC, task_id;

-- 응용 2
SELECT task_id, title, status, assignee_id
FROM task
WHERE status NOT IN ('done', 'cancelled')
  AND assignee_id IS NOT NULL
ORDER BY task_id;
```

## 17. 핵심 정리

- 완성된 조회문은 필요한 열을 명시합니다.
- `ORDER BY`가 없으면 행 순서는 보장되지 않습니다.
- 별칭과 계산식은 원본 데이터를 바꾸지 않습니다.
- `DISTINCT`는 선택한 열 조합의 중복을 제거합니다.
- 복합 논리 조건은 괄호로 의도를 명확히 합니다.
- `BETWEEN`은 양 끝을 포함하고 `IN`은 값 목록을 검사합니다.
- NULL은 `IS NULL`과 `IS NOT NULL`로 검사합니다.
- `LIKE`는 패턴 검색, `ILIKE`는 PostgreSQL의 대소문자 비구분 패턴 검색입니다.
- `LIMIT`에는 동률까지 해소하는 안정적인 정렬을 사용합니다.

## 18. 확인 문제

1. `SELECT *`를 애플리케이션 조회에 항상 사용하지 않는 이유는 무엇인가요?
2. `DISTINCT status, priority`는 무엇을 중복으로 판단하나요?
3. `AND`와 `OR`를 함께 쓸 때 괄호가 중요한 이유는 무엇인가요?
4. `BETWEEN`은 끝값을 포함하나요?
5. `assignee_id = NULL`이 올바르지 않은 이유는 무엇인가요?
6. `LIMIT`과 함께 기본 키까지 정렬하는 이유는 무엇인가요?

## 참고한 공식 문서

- [SELECT](https://www.postgresql.org/docs/18/sql-select.html)
- [SELECT 목록](https://www.postgresql.org/docs/18/queries-select-lists.html)
- [행 정렬](https://www.postgresql.org/docs/18/queries-order.html)
- [비교 함수와 연산자](https://www.postgresql.org/docs/18/functions-comparison.html)
- [문자열 패턴 검색](https://www.postgresql.org/docs/18/functions-matching.html)

## 19. 다음 장 안내

10장에서는 원본 값을 선택하고 거르는 방법을 익혔습니다. 11장에서는 문자열, 숫자와 날짜·시간 함수, `CASE`, NULL 처리, 타입 변환과 PostgreSQL 연산자로 결과값을 가공합니다.
