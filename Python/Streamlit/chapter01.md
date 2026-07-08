# Chapter 1. Streamlit 소개

---

# 학습 목표

이번 장에서는 Streamlit이 무엇인지 이해하고, 개발 환경을 구축한 후 첫 번째 웹 애플리케이션을 실행할 수 있다.

학습이 끝나면 다음과 같은 작업을 할 수 있다.

- Streamlit의 특징을 설명할 수 있다.
- Streamlit을 설치할 수 있다.
- Streamlit 프로젝트를 생성할 수 있다.
- 첫 번째 웹 페이지를 실행할 수 있다.

---

# 1. Streamlit이란?

Streamlit은 **Python만으로 웹 애플리케이션을 만들 수 있는 오픈소스 프레임워크**이다.

기존 웹 개발은 HTML, CSS, JavaScript를 함께 사용해야 했지만,
Streamlit은 Python 코드만으로 화면을 만들 수 있다.

대표적인 활용 분야는 다음과 같다.

- 데이터 분석 결과 시각화
- 머신러닝 모델 데모
- AI 챗봇
- 대시보드
- MES 모니터링 화면
- 생산 현황 조회 시스템
- 간단한 업무용 웹 프로그램

즉,

> **Python 개발자가 가장 빠르게 웹 서비스를 만들 수 있는 프레임워크**라고 생각하면 된다.

---

# 2. Streamlit의 특징

## 매우 적은 코드

다음 한 줄만으로 웹 화면을 만들 수 있다.

```python
import streamlit as st

st.title("Hello Streamlit")
```

실행하면 브라우저에서 바로 확인할 수 있다.

---

## HTML을 몰라도 된다.

기존 웹 개발

```
Python
↓

Flask

↓

HTML

↓

CSS

↓

JavaScript

↓

웹 브라우저
```

Streamlit

```
Python

↓

Streamlit

↓

웹 브라우저
```

복잡한 프론트엔드 개발 없이도
웹 화면을 만들 수 있다.

---

## 빠른 개발 속도

예를 들어

MES 생산현황

- 생산량
- 불량률
- 설비상태

를 보여주는 페이지를 만든다면

Flask는

- HTML 작성
- CSS 작성
- JavaScript 작성

등이 필요하지만

Streamlit은 Python 코드만 작성하면 된다.

---

## 데이터 분석 라이브러리와 궁합이 좋다.

대표적으로

- Pandas
- NumPy
- Matplotlib
- Plotly
- OpenCV
- Scikit-learn
- TensorFlow
- PyTorch

등과 매우 잘 연동된다.

---

# 3. 왜 Flask, Django 대신 Streamlit인가?

많은 학생들이 이런 질문을 한다.

> "웹 개발은 Flask나 Django를 배우는 것이 아닌가요?"

물론 그렇다.

하지만 목적이 다르다.

|항목|Streamlit|Flask|Django|
|----|---------|------|-------|
|난이도|★★★★★ 쉬움|중간|어려움|
|HTML 필요|거의 없음|필요|필요|
|JavaScript|거의 없음|필요|필요|
|개발 속도|매우 빠름|보통|느림|
|AI 프로젝트|매우 적합|적합|적합|
|대규모 서비스|부적합|가능|매우 적합|

---

## Streamlit이 적합한 경우

- AI 데모
- 데이터 분석
- 강의 실습
- MES 시연
- 관리자 페이지
- 사내 업무 프로그램

---

## Flask가 적합한 경우

- REST API
- 웹 백엔드
- React와 연동

---

## Django가 적합한 경우

- 쇼핑몰
- 회원관리
- 게시판
- 기업용 웹서비스

---

# 4. 설치

먼저 Python이 설치되어 있어야 한다.

버전을 확인한다.

```bash
python --version
```

또는

```bash
python3 --version
```

---

## pip 확인

```bash
pip --version
```

---

## Streamlit 설치

```bash
pip install streamlit
```

설치가 완료되면

```bash
streamlit --version
```

으로 정상 설치 여부를 확인할 수 있다.

예)

```
Streamlit, version 1.xx.x
```

---

# 5. 첫 번째 프로젝트

새 폴더를 만든다.

```
streamlit_basic/
```

그 안에

```
app.py
```

파일을 만든다.

다음 코드를 입력한다.

```python
import streamlit as st

st.title("Hello Streamlit!")

st.write("첫 번째 Streamlit 프로그램입니다.")
```

---

# 6. 실행 방법

터미널에서

```bash
streamlit run app.py
```

를 실행한다.

예시

```bash
cd streamlit_basic

streamlit run app.py
```

그러면 자동으로 브라우저가 실행된다.

주소는 보통 다음과 같다.

```
http://localhost:8501
```

브라우저를 열면

```
Hello Streamlit!

첫 번째 Streamlit 프로그램입니다.
```

가 출력된다.

---

# 7. 프로젝트 구조

보통 다음과 같이 구성한다.

```
project/

│
├── app.py
│
├── pages/
│
├── data/
│
├── db/
│
├── images/
│
├── utils/
│
└── requirements.txt
```

---

## app.py

프로그램의 시작 파일

```
웹이 시작되는 메인 페이지
```

---

## pages/

멀티 페이지를 저장한다.

예)

```
pages/

01_dashboard.py

02_customer.py

03_inventory.py
```

---

## data/

CSV

Excel

JSON

등 데이터를 저장한다.

예)

```
sales.csv

employees.csv

product.xlsx
```

---

## db/

SQLite 데이터베이스 등을 저장한다.

예)

```
factory.db

mes.db
```

---

## images/

이미지 파일

```
logo.png

factory.jpg
```

---

## utils/

공통 함수

예)

```
db.py

chart.py

config.py
```

---

## requirements.txt

필요한 라이브러리를 기록한다.

예)

```
streamlit
pandas
plotly
openpyxl
sqlite3
```

실제 프로젝트에서는

```bash
pip freeze > requirements.txt
```

명령으로 생성하는 경우가 많다.

---

# 실습 1

새 프로젝트를 만든다.

```
streamlit_basic
```

생성 후

```
app.py
```

를 만든다.

다음 코드를 입력한다.

```python
import streamlit as st

st.title("My First Streamlit")

st.write("안녕하세요.")
```

실행한다.

```bash
streamlit run app.py
```

---

# 실습 2

다음 내용을 출력하도록 수정해 보자.

```
스마트팩토리 MES

현재 생산량

오늘 불량률

설비 가동률
```

힌트

```python
st.title()

st.header()

st.write()
```

---

# 실습 3

프로젝트를 다음과 같이 구성해 보자.

```
mes_project/

app.py

pages/

data/

db/

images/

utils/
```

---

# 핵심 정리

✔ Streamlit은 Python만으로 웹을 만들 수 있는 프레임워크이다.

✔ HTML, CSS, JavaScript를 거의 작성하지 않아도 된다.

✔ 데이터 분석과 AI 프로젝트에 매우 적합하다.

✔ 실행 명령은 다음과 같다.

```bash
streamlit run app.py
```

✔ 기본 프로젝트 구조는 다음과 같다.

```
project/

app.py

pages/

data/

db/

images/

utils/
```

---

# 연습 문제

## 문제 1

Streamlit의 가장 큰 장점은 무엇인가?

---

## 문제 2

Streamlit을 실행하는 명령어를 작성하시오.

---

## 문제 3

프로젝트에서 여러 화면을 저장하는 폴더 이름은?

---

## 문제 4

SQLite 데이터베이스 파일은 일반적으로 어느 폴더에 저장하는가?

---

## 문제 5

Flask와 Streamlit의 가장 큰 차이점을 설명하시오.
