# 12장 집계와 그룹화

업무 목록을 한 행씩 읽는 것만으로는 전체 상황을 빠르게 파악하기 어렵습니다. **집계 함수(aggregate function)**는 여러 행에서 개수, 합계, 평균, 최솟값과 최댓값 같은 하나의 결과를 계산합니다. `GROUP BY`를 더하면 상태별·프로젝트별 현황을 만들 수 있습니다. 이 장에서는 조인 없이 `task` 한 테이블을 요약하고, 13장에서 이름을 연결할 수 있는 통계의 뼈대를 만듭니다.

## 이 장에서 배울 내용

- `count`, `sum`, `avg`, `min`, `max`의 NULL 처리 방식을 구분한다.
- `GROUP BY`로 행을 그룹별 요약 행으로 바꾼다.
- `WHERE`와 `HAVING`의 필터 시점을 구분한다.
- `FILTER`와 `CASE`로 조건부 집계를 작성한다.
- 여러 열을 그룹화하고 정렬한다.
- `GROUPING SETS`, `ROLLUP`, `CUBE`로 소계와 총계를 만든다.

## 선행 지식

- 9장의 고정 샘플 데이터가 필요합니다.
- 10장의 필터와 정렬, 11장의 함수·`CASE`·NULL 처리를 이해해야 합니다.
- 결과가 다르면 경고를 확인한 뒤 샘플 데이터를 초기화합니다.

```bash
psql -X -v ON_ERROR_STOP=1 -h localhost -U task_admin \
  -d task_management -W -f examples/09_load_sample_data.sql
```

## 1. 전체 행 집계

`GROUP BY` 없이 집계 함수를 사용하면 선택된 모든 행이 하나의 그룹이 됩니다.

```sql
SELECT count(*) AS total_tasks
FROM task;
```

고정 샘플의 결과는 `12`입니다. 결과는 원본 업무 12행이 아니라 요약 1행입니다.

대표 집계 함수는 다음과 같습니다.

| 함수 | 의미 | NULL 처리 |
|---|---|---|
| `count(*)` | 입력 행 수 | NULL과 관계없이 모든 행 |
| `count(expression)` | NULL 아닌 표현식 수 | NULL 제외 |
| `sum(expression)` | 합계 | NULL 제외 |
| `avg(expression)` | 평균 | NULL 제외 |
| `min(expression)` | 최솟값 | NULL 제외 |
| `max(expression)` | 최댓값 | NULL 제외 |

## 2. count(*)와 count(column)

```sql
SELECT count(*) AS total_tasks,
       count(assignee_id) AS assigned_tasks,
       count(*) - count(assignee_id) AS unassigned_tasks
FROM task;
```

예상값은 전체 `12`, 담당자 지정 `10`, 미지정 `2`입니다. `count(assignee_id)`가 10인 이유는 NULL인 담당자 2개를 제외하기 때문입니다.

행 수를 셀 때 `count(1)`이나 `count(task_id)`보다 의도가 분명한 `count(*)`를 사용합니다. PostgreSQL은 `count(*)`를 특별한 전체 행 집계로 처리합니다.

중복을 제거한 값 개수는 `DISTINCT`를 함수 안에 씁니다.

```sql
SELECT count(DISTINCT status) AS status_count
FROM task;
```

결과는 `5`입니다.

## 3. sum, avg, min과 max

현재 업무 모델에는 금액이나 수량 열이 없으므로 날짜 차이를 집계합니다.

```sql
SELECT sum(due_date - start_date) AS total_planned_days,
       round(avg(due_date - start_date), 1) AS avg_planned_days,
       min(due_date) AS first_due_date,
       max(due_date) AS last_due_date
FROM task;
```

시작일 또는 마감일이 NULL인 행의 날짜 차이는 NULL이 되고 `sum`과 `avg`에서 제외됩니다. 평균의 분모는 전체 12행이 아니라 날짜 차이가 NULL이 아닌 행 수입니다.

이를 함께 표시하면 해석 실수를 줄일 수 있습니다.

```sql
SELECT count(*) AS total_count,
       count(due_date - start_date) AS duration_count,
       round(avg(due_date - start_date), 1) AS avg_days
FROM task;
```

집계 대상 행이 하나도 없으면 `count`는 0을 반환하지만 `sum`, `avg`, `min`, `max`는 NULL을 반환합니다.

```sql
SELECT count(*) AS row_count,
       sum(due_date - start_date) AS total_days
FROM task
WHERE status = 'not_a_status';
```

필요할 때 `coalesce(sum(...), 0)`으로 표시할 수 있지만 NULL과 실제 합계 0의 업무 의미가 같은지 먼저 판단합니다.

## 4. GROUP BY

상태값이 같은 업무를 한 그룹으로 묶습니다.

```sql
SELECT status, count(*) AS task_count
FROM task
GROUP BY status
ORDER BY status;
```

고정 샘플 결과는 다음과 같습니다.

| status | task_count |
|---|---:|
| `blocked` | 1 |
| `cancelled` | 1 |
| `done` | 5 |
| `in_progress` | 1 |
| `todo` | 4 |

`SELECT` 목록에는 다음 두 종류만 둘 수 있습니다.

- `GROUP BY`에 포함된 표현식
- 그룹 전체에서 하나의 값을 만드는 집계 표현식

다음 문장은 한 상태 그룹 안에 여러 제목이 있어 어떤 제목을 보여 줄지 정할 수 없으므로 오류입니다.

```sql
-- 잘못된 예
SELECT status, title, count(*)
FROM task
GROUP BY status;
```

## 5. 프로젝트별 집계

```sql
SELECT project_id,
       count(*) AS task_count,
       round(avg(due_date - start_date), 1) AS avg_planned_days
FROM task
GROUP BY project_id
ORDER BY project_id;
```

샘플 초기화 직후 프로젝트 ID별 업무 수는 1번 `6`, 2번 `2`, 3번 `4`입니다. Identity 값 자체에 업무 의미를 부여하지 않습니다. 13장에서는 `project`와 조인해 코드와 이름을 표시합니다.

별칭은 `ORDER BY`에서 사용할 수 있습니다.

```sql
SELECT project_id, count(*) AS task_count
FROM task
GROUP BY project_id
ORDER BY task_count DESC, project_id;
```

## 6. WHERE와 HAVING

`WHERE`는 그룹화 전에 개별 행을 걸러 냅니다. `HAVING`은 그룹화한 뒤 그룹을 걸러 냅니다.

```text
FROM → WHERE → GROUP BY → 집계 → HAVING → SELECT → ORDER BY
```

완료되지 않은 업무만 상태별로 세려면 `WHERE`를 씁니다.

```sql
SELECT status, count(*) AS task_count
FROM task
WHERE status <> 'done'
GROUP BY status
ORDER BY status;
```

업무가 3개 이상인 프로젝트 그룹만 보려면 집계 결과 조건이므로 `HAVING`을 씁니다.

```sql
SELECT project_id, count(*) AS task_count
FROM task
GROUP BY project_id
HAVING count(*) >= 3
ORDER BY project_id;
```

결과는 프로젝트 ID 1과 3입니다. 그룹화 전에 적용할 수 있는 일반 열 조건을 `HAVING`에 넣지 않습니다. `WHERE`가 더 일찍 행을 줄여 의도와 성능이 명확합니다.

두 조건을 함께 사용할 수도 있습니다.

```sql
SELECT project_id, count(*) AS open_task_count
FROM task
WHERE status IN ('todo', 'in_progress', 'blocked')
GROUP BY project_id
HAVING count(*) >= 2
ORDER BY project_id;
```

## 7. 조건부 집계

### 7.1 FILTER

한 행에 여러 조건의 개수를 나란히 표시합니다.

```sql
SELECT count(*) FILTER (WHERE status = 'done') AS done_count,
       count(*) FILTER (
           WHERE status IN ('todo', 'in_progress', 'blocked')
       ) AS open_count,
       count(*) FILTER (WHERE assignee_id IS NULL) AS unassigned_count
FROM task;
```

결과는 완료 `5`, 진행 대상 `6`, 담당자 미정 `2`입니다. `FILTER`는 PostgreSQL에서 집계 함수별 입력 행을 따로 제한합니다.

그룹과 함께 쓰면 프로젝트별 상태 열을 만들 수 있습니다.

```sql
SELECT project_id,
       count(*) AS total_count,
       count(*) FILTER (WHERE status = 'done') AS done_count,
       count(*) FILTER (WHERE status = 'todo') AS todo_count
FROM task
GROUP BY project_id
ORDER BY project_id;
```

### 7.2 CASE 방식

다른 DBMS에서도 널리 사용하는 방식입니다.

```sql
SELECT sum(CASE WHEN status = 'done' THEN 1 ELSE 0 END) AS done_count,
       sum(CASE WHEN assignee_id IS NULL THEN 1 ELSE 0 END) AS unassigned_count
FROM task;
```

단순한 조건부 개수에는 `FILTER`가 조건과 집계를 분리해 읽기 쉽습니다. `CASE`는 조건에 따라 서로 다른 숫자나 계산값을 합칠 때 유용합니다.

## 8. 다중 열 그룹화

상태와 우선순위 조합별 행 수를 셉니다.

```sql
SELECT status, priority, count(*) AS task_count
FROM task
GROUP BY status, priority
ORDER BY status, priority;
```

그룹 키는 두 열의 조합입니다. `done/high`과 `todo/high`는 서로 다른 그룹입니다.

날짜를 함수로 변환한 표현식도 그룹 키가 될 수 있습니다.

```sql
SELECT date_trunc('month', created_at) AS created_month,
       count(*) AS task_count
FROM task
GROUP BY date_trunc('month', created_at)
ORDER BY created_month;
```

시간대가 있는 시각을 월별로 묶을 때 세션 시간대가 경계에 영향을 줄 수 있습니다. 이 책은 `Asia/Seoul` 세션을 기준으로 합니다.

## 9. 문자열과 배열 집계

그룹의 값을 목록으로 모을 수도 있습니다.

```sql
SELECT status,
       string_agg(title, ', ' ORDER BY task_id) AS titles
FROM task
GROUP BY status
ORDER BY status;
```

집계 함수 안의 `ORDER BY`는 목록에 들어가는 값의 순서를 정합니다. 바깥 `ORDER BY`는 결과 그룹 행의 순서를 정합니다.

```sql
SELECT status,
       array_agg(task_id ORDER BY task_id) AS task_ids
FROM task
GROUP BY status
ORDER BY status;
```

목록이 지나치게 커지면 결과 전송과 메모리 사용량이 늘어납니다. 개별 행 조회가 필요한 상황을 억지로 한 문자열에 합치지 않습니다.

## 10. GROUPING SETS

여러 `GROUP BY` 결과를 한 번에 계산합니다.

```sql
SELECT project_id, status, count(*) AS task_count
FROM task
GROUP BY GROUPING SETS (
    (project_id, status),
    (project_id),
    ()
)
ORDER BY project_id NULLS LAST, status NULLS LAST;
```

세 그룹 집합의 의미는 다음과 같습니다.

- `(project_id, status)`: 프로젝트·상태별 상세 집계
- `(project_id)`: 프로젝트별 소계
- `()`: 전체 총계

소계·총계 행에서는 그룹에 포함되지 않은 열이 NULL로 표시됩니다. 원본 데이터의 실제 NULL과 구분하려면 `GROUPING`을 사용합니다.

## 11. ROLLUP과 CUBE

`ROLLUP(a, b)`는 계층적인 상세, `a` 소계와 전체 총계를 만듭니다.

```sql
SELECT project_id,
       status,
       count(*) AS task_count,
       GROUPING(project_id, status) AS grouping_code
FROM task
GROUP BY ROLLUP (project_id, status)
ORDER BY project_id NULLS LAST, status NULLS LAST;
```

`GROUPING(project_id, status)` 결과는 비트 마스크입니다.

| 코드 | 의미 |
|---:|---|
| 0 | 두 열 모두 상세 그룹에 포함 |
| 1 | `status`가 빠진 프로젝트 소계 |
| 3 | 두 열 모두 빠진 전체 총계 |

표시 문구를 만들 수 있습니다.

```sql
SELECT CASE WHEN GROUPING(project_id) = 1
            THEN '전체' ELSE project_id::text END AS project_group,
       CASE WHEN GROUPING(status) = 1
            THEN '소계' ELSE status END AS status_group,
       count(*) AS task_count
FROM task
GROUP BY ROLLUP (project_id, status)
ORDER BY project_id NULLS LAST, status NULLS LAST;
```

`CUBE(a, b)`는 `(a,b)`, `(a)`, `(b)`, `()`의 모든 조합을 만듭니다. 두 축의 소계가 모두 필요한 교차 보고서에 적합합니다.

```sql
SELECT status, priority, count(*) AS task_count
FROM task
GROUP BY CUBE (status, priority)
ORDER BY status NULLS LAST, priority NULLS LAST;
```

그룹 열이 많아지면 조합 수가 빠르게 증가하므로 필요한 소계만 `GROUPING SETS`로 명시하는 편이 낫습니다.

## 12. 통계 쿼리 실습

담당 여부와 상태를 한 줄의 운영 요약으로 만듭니다.

```sql
SELECT count(*) AS total_tasks,
       count(*) FILTER (WHERE status = 'done') AS done_tasks,
       round(
           100.0 * count(*) FILTER (WHERE status = 'done')
           / NULLIF(count(*), 0),
           1
       ) AS completion_rate,
       count(*) FILTER (WHERE status = 'blocked') AS blocked_tasks,
       count(*) FILTER (WHERE assignee_id IS NULL) AS unassigned_tasks
FROM task;
```

완료율은 `41.7`입니다. `100.0`을 사용해 정수 나눗셈을 피하고 `NULLIF(count(*), 0)`으로 빈 집합의 0 나누기를 방지했습니다.

기준일을 사용한 마감 현황입니다.

```sql
SELECT
    count(*) FILTER (
        WHERE due_date < DATE '2026-08-04'
          AND status NOT IN ('done', 'cancelled')
    ) AS overdue_open,
    count(*) FILTER (
        WHERE due_date BETWEEN DATE '2026-08-04' AND DATE '2026-08-11'
          AND status NOT IN ('done', 'cancelled')
    ) AS due_within_7_days,
    count(*) FILTER (WHERE due_date IS NULL) AS no_due_date
FROM task;
```

업무 기준일을 쿼리에 명시했으므로 나중에 실행해도 결과를 재현할 수 있습니다.

## 13. 따라 하기: 집계 조회 모음

```bash
psql -X -v ON_ERROR_STOP=1 -h localhost -U task_admin \
  -d task_management -W -f examples/12_aggregate_grouping.sql
```

파일은 조회만 수행합니다. 주요 확인값은 다음과 같습니다.

- 전체 12, 담당자 지정 10, 미지정 2
- 상태별 합계가 다시 12
- 프로젝트별 업무 수 6, 2, 4
- 업무 3개 이상 프로젝트는 2개
- 완료 5, 진행 대상 6, 담당자 미정 2
- `ROLLUP` 마지막 총계가 12

## 14. 원리 이해: 집계 전후의 행 의미

그룹화 전 한 행은 업무 하나지만 그룹화 후 한 행은 상태나 프로젝트 같은 그룹 하나입니다.

```text
task 원본 12행
  → WHERE로 입력 행 선택
    → GROUP BY로 묶음 생성
      → 집계 함수로 그룹당 1행
        → HAVING으로 그룹 선택
```

이 변화를 이해하면 왜 그룹화되지 않은 `title`을 결과에 바로 표시할 수 없는지 알 수 있습니다. 한 그룹에 제목이 여러 개이기 때문입니다.

PostgreSQL은 일부 기본 키의 함수 종속성을 인식해 엄격한 SQL보다 열 선택을 허용할 수 있지만, 초급 단계에서는 결과 열을 명시적으로 그룹화하거나 집계해 의도를 분명히 합니다.

## 15. 주의 및 오류 해결

### `column ... must appear in the GROUP BY clause`

`SELECT`의 일반 열이 그룹 키도 집계식도 아닙니다. 그 열을 그룹 기준에 추가할지, `min`·`max` 등으로 요약할지, 결과에서 뺄지 결정합니다.

### count 결과가 예상보다 작다

`count(column)`이 NULL을 제외했을 수 있습니다. 전체 행 수라면 `count(*)`를 사용하고 NULL 수는 두 값의 차이나 `FILTER`로 확인합니다.

### avg 결과의 분모가 예상과 다르다

평균 표현식이 NULL인 행은 제외됩니다. `count(*)`와 `count(expression)`을 함께 출력해 실제 입력 수를 확인합니다. NULL을 임의로 0으로 바꾸면 평균의 의미가 달라집니다.

### WHERE에 집계 함수를 썼다

`WHERE count(*) >= 3`은 그룹화 전이라 사용할 수 없습니다. 집계 결과 조건은 `HAVING count(*) >= 3`으로 옮깁니다.

### ROLLUP의 NULL이 실제 NULL인지 소계인지 모르겠다

`GROUPING(column)`을 함께 조회합니다. 값 1은 해당 열이 현재 그룹 집합에서 빠져 소계용 NULL이 생성됐다는 뜻입니다.

### 합계가 NULL이다

입력 행이 없으면 `sum`은 NULL입니다. 0 표시가 업무상 맞으면 `coalesce(sum(expression), 0)`을 사용합니다.

## 16. 실습 문제

1. 전체 프로젝트 수와 종료일이 있는 프로젝트 수를 조회하세요.
2. 우선순위별 업무 수를 많은 순서로 조회하세요.
3. 담당자별 업무 수를 조회하되 NULL 담당자도 하나의 그룹으로 남기세요.
4. 업무가 3개 이상인 상태만 조회하세요.
5. 프로젝트별 전체·완료·담당자 미정 업무 수를 한 행에 표시하세요.
6. 상태·우선순위별 상세과 상태별 소계, 전체 총계를 `ROLLUP`으로 만드세요.

### 응용 문제

1. 프로젝트별 완료율을 소수점 한 자리로 계산하고 업무가 없는 경우의 0 나누기를 방지하세요.
2. `count(*)`, `count(due_date)`, `count(DISTINCT due_date)`의 의미 차이를 설명하세요.

## 17. 실습 문제 정답

```sql
-- 1
SELECT count(*) AS total_projects,
       count(end_date) AS projects_with_end_date
FROM project;

-- 2
SELECT priority, count(*) AS task_count
FROM task
GROUP BY priority
ORDER BY task_count DESC, priority;

-- 3
SELECT assignee_id, count(*) AS task_count
FROM task
GROUP BY assignee_id
ORDER BY assignee_id NULLS LAST;

-- 4
SELECT status, count(*) AS task_count
FROM task
GROUP BY status
HAVING count(*) >= 3
ORDER BY status;

-- 5
SELECT project_id,
       count(*) AS total_count,
       count(*) FILTER (WHERE status = 'done') AS done_count,
       count(*) FILTER (WHERE assignee_id IS NULL) AS unassigned_count
FROM task
GROUP BY project_id
ORDER BY project_id;

-- 6
SELECT status, priority, count(*) AS task_count,
       GROUPING(status, priority) AS grouping_code
FROM task
GROUP BY ROLLUP (status, priority)
ORDER BY status NULLS LAST, priority NULLS LAST;
```

응용 1의 정답입니다.

```sql
SELECT project_id,
       round(
           100.0 * count(*) FILTER (WHERE status = 'done')
           / NULLIF(count(*), 0),
           1
       ) AS completion_rate
FROM task
GROUP BY project_id
ORDER BY project_id;
```

현재 `task`에서 그룹을 만들므로 업무가 0개인 프로젝트는 결과에 나타나지 않습니다. 그런 프로젝트까지 표시하려면 13장의 외부 조인이 필요합니다.

응용 2: `count(*)`는 모든 업무 행, `count(due_date)`는 마감일이 있는 행, `count(DISTINCT due_date)`는 NULL을 제외한 서로 다른 마감일의 수를 셉니다.

## 18. 핵심 정리

- `GROUP BY` 없는 집계는 선택된 모든 행을 하나의 그룹으로 봅니다.
- `count(*)`는 모든 행, `count(column)`은 NULL 아닌 값만 셉니다.
- `sum`, `avg`, `min`, `max`는 NULL 입력을 제외합니다.
- `GROUP BY` 뒤 한 결과 행은 원본 행이 아니라 그룹 하나를 뜻합니다.
- `WHERE`는 그룹화 전 행, `HAVING`은 그룹화 후 그룹을 거릅니다.
- `FILTER`는 집계 함수별 조건을 명확히 분리합니다.
- 여러 열 그룹화는 열 값 조합을 그룹 키로 사용합니다.
- `ROLLUP`은 계층 소계, `CUBE`는 모든 조합의 소계를 만듭니다.
- `GROUPING`으로 실제 NULL과 소계용 NULL을 구분합니다.
- 비율 계산은 숫자 타입과 0 나누기를 함께 확인합니다.

## 19. 확인 문제

1. `count(*)`와 `count(assignee_id)`가 다른 이유는 무엇인가요?
2. 입력 행이 없을 때 `count`와 `sum`은 각각 무엇을 반환하나요?
3. 그룹화된 조회의 일반 열에 적용되는 규칙은 무엇인가요?
4. `WHERE`와 `HAVING`은 언제 적용되나요?
5. `FILTER`와 `CASE` 조건부 집계의 차이는 무엇인가요?
6. `ROLLUP(a, b)`는 어떤 그룹 집합을 만드나요?
7. `GROUPING(a, b)` 값 3은 무엇을 뜻하나요?

## 참고한 공식 문서

- [집계 함수](https://www.postgresql.org/docs/18/functions-aggregate.html)
- [GROUP BY와 HAVING](https://www.postgresql.org/docs/18/queries-table-expressions.html#QUERIES-GROUP)
- [집계 표현식과 FILTER](https://www.postgresql.org/docs/18/sql-expressions.html#SYNTAX-AGGREGATES)
- [GROUPING SETS, CUBE와 ROLLUP](https://www.postgresql.org/docs/18/queries-table-expressions.html#QUERIES-GROUPING-SETS)
- [SELECT 처리 순서](https://www.postgresql.org/docs/18/sql-select.html)

## 20. 다음 장 안내

12장까지는 한 테이블 안에서 데이터를 입력하고 조회·가공·집계했습니다. 13장에서는 `task.project_id`와 `project.project_id`, 담당자 ID와 사용자 ID를 연결해 숫자 키 대신 프로젝트명과 사용자 이름이 포함된 실무형 결과를 만듭니다.
