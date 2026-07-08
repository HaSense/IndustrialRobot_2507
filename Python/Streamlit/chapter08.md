# Chapter 8. Pandas 연동

---

# 학습 목표

이번 장에서는 Streamlit과 Pandas를 함께 사용하는 방법을 학습한다.

Pandas는 Python에서 가장 많이 사용하는 데이터 분석 라이브러리이며,
CSV, Excel, 데이터베이스의 데이터를 매우 쉽게 처리할 수 있다.

이번 장을 마치면 다음과 같은 프로그램을 만들 수 있다.

- 생산실적 조회
- 품질 데이터 조회
- 불량률 조회
- 제품 검색
- 생산량 정렬

이번 장에서 학습할 내용

- CSV 읽기
- DataFrame
- 필터링(Filter)
- 정렬(Sort)
- 검색(Search)

---

# 1. Pandas란?

Pandas는 Python에서 데이터를 처리하기 위한 대표적인 라이브러리이다.

주요 기능

- CSV 읽기
- Excel 읽기
- 데이터 검색
- 데이터 정렬
- 데이터 그룹화
- 데이터 통계
- 데이터 분석

AI, 데이터 분석, 스마트팩토리, MES 프로젝트에서 가장 많이 사용된다.

---

# 2. Pandas 설치

설치

```bash
pip install pandas
```

버전 확인

```bash
pip show pandas
```

---

# 3. Pandas 가져오기

```python
import pandas as pd
```

관례적으로

```
pd
```

라는 별칭을 사용한다.

---

# 4. CSV 읽기

가장 많이 사용하는 함수이다.

```python
import pandas as pd

df = pd.read_csv("production.csv")
```

---

## CSV 예제

```text
date,product,production,defect

2026-07-01,A100,120,2

2026-07-02,A200,150,1

2026-07-03,A100,130,3
```

---

## 출력

```python
print(df)
```

또는

```python
st.dataframe(df)
```

---

# 5. DataFrame

Pandas에서 가장 중요한 자료구조이다.

쉽게 말하면

```
테이블(Table)
```

이라고 생각하면 된다.

예)

|date|product|production|
|----|--------|----------|
|2026-07-01|A100|120|
|2026-07-02|A200|150|

---

## 컬럼 선택

```python
df["production"]
```

---

## 여러 컬럼 선택

```python
df[
    [
        "product",
        "production"
    ]
]
```

---

# 6. 필터링(Filter)

조건에 맞는 데이터만 조회한다.

예)

생산량이 130 이상

```python
result = df[
    df["production"] >= 130
]

st.dataframe(result)
```

---

## 제품이 A100인 데이터

```python
result = df[
    df["product"] == "A100"
]
```

---

## 여러 조건

```python
result = df[

    (df["production"] >= 130)

    &

    (df["defect"] <= 2)

]
```

---

# 7. 정렬(Sort)

생산량 기준 오름차순

```python
df.sort_values(
    by="production"
)
```

---

## 내림차순

```python
df.sort_values(

    by="production",

    ascending=False

)
```

---

## 여러 컬럼 정렬

```python
df.sort_values(

    by=[

        "product",

        "production"

    ]

)
```

---

# 8. 검색(Search)

사용자가 입력한 제품명을 검색한다.

```python
import streamlit as st

keyword = st.text_input(

    "제품명"

)

result = df[
    df["product"] == keyword
]

st.dataframe(result)
```

---

## 문자열 포함 검색

```python
result = df[

    df["product"].str.contains(

        keyword,

        case=False,

        na=False

    )

]
```

---

# 9. 데이터 확인 함수

상위 데이터

```python
df.head()
```

---

하위 데이터

```python
df.tail()
```

---

컬럼 정보

```python
df.columns
```

---

데이터 개수

```python
len(df)
```

---

기본 통계

```python
df.describe()
```

---

# 자주 사용하는 함수

|함수|설명|
|----|----|
|read_csv()|CSV 읽기|
|head()|상위 데이터|
|tail()|하위 데이터|
|describe()|기본 통계|
|sort_values()|정렬|
|columns|컬럼 조회|

---

# 실습 1. CSV 읽기

```python
import streamlit as st
import pandas as pd

df = pd.read_csv(
    "production.csv"
)

st.dataframe(df)
```

---

# 실습 2. 생산량 정렬

```python
result = df.sort_values(

    by="production",

    ascending=False

)

st.dataframe(result)
```

---

# 실습 3. 제품 검색

```python
keyword = st.text_input(

    "제품명"

)

result = df[

    df["product"] == keyword

]

st.dataframe(result)
```

---

# 종합 실습 : 불량률 조회

예제 CSV

```text
date,product,production,defect_rate

2026-07-01,A100,120,2.1

2026-07-02,A200,150,1.5

2026-07-03,A100,140,2.8

2026-07-04,A300,160,1.1

2026-07-05,A100,170,2.4
```

---

```python
import streamlit as st
import pandas as pd

st.title("불량률 조회")

df = pd.read_csv(

    "production.csv"

)

keyword = st.text_input(

    "제품명"

)

if keyword:

    result = df[

        df["product"] == keyword

    ]

    st.dataframe(result)

else:

    st.dataframe(df)

st.subheader("평균 불량률")

st.write(

    df["defect_rate"].mean()

)
```

---

# 도전 실습 1

생산량이 150 이상인 데이터만 출력해 보자.

---

# 도전 실습 2

제품명을 입력하면 해당 제품만 검색되도록 작성해 보자.

---

# 도전 실습 3

생산량 기준 내림차순으로 정렬하여 출력해 보자.

---

# 도전 실습 4

평균 생산량과 최대 생산량을 계산하여 출력해 보자.

---

# 도전 실습 5

제품명을 선택하면

- 생산량
- 불량률
- 평균 생산량

을 함께 출력하는 프로그램을 작성해 보자.

---

# 실무 활용 사례

Pandas는 다음과 같은 업무에서 자주 사용된다.

- 생산실적 분석
- 품질 데이터 분석
- 재고 조회
- ERP 데이터 분석
- MES 데이터 조회
- LOT 추적
- 설비 로그 분석
- AI 학습 데이터 전처리

---

# 핵심 정리

✔ Pandas는 Python의 대표적인 데이터 분석 라이브러리이다.

✔ `pd.read_csv()`를 이용하여 CSV 파일을 읽을 수 있다.

✔ DataFrame은 테이블 형태의 데이터를 저장하는 자료구조이다.

✔ 조건식을 이용하여 원하는 데이터만 필터링할 수 있다.

✔ `sort_values()`를 이용하여 데이터를 정렬할 수 있다.

✔ 문자열 검색 기능을 이용하여 원하는 제품이나 데이터를 쉽게 찾을 수 있다.

---

# 연습 문제

## 문제 1

CSV 파일을 읽어 DataFrame으로 출력하시오.

---

## 문제 2

생산량이 150 이상인 데이터만 출력하시오.

---

## 문제 3

생산량 기준 내림차순으로 정렬하시오.

---

## 문제 4

제품명이 A100인 데이터만 검색하시오.

---

## 문제 5

불량률 CSV를 읽어

- 평균 생산량
- 평균 불량률
- 최대 생산량
- 최소 생산량

을 출력하는 프로그램을 작성하시오.

---

# 다음 장 예고

다음 장에서는 **차트(Charts)** 를 이용하여 데이터를 시각화하는 방법을 학습한다.

학습 내용

- Line Chart
- Bar Chart
- Area Chart
- Scatter Chart
- Plotly

CSV와 Pandas로 읽은 데이터를 다양한 차트로 표현하여 Mini MES Dashboard를 구현해 본다.
