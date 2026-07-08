# Chapter 10. SQLite 연동

---

# 학습 목표

이번 장에서는 Streamlit과 SQLite를 연동하여 실제 업무 프로그램을 만드는 방법을 학습한다.

이전 장에서는 CSV 파일을 이용하여 데이터를 조회하였다.

하지만 실제 업무에서는

- 고객정보
- 제품정보
- 생산실적
- LOT
- 작업지시
- 재고관리

등의 데이터를 **데이터베이스(DBMS)** 에 저장한다.

이번 장을 마치면 다음과 같은 프로그램을 만들 수 있다.

- 고객 관리
- 제품 관리
- 직원 관리
- 생산실적 관리
- Mini MES

이번 장에서 학습할 내용

- sqlite3
- 데이터베이스 연결
- CRUD
- SELECT
- INSERT
- UPDATE
- DELETE

---

# 1. 왜 데이터베이스를 사용할까?

CSV 파일도 데이터를 저장할 수 있다.

하지만 다음과 같은 문제가 있다.

```
동시에 여러 사람이 사용할 수 없다.

검색 속도가 느리다.

수정하기 어렵다.

데이터가 중복되기 쉽다.

관계(Relation)를 표현하기 어렵다.
```

그래서 대부분의 업무 프로그램은 데이터베이스를 사용한다.

---

# CSV와 SQLite 비교

|항목|CSV|SQLite|
|----|----|-------|
|데이터 저장|가능|가능|
|검색|느림|빠름|
|수정|어려움|쉬움|
|삭제|어려움|쉬움|
|동시 접근|거의 불가능|가능|
|관계형 모델|불가능|가능|

---

# 2. SQLite란?

SQLite는 가장 가벼운 관계형 데이터베이스(RDBMS)이다.

특징

- 설치가 필요 없다.
- 하나의 파일(.db)에 저장된다.
- Python 기본 라이브러리이다.
- 속도가 매우 빠르다.
- 학습용과 실무용 모두 사용된다.

예)

```
customer.db
```

---

# 3. sqlite3 모듈

Python에서는 기본으로 제공된다.

```python
import sqlite3
```

별도의 설치가 필요 없다.

---

# 4. 데이터베이스 연결

```python
import sqlite3

conn = sqlite3.connect("customer.db")
```

파일이 없으면 자동 생성된다.

---

# Cursor 생성

SQL을 실행하기 위해 Cursor를 만든다.

```python
cur = conn.cursor()
```

---

# 연결 종료

```python
conn.close()
```

---

# 5. 테이블 생성

```python
cur.execute("""

CREATE TABLE IF NOT EXISTS customers
(
    customer_id INTEGER PRIMARY KEY,

    customer_name TEXT,

    phone TEXT
)

""")

conn.commit()
```

---

# 6. CRUD란?

CRUD는 업무 프로그램의 가장 기본 기능이다.

|기능|SQL|
|----|----|
|Create|INSERT|
|Read|SELECT|
|Update|UPDATE|
|Delete|DELETE|

거의 모든 ERP, MES, 쇼핑몰, 은행 시스템은 CRUD를 기반으로 동작한다.

---

# 7. INSERT

데이터 추가

```python
cur.execute(

"""

INSERT INTO customers

(customer_name, phone)

VALUES (?,?)

""",

("홍길동","010-1111-1111")

)

conn.commit()
```

---

# 8. SELECT

데이터 조회

```python
cur.execute(

"""

SELECT *

FROM customers

"""

)

rows = cur.fetchall()

print(rows)
```

---

# 9. UPDATE

데이터 수정

```python
cur.execute(

"""

UPDATE customers

SET phone=?

WHERE customer_id=?

""",

("010-9999-9999",1)

)

conn.commit()
```

---

# 10. DELETE

데이터 삭제

```python
cur.execute(

"""

DELETE

FROM customers

WHERE customer_id=?

""",

(1,)

)

conn.commit()
```

---

# 11. Streamlit과 SQLite

SQLite와 Streamlit을 함께 사용하면

```
사용자 입력

↓

SQLite 저장

↓

조회

↓

수정

↓

삭제
```

형태의 업무 프로그램을 만들 수 있다.

---

# 실습 1. 고객 등록

```python
import sqlite3

conn = sqlite3.connect("customer.db")

cur = conn.cursor()

cur.execute(

"""

INSERT INTO customers

(customer_name,phone)

VALUES (?,?)

""",

("이순신","010-2222-2222")

)

conn.commit()

conn.close()
```

---

# 실습 2. 고객 조회

```python
import sqlite3
import pandas as pd

conn = sqlite3.connect("customer.db")

df = pd.read_sql(

"""

SELECT *

FROM customers

""",

conn

)

print(df)

conn.close()
```

---

# 실습 3. 고객 수정

```python
cur.execute(

"""

UPDATE customers

SET phone=?

WHERE customer_id=?

""",

("010-0000-0000",2)

)

conn.commit()
```

---

# 실습 4. 고객 삭제

```python
cur.execute(

"""

DELETE

FROM customers

WHERE customer_id=?

""",

(2,)

)

conn.commit()
```

---

# 종합 실습 : 고객 관리 프로그램

기능

```
고객 등록

↓

고객 조회

↓

고객 수정

↓

고객 삭제
```

---

예제 화면

```
고객관리

--------------------

고객명

전화번호

[등록]

--------------------

고객 목록

--------------------

수정

삭제
```

---

```python
import streamlit as st
import sqlite3
import pandas as pd

conn = sqlite3.connect("customer.db")

name = st.text_input("고객명")

phone = st.text_input("전화번호")

if st.button("등록"):

    conn.execute(

        """

        INSERT INTO customers

        (customer_name,phone)

        VALUES (?,?)

        """,

        (name,phone)

    )

    conn.commit()

df = pd.read_sql(

"""

SELECT *

FROM customers

ORDER BY customer_id

""",

conn

)

st.dataframe(df)

conn.close()
```

---

# 실무 TIP

SQLite 작업은 다음 순서를 항상 기억하자.

```
Connect

↓

Cursor

↓

Execute

↓

Commit

↓

Close
```

이 순서는 PostgreSQL, MySQL에서도 거의 동일하다.

---

# SQLite 작업 순서

```
프로그램 시작

↓

DB 연결

↓

SQL 실행

↓

Commit

↓

DB 종료
```

---

# 핵심 정리

✔ SQLite는 Python 기본 내장 데이터베이스이다.

✔ sqlite3 모듈을 이용하여 데이터베이스를 사용할 수 있다.

✔ CRUD는 대부분의 업무 프로그램에서 가장 중요한 기능이다.

✔ INSERT는 데이터를 추가한다.

✔ SELECT는 데이터를 조회한다.

✔ UPDATE는 데이터를 수정한다.

✔ DELETE는 데이터를 삭제한다.

✔ Streamlit과 SQLite를 연동하면 실제 업무 프로그램을 만들 수 있다.

---

# 연습 문제

## 문제 1

customer.db를 생성하시오.

---

## 문제 2

customers 테이블을 생성하시오.

---

## 문제 3

고객을 등록하는 프로그램을 작성하시오.

---

## 문제 4

고객 목록을 조회하는 프로그램을 작성하시오.

---

## 문제 5

Streamlit을 이용하여 고객관리 CRUD 프로그램을 작성하시오.

---

# 다음 장 예고

다음 장에서는 SQLite와 Streamlit을 이용하여 **Mini MES 프로젝트**를 구현한다.

학습 내용

- 고객관리
- 제품관리
- 생산관리
- 작업지시
- 생산실적
- Dashboard

지금까지 학습한 Streamlit, Pandas, SQLite를 하나의 프로젝트로 통합하여 실제 업무 시스템과 유사한 Mini MES를 완성한다.
