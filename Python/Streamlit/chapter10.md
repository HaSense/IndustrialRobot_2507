# Chapter 6. Session State

---

# 학습 목표

이번 장에서는 웹 프로그램이 일반 프로그램과 어떻게 다른지 이해하고,
Streamlit에서 상태(State)를 유지하는 방법을 학습한다.

이번 장을 마치면 다음과 같은 프로그램을 만들 수 있다.

- 로그인 유지
- 장바구니
- 주문 내역 관리
- 조회 조건 유지
- 방문 횟수 카운터

이번 장에서 학습할 내용

- 웹이 변수값을 기억하지 못하는 이유
- `st.session_state`
- 상태(State) 저장
- 상태(State) 삭제

---

# 1. 일반 프로그램과 웹 프로그램의 차이

먼저 일반적인 Python 프로그램을 생각해 보자.

```python
count = 0

count += 1

print(count)
```

실행 결과

```
1
```

프로그램이 종료되기 전까지는

```
count
```

변수가 계속 살아있다.

---

반면 웹 프로그램은 조금 다르다.

사용자가

- 버튼을 누르거나
- 입력을 하거나
- 선택을 변경하면

Streamlit은 프로그램을 **처음부터 다시 실행한다.**

즉,

```
사용자 입력

↓

Python 코드 처음부터 실행

↓

화면 다시 생성
```

이라는 구조이다.

---

# 2. Streamlit은 왜 값을 잃어버리는가?

예제를 보자.

```python
import streamlit as st

count = 0

if st.button("증가"):

    count += 1

st.write(count)
```

학생들은

```
1

2

3

...
```

을 기대한다.

하지만 실제 결과는

```
1

1

1

1
```

이다.

왜 그럴까?

버튼을 누를 때마다

```
count = 0
```

부터 다시 실행되기 때문이다.

---

# 3. Session State란?

Session State는

사용자마다 유지되는 변수 저장 공간이다.

쉽게 말하면

```
브라우저 메모리
```

라고 생각하면 된다.

일반 변수

```
count
```

↓

버튼 클릭

↓

초기화

---

Session State

```
st.session_state["count"]
```

↓

버튼 클릭

↓

값 유지

---

# 4. Session State 만들기

처음 사용할 때는

변수를 생성해야 한다.

```python
import streamlit as st

if "count" not in st.session_state:

    st.session_state.count = 0
```

---

# 5. 값 변경하기

```python
if st.button("증가"):

    st.session_state.count += 1

st.write(st.session_state.count)
```

이번에는

```
1

2

3

4

5
```

처럼 증가한다.

---

# 6. Session State 읽기

```python
st.write(
    st.session_state.count
)
```

또는

```python
st.write(
    st.session_state["count"]
)
```

두 방법 모두 가능하다.

---

# 7. Session State 삭제

```python
del st.session_state["count"]
```

또는

```python
st.session_state.clear()
```

모든 상태를 삭제한다.

---

# Session State 주요 함수

|기능|코드|
|----|----|
|생성|st.session_state["key"]=값|
|조회|st.session_state["key"]|
|수정|값 변경|
|삭제|del|
|전체 삭제|clear()|

---

# 실습 1. 방문 횟수

```python
import streamlit as st

if "visit" not in st.session_state:

    st.session_state.visit = 0

if st.button("방문"):

    st.session_state.visit += 1

st.write(
    "방문 횟수 :",
    st.session_state.visit
)
```

---

# 실습 2. 로그인 유지

```python
import streamlit as st

if "login" not in st.session_state:

    st.session_state.login = False

if not st.session_state.login:

    if st.button("로그인"):

        st.session_state.login = True

else:

    st.success("로그인 상태입니다.")

    if st.button("로그아웃"):

        st.session_state.login = False
```

---

# 실습 3. 장바구니

```python
import streamlit as st

if "cart" not in st.session_state:

    st.session_state.cart = []

product = st.selectbox(

    "상품",

    [

        "노트북",

        "키보드",

        "마우스"

    ]

)

if st.button("장바구니 담기"):

    st.session_state.cart.append(product)

st.write(

    st.session_state.cart

)
```

---

# 종합 실습 : 주문 내역

```python
import streamlit as st

st.title("주문 시스템")

if "orders" not in st.session_state:

    st.session_state.orders = []

product = st.selectbox(

    "제품",

    [

        "PLC",

        "SCADA",

        "MES"

    ]

)

qty = st.number_input(

    "수량",

    min_value=1

)

if st.button("주문"):

    st.session_state.orders.append(

        {

            "제품":product,

            "수량":qty

        }

    )

st.write("주문 내역")

st.write(

    st.session_state.orders

)
```

---

# 실무 활용 사례

Session State는 다음과 같은 기능에서 많이 사용된다.

- 로그인 상태 유지
- 장바구니
- 검색 조건 유지
- 페이지 이동 상태
- 선택한 행 기억
- 사용자 설정 저장
- 다단계 입력 화면(Wizard)
- 임시 데이터 저장

---

# 핵심 정리

✔ Streamlit은 사용자의 입력이 발생할 때마다 프로그램을 처음부터 다시 실행한다.

✔ 일반 변수는 다시 초기화되므로 이전 값을 유지하지 못한다.

✔ `st.session_state`는 사용자별 상태를 저장하는 공간이다.

✔ 로그인, 장바구니, 주문 내역과 같은 기능은 `st.session_state`를 이용하여 구현한다.

---

# 연습 문제

## 문제 1

버튼을 누를 때마다 숫자가 증가하는 프로그램을 작성하시오.

---

## 문제 2

로그인 상태를 유지하는 프로그램을 작성하시오.

---

## 문제 3

장바구니 기능을 구현하시오.

---

## 문제 4

주문 내역을 Session State에 저장하는 프로그램을 작성하시오.

---

## 문제 5

Session State가 필요한 이유를 설명하시오.

---

# 다음 장 예고

다음 장에서는 SQLite와 Streamlit을 연동하여 실제 데이터를 저장하고 조회하는 방법을 학습한다.

학습 내용

- sqlite3 연결
- CRUD 구현
- DataFrame 출력
- Streamlit과 SQLite 연동
- Mini MES 데이터 관리
