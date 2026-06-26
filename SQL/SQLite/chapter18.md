# Chapter 10. SQLite 일반 함수 응용 실습

## 학습 목표

이번 장에서는 SQLite에서 자주 사용하는 일반 함수(Function)를 학습한다.

* 문자열 함수
* 숫자 함수
* 날짜 함수
* NULL 처리 함수
* 실무형 응용 예제

---

# 실습 데이터 준비

```sql
CREATE TABLE production (
    id INTEGER PRIMARY KEY,
    product_name TEXT,
    qty INTEGER,
    price INTEGER,
    prod_date TEXT
);

INSERT INTO production VALUES
(1,'Motor',100,5000,'2026-06-22'),
(2,'Sensor',200,12000,'2026-06-22'),
(3,'Pump',150,8000,'2026-06-23'),
(4,'Bearing',80,3000,'2026-06-23'),
(5,'Valve',120,9000,'2026-06-24');
```

---

# 1. 문자열 길이

문자열의 길이를 구한다.

```sql
SELECT
product_name,
LENGTH(product_name)
FROM production;
```

결과

| 제품명    | 길이 |
| ------ | -- |
| Motor  | 5  |
| Sensor | 6  |

---

# 실습 1

제품명과 제품명의 글자수를 출력하시오.

---

# 2. 대문자 변환

```sql
SELECT
UPPER(product_name)
FROM production;
```

결과

```
MOTOR
SENSOR
PUMP
```

---

# 실습 2

모든 제품명을 대문자로 출력하시오.

---

# 3. 소문자 변환

```sql
SELECT
LOWER(product_name)
FROM production;
```

---

# 실습 3

제품명을 모두 소문자로 출력하시오.

---

# 4. 문자열 연결

```sql
SELECT
product_name || ' 생산완료'
FROM production;
```

결과

```
Motor 생산완료
Sensor 생산완료
```

---

# 실습 4

다음과 같이 출력하시오.

```
Motor : 100개
Sensor : 200개
Pump : 150개
```

힌트

```
||
```

연산자를 사용한다.

---

# 5. 문자열 일부 추출

앞 3글자

```sql
SELECT
SUBSTR(product_name,1,3)
FROM production;
```

결과

```
Mot
Sen
Pum
```

---

# 실습 5

제품명의 앞 두 글자만 출력하시오.

---

# 6. 문자열 치환

```sql
SELECT
REPLACE(product_name,'Motor','Engine')
FROM production;
```

결과

```
Engine
Sensor
Pump
```

---

# 실습 6

Valve를 Pipe로 변경하여 출력하시오.

(실제 데이터는 변경되지 않는다.)

---

# 7. 반올림

```sql
SELECT
ROUND(123.456,2);
```

결과

```
123.46
```

---

# 실습 7

다음을 실행해보시오.

```sql
SELECT
ROUND(987.6543,1);
```

---

# 8. 절댓값

```sql
SELECT
ABS(-350);
```

결과

```
350
```

---

# 실습 8

다음을 실행하여 결과를 확인하시오.

```sql
SELECT
ABS(-1250);
```

---

# 9. 현재 날짜

```sql
SELECT
DATE('now');
```

---

현재 날짜와 시간을 함께 출력

```sql
SELECT
DATETIME('now');
```

---

# 실습 9

현재 날짜를 출력하시오.

---

# 10. 날짜 계산

내일

```sql
SELECT
DATE('now','+1 day');
```

일주일 후

```sql
SELECT
DATE('now','+7 day');
```

30일 전

```sql
SELECT
DATE('now','-30 day');
```

---

# 실습 10

오늘부터 100일 후 날짜를 출력하시오.

---

# 11. IFNULL()

NULL이면 다른 값 출력

```sql
SELECT
IFNULL(NULL,'없음');
```

결과

```
없음
```

---

실습용

```sql
CREATE TABLE customer(
id INTEGER,
name TEXT,
phone TEXT
);

INSERT INTO customer VALUES
(1,'홍길동','010-1111-1111'),
(2,'김철수',NULL),
(3,'이영희','010-3333-3333');
```

조회

```sql
SELECT
name,
IFNULL(phone,'전화번호 없음')
FROM customer;
```

---

# 실습 11

전화번호가 NULL이면

```
미등록
```

으로 출력하시오.

---

# 12. 총 생산금액 계산

```sql
SELECT
product_name,
quantity,
price,
quantity * price AS total_price
FROM production;
```

결과

| 제품    | 수량  | 단가   | 총금액    |
| ----- | --- | ---- | ------ |
| Motor | 100 | 5000 | 500000 |

---

# 실습 12

모든 제품의 총 생산금액을 계산하시오.

---

# 13. 부가세 계산

부가세를 10%라고 가정

```sql
SELECT
product_name,
price,
ROUND(price*0.1,0) AS vat
FROM production;
```

---

# 실습 13

제품명

단가

부가세

총금액(단가+부가세)

를 출력하시오.

---

# 14. 제조업 응용 실습

다음과 같이 출력하시오.

```
Motor 생산수량 : 100개

단가 : 5000원

총금액 : 500000원
```

힌트

```
||

AS
```

를 활용한다.

---

# 종합 실습 1

생산일이

```
2026-06-22
```

인 제품에 대해

* 제품명
* 글자수
* 생산금액

을 출력하시오.

---

# 종합 실습 2

총 생산금액이 큰 순으로 출력하시오.

출력

* 제품명
* 수량
* 단가
* 총금액

---

# 종합 실습 3

다음 형식으로 출력하시오.

```
Motor(100개) - 500000원

Sensor(200개) - 2400000원

Pump(150개) - 1200000원
```

힌트

```
||

ROUND()

quantity * price
```

---

# 이번 장 핵심 정리

| 함수         | 설명        |   |        |
| ---------- | --------- | - | ------ |
| LENGTH()   | 문자열 길이    |   |        |
| UPPER()    | 대문자       |   |        |
| LOWER()    | 소문자       |   |        |
| SUBSTR()   | 문자열 추출    |   |        |
| REPLACE()  | 문자열 치환    |   |        |
| ROUND()    | 반올림       |   |        |
| ABS()      | 절댓값       |   |        |
| DATE()     | 현재 날짜     |   |        |
| DATETIME() | 현재 날짜와 시간 |   |        |
| IFNULL()   | NULL 처리   |   |        |
| `          |           | ` | 문자열 연결 |

> **실무 팁**
>
> 제조 데이터에서는 SQL 함수만으로도 보고서 형태의 결과를 만들 수 있습니다. 예를 들어 `quantity * price`로 생산금액을 계산하고, `ROUND()`로 반올림하며, `||`를 사용해 "Motor(100개) - 500000원"과 같은 사람이 읽기 쉬운 형식으로 출력할 수 있습니다. 이러한 기능은 별도의 프로그램 없이도 데이터 분석과 보고에 매우 유용하게 활용됩니다.
