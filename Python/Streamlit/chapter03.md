# Chapter 3. 입력 컴포넌트(Input Components)

---

# 학습 목표

이번 장에서는 사용자가 웹 페이지에 데이터를 입력할 수 있도록 하는 **입력 컴포넌트(Input Widget)** 를 학습한다.

이번 장을 마치면 다음과 같은 프로그램을 만들 수 있다.

- 직원 등록 화면
- 회원 가입 화면
- 제품 등록 화면
- 고객 정보 입력 화면
- 생산 정보 입력 화면

이번 장에서 학습할 컴포넌트는 다음과 같다.

- `st.text_input()`
- `st.number_input()`
- `st.date_input()`
- `st.checkbox()`
- `st.radio()`
- `st.selectbox()`
- `st.multiselect()`
- `st.slider()`

---

# 1. 입력 컴포넌트란?

웹 프로그램은 단순히 정보를 출력하는 것만으로는 충분하지 않다.

사용자로부터 데이터를 입력받아야 한다.

예를 들어

- 로그인
- 회원가입
- 직원등록
- 주문등록
- 제품등록
- 생산실적 입력

모두 입력 컴포넌트를 사용한다.

Streamlit은 Python 코드만으로 이러한 입력 화면을 쉽게 만들 수 있다.

---

# 2. st.text_input()

문자열을 입력받는다.

가장 많이 사용하는 입력 컴포넌트이다.

```python
import streamlit as st

name = st.text_input("이름")

st.write(name)
```

실행 결과

```
이름

____________________
```

입력하면 즉시 화면에 출력된다.

---

## 기본값 지정

```python
name = st.text_input(
    "이름",
    value="홍길동"
)
```

---

## placeholder 사용

```python
name = st.text_input(
    "이름",
    placeholder="이름을 입력하세요."
)
```

---

# 3. st.number_input()

숫자를 입력받는다.

```python
age = st.number_input("나이")
```

---

## 최소값과 최대값

```python
age = st.number_input(
    "나이",
    min_value=20,
    max_value=60
)
```

---

## 증가 단위 지정

```python
salary = st.number_input(
    "연봉",
    step=100
)
```

---

# 4. st.date_input()

날짜를 입력받는다.

```python
birthday = st.date_input("입사일")
```

---

## 결과 출력

```python
st.write(birthday)
```

---

# 5. st.checkbox()

체크 여부를 선택한다.

```python
agree = st.checkbox("동의합니다.")
```

---

## 예제

```python
if agree:
    st.write("동의 완료")
else:
    st.write("동의하지 않음")
```

---

# 6. st.radio()

여러 개 중 하나를 선택한다.

```python
gender = st.radio(
    "성별",
    ["남성", "여성"]
)
```

---

## 예제

```python
st.write(gender)
```

---

# 7. st.selectbox()

드롭다운 메뉴이다.

```python
dept = st.selectbox(
    "부서",
    [
        "생산팀",
        "품질팀",
        "설비팀",
        "AI개발팀"
    ]
)
```

---

## 결과 출력

```python
st.write(dept)
```

---

# 8. st.multiselect()

여러 개를 선택할 수 있다.

```python
skills = st.multiselect(
    "보유 기술",
    [
        "Python",
        "C++",
        "SQL",
        "PLC",
        "ROS"
    ]
)
```

---

## 결과 출력

```python
st.write(skills)
```

---

# 9. st.slider()

범위를 선택한다.

```python
score = st.slider(
    "평가 점수",
    0,
    100
)
```

---

## 범위 지정

```python
experience = st.slider(
    "경력(년)",
    0,
    30,
    5
)
```

---

# 입력 컴포넌트 비교

|컴포넌트|용도|
|---------|----|
|text_input|문자 입력|
|number_input|숫자 입력|
|date_input|날짜 입력|
|checkbox|체크 여부|
|radio|한 개 선택|
|selectbox|드롭다운 선택|
|multiselect|여러 개 선택|
|slider|범위 선택|

---

# 실습 1

간단한 회원 정보를 입력받아 출력해 보자.

```python
import streamlit as st

name = st.text_input("이름")

age = st.number_input(
    "나이",
    min_value=20,
    max_value=70
)

st.write(name)
st.write(age)
```

---

# 실습 2

부서를 선택해 보자.

```python
dept = st.selectbox(
    "부서",
    [
        "생산",
        "품질",
        "설비",
        "영업"
    ]
)

st.write(dept)
```

---

# 실습 3

보유 기술을 선택해 보자.

```python
skills = st.multiselect(
    "보유 기술",
    [
        "Python",
        "SQL",
        "PLC",
        "AI",
        "Linux"
    ]
)

st.write(skills)
```

---

# 종합 실습 : 직원 등록 화면 만들기

이번 장에서 배운 입력 컴포넌트를 이용하여 직원 등록 화면을 만들어 보자.

파일 이름

```
app.py
```

```python
import streamlit as st

st.title("직원 등록")

st.markdown("---")

emp_name = st.text_input(
    "직원 이름",
    placeholder="이름을 입력하세요."
)

emp_no = st.number_input(
    "사번",
    min_value=1000,
    step=1
)

hire_date = st.date_input("입사일")

gender = st.radio(
    "성별",
    [
        "남성",
        "여성"
    ]
)

department = st.selectbox(
    "부서",
    [
        "생산팀",
        "품질팀",
        "설비팀",
        "AI개발팀",
        "경영지원팀"
    ]
)

skills = st.multiselect(
    "보유 기술",
    [
        "Python",
        "SQL",
        "PLC",
        "OpenCV",
        "ROS",
        "Linux"
    ]
)

career = st.slider(
    "경력(년)",
    0,
    30,
    1
)

is_active = st.checkbox(
    "현재 재직 중"
)

st.markdown("---")

st.header("입력 결과")

st.write("직원명 :", emp_name)
st.write("사번 :", emp_no)
st.write("입사일 :", hire_date)
st.write("성별 :", gender)
st.write("부서 :", department)
st.write("보유기술 :", skills)
st.write("경력 :", career)
st.write("재직여부 :", is_active)
```

---

# 도전 실습 1

직원 등록 화면에 다음 항목을 추가해 보자.

- 이메일
- 전화번호
- 주소
- 직급
- 연봉

---

# 도전 실습 2

다음 화면을 만들어 보자.

```
제품 등록 화면

제품명

제품번호

생산일

제품 종류

사용 가능한 설비

품질 등급

재고 여부
```

---

# 도전 실습 3

고객 등록 화면을 만들어 보자.

입력 항목

- 고객명
- 전화번호
- 주소
- 거래 시작일
- 거래 상태
- 구매 제품
- 거래 등급

---

# 핵심 정리

✔ `st.text_input()`은 문자열을 입력받는다.

✔ `st.number_input()`은 숫자를 입력받는다.

✔ `st.date_input()`은 날짜를 선택한다.

✔ `st.checkbox()`는 참(True) 또는 거짓(False)을 선택한다.

✔ `st.radio()`는 여러 항목 중 하나를 선택한다.

✔ `st.selectbox()`는 드롭다운 목록을 제공한다.

✔ `st.multiselect()`는 여러 항목을 동시에 선택할 수 있다.

✔ `st.slider()`는 일정 범위의 값을 선택할 때 사용한다.

---

# 연습 문제

## 문제 1

직원 이름을 입력받는 입력창을 작성하시오.

---

## 문제 2

20~60세 사이의 나이를 입력받는 숫자 입력창을 작성하시오.

---

## 문제 3

생산팀, 품질팀, 설비팀 중 하나를 선택하는 드롭다운을 작성하시오.

---

## 문제 4

Python, SQL, PLC를 여러 개 선택할 수 있는 입력 컴포넌트를 작성하시오.

---

## 문제 5

오늘 배운 모든 입력 컴포넌트를 이용하여 **직원 등록 화면** 또는 **회원 가입 화면**을 작성해 보시오.

---

# 다음 장 예고

다음 장에서는 **버튼과 이벤트 처리**를 학습한다.

학습 내용

- `st.button()`
- `st.form()`
- `st.form_submit_button()`
- 입력 데이터 검증
- 등록 버튼 구현
- 초기화 버튼 구현
- 간단한 CRUD 화면 만들기

이를 통해 입력받은 데이터를 실제 프로그램처럼 처리하는 방법을 익히게 된다.
