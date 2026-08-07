# 11장 PostgreSQL 함수와 표현식

조회한 값은 그대로 표시할 수도 있지만 화면과 보고서에서는 이름을 정리하고, 날짜를 계산하고, 코드를 읽기 쉬운 문구로 바꿔야 합니다. **함수(function)**는 입력값을 받아 결과값을 만들고, **표현식(expression)**은 열·상수·연산자·함수를 조합해 하나의 값을 계산합니다. 이 장에서는 원본을 변경하지 않고 결과를 가공하는 방법을 배웁니다.

## 이 장에서 배울 내용

- 문자열, 숫자와 날짜·시간 함수를 사용한다.
- `CASE`로 조건에 따른 결과값을 만든다.
- `COALESCE`와 `NULLIF`로 NULL을 안전하게 처리한다.
- 명시적 타입 변환과 PostgreSQL 연산자를 사용한다.
- `AT TIME ZONE`으로 동일한 순간을 다른 지역 시각으로 표시한다.
- 함수 적용 위치와 NULL 전파 원리를 설명한다.

## 선행 지식

- 9장의 공통 샘플 데이터가 필요합니다.
- 10장의 열 선택, `WHERE`, 정렬과 NULL 검사를 이해해야 합니다.
- 데이터가 달라졌다면 경고를 확인하고 초기화 파일을 다시 실행합니다.

```bash
psql -X -v ON_ERROR_STOP=1 -h localhost -U task_admin \
  -d task_management -W -f examples/09_load_sample_data.sql
```

## 1. 함수와 표현식

다음 `SELECT` 목록의 각 항목은 하나의 표현식입니다.

```sql
SELECT title,
       char_length(title) AS title_length,
       due_date - start_date AS planned_days
FROM task;
```

- `title`: 열 참조
- `char_length(title)`: 함수 호출
- `due_date - start_date`: 연산자 표현식

함수 이름 뒤 괄호 안의 입력을 **인수(argument)**라고 합니다. 반환 타입은 함수와 입력 타입에 따라 정해집니다.

```sql
SELECT pg_typeof(char_length('PostgreSQL')),
       pg_typeof(DATE '2026-08-10' - DATE '2026-08-04');
```

두 결과 타입은 `integer`입니다.

## 2. 문자열 함수와 연산자

### 2.1 대소문자와 길이

```sql
SELECT department_code,
       lower(department_code) AS lower_code,
       upper(name) AS upper_name,
       char_length(name) AS characters
FROM department
ORDER BY department_code;
```

`char_length`는 문자 수를 셉니다. 저장 바이트 수가 필요할 때만 `octet_length`를 사용합니다.

### 2.2 공백 제거와 일부 문자열

```sql
SELECT btrim('  PostgreSQL  ') AS trimmed,
       left('PostgreSQL', 4) AS first_four,
       substring('PostgreSQL' FROM 5 FOR 3) AS middle_three;
```

핵심 결과는 `PostgreSQL`, `Post`, `gre`입니다. `btrim`은 양끝 공백을 제거하며 중간 공백은 유지합니다.

### 2.3 문자열 연결

`||` 연산자는 문자열을 연결합니다.

```sql
SELECT department_code || ' - ' || name AS department_label
FROM department
ORDER BY department_code;
```

단, `NULL || '문자열'`의 결과는 NULL입니다. NULL을 건너뛰며 연결하려면 `concat` 또는 `concat_ws`를 사용할 수 있습니다.

```sql
SELECT concat_ws(' / ', title, description) AS task_label
FROM task
ORDER BY task_id;
```

`concat_ws`는 구분자를 첫 인수로 받고 NULL 인수를 무시합니다. 빈 문자열은 NULL과 다르므로 그대로 포함합니다.

### 2.4 문자열 치환

```sql
SELECT replace('업무 관리 시스템', '업무', '작업') AS replaced;
```

결과는 `작업 관리 시스템`입니다. 함수는 조회 결과만 바꾸며 원본 열을 수정하지 않습니다.

## 3. 숫자 함수와 산술 연산자

```sql
SELECT abs(-15) AS absolute_value,
       round(1234.567::numeric, 2) AS rounded,
       ceil(3.2::numeric) AS ceiling,
       floor(3.8::numeric) AS floor_value,
       power(2, 10) AS power_value;
```

예상 핵심값은 `15`, `1234.57`, `4`, `3`, `1024`입니다.

기본 산술 연산자는 `+`, `-`, `*`, `/`, `%`입니다. 정수끼리 나누면 소수 부분이 버려집니다.

```sql
SELECT 5 / 2 AS integer_division,
       5::numeric / 2 AS numeric_division,
       5 % 2 AS remainder;
```

결과는 각각 `2`, `2.5`, `1`입니다. 평균과 비율에서 입력 타입을 확인해야 합니다.

0으로 나누면 오류가 발생합니다. 분모가 0일 수 있다면 뒤에서 배우는 `NULLIF`로 보호할 수 있습니다.

## 4. 날짜와 시간 함수

### 4.1 날짜 계산

```sql
SELECT title,
       due_date,
       due_date - DATE '2026-08-04' AS days_from_base
FROM task
WHERE due_date IS NOT NULL
ORDER BY due_date, task_id;
```

결과가 음수이면 기준일 전에 마감했고 양수이면 기준일 뒤에 마감합니다. 재현 가능한 결과를 위해 `CURRENT_DATE` 대신 고정 기준일을 사용했습니다.

### 4.2 날짜 부분 추출

```sql
SELECT project_code,
       extract(year FROM start_date) AS start_year,
       extract(month FROM start_date) AS start_month
FROM project
ORDER BY project_code;
```

`extract` 결과 타입은 `numeric`입니다. 날짜, 시각과 interval에서 연도·월·요일·시간 등을 꺼낼 수 있습니다.

### 4.3 시각 자르기

```sql
SELECT date_trunc('day',
           TIMESTAMPTZ '2026-08-04 15:34:21+09') AS day_start;
```

`Asia/Seoul` 세션에서 결과는 `2026-08-04 00:00:00+09`입니다. `date_trunc`는 시각을 특정 단위의 시작점으로 맞춰 월별·일별 그룹화에 자주 사용합니다.

### 4.4 현재 시각 함수

`CURRENT_DATE`와 `CURRENT_TIMESTAMP`는 현재 트랜잭션 시작 시각을 기준으로 합니다.

```sql
SELECT CURRENT_DATE, CURRENT_TIMESTAMP;
```

실행할 때마다 달라지는 값이므로 교재의 예상 결과가 필요한 쿼리에는 고정 리터럴을 사용합니다. 기본값과 실제 운영 기록에는 현재 시각 함수가 적합합니다.

## 5. CASE 조건 표현식

단순 `CASE`는 하나의 값을 여러 후보와 비교합니다.

```sql
SELECT title,
       CASE priority
           WHEN 'urgent' THEN '긴급'
           WHEN 'high' THEN '높음'
           WHEN 'medium' THEN '보통'
           WHEN 'low' THEN '낮음'
           ELSE '알 수 없음'
       END AS priority_name
FROM task
ORDER BY task_id;
```

검색 `CASE`는 각 `WHEN`에 독립적인 조건을 씁니다.

```sql
SELECT title, due_date,
       CASE
           WHEN due_date IS NULL THEN '일정 미정'
           WHEN due_date < DATE '2026-08-04' THEN '기준일 이전'
           WHEN due_date = DATE '2026-08-04' THEN '기준일 당일'
           ELSE '기준일 이후'
       END AS due_category
FROM task
ORDER BY task_id;
```

위에서부터 처음 참인 분기의 결과를 사용합니다. 넓은 조건을 먼저 쓰면 뒤의 세부 조건에 도달하지 못할 수 있습니다. 모든 `THEN`과 `ELSE` 결과는 하나의 공통 타입으로 변환 가능해야 합니다.

## 6. COALESCE, NULLIF, GREATEST와 LEAST

### 6.1 첫 번째 NULL 아닌 값

`COALESCE`는 왼쪽부터 첫 번째 NULL이 아닌 값을 반환합니다.

```sql
SELECT title,
       coalesce(description, '설명 없음') AS description,
       coalesce(due_date::text, '미정') AS due_date_text
FROM task
ORDER BY task_id;
```

인수들이 공통 타입으로 변환 가능해야 하므로 `date`를 `text`로 명시 변환했습니다. NULL을 화면용 문구로 바꿔도 원본 열은 여전히 NULL입니다.

### 6.2 특정 값을 NULL로

`NULLIF(a, b)`는 두 값이 같으면 NULL, 다르면 첫 값을 반환합니다.

```sql
SELECT 100::numeric / NULLIF(0, 0) AS safe_division;
```

분모가 NULL이 되어 결과도 NULL이므로 0 나누기 오류를 피합니다. 오류를 숨길지, 0 또는 별도 문구로 표시할지는 업무 의미에 따라 결정합니다.

### 6.3 여러 값의 최댓값과 최솟값

```sql
SELECT greatest(DATE '2026-08-04', DATE '2026-08-10') AS later_date,
       least(10, 20, 5) AS smallest;
```

PostgreSQL의 `GREATEST`와 `LEAST`는 NULL 인수를 무시하고 모든 인수가 NULL일 때 NULL을 반환합니다. 다른 DBMS와 동작이 다를 수 있으므로 이식 시 확인합니다.

## 7. 타입 변환과 포맷팅

표준 `CAST`와 PostgreSQL `::` 표기는 같은 목적에 사용합니다.

```sql
SELECT CAST('2026-08-04' AS date) AS standard_cast,
       '2026-08-04'::date AS short_cast,
       42::text AS number_text;
```

표시 형식이 필요하면 `to_char`를 사용합니다.

```sql
SELECT to_char(
           TIMESTAMPTZ '2026-08-04 15:30:00+09',
           'YYYY-MM-DD HH24:MI'
       ) AS formatted_time;
```

결과는 `2026-08-04 15:30`입니다. `to_char` 결과는 정렬·계산용 시각이 아니라 표시용 `text`입니다. 원본 타입을 유지한 채 애플리케이션 표시 단계에서 형식을 정하는 방법도 고려합니다.

## 8. PostgreSQL 연산자

PostgreSQL은 타입별 연산자를 제공합니다. JSONB에서 `->`는 JSON 값을, `->>`는 텍스트 값을 꺼냅니다.

```sql
SELECT event_type,
       details ->> 'status' AS initial_status
FROM task_history
ORDER BY history_id;
```

JSONB가 특정 구조를 포함하는지는 `@>`로 검사합니다.

```sql
SELECT history_id, details
FROM task_history
WHERE details @> '{"status":"done"}'::jsonb
ORDER BY history_id;
```

배열에는 포함 연산자도 있습니다.

```sql
SELECT ARRAY['sql', 'postgresql']::text[] @> ARRAY['sql']::text[]
       AS contains_sql;
```

연산자도 입력 타입에 따라 의미가 정해진 함수와 비슷합니다. `+`가 숫자에는 덧셈이지만 날짜와 정수 조합에는 날짜 이동이 되는 이유입니다.

## 9. 시간대 처리

같은 `timestamptz` 값을 두 지역의 벽시계 시각으로 표시합니다.

```sql
SELECT TIMESTAMPTZ '2026-08-04 09:00:00 Asia/Seoul'
           AT TIME ZONE 'Asia/Seoul' AS seoul_clock,
       TIMESTAMPTZ '2026-08-04 09:00:00 Asia/Seoul'
           AT TIME ZONE 'UTC' AS utc_clock;
```

결과는 각각 시간대 없는 `timestamp` 값 `2026-08-04 09:00:00`, `2026-08-04 00:00:00`입니다. 동일한 순간을 다른 지역 벽시계로 본 것입니다.

반대로 시간대 없는 `timestamp AT TIME ZONE zone`은 그 벽시계 값이 해당 지역에서 발생했다고 해석해 `timestamptz` 순간을 만듭니다.

```sql
SELECT TIMESTAMP '2026-08-04 09:00:00'
       AT TIME ZONE 'Asia/Seoul' AS instant;
```

입력 타입에 따라 방향이 달라지므로 `pg_typeof`로 확인하면 좋습니다. 고정 숫자 오프셋 `+09`보다 `Asia/Seoul` 같은 IANA 이름이 지역 규칙을 명확히 표현합니다.

## 10. 함수와 NULL 전파

대부분의 일반 함수와 산술 연산자는 입력이 NULL이면 결과도 NULL입니다.

```sql
SELECT upper(NULL::text) AS upper_null,
       10 + NULL::integer AS sum_null;
```

둘 다 NULL입니다. `COALESCE`, `NULLIF`, `CASE`, `concat`처럼 NULL을 특별히 처리하는 표현식과 함수도 있습니다. 함수마다 가정하지 말고 공식 설명이나 간단한 `SELECT`로 확인합니다.

`WHERE`에서 열에 함수를 적용하면 인덱스 사용 방식이 달라질 수 있습니다.

```sql
SELECT user_id, email
FROM app_user
WHERE lower(email) = 'haneul.kim@example.test';
```

현재 데이터는 작아 문제가 없지만 큰 테이블에서는 원본 값을 정규화해 저장하거나 표현식 인덱스를 검토합니다. 17장에서 실행 계획과 함께 다룹니다.

## 11. 따라 하기: 함수와 표현식 모음

```bash
psql -X -v ON_ERROR_STOP=1 -h localhost -U task_admin \
  -d task_management -W -f examples/11_functions_expressions.sql
```

파일은 조회만 수행합니다. 다음을 확인합니다.

- 문자열 함수가 원본을 바꾸지 않고 새 결과 열을 만든다.
- 날짜 차이는 정수 일수로 나타난다.
- `CASE`가 네 우선순위를 한국어 문구로 바꾼다.
- 설명과 마감일 NULL이 표시용 기본 문구로 바뀐다.
- JSONB 이력에서 초기 상태 문자열을 꺼낸다.
- 서울 오전 9시와 UTC 오전 0시가 같은 순간이다.

## 12. 원리 이해: 서버 계산과 표시 책임

SQL 함수로 계산하면 필터·정렬·그룹화에 같은 규칙을 적용할 수 있습니다. 반면 언어와 화면별 날짜·숫자 형식은 애플리케이션이 맡는 편이 유연할 수 있습니다.

```text
데이터 의미와 검색 조건 → SQL 타입·표현식
사용자 언어와 화면 형식 → 애플리케이션 표시 계층
```

상태 코드의 의미를 `CASE`로 빠르게 표시할 수 있지만 여러 화면에서 반복된다면 참조 테이블, 뷰 또는 애플리케이션 번역 체계를 검토합니다. 같은 복잡한 표현식을 곳곳에 복사하지 않습니다.

## 13. 주의 및 오류 해결

### `function ... does not exist`

함수 이름보다 인수 타입이 맞지 않는 경우가 많습니다. `pg_typeof`로 확인하고 필요한 인수를 명시 변환합니다. 오류 메시지가 제안하는 변환을 의미 검토 없이 붙이지 않습니다.

### `CASE types ... cannot be matched`

`THEN`과 `ELSE` 결과가 공통 타입으로 변환되지 않습니다. 숫자와 표시 문자열을 섞지 말고 결과 목적에 맞는 하나의 타입으로 통일합니다.

### `division by zero`

분모가 0인지 먼저 제한하거나 `NULLIF(분모, 0)`을 사용합니다. NULL 결과를 0으로 바꾸는 것이 업무상 맞는지는 별도로 판단합니다.

### 날짜 표시가 예상과 다르다

입력 타입과 세션 시간대를 확인합니다.

```sql
SELECT pg_typeof(created_at) FROM task LIMIT 1;
SHOW TimeZone;
```

`timestamptz`를 `AT TIME ZONE`으로 바꾸면 결과가 시간대 없는 `timestamp`라는 점도 확인합니다.

### 별칭을 WHERE에서 사용할 수 없다

`WHERE`가 `SELECT` 목록보다 논리적으로 먼저 처리됩니다. 표현식을 반복하거나 이후 장의 서브쿼리·CTE로 단계를 나눕니다.

## 14. 실습 문제

1. 모든 사용자 이름의 문자 수를 조회하세요.
2. 프로젝트 이름과 시작 연도·월을 조회하세요.
3. 업무 상태를 `대기`, `진행`, `차단`, `완료`, `취소`로 표시하세요.
4. 마감일이 NULL이면 `미정`, 아니면 ISO 문자열로 표시하세요.
5. `task_history.details`에서 `status`를 텍스트로 꺼내세요.
6. 서울 `2026-12-01 18:00`을 UTC 벽시계 시각으로 표시하세요.

### 응용 문제

1. 기준일 `2026-08-04`와 마감일 차이에 따라 `기한 지남`, `7일 이내`, `여유 있음`, `미정`으로 분류하세요.
2. 정수 `5 / 2`와 정확한 십진 나눗셈 결과가 다른 이유를 설명하세요.

## 15. 실습 문제 정답

```sql
-- 1
SELECT name, char_length(name) AS name_length
FROM app_user ORDER BY user_id;

-- 2
SELECT name,
       extract(year FROM start_date) AS start_year,
       extract(month FROM start_date) AS start_month
FROM project ORDER BY project_code;

-- 3
SELECT title,
       CASE status
           WHEN 'todo' THEN '대기'
           WHEN 'in_progress' THEN '진행'
           WHEN 'blocked' THEN '차단'
           WHEN 'done' THEN '완료'
           WHEN 'cancelled' THEN '취소'
           ELSE '알 수 없음'
       END AS status_name
FROM task ORDER BY task_id;

-- 4
SELECT title, coalesce(due_date::text, '미정') AS due_date_text
FROM task ORDER BY task_id;

-- 5
SELECT history_id, details ->> 'status' AS status
FROM task_history ORDER BY history_id;

-- 6
SELECT TIMESTAMPTZ '2026-12-01 18:00:00 Asia/Seoul'
       AT TIME ZONE 'UTC' AS utc_clock;
```

응용 1의 정답입니다.

```sql
SELECT title, due_date,
       CASE
           WHEN due_date IS NULL THEN '미정'
           WHEN due_date < DATE '2026-08-04' THEN '기한 지남'
           WHEN due_date <= DATE '2026-08-11' THEN '7일 이내'
           ELSE '여유 있음'
       END AS due_status
FROM task
ORDER BY task_id;
```

응용 2: 두 정수의 `/`는 정수 나눗셈이어서 소수 부분을 버립니다. 한쪽을 `numeric`으로 변환하면 정확한 십진 결과 `2.5`를 얻습니다.

## 16. 핵심 정리

- 표현식은 열, 상수, 연산자와 함수를 조합해 값을 만듭니다.
- `char_length`는 문자 수, `octet_length`는 바이트 수를 반환합니다.
- 정수 나눗셈은 소수 부분을 버리므로 비율의 입력 타입을 확인합니다.
- `CASE`는 위에서부터 처음 참인 결과를 사용합니다.
- `COALESCE`는 첫 NULL 아닌 값, `NULLIF`는 같은 두 값을 NULL로 바꿉니다.
- `to_char` 결과는 계산용 날짜가 아니라 표시용 문자열입니다.
- JSONB의 `->>`는 값을 텍스트로 꺼냅니다.
- `timestamptz AT TIME ZONE`은 특정 지역의 시간대 없는 벽시계 시각을 만듭니다.
- 대부분의 함수와 연산자는 NULL을 결과로 전파합니다.

## 17. 확인 문제

1. 함수와 표현식은 어떤 관계인가요?
2. `||`와 `concat_ws`는 NULL을 어떻게 다르게 처리하나요?
3. `5 / 2`와 `5::numeric / 2`의 결과는 무엇인가요?
4. 검색 `CASE`에서 조건 순서가 중요한 이유는 무엇인가요?
5. `COALESCE(due_date, '미정')`가 타입 오류가 되는 이유는 무엇인가요?
6. `AT TIME ZONE`의 입력이 `timestamp`인지 `timestamptz`인지 확인해야 하는 이유는 무엇인가요?

## 참고한 공식 문서

- [문자열 함수와 연산자](https://www.postgresql.org/docs/18/functions-string.html)
- [수학 함수와 연산자](https://www.postgresql.org/docs/18/functions-math.html)
- [날짜·시간 함수와 연산자](https://www.postgresql.org/docs/18/functions-datetime.html)
- [조건 표현식](https://www.postgresql.org/docs/18/functions-conditional.html)
- [데이터 타입 포맷 함수](https://www.postgresql.org/docs/18/functions-formatting.html)
- [JSON 함수와 연산자](https://www.postgresql.org/docs/18/functions-json.html)

## 18. 다음 장 안내

한 행의 값을 가공하는 방법을 익혔습니다. 12장에서는 여러 행을 한 그룹으로 모아 개수, 합계와 평균을 계산합니다. `GROUP BY`, `HAVING`, 조건부 집계와 `ROLLUP`으로 업무 현황표를 만듭니다.
