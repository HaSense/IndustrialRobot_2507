# Chapter 7. 파일 업로드(File Upload)

---

# 학습 목표

이번 장에서는 Streamlit에서 파일을 업로드하는 방법을 학습한다.

실제 업무에서는 생산실적, 품질 데이터, 고객 정보 등을 CSV 또는 Excel 파일로 관리하는 경우가 많다.

Streamlit에서는 `st.file_uploader()`를 이용하여 이러한 파일을 쉽게 업로드하고 분석할 수 있다.

이번 장을 마치면 다음과 같은 프로그램을 만들 수 있다.

- 생산실적 CSV 업로드
- Excel 데이터 조회
- 이미지 미리보기
- PDF 업로드
- SQLite 저장용 데이터 업로드

이번 장에서 학습할 내용

- `st.file_uploader()`
- CSV 읽기
- Excel 읽기
- Image 업로드
- PDF 업로드

---

# 1. 파일 업로드란?

웹 프로그램에서는 사용자가 자신의 컴퓨터에 있는 파일을 서버로 업로드할 수 있다.

대표적인 예는 다음과 같다.

- 생산실적 CSV
- 제품 목록 Excel
- 고객 사진
- 계약서 PDF
- 품질 검사 결과

Streamlit에서는 `st.file_uploader()` 하나만으로 파일 업로드 기능을 구현할 수 있다.

---

# 2. st.file_uploader()

가장 기본적인 파일 업로드 함수이다.

```python
import streamlit as st

uploaded_file = st.file_uploader("파일 선택")
```

실행 화면

```
파일 선택

[파일 선택]
```

파일을 선택하면 Python 객체로 전달된다.

---

# 반환값

업로드 전

```python
None
```

업로드 후

```python
UploadedFile
```

따라서 일반적으로 다음과 같이 작성한다.

```python
if uploaded_file is not None:

    st.write("업로드 완료")
```

---

# 3. CSV 파일 업로드

CSV는 가장 많이 사용하는 데이터 파일 형식이다.

```python
import streamlit as st
import pandas as pd

uploaded_file = st.file_uploader(
    "CSV 선택",
    type="csv"
)

if uploaded_file is not None:

    df = pd.read_csv(uploaded_file)

    st.dataframe(df)
```

---

## CSV 예제

```text
date,production,defect

2026-07-01,120,2

2026-07-02,130,3

2026-07-03,140,1
```

---

# 4. Excel 파일 업로드

Excel도 매우 많이 사용한다.

```python
uploaded_file = st.file_uploader(

    "Excel 선택",

    type="xlsx"

)

if uploaded_file is not None:

    df = pd.read_excel(uploaded_file)

    st.dataframe(df)
```

---

## Excel 읽기

Excel 파일을 읽기 위해서는

```text
openpyxl
```

라이브러리가 필요하다.

설치

```bash
pip install openpyxl
```

---

# 5. 이미지 업로드

이미지 파일도 쉽게 업로드할 수 있다.

```python
uploaded_file = st.file_uploader(

    "이미지",

    type=["png","jpg","jpeg"]

)

if uploaded_file is not None:

    st.image(uploaded_file)
```

---

## 사용 사례

- 제품 사진

- 불량 사진

- 설비 사진

- 작업 현장

---

# 6. PDF 업로드

PDF 파일도 업로드할 수 있다.

```python
uploaded_file = st.file_uploader(

    "PDF",

    type="pdf"

)

if uploaded_file is not None:

    st.write(uploaded_file.name)

    st.write(uploaded_file.size)
```

---

## PDF 활용

- 작업표준서

- 검사성적서

- 계약서

- 매뉴얼

---

# 7. 여러 개의 파일 업로드

```python
files = st.file_uploader(

    "파일",

    accept_multiple_files=True

)

for file in files:

    st.write(file.name)
```

---

# 8. 업로드 가능한 파일 형식 제한

CSV만 허용

```python
type="csv"
```

여러 개 허용

```python
type=[

    "csv",

    "xlsx",

    "pdf"

]
```

---

# 파일 업로드 비교

|파일|읽기 방법|
|----|---------|
|CSV|`pd.read_csv()`|
|Excel|`pd.read_excel()`|
|Image|`st.image()`|
|PDF|파일 정보 또는 별도 라이브러리|

---

# 실습 1. CSV 업로드

```python
import streamlit as st
import pandas as pd

file = st.file_uploader(

    "CSV",

    type="csv"

)

if file is not None:

    df = pd.read_csv(file)

    st.dataframe(df)
```

---

# 실습 2. Excel 업로드

```python
import streamlit as st
import pandas as pd

file = st.file_uploader(

    "Excel",

    type="xlsx"

)

if file is not None:

    df = pd.read_excel(file)

    st.dataframe(df)
```

---

# 실습 3. 이미지 업로드

```python
import streamlit as st

image = st.file_uploader(

    "이미지",

    type=["jpg","png"]

)

if image is not None:

    st.image(image)
```

---

# 종합 실습 : 생산실적 CSV 업로드

예제 CSV

```text
date,product,production,defect

2026-07-01,A100,120,2

2026-07-02,A100,130,1

2026-07-03,A100,140,3
```

---

```python
import streamlit as st
import pandas as pd

st.title("생산실적 업로드")

file = st.file_uploader(

    "생산실적 CSV",

    type="csv"

)

if file is not None:

    df = pd.read_csv(file)

    st.success("업로드 완료")

    st.dataframe(df)

    st.subheader("총 생산량")

    st.write(

        df["production"].sum()

    )

    st.subheader("평균 불량")

    st.write(

        df["defect"].mean()

    )
```

---

# 도전 실습 1

업로드한 CSV를 이용하여

- 생산량 합계
- 평균 생산량
- 최대 생산량
- 최소 생산량

을 출력해 보자.

---

# 도전 실습 2

Excel 파일을 업로드한 후

품질 데이터만 출력해 보자.

---

# 도전 실습 3

업로드한 생산실적 CSV를 SQLite 데이터베이스에 저장해 보자.

---

# 실무 활용 사례

실제 업무에서는 다음과 같은 파일을 자주 업로드한다.

- ERP 생산실적
- MES 작업이력
- LOT 정보
- 품질 검사 결과
- 설비 로그
- 고객 목록
- 재고 현황

Streamlit은 이러한 파일을 업로드하여 데이터 분석이나 데이터베이스 저장 기능과 쉽게 연계할 수 있다.

---

# 핵심 정리

✔ `st.file_uploader()`는 파일 업로드 기능을 제공한다.

✔ CSV 파일은 `pd.read_csv()`로 읽는다.

✔ Excel 파일은 `pd.read_excel()`로 읽는다.

✔ 이미지는 `st.image()`로 표시할 수 있다.

✔ PDF는 파일 정보를 확인하거나 별도의 라이브러리와 함께 사용할 수 있다.

✔ 업로드한 데이터는 DataFrame으로 분석하거나 SQLite, PostgreSQL 등에 저장할 수 있다.

---

# 연습 문제

## 문제 1

CSV 파일을 업로드하여 DataFrame으로 출력하시오.

---

## 문제 2

Excel 파일을 업로드하여 내용을 출력하시오.

---

## 문제 3

이미지를 업로드하여 화면에 표시하시오.

---

## 문제 4

여러 개의 PDF 파일을 업로드하여 파일명을 출력하시오.

---

## 문제 5

생산실적 CSV를 업로드한 후

- 총 생산량
- 평균 생산량
- 평균 불량률

을 출력하는 프로그램을 작성하시오.

---

# 다음 장 예고

다음 장에서는 **SQLite 데이터베이스 연동**을 학습한다.

학습 내용

- sqlite3 연결
- CREATE
- INSERT
- SELECT
- UPDATE
- DELETE
- Streamlit + SQLite CRUD

업로드한 CSV 데이터를 SQLite에 저장하고, 웹 화면에서 조회하는 Mini MES를 구현해 본다.
