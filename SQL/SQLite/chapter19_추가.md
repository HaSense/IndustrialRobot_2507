# 추가 실습. GROUP BY와 WHERE 함께 사용하기

---

# 학습목표

본 실습을 완료하면 다음 내용을 이해할 수 있다.

* WHERE와 GROUP BY의 실행 순서를 이해할 수 있다.
* 조건에 맞는 데이터만 그룹화할 수 있다.
* WHERE와 HAVING의 차이를 설명할 수 있다.

---

# 실습용 테이블

inspection

|id|product_name|inspector|defect_qty|inspect_date|
|--|------------|----------|----------|------------|
|1|Motor|Kim|2|2026-06-22|
|2|Motor|Lee|1|2026-06-22|
|3|Sensor|Kim|4|2026-06-23|
|4|Sensor|Park|3|2026-06-23|
|5|Pump|Kim|0|2026-06-24|
|6|Pump|Lee|2|2026-06-24|
|7|Motor|Kim|1|2026-06-25|
|8|Valve|Park|5|2026-06-25|

---

# 실습 1. 특정 날짜의 불량 수량

2026-06-23에 검사한 제품별 불량 수량을 구하시오.

```sql
SELECT
    product_name,
    SUM(defect_qty)
FROM inspection
WHERE inspect_date='2026-06-23'
GROUP BY product_name;
```

---

# 실습 2. 불량이 발생한 제품만 그룹화하기

불량 수량이 1개 이상인 데이터만 제품별로 합계를 구하시오.

```sql
SELECT
    product_name,
    SUM(defect_qty)
FROM inspection
WHERE defect_qty >= 1
GROUP BY product_name;
```

---

# 실습 3. Kim 검사자의 제품별 불량 수량

```sql
SELECT
    product_name,
    SUM(defect_qty)
FROM inspection
WHERE inspector='Kim'
GROUP BY product_name;
```

---

# 실습 4. Lee 검사자의 평균 불량 수량

```sql
SELECT
    product_name,
    AVG(defect_qty)
FROM inspection
WHERE inspector='Lee'
GROUP BY product_name;
```

---

# 실습 5. Motor 제품의 검사자별 불량 수량

```sql
SELECT
    inspector,
    SUM(defect_qty)
FROM inspection
WHERE product_name='Motor'
GROUP BY inspector;
```

---

# 실습 6. 불량이 2개 이상인 검사만 집계하기

```sql
SELECT
    inspector,
    SUM(defect_qty)
FROM inspection
WHERE defect_qty >= 2
GROUP BY inspector;
```

---

# 실습 7. 특정 날짜 이후 검사 데이터

2026-06-23 이후 검사한 제품별 불량 수량을 구하시오.

```sql
SELECT
    product_name,
    SUM(defect_qty)
FROM inspection
WHERE inspect_date >= '2026-06-23'
GROUP BY product_name;
```

---

# 실습 8. 불량이 발생한 데이터의 평균 불량 수량

```sql
SELECT
    product_name,
    AVG(defect_qty)
FROM inspection
WHERE defect_qty > 0
GROUP BY product_name;
```

---

# WHERE와 HAVING 함께 사용하기

불량이 1개 이상 발생한 데이터만 이용하여 제품별 총 불량 수량을 계산하고,

총 불량 수량이 5개 이상인 제품만 출력하시오.

```sql
SELECT
    product_name,
    SUM(defect_qty)
FROM inspection
WHERE defect_qty >= 1
GROUP BY product_name
HAVING SUM(defect_qty) >= 5;
```

---

# 종합 실습

다음 조건을 만족하는 SQL을 작성하시오.

조건

* 검사자는 Kim이다.
* 불량 수량은 1개 이상이다.
* 제품별 총 불량 수량을 계산한다.
* 총 불량 수량이 3개 이상인 제품만 출력한다.
* 총 불량 수량이 많은 순으로 정렬한다.

정답

```sql
SELECT
    product_name,
    SUM(defect_qty) AS total_defect
FROM inspection
WHERE inspector='Kim'
  AND defect_qty >= 1
GROUP BY product_name
HAVING SUM(defect_qty) >= 3
ORDER BY total_defect DESC;
```

---

# 핵심정리

* WHERE는 그룹화 전에 데이터를 선택한다.
* GROUP BY는 같은 값을 하나의 그룹으로 묶는다.
* HAVING은 그룹화된 결과를 다시 조건으로 필터링한다.
* WHERE와 HAVING은 함께 사용하는 경우가 매우 많다.
* 품질관리 시스템에서는 제품별 불량 수량, 검사자별 불량 수량 등의 통계를 GROUP BY로 자주 계산한다.
