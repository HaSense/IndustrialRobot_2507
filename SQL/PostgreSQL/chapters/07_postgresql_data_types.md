# 7장 PostgreSQL 데이터 타입

데이터 타입(data type)은 열에 저장할 값의 종류와 가능한 연산을 정합니다. 숫자를 모두 문자열로 저장하면 합계와 크기 비교가 어려워지고, 시각을 시간대 없는 값으로 저장하면 서로 다른 지역의 사용자가 같은 순간을 다르게 해석할 수 있습니다. 이 장에서는 업무 관리 시스템의 각 열에 맞는 PostgreSQL 타입을 선택하고 8장에서 사용할 타입 정책을 확정합니다.

> **버전 기준**  
> 이 장은 PostgreSQL 18 공식 데이터 타입 문서를 2026년 8월 4일 기준으로 확인했습니다. 예제는 `task_management` 데이터베이스의 기본 시간대가 `Asia/Seoul`인 상태를 전제로 합니다.

## 이 장에서 배울 내용

- 정확한 수와 근삿값에 맞는 숫자 타입을 선택한다.
- 문자열 길이 제한과 `text`, `varchar`, `char`의 차이를 설명한다.
- `date`, `timestamp`, `timestamptz`를 목적에 맞게 구분한다.
- `boolean`, UUID, JSONB, 배열과 열거형의 쓰임을 판단한다.
- 명시적 타입 변환을 사용하고 변환 오류를 해석한다.
- 업무 관리 시스템의 열별 타입 정책을 확정한다.

## 선행 지식

- 5장에서 `task_management` 데이터베이스와 `task_admin` 역할을 만들었습니다.
- 6장에서 7개 테이블, 열, 키와 NULL 허용 여부를 설계했습니다.
- 아직 영구 테이블은 만들지 않았으므로 이 장의 예제는 값과 타입만 확인합니다.

**Ubuntu Bash**에서 접속합니다.

```bash
psql -X -h localhost -U task_admin -d task_management -W
```

현재 데이터베이스와 시간대를 확인합니다.

```sql
SELECT current_database(), current_user;
SHOW TimeZone;
```

핵심 예상값은 `task_management`, `task_admin`, `Asia/Seoul`입니다.

## 1. 타입이 필요한 이유

PostgreSQL은 **강한 타입 시스템(strong type system)**을 사용합니다. 열의 타입은 다음을 함께 결정합니다.

- 저장할 수 있는 값과 범위
- 값이 차지하는 저장 공간
- 사용할 수 있는 연산자와 함수
- 비교와 정렬 방식
- 잘못된 입력을 거부하는 기준

값의 타입은 `pg_typeof`로 확인할 수 있습니다.

```sql
SELECT pg_typeof(42),
       pg_typeof(42::bigint),
       pg_typeof('2026-08-04'::date);
```

`::`는 PostgreSQL의 간결한 타입 변환 표기입니다. 표준 SQL의 `CAST`도 사용할 수 있습니다.

## 2. 정수 타입

| 타입 | 저장 공간 | 대략적인 범위 | 주 용도 |
|---|---:|---:|---|
| `smallint` | 2바이트 | ±3만 | 매우 작은 코드·수량 |
| `integer` | 4바이트 | ±21억 | 일반적인 정수 |
| `bigint` | 8바이트 | 약 ±9×10¹⁸ | 오래 증가하는 식별자·큰 합계 |

업무 관리 시스템의 대리 키는 모두 `bigint`를 사용합니다. 작은 실습에서는 `integer`도 충분하지만, 나중에 키 타입을 바꾸면 모든 외래 키까지 함께 변경해야 합니다. 교재 전체의 일관성과 장기 확장을 위해 처음부터 `bigint`로 통일합니다.

```sql
SELECT 32767::smallint AS small_value,
       2147483647::integer AS integer_value,
       9000000000::bigint AS bigint_value;
```

범위를 벗어나면 조용히 잘못된 값으로 바뀌지 않고 오류가 발생합니다.

```sql
-- 오류 확인용이며 실행 후 값은 저장되지 않습니다.
SELECT 40000::smallint;
```

예상 오류의 핵심은 `smallint out of range`입니다.

## 3. 정확한 숫자와 근사 숫자

### 3.1 numeric

`numeric(precision, scale)`은 정확한 십진 계산이 필요한 금액과 비율에 사용합니다.

- precision: 전체 유효 자릿수
- scale: 소수점 아래 자릿수

```sql
SELECT 12345678.90::numeric(10, 2) AS amount;
```

현재 업무 관리 모델에는 금액 열이 없지만 향후 예산을 추가한다면 `numeric(14, 2)` 같은 타입을 검토합니다. 돈을 부동소수점 타입으로 저장하지 않습니다.

### 3.2 real과 double precision

`real`과 `double precision`은 넓은 범위의 값을 빠르게 계산하는 **근사 숫자(approximate number)** 타입입니다. 측정값과 과학 계산에는 적합하지만 십진 소수를 항상 정확히 표현하지는 못합니다.

```sql
SELECT 0.1::double precision + 0.2::double precision AS approximate_sum,
       0.1::numeric + 0.2::numeric AS exact_sum;
```

환경의 출력 형식에 따라 첫 값이 `0.30000000000000004`처럼 보일 수 있고 `numeric` 결과는 정확히 `0.3`입니다.

## 4. 문자열 타입

PostgreSQL의 대표 문자열 타입은 다음과 같습니다.

| 타입 | 특징 | 선택 기준 |
|---|---|---|
| `text` | 선언 길이 제한 없음 | 설명, 댓글, JSON과 별도인 일반 본문 |
| `varchar(n)` | 최대 문자 수 검사 | 명확한 업무상 길이 제한이 있을 때 |
| `varchar` | 선언 길이 제한 없음 | `text`와 유사 |
| `char(n)` | 고정 길이, 공백 채움 | 특별한 고정 형식이 아니면 피함 |

PostgreSQL에서는 `text`와 길이 제한 없는 `varchar` 사이에 성능상 이점이 없습니다. 따라서 `varchar(n)`은 저장 공간 절약이 아니라 업무 규칙을 표현할 때 사용합니다.

```sql
SELECT char_length('한글') AS characters,
       octet_length('한글') AS bytes;
```

UTF-8에서 한글 두 글자는 문자 수 2이지만 바이트 수는 보통 6입니다. `varchar(20)`의 20은 바이트가 아니라 문자 수입니다.

이 책의 기준은 다음과 같습니다.

- 짧은 코드와 상태: `varchar(n)`
- 이름, 이메일과 제목: 합리적인 상한을 둔 `varchar(n)`
- 설명과 댓글: `text`

`char(n)`은 뒤쪽 공백 처리 때문에 초보자가 예상하지 못한 비교 결과를 만들 수 있어 사용하지 않습니다.

## 5. 날짜와 시간

| 타입 | 저장 내용 | 예제 열 |
|---|---|---|
| `date` | 달력의 날짜 | `start_date`, `due_date` |
| `time` | 날짜 없는 시각 | 현재 모델에서는 사용하지 않음 |
| `timestamp` | 시간대 없는 날짜와 시각 | 지역 고정 일정에 제한적으로 사용 |
| `timestamptz` | 실제 순간 | 생성·수정·완료 시각 |
| `interval` | 시간의 길이 | 경과 시간 계산 |

`timestamptz`는 `timestamp with time zone`의 별칭입니다. 이름과 달리 입력한 시간대 이름 자체를 보존하지는 않습니다. 동일한 순간을 내부적으로 UTC 기준으로 저장하고 세션의 `TimeZone`에 맞춰 표시합니다.

```sql
SELECT TIMESTAMPTZ '2026-08-04 09:00:00 Asia/Seoul' AS seoul_display,
       TIMESTAMPTZ '2026-08-04 09:00:00 Asia/Seoul'
           AT TIME ZONE 'UTC' AS utc_clock;
```

첫 값은 서울 시간대 세션에서 `2026-08-04 09:00:00+09`, 두 번째 값은 같은 순간의 UTC 벽시계 시각 `2026-08-04 00:00:00`이 핵심입니다.

업무 기간처럼 하루 단위로 의미가 있는 값은 `date`, 생성 사건처럼 실제 발생 순간은 `timestamptz`를 사용합니다.

```sql
SELECT DATE '2026-08-10' - DATE '2026-08-04' AS days;
```

결과는 `6`입니다. 모호한 `08/04/2026` 대신 항상 ISO 형식 `YYYY-MM-DD`를 사용합니다.

## 6. 논리값

`boolean`은 참, 거짓과 NULL을 표현합니다.

```sql
SELECT TRUE::boolean, FALSE::boolean, NULL::boolean;
```

입력에는 `true`, `false`, `yes`, `no`, `1`, `0` 등의 표기가 허용되지만 예제와 애플리케이션에서는 `TRUE`와 `FALSE`만 사용합니다.

`app_user.is_active`는 기본값 `TRUE`인 `boolean NOT NULL`로 정합니다. `0`과 `1`을 정수로 저장하는 것보다 의미와 허용 값이 명확합니다.

## 7. UUID

**UUID(Universally Unique Identifier)**는 분산 환경에서도 충돌 가능성이 매우 낮은 128비트 식별자입니다.

```sql
SELECT '550e8400-e29b-41d4-a716-446655440000'::uuid;
```

여러 시스템이 각자 키를 생성하거나 외부에 순차 번호를 노출하지 않으려면 유용합니다. 다만 사람이 읽기 어렵고 `bigint`보다 크므로 이 교재의 단일 PostgreSQL 업무 시스템은 `bigint` Identity를 기본 키로 사용합니다. UUID는 타입의 용도만 익히고 현재 모델에는 적용하지 않습니다.

## 8. JSON과 JSONB

`json`과 `jsonb`는 JSON 문서를 저장하지만 처리 방식이 다릅니다.

- `json`: 입력 텍스트의 공백과 키 순서를 보존하고 매번 다시 해석
- `jsonb`: 분해된 이진 형식으로 저장하고 검색·인덱싱에 유리

```sql
SELECT '{"status":"todo","labels":["sql","beginner"]}'::jsonb
       ->> 'status' AS status;
```

결과는 `todo`입니다. `task_history.details`는 사건마다 세부 필드가 달라질 수 있어 `jsonb NOT NULL DEFAULT '{}'::jsonb`로 정합니다.

사용자 이메일, 업무 상태와 마감일처럼 항상 검색하고 제약해야 하는 핵심 속성을 JSONB 안에 숨기지 않습니다. 관계형 열을 기본으로 하고 구조가 유동적인 부가 상세만 JSONB에 둡니다.

## 9. 배열

PostgreSQL 배열은 한 열에 같은 타입의 여러 값을 저장합니다.

```sql
SELECT ARRAY['sql', 'postgresql']::text[] AS tags,
       'sql' = ANY (ARRAY['sql', 'postgresql']) AS contains_sql;
```

태그처럼 독립 속성이 거의 없는 작은 목록에는 편리할 수 있습니다. 하지만 프로젝트 참여자처럼 각 항목을 외래 키로 검사하고 역할·참여일을 저장해야 한다면 배열이 아니라 `project_member` 연결 테이블을 사용해야 합니다.

현재 기준 모델에는 배열 열을 넣지 않습니다.

## 10. 열거형과 상태 코드

PostgreSQL **열거형(enum)**은 미리 정의한 값만 허용하는 사용자 정의 타입입니다.

```sql
-- 개념 예시이며 현재 모델에는 생성하지 않습니다.
CREATE TYPE task_status AS ENUM
    ('todo', 'in_progress', 'blocked', 'done', 'cancelled');
```

값의 종류를 강하게 제한하고 정렬 순서를 정의할 수 있지만, 여러 테이블과 배포 환경에서 타입 객체의 변경 순서를 관리해야 합니다. 입문 교재에서는 상태값 변경과 테이블 정의를 한곳에서 보기 쉽도록 `varchar`와 이름 있는 `CHECK` 제약을 사용합니다.

```text
status varchar(20) NOT NULL
CHECK (status IN ('todo', 'in_progress', 'blocked', 'done', 'cancelled'))
```

열거형은 나쁜 타입이 아니라 변경 빈도와 운영 방식에 따라 선택하는 타입입니다.

## 11. 타입 변환

PostgreSQL이 안전하다고 판단하면 **암시적 변환(implicit cast)**을 수행하지만, 의도를 분명히 하려면 **명시적 변환(explicit cast)**을 사용합니다.

```sql
SELECT CAST('42' AS integer) AS standard_cast,
       '2026-08-04'::date AS postgresql_cast,
       42::text AS text_value;
```

잘못된 문자열은 오류가 됩니다.

```sql
-- 오류 확인용
SELECT '사십이'::integer;
```

애플리케이션에서 변환 실패를 피하려고 모든 값을 `text`로 저장하지 않습니다. 입력을 검증하고 실제 의미에 맞는 타입에 저장해야 비교, 계산과 제약이 올바르게 동작합니다.

## 12. 업무 관리 시스템 타입 정책

| 열 종류 | 확정 타입 | 이유 |
|---|---|---|
| 모든 대리 키·외래 키 | `bigint` | 장기 증가와 타입 일치 |
| 부서·프로젝트 코드 | `varchar(20)` | 업무상 짧은 코드 |
| 사용자 이메일 | `varchar(254)` | 합리적인 입력 상한 |
| 사람·부서·프로젝트 이름 | `varchar(100)` | 화면 입력 길이 제한 |
| 업무 제목 | `varchar(200)` | 제목과 본문 구분 |
| 상태·역할·우선순위 | `varchar(20)` | `CHECK`와 함께 사용 |
| 설명·댓글 | `text` | 자연스러운 가변 본문 |
| 시작일·마감일 | `date` | 하루 단위 업무 의미 |
| 생성·수정·완료 시각 | `timestamptz` | 실제 순간과 시간대 변환 |
| 활성 여부 | `boolean` | 참·거짓 의미 |
| 변경 상세 | `jsonb` | 사건별 유동적인 부가 정보 |

8장에서는 대리 키에 `GENERATED BY DEFAULT AS IDENTITY`, 시각 기본값에 `CURRENT_TIMESTAMP`를 사용합니다.

## 13. 따라 하기: 예제 파일 실행

`psql`을 종료했다면 저장소 루트의 **Ubuntu Bash**에서 실행합니다.

```bash
psql -X -v ON_ERROR_STOP=1 -h localhost -U task_admin \
  -d task_management -W -f examples/07_data_types.sql
```

이 파일은 조회만 수행하므로 여러 번 실행해도 데이터베이스 상태가 바뀌지 않습니다. 결과에서 다음을 확인합니다.

- `pg_typeof`가 지정한 타입을 표시한다.
- 정확한 `numeric` 합계는 `0.3`이다.
- 한글 두 글자의 UTF-8 바이트 수는 6이다.
- 서울 오전 9시는 UTC 오전 0시와 같은 순간이다.
- JSONB, 배열과 UUID 값이 각 타입으로 변환된다.

## 14. 원리 이해: 타입은 가장 가까운 검증 장치다

사용자 화면과 Python 코드에서 값을 검사해도 다른 프로그램이나 직접 SQL로 잘못된 값이 들어올 수 있습니다. 데이터 타입은 데이터가 저장되기 직전에 항상 적용되는 첫 번째 방어선입니다.

```text
사용자 입력
  → 애플리케이션 검증
    → SQL 타입 변환
      → 열 데이터 타입
        → NOT NULL·CHECK·외래 키
```

타입은 값의 종류를 검사하고, 제약조건은 그 타입 안에서 업무 규칙을 더 좁힙니다. 예를 들어 `varchar(20)`은 문자열 길이를 제한하지만 상태가 `todo`인지 확인하지는 않습니다. 그 역할은 8장의 `CHECK`가 담당합니다.

## 15. 주의 및 오류 해결

### `invalid input syntax for type integer`

숫자가 아닌 문자열을 정수로 변환했습니다. 원본 값을 확인하고 입력 단계에서 숫자 형식을 검증합니다. 오류를 숨기기 위해 대상 열을 `text`로 바꾸지 않습니다.

### `value too long for type character varying`

문자 수가 `varchar(n)` 제한을 넘었습니다. 값이 잘못 긴 것인지 업무상 제한이 너무 작은지 먼저 판단한 뒤 데이터 또는 스키마를 수정합니다.

### 날짜의 월과 일이 바뀐다

`08/04/2026`처럼 지역에 따라 모호한 입력을 사용하지 않습니다. `DATE '2026-08-04'` 또는 ISO 문자열을 사용합니다.

### timestamptz 출력 시각이 예상과 다르다

저장된 순간이 바뀐 것이 아니라 세션 `TimeZone`에 맞춰 표시된 것일 수 있습니다.

```sql
SHOW TimeZone;
SELECT current_setting('TimeZone');
```

이 책의 실습은 `Asia/Seoul`로 맞춥니다.

### JSON과 JSONB를 모든 데이터에 사용하고 싶다

JSONB는 유연하지만 외래 키와 일반 열 제약, 단순한 통계 쿼리가 어려워질 수 있습니다. 구조가 고정된 핵심 속성은 관계형 열에 두고 변경 이력의 부가 상세처럼 유동적인 부분에만 사용합니다.

## 16. 실습 문제

1. `42`, `42::bigint`, `42.00::numeric(10,2)`의 타입을 확인하세요.
2. 금액에 `double precision`보다 `numeric`이 적합한 이유를 설명하세요.
3. 프로젝트 시작일과 생성 시각에 각각 어떤 타입을 사용할지 고르세요.
4. `'2026-12-31'`을 `date`로 두 방식으로 변환하세요.
5. `task_history.details`에 JSONB를 선택한 이유를 설명하세요.
6. 업무 상태에 enum 대신 `varchar + CHECK`를 선택한 이유를 쓰세요.

### 응용 문제

1. 서울의 `2026-08-04 18:30`을 `timestamptz`로 만들고 UTC 벽시계 시각으로 표시하세요.
2. 배열에 프로젝트 참여자를 저장하면 안 되는 이유를 6장의 관계 모델과 연결해 설명하세요.

## 17. 실습 문제 정답

1. 다음과 같이 확인합니다.

   ```sql
   SELECT pg_typeof(42),
          pg_typeof(42::bigint),
          pg_typeof(42.00::numeric(10, 2));
   ```

   핵심 결과는 `integer`, `bigint`, `numeric`입니다.

2. `numeric`은 십진 값을 정확히 저장하지만 부동소수점은 이진 근삿값이라 금액 합계에서 미세한 오차가 생길 수 있습니다.

3. 프로젝트 시작일은 하루 단위의 `date`, 생성 시각은 실제 순간을 나타내는 `timestamptz`가 적합합니다.

4. 두 표기는 다음과 같습니다.

   ```sql
   SELECT CAST('2026-12-31' AS date),
          '2026-12-31'::date;
   ```

5. 변경 사건마다 이전·이후 상태, 담당자 등 세부 필드가 달라질 수 있으므로 유동적인 부가 구조에 적합합니다. 자주 제약하고 관계를 맺는 핵심 값은 일반 열에 남깁니다.

6. 이 교재에서는 허용값과 제약을 테이블 정의에서 함께 읽고 상태 변경을 쉽게 실습하기 위해 `varchar(20)`과 이름 있는 `CHECK`를 사용합니다.

응용 1의 정답입니다.

```sql
SELECT TIMESTAMPTZ '2026-08-04 18:30:00 Asia/Seoul'
       AT TIME ZONE 'UTC' AS utc_clock;
```

결과는 `2026-08-04 09:30:00`입니다.

응용 2: 참여자는 사용자 외래 키, 참여 역할과 참여 시각을 각각 가져야 하고 같은 사용자의 중복 참여도 막아야 합니다. 배열 한 열보다 `project_member` 연결 테이블과 복합 기본 키가 이 규칙을 정확히 표현합니다.

## 18. 핵심 정리

- 타입은 저장 가능한 값, 연산과 비교 규칙을 결정합니다.
- 대리 키와 외래 키는 모두 `bigint`로 통일합니다.
- 정확한 십진 계산은 `numeric`, 근사 측정값은 부동소수점 타입을 사용합니다.
- `varchar(n)`은 저장 공간보다 업무상 문자 수 제한을 표현합니다.
- 하루 단위 값은 `date`, 실제 발생 순간은 `timestamptz`를 사용합니다.
- `timestamptz`는 같은 순간을 저장하고 세션 시간대에 맞춰 표시합니다.
- 핵심 속성은 관계형 열, 유동적인 부가 상세는 JSONB에 둡니다.
- 현재 모델의 상태값은 `varchar + CHECK`로 제한합니다.
- 명시적 타입 변환은 SQL의 의도를 분명하게 합니다.

## 19. 확인 문제

1. `integer`와 `bigint`의 주요 차이는 무엇인가요?
2. `text`와 `varchar(n)`은 어떤 기준으로 선택하나요?
3. `timestamp`와 `timestamptz`의 차이는 무엇인가요?
4. UUID를 현재 모델의 기본 키로 선택하지 않은 이유는 무엇인가요?
5. 배열과 연결 테이블의 선택 기준은 무엇인가요?
6. 타입과 `CHECK` 제약조건은 각각 무엇을 검사하나요?

## 참고한 공식 문서

- [PostgreSQL 18 데이터 타입](https://www.postgresql.org/docs/18/datatype.html)
- [숫자 타입](https://www.postgresql.org/docs/18/datatype-numeric.html)
- [문자 타입](https://www.postgresql.org/docs/18/datatype-character.html)
- [날짜와 시간 타입](https://www.postgresql.org/docs/18/datatype-datetime.html)
- [JSON 타입](https://www.postgresql.org/docs/18/datatype-json.html)
- [배열](https://www.postgresql.org/docs/18/arrays.html)
- [열거형](https://www.postgresql.org/docs/18/datatype-enum.html)

## 20. 다음 장 안내

데이터 타입 정책을 확정했으므로 8장에서는 7개 테이블을 실제로 만듭니다. Identity 기본 키, 기본값, `NOT NULL`, `UNIQUE`, `CHECK`, 기본 키와 외래 키를 적용하고 `ALTER TABLE`과 안전한 삭제 방법도 연습합니다.
