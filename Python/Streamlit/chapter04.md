# Chapter 4. 버튼과 이벤트(Button & Event)

---

# 학습 목표

이번 장에서는 **버튼(Button)** 과 **이벤트(Event)** 의 개념을 이해하고,
사용자의 입력을 처리하는 방법을 학습한다.

이번 장을 마치면 다음과 같은 프로그램을 만들 수 있다.

- 로그인 화면
- 회원 가입 화면
- 직원 등록 화면
- 제품 등록 화면
- 데이터 저장 버튼 구현

이번 장에서 학습할 내용

- `st.button()`
- `st.form()`
- `st.form_submit_button()`

---

# 1. 이벤트(Event)란?

웹 프로그램은 사용자의 행동에 따라 동작한다.

예를 들어

- 버튼 클릭
- 로그인
- 검색
- 저장
- 삭제
- 수정

모두 이벤트(Event)이다.

예를 들어 로그인 화면에서는

```
아이디 입력

↓

비밀번호 입력

↓

로그인 버튼 클릭

↓

로그인 처리
```

이러한 흐름으로 프로그램이 동작한다.

---

# 2. st.button()

가장 많이 사용하는 버튼이다.

버튼을 누르면 **True**를 반환한다.

```python
import streamlit as st

if st.button("클릭"):
    st.write("버튼이 눌렸습니다.")
```

실행 화면

```
[ 클릭 ]
```

버튼을 누르는 순간

```
버튼이 눌렸습니다.
```

가 출력된다.

---

## 버튼 반환값

```python
result = st.button("저장")

st.write(result)
```

버튼을 누르기 전

```
False
```

버튼을 누른 순간

```
True
```

---

# 3. 버튼과 입력창 함께 사용하기

```python
import streamlit as st

name = st.text_input("이름")

if st.button("확인"):
    st.write("입력한 이름 :", name)
```

실행 순서

```
이름 입력

↓

확인 버튼 클릭

↓

결과 출력
```

---

# 4. 여러 개의 버튼

```python
if st.button("등록"):
    st.write("등록")

if st.button("삭제"):
    st.write("삭제")

if st.button("수정"):
    st.write("수정")
```

실행 화면

```
[등록]

[삭제]

[수정]
```

---

# 5. st.form()

입력 컴포넌트를 하나의 그룹으로 묶는다.

실무에서는 매우 많이 사용하는 기능이다.

```python
import streamlit as st

with st.form("employee"):

    name = st.text_input("이름")

    age = st.number_input("나이")

    submit = st.form_submit_button("등록")
```

---

## Form을 사용하는 이유

Form을 사용하지 않으면

텍스트를 한 글자 입력할 때마다

Streamlit이 프로그램을 다시 실행한다.

예를 들어

```
홍

↓

홍길

↓

홍길동
```

입력할 때마다

프로그램이 다시 실행된다.

---

Form을 사용하면

```
입력 완료

↓

등록 버튼 클릭

↓

한 번만 실행
```

된다.

따라서

회원가입

직원등록

제품등록

로그인

등에서는 Form 사용을 권장한다.

---

# 6. st.form_submit_button()

Form 내부에서 사용하는 버튼이다.

```python
with st.form("member"):

    name = st.text_input("이름")

    submit = st.form_submit_button("가입")
```

---

## 결과 출력

```python
if submit:
    st.write(name)
```

---

# 7. 버튼과 Form의 차이

## 일반 버튼

```python
st.button("저장")
```

- 간단한 기능
- 조회 버튼
- 새로고침

등에 적합하다.

---

## Form

```python
with st.form():

    ...

    st.form_submit_button()
```

- 회원가입

- 로그인

- 직원등록

- 제품등록

- 주문등록

등 여러 입력을 한 번에 처리할 때 사용한다.

---

# Button와 Form 비교

|항목|Button|Form|
|----|------|----|
|사용 목적|간단한 이벤트|입력 데이터 처리|
|입력 그룹|불가능|가능|
|실무 사용 빈도|높음|매우 높음|
|회원가입|△|◎|
|로그인|△|◎|
|등록 화면|△|◎|

---

# 실습 1

버튼 클릭

```python
import streamlit as st

st.title("버튼 예제")

if st.button("클릭"):
    st.write("안녕하세요.")
```

---

# 실습 2

입력 후 버튼 클릭

```python
import streamlit as st

name = st.text_input("이름")

if st.button("확인"):
    st.success(name)
```

---

# 실습 3

Form 만들기

```python
import streamlit as st

with st.form("employee"):

    name = st.text_input("이름")

    age = st.number_input("나이")

    submit = st.form_submit_button("등록")

if submit:

    st.write(name)

    st.write(age)
```

---

# 종합 실습 : 로그인 화면 만들기

파일명

```
app.py
```

```python
import streamlit as st

st.title("MES 로그인")

st.markdown("---")

with st.form("login"):

    user_id = st.text_input(
        "아이디"
    )

    password = st.text_input(
        "비밀번호",
        type="password"
    )

    login = st.form_submit_button(
        "로그인"
    )

if login:

    if user_id == "admin" and password == "1234":

        st.success("로그인 성공")

    else:

        st.error("아이디 또는 비밀번호가 올바르지 않습니다.")
```

---

# 도전 실습 1

로그인 화면에 다음 항목을 추가해 보자.

- 로그인 유지

- 관리자 로그인

- 비밀번호 표시 여부

힌트

```
checkbox
```

---

# 도전 실습 2

회원 가입 화면을 만들어 보자.

입력 항목

- 아이디

- 비밀번호

- 이름

- 이메일

- 전화번호

- 가입 버튼

---

# 도전 실습 3

직원 등록 화면을 만들어 보자.

입력 항목

- 사번

- 이름

- 부서

- 입사일

- 등록 버튼

등록이 완료되면

```
등록되었습니다.
```

메시지를 출력하도록 작성해 보자.

---

# 핵심 정리

✔ `st.button()`은 가장 기본적인 버튼이다.

✔ 버튼을 클릭하면 `True`를 반환한다.

✔ `st.form()`은 여러 입력 컴포넌트를 하나의 그룹으로 묶는다.

✔ `st.form_submit_button()`은 Form 내부에서 사용하는 제출 버튼이다.

✔ 로그인, 회원가입, 직원등록과 같은 화면은 `Form`을 사용하는 것이 권장된다.

---

# 연습 문제

## 문제 1

버튼을 클릭하면 "Hello Streamlit"을 출력하는 프로그램을 작성하시오.

---

## 문제 2

이름을 입력받은 후 버튼을 누르면 이름을 출력하는 프로그램을 작성하시오.

---

## 문제 3

`st.form()`을 사용하는 이유를 설명하시오.

---

## 문제 4

아이디와 비밀번호를 입력받는 로그인 화면을 작성하시오.

---

## 문제 5

오늘 배운 내용을 이용하여 **회원가입 화면**, **직원 등록 화면**, 또는 **제품 등록 화면** 중 하나를 구현해 보시오.

---

# 실무 TIP

실제 업무에서는 로그인 버튼을 눌렀을 때 아이디와 비밀번호를 코드에 직접 비교하지 않는다.

일반적으로 다음과 같은 흐름으로 로그인 기능을 구현한다.

```
사용자 입력

↓

로그인 버튼 클릭

↓

SQLite 또는 PostgreSQL 조회

↓

사용자 존재 여부 확인

↓

로그인 성공 또는 실패
```

다음 장부터는 **SQLite 데이터베이스와 연동하여 로그인, 회원 관리, 직원 관리 등 실제 CRUD(Create, Read, Update, Delete) 프로그램을 구현**해 본다.
