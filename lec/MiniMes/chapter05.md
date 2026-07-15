# Chapter 5. 기본 SQL 조회

## 1. 학습 목표

이 장을 마치면 다음을 할 수 있다.

- `SELECT` 문으로 테이블 데이터를 조회할 수 있다.
- 필요한 컬럼만 선택해서 볼 수 있다.
- `WHERE`로 조건에 맞는 행만 조회할 수 있다.
- `ORDER BY`로 결과를 정렬할 수 있다.
- `LIMIT`로 조회 결과의 일부만 확인할 수 있다.

이 장부터는 SQL 문법을 본격적으로 사용한다. 목표는 어려운 문법을 많이 외우는 것이 아니라, MES 데이터를 질문에 맞게 꺼내 보는 기본 습관을 만드는 것이다.

## 2. 현장 상황

라면공장 관리자가 아침 회의 전에 Mini MES 데이터를 빠르게 확인하려고 한다.

| 확인 질문 | 필요한 SQL 기능 |
| --- | --- |
| 등록된 품목은 무엇인가? | `SELECT`, `FROM` |
| 제품만 보고 싶은가? | `WHERE` |
| LOT를 날짜 순서로 보고 싶은가? | `ORDER BY` |
| 최근 몇 건만 먼저 보고 싶은가? | `LIMIT` |

처음 SQL을 배울 때는 모든 데이터를 한 번에 보려는 경우가 많다. 하지만 실제 현장에서는 필요한 질문이 먼저 있고, SQL은 그 질문에 맞는 행과 컬럼을 고르는 도구다.

예를 들어 `lot` 테이블에는 원자재 LOT와 완제품 LOT가 함께 들어 있다. 창고 담당자가 원자재 입고 LOT만 보고 싶다면 전체 LOT를 모두 볼 필요가 없다. `WHERE lot_type = 'RECEIPT'` 조건을 사용하면 된다.

## 3. 핵심 개념

### `SELECT`

`SELECT`는 어떤 컬럼을 볼지 정하는 키워드다.

```sql
SELECT
    item_id,
    item_code,
    item_name
FROM item;
```

위 SQL은 `item` 테이블에서 `item_id`, `item_code`, `item_name` 컬럼만 조회한다.

### `FROM`

`FROM`은 어느 테이블에서 데이터를 가져올지 정한다.

| 질문 | 사용할 테이블 |
| --- | --- |
| 품목 목록을 보고 싶다 | `item` |
| LOT 목록을 보고 싶다 | `lot` |
| 생산 실적을 보고 싶다 | `production` |
| 원자재 투입 이력을 보고 싶다 | `production_material` |

### `WHERE`

`WHERE`는 필요한 행만 고르는 조건이다.

```sql
SELECT
    item_code,
    item_name
FROM item
WHERE item_type = 'PRODUCT';
```

이 SQL은 제품 품목만 조회한다.

### `ORDER BY`

`ORDER BY`는 결과를 정렬한다. 생산일자 순서, LOT 번호 순서, 수량이 큰 순서로 정렬할 때 사용한다.

### `LIMIT`

`LIMIT`는 결과 행 수를 제한한다. 데이터가 많을 때 일부만 먼저 확인하기 좋다.

## 4. 모델링 설명

기본 SQL 조회는 데이터 모델을 이해하는 첫 번째 도구다. 테이블을 설계했다면, 그 테이블에서 어떤 질문에 답할 수 있는지 조회해 봐야 한다.

| 테이블 | 기본 조회로 확인할 내용 |
| --- | --- |
| `item` | 제품과 원자재 기준정보가 잘 들어갔는가 |
| `lot` | 입고 LOT와 생산 LOT가 구분되는가 |
| `production` | 생산일자와 생산 수량이 기록되는가 |
| `production_material` | 생산별 원자재 투입 행이 기록되는가 |

처음에는 `JOIN` 없이 한 테이블씩 조회한다. 한 테이블을 정확히 읽을 수 있어야 여러 테이블을 연결하는 조회도 이해할 수 있다.

```mermaid
flowchart LR
    Q[현장 질문] --> T[조회할 테이블 선택]
    T --> C[필요한 컬럼 선택]
    C --> W[조건 지정]
    W --> O[정렬]
    O --> R[결과 해석]
```

SQL은 이 흐름을 문장으로 표현한 것이다.

## 5. SQL 예제

### 5.1 `item` 전체 조회

```sql
SELECT
    item_id,
    item_code,
    item_name,
    item_type,
    unit,
    is_active
FROM item;
```

이 SQL은 품목 기준정보 전체를 조회한다. 처음 데이터가 잘 들어갔는지 확인할 때 사용한다.

### 5.2 필요한 컬럼만 조회

```sql
SELECT
    item_code,
    item_name,
    item_type
FROM item;
```

항상 모든 컬럼이 필요한 것은 아니다. 현장 화면이나 보고서에서는 필요한 컬럼만 보여 주는 편이 읽기 쉽다.

### 5.3 제품만 조회

```sql
SELECT
    item_code,
    item_name,
    unit
FROM item
WHERE item_type = 'PRODUCT';
```

`WHERE` 조건을 사용해 제품 품목만 조회한다.

### 5.4 원자재 LOT만 조회

```sql
SELECT
    lot_no,
    item_id,
    qty,
    received_date,
    expire_date
FROM lot
WHERE lot_type = 'RECEIPT';
```

이 SQL은 입고로 생긴 원자재 LOT만 보여 준다.

### 5.5 생산일자 순으로 생산 실적 조회

```sql
SELECT
    production_no,
    production_date,
    item_id,
    qty,
    status
FROM production
ORDER BY production_date;
```

`ORDER BY production_date`는 생산 실적을 날짜 순서로 정렬한다.

### 5.6 생산 수량이 큰 순서로 조회

```sql
SELECT
    production_no,
    production_date,
    qty
FROM production
ORDER BY qty DESC;
```

`DESC`는 내림차순이다. 생산 수량이 큰 행부터 확인할 수 있다.

### 5.7 일부 행만 조회

```sql
SELECT
    lot_no,
    lot_type,
    qty
FROM lot
ORDER BY lot_id
LIMIT 3;
```

`LIMIT 3`은 결과 중 앞의 3행만 보여 준다. 데이터가 많을 때 먼저 구조를 확인하기 좋다.

### 5.8 조건을 두 개 사용하기

```sql
SELECT
    lot_no,
    lot_type,
    qty,
    expire_date
FROM lot
WHERE lot_type = 'PRODUCTION'
  AND qty >= 2500
ORDER BY qty DESC;
```

`AND`는 조건을 모두 만족하는 행만 조회한다. 이 SQL은 생산 LOT 중 수량이 2,500 이상인 행을 찾는다.

## 6. 데이터 해석

기본 조회 결과를 볼 때는 행과 컬럼을 구분해서 읽어야 한다.

| 관점 | 읽는 방법 |
| --- | --- |
| 행 | 하나의 데이터 단위다. `item`에서는 품목 1개, `production`에서는 생산 이벤트 1건이다. |
| 컬럼 | 행을 설명하는 속성이다. `qty`, `status`, `lot_type`처럼 의미가 정해져 있다. |

예를 들어 `production` 조회 결과에서 한 행이 다음과 같다고 하자.

| `production_no` | `production_date` | `item_id` | `qty` | `status` |
| --- | --- | ---: | ---: | --- |
| `PRD-20260710-001` | `2026-07-10` | 1 | 3000 | `COMPLETED` |

이 행은 `2026-07-10에 item_id가 1인 제품을 3,000개 생산 완료했다`는 뜻이다. 아직 품목명은 보이지 않는다. 품목명을 보려면 뒤에서 배울 `JOIN`이 필요하다.

`lot` 테이블의 `qty`와 `production` 테이블의 `qty`는 둘 다 수량이지만 의미가 다르다.

| 컬럼 | 의미 |
| --- | --- |
| `lot.qty` | LOT 기준 수량 |
| `production.qty` | 생산 수량 |
| `production_material.qty` | 원자재 투입 수량 |

## 7. 잘못된 설계 사례

### 7.1 `SELECT *`만 계속 사용하는 경우

`SELECT *`는 모든 컬럼을 보여 준다. 처음 확인할 때는 편리하지만, 보고서나 교재 예제로는 필요한 컬럼을 명시하는 편이 좋다.

| 방식 | 장점 | 문제점 |
| --- | --- | --- |
| `SELECT *` | 빠르게 전체 구조 확인 | 어떤 컬럼을 의도했는지 불명확하다 |
| 컬럼명 명시 | 읽는 사람이 의도를 알 수 있다 | 처음 작성할 때 조금 더 길다 |

이 교재에서는 학습자가 컬럼 의미를 익히도록 필요한 컬럼을 직접 적는 방식을 기본으로 한다.

### 7.2 조건 없이 모든 데이터를 보는 습관

현장 데이터가 많아지면 조건 없는 조회는 읽기 어렵다. `WHERE`, `ORDER BY`, `LIMIT`를 사용해 질문에 맞게 좁혀야 한다.

### 7.3 문자열 값을 따옴표 없이 쓰는 경우

SQLite에서 문자열 조건은 작은따옴표로 감싸야 한다.

| 잘못된 조건 | 올바른 조건 |
| --- | --- |
| `item_type = PRODUCT` | `item_type = 'PRODUCT'` |
| `status = COMPLETED` | `status = 'COMPLETED'` |

## 8. 실습

### 실습 1. 활성 품목 조회하기

```sql
SELECT
    item_code,
    item_name,
    item_type
FROM item
WHERE is_active = 'Y'
ORDER BY item_code;
```

확인할 내용:

- 활성 품목은 몇 개인가?
- 제품과 원자재가 함께 조회되는가?

### 실습 2. 완제품 LOT 조회하기

```sql
SELECT
    lot_no,
    item_id,
    qty,
    produced_date
FROM lot
WHERE lot_type = 'PRODUCTION'
ORDER BY produced_date;
```

확인할 내용:

- 완제품 LOT는 몇 개인가?
- 생산일자가 가장 빠른 LOT는 무엇인가?

### 실습 3. 생산 완료 실적 중 일부만 보기

```sql
SELECT
    production_no,
    production_date,
    qty
FROM production
WHERE status = 'COMPLETED'
ORDER BY production_date
LIMIT 2;
```

확인할 내용:

- 결과는 몇 행인가?
- `LIMIT`를 제거하면 몇 행이 조회되는가?

### 실습 4. 원자재 투입 수량이 큰 순서로 보기

```sql
SELECT
    production_id,
    material_item_id,
    material_lot_id,
    qty
FROM production_material
ORDER BY qty DESC, production_id;
```

확인할 내용:

- 가장 큰 투입 수량은 얼마인가?
- 같은 수량일 때 어떤 기준으로 다시 정렬되는가?

## 9. 확인 문제

1. `SELECT`와 `FROM`의 역할을 각각 설명하시오.
2. `WHERE`는 어떤 상황에서 사용하는가?
3. 생산 수량이 큰 순서로 조회하려면 `ORDER BY qty` 뒤에 어떤 키워드를 붙이는가?
4. `LIMIT`는 왜 사용하는가?
5. `lot.qty`와 `production.qty`의 의미 차이를 설명하시오.

## 10. 핵심 정리

- `SELECT`는 조회할 컬럼을 정한다.
- `FROM`은 조회할 테이블을 정한다.
- `WHERE`는 조건에 맞는 행만 고른다.
- `ORDER BY`는 결과를 정렬한다.
- `LIMIT`는 결과 행 수를 제한한다.
- 기본 SQL 조회는 MES 데이터를 이해하는 첫 번째 실습 도구다.
