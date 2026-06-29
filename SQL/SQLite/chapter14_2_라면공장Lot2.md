# 6. 완제품 LOT와 원재료 LOT 연결

지금까지는 완제품 LOT만 관리하였다.

하지만 실제 MES에서는 다음 질문에 답할 수 있어야 한다.

```text
RAMEN_20260622는

어떤 원재료 LOT를 사용하여 생산되었는가?
```

이를 위해서는

* 원재료 LOT 테이블
* 원재료 사용 이력 테이블

이 필요하다.

---

# 원재료 LOT 테이블

```sql
CREATE TABLE material_lot(
    material_lot_id TEXT,
    material_name TEXT,
    supplier TEXT,
    received_date TEXT
);
```

---

# 원재료 LOT 등록

```sql
INSERT INTO material_lot
VALUES
('FLOUR_20260620_001','밀가루','대한제분','2026-06-20');

INSERT INTO material_lot
VALUES
('SOUP_20260621_001','분말스프','스프협력사A','2026-06-21');

INSERT INTO material_lot
VALUES
('OIL_20260619_001','팜유','유지협력사B','2026-06-19');
```

---

# 원재료 LOT 조회

```sql
SELECT *
FROM material_lot;
```

예상 결과

```text
FLOUR_20260620_001 | 밀가루 | 대한제분 | 2026-06-20
SOUP_20260621_001  | 분말스프 | 스프협력사A | 2026-06-21
OIL_20260619_001   | 팜유 | 유지협력사B | 2026-06-19
```

---

# 원재료 사용 이력 테이블

완제품 LOT 하나에는 여러 개의 원재료 LOT가 사용된다.

이를 관리하기 위해 사용 이력 테이블을 만든다.

```sql
CREATE TABLE ramen_material_usage(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    ramen_lot_no TEXT,
    material_lot_id TEXT,
    used_qty REAL,
    unit TEXT
);
```

---

# 사용 이력 등록

```sql
INSERT INTO ramen_material_usage
(ramen_lot_no, material_lot_id, used_qty, unit)
VALUES
('RAMEN_20260622','FLOUR_20260620_001',60,'kg');

INSERT INTO ramen_material_usage
(ramen_lot_no, material_lot_id, used_qty, unit)
VALUES
('RAMEN_20260622','SOUP_20260621_001',10,'kg');

INSERT INTO ramen_material_usage
(ramen_lot_no, material_lot_id, used_qty, unit)
VALUES
('RAMEN_20260622','OIL_20260619_001',15,'kg');
```

---

# 데이터 구조

```text
ramen_lot
---------------------
lot_no
product_name
qty
prod_date

        │

        ▼

ramen_material_usage
---------------------
ramen_lot_no
material_lot_id
used_qty
unit

        │

        ▼

material_lot
---------------------
material_lot_id
material_name
supplier
received_date
```

---

# 실습 11. 완제품 LOT에 사용된 원재료 조회

다음 SQL을 실행하시오.

```sql
SELECT
    r.lot_no,
    r.product_name,
    m.material_name,
    m.material_lot_id,
    u.used_qty,
    u.unit
FROM ramen_lot r
JOIN ramen_material_usage u
ON r.lot_no = u.ramen_lot_no
JOIN material_lot m
ON u.material_lot_id = m.material_lot_id
WHERE r.lot_no='RAMEN_20260622';
```

---

예상 결과

| LOT            | 제품   | 원재료 LOT            | 원재료  | 사용량 | 단위 |
| -------------- | ---- | ------------------ | ---- | --- | -- |
| RAMEN_20260622 | 매운라면 | FLOUR_20260620_001 | 밀가루  | 60  | kg |
| RAMEN_20260622 | 매운라면 | SOUP_20260621_001  | 분말스프 | 10  | kg |
| RAMEN_20260622 | 매운라면 | OIL_20260619_001   | 팜유   | 15  | kg |

---

# 결과 해석

위 결과는 다음 의미이다.

```text
RAMEN_20260622 LOT는

밀가루 LOT
분말스프 LOT
팜유 LOT

를 사용하여 생산되었다.
```

이것이 MES의 LOT 추적이다.

---

# 실습 12. 원재료 LOT로 완제품 찾기

상황

```text
분말스프 LOT에서 문제가 발견되었다.
```

어떤 완제품을 회수해야 하는가?

다음 SQL을 작성하시오.

```sql
SELECT
    m.material_lot_id,
    m.material_name,
    r.lot_no,
    r.product_name,
    r.qty
FROM material_lot m
JOIN ramen_material_usage u
ON m.material_lot_id = u.material_lot_id
JOIN ramen_lot r
ON u.ramen_lot_no = r.lot_no
WHERE m.material_lot_id='SOUP_20260621_001';
```

---

예상 결과

| 원재료 LOT           | 원재료  | 완제품 LOT        | 제품   | 생산수량 |
| ----------------- | ---- | -------------- | ---- | ---- |
| SOUP_20260621_001 | 분말스프 | RAMEN_20260622 | 매운라면 | 100  |

---

# 리콜(Recall)

만약

```text
SOUP_20260621_001

분말스프 LOT에서

세균이 검출되었다.
```

라면

```text
RAMEN_20260622
```

LOT를 회수하면 된다.

모든 제품을 회수할 필요는 없다.

---

# LOT 추적 방향

실제 MES에서는 두 가지 방향의 추적이 가능해야 한다.

```text
완제품 LOT

↓

사용된 원재료 LOT 조회
```

그리고

```text
원재료 LOT

↓

영향을 받은 완제품 LOT 조회
```

이 두 기능이 바로 LOT 추적(Traceability)의 핵심이다.

---

# 핵심 정리

* 완제품 LOT만으로는 추적성이 부족하다.
* 원재료 LOT를 함께 관리해야 한다.
* 완제품 LOT와 원재료 LOT는 사용 이력 테이블로 연결한다.
* JOIN을 이용하여 사용된 원재료를 조회할 수 있다.
* 반대로 원재료 LOT를 이용하여 영향을 받은 완제품도 찾을 수 있다.
* 이것이 실제 MES에서 사용하는 LOT 추적 시스템의 기본 구조이다.
