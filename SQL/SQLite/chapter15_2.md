# LEFT JOIN / RIGHT JOIN 실습

---

# 실습 목표

이번 실습에서는 다음 내용을 연습한다.

* LEFT JOIN의 동작 이해
* RIGHT JOIN의 개념 이해
* 기준 테이블이 바뀌면 결과가 달라지는 이유 이해
* 매칭되지 않는 데이터가 NULL로 표시되는 구조 이해

---

# 실습 시나리오

스마트팩토리에서 다음 두 테이블을 사용한다.

* `item` : 제품 기준 정보
* `work_result` : 생산 실적 정보

제품 기준 정보에는 등록되어 있지만 아직 생산되지 않은 제품이 있을 수 있다.

반대로 생산 실적에는 기록되어 있지만 제품 기준 정보에 등록되지 않은 제품 코드가 있을 수도 있다.

---

# 1. 테이블 생성

```sql
DROP TABLE IF EXISTS item;
DROP TABLE IF EXISTS work_result;

CREATE TABLE item (
    item_id INTEGER PRIMARY KEY,
    item_name TEXT NOT NULL,
    category TEXT NOT NULL
);

CREATE TABLE work_result (
    result_id INTEGER PRIMARY KEY,
    item_id INTEGER,
    qty INTEGER NOT NULL,
    work_date TEXT NOT NULL
);
```

---

# 2. 데이터 입력

## item 테이블

```sql
INSERT INTO item VALUES
(1, 'Motor', 'Part'),
(2, 'Sensor', 'Part'),
(3, 'Pump', 'Part'),
(4, 'Valve', 'Part'),
(5, 'Battery', 'Module');
```

`Battery`는 제품 기준 정보에는 있지만 생산 실적은 없는 제품이다.

---

## work_result 테이블

```sql
INSERT INTO work_result VALUES
(1, 1, 100, '2026-06-22'),
(2, 2, 150, '2026-06-22'),
(3, 3, 120, '2026-06-23'),
(4, 1, 180, '2026-06-23'),
(5, 6, 90, '2026-06-24');
```

`item_id = 6`은 생산 실적에는 있지만 `item` 테이블에는 없는 제품이다.

---

# 3. 데이터 확인

```sql
SELECT *
FROM item;
```

```sql
SELECT *
FROM work_result;
```

---

# 4. INNER JOIN 복습

양쪽 테이블에 모두 존재하는 데이터만 조회한다.

```sql
SELECT
    i.item_id,
    i.item_name,
    w.qty,
    w.work_date
FROM item i
INNER JOIN work_result w
ON i.item_id = w.item_id;
```

## 결과

| item_id | item_name | qty | work_date |
|--------:|-----------|----:|-----------|
| 1 | Motor | 100 | 2026-06-22 |
| 2 | Sensor | 150 | 2026-06-22 |
| 3 | Pump | 120 | 2026-06-23 |
| 1 | Motor | 180 | 2026-06-23 |

`Battery`는 생산 실적이 없으므로 출력되지 않는다.

`item_id = 6`은 제품 기준 정보가 없으므로 출력되지 않는다.

---

# 5. LEFT JOIN

LEFT JOIN은 왼쪽 테이블의 데이터를 모두 출력한다.

```sql
SELECT
    i.item_id,
    i.item_name,
    w.qty,
    w.work_date
FROM item i
LEFT JOIN work_result w
ON i.item_id = w.item_id;
```

## 결과

| item_id | item_name | qty | work_date |
|--------:|-----------|----:|-----------|
| 1 | Motor | 100 | 2026-06-22 |
| 1 | Motor | 180 | 2026-06-23 |
| 2 | Sensor | 150 | 2026-06-22 |
| 3 | Pump | 120 | 2026-06-23 |
| 4 | Valve | NULL | NULL |
| 5 | Battery | NULL | NULL |

`Valve`, `Battery`는 제품 기준 정보에는 있지만 생산 실적이 없기 때문에 `qty`, `work_date`가 `NULL`로 출력된다.

---

# 6. 생산 실적이 없는 제품 찾기

LEFT JOIN에서 오른쪽 테이블의 값이 NULL인 데이터를 찾으면 된다.

```sql
SELECT
    i.item_id,
    i.item_name
FROM item i
LEFT JOIN work_result w
ON i.item_id = w.item_id
WHERE w.result_id IS NULL;
```

## 결과

| item_id | item_name |
|--------:|-----------|
| 4 | Valve |
| 5 | Battery |

---

# 7. RIGHT JOIN 개념

RIGHT JOIN은 오른쪽 테이블의 데이터를 모두 출력한다.

즉, 다음과 같은 의미이다.

```sql
FROM item i
RIGHT JOIN work_result w
ON i.item_id = w.item_id;
```

오른쪽 테이블인 `work_result`의 모든 데이터가 출력된다.

제품 기준 정보가 없는 생산 실적도 출력된다.

---

# 8. SQLite에서 RIGHT JOIN 대체하기

SQLite 수업에서는 RIGHT JOIN을 직접 쓰기보다  
테이블 순서를 바꾸고 LEFT JOIN을 사용하면 된다.

RIGHT JOIN 개념

```sql
SELECT
    i.item_id,
    i.item_name,
    w.result_id,
    w.qty,
    w.work_date
FROM item i
RIGHT JOIN work_result w
ON i.item_id = w.item_id;
```

SQLite에서 대체

```sql
SELECT
    w.item_id,
    i.item_name,
    w.result_id,
    w.qty,
    w.work_date
FROM work_result w
LEFT JOIN item i
ON w.item_id = i.item_id;
```

## 결과

| item_id | item_name | result_id | qty | work_date |
|--------:|-----------|----------:|----:|-----------|
| 1 | Motor | 1 | 100 | 2026-06-22 |
| 2 | Sensor | 2 | 150 | 2026-06-22 |
| 3 | Pump | 3 | 120 | 2026-06-23 |
| 1 | Motor | 4 | 180 | 2026-06-23 |
| 6 | NULL | 5 | 90 | 2026-06-24 |

`item_id = 6`은 생산 실적에는 있지만 제품 기준 정보가 없기 때문에 `item_name`이 `NULL`로 출력된다.

---

# 9. 제품 기준 정보가 없는 생산 실적 찾기

```sql
SELECT
    w.result_id,
    w.item_id,
    w.qty,
    w.work_date
FROM work_result w
LEFT JOIN item i
ON w.item_id = i.item_id
WHERE i.item_id IS NULL;
```

## 결과

| result_id | item_id | qty | work_date |
|----------:|--------:|----:|-----------|
| 5 | 6 | 90 | 2026-06-24 |

이 데이터는 생산 실적은 있지만 제품 기준 정보가 없는 데이터이다.

실무에서는 이런 데이터를 **기준정보 누락**, **마스터 데이터 누락**이라고 볼 수 있다.

---

# 10. 실습 문제 1

제품 기준 정보에 등록된 모든 제품을 출력하고, 생산 실적이 있으면 생산량도 함께 출력하시오.

조건

* 기준 테이블은 `item`
* 생산 실적이 없어도 제품은 출력되어야 한다.

정답

```sql
SELECT
    i.item_id,
    i.item_name,
    w.qty
FROM item i
LEFT JOIN work_result w
ON i.item_id = w.item_id;
```

---

# 11. 실습 문제 2

아직 한 번도 생산되지 않은 제품만 조회하시오.

정답

```sql
SELECT
    i.item_id,
    i.item_name
FROM item i
LEFT JOIN work_result w
ON i.item_id = w.item_id
WHERE w.result_id IS NULL;
```

---

# 12. 실습 문제 3

생산 실적을 기준으로 모든 생산 데이터를 출력하고, 제품명이 있으면 함께 출력하시오.

정답

```sql
SELECT
    w.result_id,
    w.item_id,
    i.item_name,
    w.qty,
    w.work_date
FROM work_result w
LEFT JOIN item i
ON w.item_id = i.item_id;
```

---

# 13. 실습 문제 4

제품 기준 정보에 없는 생산 실적만 조회하시오.

정답

```sql
SELECT
    w.result_id,
    w.item_id,
    w.qty,
    w.work_date
FROM work_result w
LEFT JOIN item i
ON w.item_id = i.item_id
WHERE i.item_id IS NULL;
```

---

# 14. 실습 문제 5

제품별 총 생산량을 구하시오.

단, 생산 실적이 없는 제품도 출력하시오.

```sql
SELECT
    i.item_id,
    i.item_name,
    SUM(w.qty) AS total_qty
FROM item i
LEFT JOIN work_result w
ON i.item_id = w.item_id
GROUP BY i.item_id, i.item_name;
```

## 결과

| item_id | item_name | total_qty |
|--------:|-----------|----------:|
| 1 | Motor | 280 |
| 2 | Sensor | 150 |
| 3 | Pump | 120 |
| 4 | Valve | NULL |
| 5 | Battery | NULL |

---

# 15. 실습 문제 6

생산 실적이 없는 제품의 생산량을 0으로 표시하시오.

힌트

```sql
COALESCE()
```

정답

```sql
SELECT
    i.item_id,
    i.item_name,
    COALESCE(SUM(w.qty), 0) AS total_qty
FROM item i
LEFT JOIN work_result w
ON i.item_id = w.item_id
GROUP BY i.item_id, i.item_name;
```

## 결과

| item_id | item_name | total_qty |
|--------:|-----------|----------:|
| 1 | Motor | 280 |
| 2 | Sensor | 150 |
| 3 | Pump | 120 |
| 4 | Valve | 0 |
| 5 | Battery | 0 |

---

# 핵심 정리

* INNER JOIN은 양쪽 테이블에 모두 존재하는 데이터만 출력한다.
* LEFT JOIN은 왼쪽 테이블의 데이터를 모두 출력한다.
* RIGHT JOIN은 오른쪽 테이블의 데이터를 모두 출력한다.
* SQLite에서는 RIGHT JOIN 대신 테이블 순서를 바꾸고 LEFT JOIN을 사용하면 된다.
* LEFT JOIN은 누락 데이터 확인에 매우 유용하다.
* `IS NULL` 조건을 사용하면 매칭되지 않은 데이터를 찾을 수 있다.
* `COALESCE()`를 사용하면 NULL을 0 또는 다른 값으로 바꿀 수 있다.
