# Chapter 5. 레이아웃(Layout)

---

# 학습 목표

이번 장에서는 Streamlit에서 화면을 보기 좋게 구성하는 **레이아웃(Layout)** 기능을 학습한다.

레이아웃을 잘 활용하면 복잡한 정보를 사용자가 쉽게 이해할 수 있도록 구성할 수 있으며, 실제 업무 시스템과 유사한 화면을 만들 수 있다.

이번 장을 마치면 다음과 같은 화면을 만들 수 있다.

- MES 메인 대시보드
- 생산 현황 화면
- 관리자 화면
- 통계 화면
- AI 모니터링 화면

이번 장에서 학습할 내용

- `st.columns()`
- `st.tabs()`
- `st.expander()`
- `st.sidebar()`
- `st.container()`

---

# 1. 레이아웃(Layout)이란?

레이아웃(Layout)은 화면을 보기 좋게 배치하는 기술이다.

예를 들어 MES 화면은 다음과 같이 구성될 수 있다.

```
--------------------------------------
 MES Dashboard
--------------------------------------

생산량      불량률      설비가동률

--------------------------------------

생산현황

--------------------------------------

공지사항

--------------------------------------
```

레이아웃을 사용하지 않으면 모든 내용이 세로 방향으로만 출력된다.

레이아웃을 사용하면

- 좌우 배치
- 탭 구성
- 접기/펼치기
- 사이드 메뉴

등을 구현할 수 있다.

---

# 2. st.columns()

여러 개의 열(Column)을 만든다.

대시보드에서 가장 많이 사용하는 기능이다.

```python
import streamlit as st

col1, col2 = st.columns(2)

with col1:
    st.write("왼쪽")

with col2:
    st.write("오른쪽")
```

실행 화면

```
-------------------------

왼쪽        오른쪽

-------------------------
```

---

## 3개의 컬럼

```python
col1, col2, col3 = st.columns(3)

with col1:
    st.write("생산량")

with col2:
    st.write("불량")

with col3:
    st.write("가동률")
```

---

## 컬럼 비율 지정

```python
left, right = st.columns([2,1])

with left:
    st.write("메인")

with right:
    st.write("메뉴")
```

비율

```
2 : 1
```

---

# 3. st.tabs()

탭(Tab)을 만든다.

하나의 화면에서 여러 페이지처럼 사용할 수 있다.

```python
tab1, tab2 = st.tabs(
    [
        "생산",
        "품질"
    ]
)
```

---

## 예제

```python
tab1, tab2, tab3 = st.tabs(
    [
        "생산",
        "품질",
        "설비"
    ]
)

with tab1:
    st.write("생산 정보")

with tab2:
    st.write("품질 정보")

with tab3:
    st.write("설비 정보")
```

---

# 4. st.expander()

접었다 펼칠 수 있는 영역이다.

상세 정보를 숨길 때 많이 사용한다.

```python
with st.expander("상세 보기"):

    st.write("생산량")

    st.write("불량률")

    st.write("설비상태")
```

---

## 예제

```python
with st.expander("제품 상세 정보"):

    st.write("제품명")

    st.write("LOT")

    st.write("생산일")
```

---

# 5. st.sidebar()

화면 왼쪽에 사이드 메뉴를 만든다.

관리자 시스템에서 매우 많이 사용한다.

```python
st.sidebar.title("MES")

menu = st.sidebar.selectbox(
    "메뉴",
    [
        "생산",
        "품질",
        "설비"
    ]
)

st.write(menu)
```

---

## 사이드바 입력

```python
name = st.sidebar.text_input("사용자")

st.write(name)
```

---

# 6. st.container()

화면을 그룹으로 묶는다.

코드를 읽기 쉽게 만들고,
관련된 화면을 하나의 영역으로 관리할 수 있다.

```python
container = st.container()

with container:

    st.header("생산현황")

    st.write("생산량")

    st.write("불량률")
```

---

## 예제

```python
with st.container():

    st.header("공지사항")

    st.write("정기 점검")

    st.write("설비 업그레이드")
```

---

# 레이아웃 컴포넌트 비교

|컴포넌트|용도|
|---------|----|
|columns|좌우 배치|
|tabs|탭 메뉴|
|expander|접기/펼치기|
|sidebar|왼쪽 메뉴|
|container|화면 그룹|

---

# 실습 1

컬럼 만들기

```python
import streamlit as st

col1, col2, col3 = st.columns(3)

with col1:
    st.metric("생산량", "1,250")

with col2:
    st.metric("불량률", "1.5%")

with col3:
    st.metric("가동률", "97%")
```

---

# 실습 2

탭 만들기

```python
tab1, tab2 = st.tabs(
    [
        "생산",
        "품질"
    ]
)

with tab1:
    st.write("생산 정보")

with tab2:
    st.write("품질 정보")
```

---

# 실습 3

사이드바 만들기

```python
st.sidebar.title("MES")

menu = st.sidebar.selectbox(
    "메뉴",
    [
        "Dashboard",
        "생산관리",
        "품질관리",
        "설비관리"
    ]
)

st.write(menu)
```

---

# 종합 실습 : MES 메인 화면 만들기

파일명

```
app.py
```

```python
import streamlit as st

st.set_page_config(
    page_title="Mini MES",
    layout="wide"
)

st.title("Mini MES Dashboard")

st.sidebar.title("메뉴")

menu = st.sidebar.selectbox(
    "선택",
    [
        "Dashboard",
        "생산관리",
        "품질관리",
        "설비관리"
    ]
)

col1, col2, col3 = st.columns(3)

with col1:
    st.metric("오늘 생산량", "1,250 EA")

with col2:
    st.metric("불량률", "1.8 %")

with col3:
    st.metric("설비 가동률", "96 %")

st.markdown("---")

tab1, tab2, tab3 = st.tabs(
    [
        "생산",
        "품질",
        "설비"
    ]
)

with tab1:

    st.subheader("생산 현황")

    st.write("생산량 증가")

with tab2:

    st.subheader("품질 현황")

    st.write("불량률 감소")

with tab3:

    st.subheader("설비 현황")

    st.write("정상 가동")

st.markdown("---")

with st.expander("공지사항"):

    st.write("- 금요일 설비 점검")

    st.write("- MES 업데이트 예정")

with st.container():

    st.header("금일 작업 요약")

    st.write("총 생산량 : 1,250 EA")

    st.write("불량률 : 1.8 %")

    st.write("설비 가동률 : 96 %")
```

---

# 도전 실습 1

다음과 같은 Dashboard를 만들어 보자.

```
생산량

불량률

가동률

작업자 수
```

각 항목을 하나의 컬럼으로 배치한다.

---

# 도전 실습 2

다음과 같은 탭을 만들어 보자.

```
생산관리

품질관리

설비관리

LOT 조회

재고관리
```

각 탭에 임의의 내용을 출력한다.

---

# 도전 실습 3

MES 관리 시스템 화면을 만들어 보자.

사이드바 메뉴

```
Dashboard

생산관리

품질관리

설비관리

LOT 추적

재고관리

환경설정
```

메인 화면

```
생산량

불량률

설비가동률

공지사항
```

---

# 핵심 정리

✔ `st.columns()`은 화면을 여러 개의 열로 나누어 정보를 나란히 배치할 수 있다.

✔ `st.tabs()`는 하나의 화면 안에서 여러 개의 탭을 구성할 수 있다.

✔ `st.expander()`는 상세 정보를 접거나 펼칠 수 있는 영역을 만든다.

✔ `st.sidebar()`는 화면 왼쪽에 메뉴나 입력 영역을 구성할 수 있다.

✔ `st.container()`는 관련된 화면 요소를 하나의 그룹으로 묶어 관리할 수 있다.

✔ 레이아웃을 적절히 활용하면 실제 MES, ERP, CRM과 같은 업무 시스템과 유사한 화면을 구현할 수 있다.

---

# 연습 문제

## 문제 1

화면을 좌우 두 개의 영역으로 나누는 코드를 작성하시오.

---

## 문제 2

생산, 품질, 설비의 세 개 탭을 생성하시오.

---

## 문제 3

사이드바에 "Dashboard", "생산관리", "품질관리" 메뉴를 추가하시오.

---

## 문제 4

공지사항을 `st.expander()`를 이용하여 접기/펼치기 형태로 구현하시오.

---

## 문제 5

오늘 배운 레이아웃 컴포넌트를 모두 활용하여 **Mini MES Dashboard** 화면을 구현해 보시오.

---

# 다음 장 예고

다음 장에서는 **데이터 표시(Data Display)** 기능을 학습한다.

학습 내용

- `st.dataframe()`
- `st.table()`
- `st.metric()`
- `st.json()`
- `st.image()`
- `st.download_button()`

이를 통해 SQLite나 Pandas의 데이터를 표 형태로 출력하고, 대시보드와 보고서 화면을 구성하는 방법을 익히게 된다.
