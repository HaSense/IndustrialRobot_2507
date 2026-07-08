# Chapter 2. 첫 번째 웹 만들기

---

# 학습 목표

이번 장에서는 Streamlit에서 가장 많이 사용하는 출력 함수를 익히고,
Python 코드만으로 간단한 웹 페이지를 만드는 방법을 학습한다.

학습이 끝나면 다음과 같은 작업을 할 수 있다.

- 제목과 부제목을 출력할 수 있다.
- 일반 텍스트를 출력할 수 있다.
- Markdown 문법을 사용할 수 있다.
- 코드 블록을 출력할 수 있다.
- 캡션을 작성할 수 있다.
- 간단한 회사 소개 페이지를 만들 수 있다.

---

# 1. Streamlit 출력 함수

웹 페이지는 결국 **사용자에게 정보를 보여주는 화면**이다.

Streamlit은 다양한 출력 함수를 제공한다.

이번 장에서 배울 함수는 다음과 같다.

|함수|설명|
|----|----|
|st.title()|가장 큰 제목|
|st.header()|중간 제목|
|st.subheader()|작은 제목|
|st.write()|일반 텍스트 및 객체 출력|
|st.markdown()|Markdown 문법 출력|
|st.code()|소스 코드 출력|
|st.caption()|작은 설명(캡션) 출력|

---

# 2. st.title()

가장 큰 제목을 출력한다.

웹 사이트의 메인 제목에 사용한다.

```python
import streamlit as st

st.title("스마트팩토리 MES")
```

실행 결과

```
스마트팩토리 MES
================
```

---

## 여러 개 작성 가능

```python
st.title("회사 소개")

st.title("제품 소개")
```

하지만 일반적으로 **페이지에는 하나의 title만 사용하는 것이 좋다.**

---

# 3. st.header()

중간 크기의 제목을 출력한다.

주제별 구분에 많이 사용한다.

```python
import streamlit as st

st.header("회사 소개")
```

---

## 예제

```python
st.title("ABC Company")

st.header("회사소개")

st.header("주요사업")

st.header("오시는 길")
```

---

# 4. st.subheader()

Header보다 작은 제목이다.

세부 항목을 구분할 때 사용한다.

```python
st.subheader("AI 사업부")
```

---

## 예제

```python
st.header("사업 분야")

st.subheader("AI")

st.subheader("스마트팩토리")

st.subheader("클라우드")
```

---

# 5. st.write()

가장 많이 사용하는 함수이다.

문자열은 물론 숫자, 리스트, DataFrame 등도 출력할 수 있다.

```python
st.write("안녕하세요.")
```

---

## 여러 줄 출력

```python
st.write("회사명 : ABC")

st.write("대표 : 홍길동")

st.write("설립 : 2020")
```

---

## 숫자 출력

```python
sales = 3500

st.write(sales)
```

---

## 리스트 출력

```python
items = ["PLC", "MES", "SCADA"]

st.write(items)
```

---

# 6. st.markdown()

Markdown 문법을 사용할 수 있다.

GitHub에서 사용하는 Markdown과 거의 동일하다.

```python
st.markdown("# 제목")
```

---

## 굵은 글씨

```python
st.markdown("**스마트팩토리**")
```

결과

**스마트팩토리**

---

## 기울임

```python
st.markdown("*Python*")
```

---

## 목록

```python
st.markdown("""
- MES
- SCADA
- ERP
""")
```

---

## 번호 목록

```python
st.markdown("""
1. 생산관리
2. 품질관리
3. 설비관리
""")
```

---

## 인용문

```python
st.markdown("> Streamlit은 매우 빠른 웹 개발 도구입니다.")
```

---

## 수평선

```python
st.markdown("---")
```

---

# 7. st.code()

소스 코드를 보기 좋게 출력한다.

```python
code = '''
for i in range(5):
    print(i)
'''

st.code(code)
```

---

## 언어 지정

```python
st.code(code, language="python")
```

---

## SQL 출력

```python
sql = '''
SELECT *
FROM customers;
'''

st.code(sql, language="sql")
```

---

# 8. st.caption()

작은 설명을 출력한다.

보통 이미지 설명이나 참고 문헌 등에 사용한다.

```python
st.caption("Version 1.0")
```

---

## 예제

```python
st.caption("Copyright © 2026 ABC Company")
```

---

# 출력 함수 비교

|함수|크기|용도|
|----|----|----|
|title|★★★★★|메인 제목|
|header|★★★★|큰 항목|
|subheader|★★★|세부 항목|
|write|★★|일반 내용|
|markdown|다양|Markdown 출력|
|code|코드|소스 코드|
|caption|작음|설명|

---

# 실습 1

다음 화면을 만들어 보자.

```python
import streamlit as st

st.title("스마트팩토리")

st.header("MES")

st.subheader("생산관리")

st.write("MES는 Manufacturing Execution System입니다.")

st.caption("Version 1.0")
```

---

# 실습 2

다음 Markdown을 출력해 보자.

```python
st.markdown("""
# Python

## Streamlit

- AI
- Dashboard
- MES

**Python Web**
""")
```

---

# 실습 3

다음 Python 코드를 출력해 보자.

```python
code = '''
for i in range(10):
    print(i)
'''

st.code(code, language="python")
```

---

# 실습 4

SQL 문장을 출력해 보자.

```python
sql = '''
SELECT *
FROM employee
WHERE dept='AI';
'''

st.code(sql, language="sql")
```

---

# 종합 실습 : 회사 소개 페이지 만들기

이번 장에서 배운 함수만 사용하여 회사 소개 페이지를 만들어 보자.

파일 이름

```
app.py
```

소스 코드

```python
import streamlit as st

st.title("Vertha Systems")

st.caption("AI · Smart Factory · Software Development")

st.markdown("---")

st.header("회사 소개")

st.write("""
Vertha Systems는 AI와 스마트팩토리 기술을 기반으로
기업의 디지털 전환(DX)과 AI 전환(AX)을 지원하는
소프트웨어 전문 기업입니다.
""")

st.header("주요 사업")

st.markdown("""
- AI 솔루션 개발
- 스마트팩토리 구축
- MES 개발
- 데이터 분석
- Python 교육
- 소프트웨어 컨설팅
""")

st.header("핵심 기술")

st.markdown("""
- Python
- Streamlit
- FastAPI
- SQLite
- PostgreSQL
- Pandas
- OpenCV
- AI Agent
""")

st.header("예제 코드")

code = '''
def hello():
    print("Welcome Vertha Systems")
'''

st.code(code, language="python")

st.header("연락처")

st.write("Email : contact@verthasys.co.kr")

st.caption("Copyright © 2026 Vertha Systems")
```

---

# 도전 실습

다음 항목을 추가하여 회사 소개 페이지를 완성해 보자.

- 회사 연혁
- 조직도
- 대표 인사말
- 서비스 소개
- 교육 과정
- 개발 기술
- 고객사
- 찾아오는 길
- 버전 정보

---

# 핵심 정리

✔ **st.title()**은 페이지의 가장 큰 제목을 출력한다.

✔ **st.header()**는 큰 항목을 구분한다.

✔ **st.subheader()**는 세부 항목을 구분한다.

✔ **st.write()**는 가장 많이 사용하는 출력 함수이며 문자열, 숫자, 리스트 등 다양한 객체를 출력할 수 있다.

✔ **st.markdown()**은 GitHub Markdown 문법을 그대로 사용할 수 있다.

✔ **st.code()**는 소스 코드를 보기 좋게 출력한다.

✔ **st.caption()**은 작은 설명이나 버전 정보를 출력할 때 사용한다.

---

# 연습 문제

## 문제 1

페이지의 가장 큰 제목을 출력하는 함수는 무엇인가?

---

## 문제 2

Markdown 문법을 출력하는 함수는 무엇인가?

---

## 문제 3

Python 코드를 보기 좋게 출력하는 함수는 무엇인가?

---

## 문제 4

다음과 같은 화면을 만들어 보시오.

```
회사 소개

사업 분야

AI

Python으로 개발합니다.
```

---

## 문제 5

오늘 배운 함수만 이용하여 **자신의 회사 또는 가상의 IT 회사를 소개하는 웹 페이지**를 작성해 보시오.
