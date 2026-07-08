# Chapter 9. 차트(Charts)

---

# 학습 목표

이번 장에서는 Streamlit에서 제공하는 다양한 차트 기능을 이용하여 데이터를 시각화하는 방법을 학습한다.

데이터를 숫자로만 보는 것보다 차트로 표현하면 변화 추세와 이상 현상을 훨씬 쉽게 확인할 수 있다.

이번 장을 마치면 다음과 같은 화면을 만들 수 있다.

- 생산량 추이
- 불량률 추이
- 설비 온도 모니터링
- 공정 분석 화면
- MES Dashboard

이번 장에서 학습할 내용

- `st.line_chart()`
- `st.bar_chart()`
- `st.area_chart()`
- `st.scatter_chart()`
- Plotly 연동

---

# 1. Streamlit 차트란?

Streamlit은 Pandas DataFrame만 준비되면 손쉽게 차트를 그릴 수 있다.

대표적인 차트는 다음과 같다.

|차트|용도|
|----|----|
|Line Chart|시간에 따른 변화|
|Bar Chart|항목 비교|
|Area Chart|누적 변화|
|Scatter Chart|상관관계 분석|
|Plotly|고급 인터랙티브 차트|

---

# 2. 준비하기

예제에서는 Pandas를 사용한다.

```python
import streamlit as st
import pandas as pd
```

예제 데이터

```python
data = pd.DataFrame(
    {
        "Day":[1,2,3,4,5],
        "Production":[120,135,150,142,160]
    }
)
```

---

# 3. st.line_chart()

가장 많이 사용하는 차트이다.

시간의 흐름에 따른 변화를 표현한다.

```python
st.line_chart(
    data,
    x="Day",
    y="Production"
)
```

결과

```
생산량 증가 추세
```

---

## 사용 사례

- 생산량
- 온도
- 압력
- 전력 사용량
- 재고 변화

---

# 4. st.bar_chart()

항목별 비교를 할 때 사용한다.

```python
data = pd.DataFrame(
    {
        "Department":[
            "생산",
            "품질",
            "설비"
        ],

        "Employee":[
            30,
            12,
            15
        ]
    }
)

st.bar_chart(
    data,
    x="Department",
    y="Employee"
)
```

---

## 사용 사례

- 부서별 인원

- 제품별 판매량

- 설비별 생산량

---

# 5. st.area_chart()

면적으로 표현하는 차트이다.

Line Chart와 비슷하지만
누적량을 표현하기 좋다.

```python
st.area_chart(
    data,
    x="Day",
    y="Production"
)
```

---

## 사용 사례

- 누적 생산량

- 누적 전력 사용량

- 월별 매출

---

# 6. st.scatter_chart()

두 변수의 관계를 분석한다.

```python
data = pd.DataFrame(
    {
        "Temperature":[
            20,
            25,
            30,
            35,
            40
        ],

        "Defect":[
            1,
            2,
            3,
            5,
            8
        ]
    }
)

st.scatter_chart(
    data,
    x="Temperature",
    y="Defect"
)
```

---

## 사용 사례

- 온도와 불량률

- 압력과 생산량

- 속도와 소비전력

---

# 7. Plotly

Streamlit은 Plotly와도 매우 잘 연동된다.

Plotly는 확대, 축소, 마우스 오버 등
인터랙티브 기능을 제공한다.

설치

```bash
pip install plotly
```

---

## Plotly 예제

```python
import plotly.express as px
import pandas as pd
import streamlit as st

data = pd.DataFrame(
    {
        "Day":[1,2,3,4,5],

        "Production":[
            120,
            135,
            150,
            145,
            160
        ]
    }
)

fig = px.line(
    data,
    x="Day",
    y="Production",
    title="생산량 추이"
)

st.plotly_chart(fig)
```

---

## Plotly의 장점

- 확대/축소

- 마우스 정보 표시

- 다양한 테마

- 3D 차트 지원

- Pie Chart 지원

- Dash와도 연동 가능

---

# 차트 비교

|차트|용도|
|----|----|
|Line|시간 변화|
|Bar|항목 비교|
|Area|누적 변화|
|Scatter|상관관계|
|Plotly|고급 시각화|

---

# 실습 1. 생산량 그래프

```python
import streamlit as st
import pandas as pd

data = pd.DataFrame(
    {
        "Day":[1,2,3,4,5,6,7],

        "Production":[
            120,
            135,
            150,
            145,
            170,
            180,
            190
        ]
    }
)

st.title("생산량")

st.line_chart(
    data,
    x="Day",
    y="Production"
)
```

---

# 실습 2. 불량률 그래프

```python
import streamlit as st
import pandas as pd

data = pd.DataFrame(
    {
        "Day":[1,2,3,4,5,6,7],

        "Defect":[
            2.1,
            1.9,
            2.3,
            1.7,
            1.5,
            1.3,
            1.2
        ]
    }
)

st.title("불량률")

st.area_chart(
    data,
    x="Day",
    y="Defect"
)
```

---

# 실습 3. 온도 그래프

```python
import streamlit as st
import pandas as pd

data = pd.DataFrame(
    {
        "Time":[
            "09:00",
            "10:00",
            "11:00",
            "12:00",
            "13:00",
            "14:00"
        ],

        "Temperature":[
            23,
            25,
            28,
            31,
            30,
            29
        ]
    }
)

st.line_chart(
    data,
    x="Time",
    y="Temperature"
)
```

---

# 종합 실습 : Mini MES Dashboard

다음과 같은 Dashboard를 만들어 보자.

```
-------------------------------

생산량(Line Chart)

-------------------------------

불량률(Area Chart)

-------------------------------

부서별 생산량(Bar Chart)

-------------------------------

온도와 불량률(Scatter Chart)

-------------------------------
```

예제 코드

```python
import streamlit as st
import pandas as pd

st.title("Mini MES Dashboard")

production = pd.DataFrame(
    {
        "Day":[1,2,3,4,5],
        "Production":[120,140,150,145,165]
    }
)

defect = pd.DataFrame(
    {
        "Day":[1,2,3,4,5],
        "Defect":[2.3,2.0,1.8,1.7,1.5]
    }
)

department = pd.DataFrame(
    {
        "Department":[
            "A라인",
            "B라인",
            "C라인"
        ],

        "Output":[
            150,
            180,
            130
        ]
    }
)

temperature = pd.DataFrame(
    {
        "Temperature":[20,25,30,35,40],

        "Defect":[1,2,3,5,8]
    }
)

st.subheader("생산량")
st.line_chart(production, x="Day", y="Production")

st.subheader("불량률")
st.area_chart(defect, x="Day", y="Defect")

st.subheader("라인별 생산량")
st.bar_chart(department, x="Department", y="Output")

st.subheader("온도와 불량률")
st.scatter_chart(
    temperature,
    x="Temperature",
    y="Defect"
)
```

---

# 도전 실습 1

SQLite에서 생산 데이터를 읽어와 Line Chart를 출력해 보자.

---

# 도전 실습 2

CSV 파일을 읽어 생산량 그래프를 출력해 보자.

---

# 도전 실습 3

Plotly를 이용하여 인터랙티브 생산량 그래프를 작성해 보자.

---

# 핵심 정리

✔ `st.line_chart()`는 시간의 흐름에 따른 변화를 표현할 때 사용한다.

✔ `st.bar_chart()`는 항목 간의 값을 비교할 때 적합하다.

✔ `st.area_chart()`는 누적 데이터나 변화량을 강조할 때 사용한다.

✔ `st.scatter_chart()`는 두 변수 간의 상관관계를 분석할 때 사용한다.

✔ Plotly는 확대, 축소, 마우스 오버 등 인터랙티브 기능을 제공하는 강력한 시각화 라이브러리이다.

✔ Streamlit과 Pandas를 함께 사용하면 적은 코드로 다양한 대시보드와 모니터링 화면을 구현할 수 있다.

---

# 연습 문제

## 문제 1

7일간 생산량을 Line Chart로 출력하는 프로그램을 작성하시오.

---

## 문제 2

부서별 직원 수를 Bar Chart로 출력하시오.

---

## 문제 3

시간에 따른 설비 온도를 Area Chart로 출력하시오.

---

## 문제 4

온도와 불량률의 관계를 Scatter Chart로 출력하시오.

---

## 문제 5

Line Chart, Bar Chart, Area Chart, Scatter Chart를 모두 포함하는 **Mini MES Dashboard**를 작성하시오.

---

# 다음 장 예고

다음 장에서는 **파일 업로드 및 다운로드(File I/O)** 기능을 학습한다.

학습 내용

- `st.file_uploader()`
- `st.download_button()`
- CSV 파일 읽기
- Excel 파일 읽기
- 업로드한 데이터를 DataFrame으로 출력
- 업로드한 데이터를 SQLite에 저장

이를 통해 실제 MES나 업무 시스템에서 사용하는 데이터 업로드 및 다운로드 기능을 구현해 본다.
