# Chapter 12. 데이터 시각화(Dashboard Visualization)

---

# 학습 목표

이번 장에서는 Streamlit을 이용하여 **공장 대시보드(Dashboard)** 를 구성하는 방법을 학습한다.

대시보드는 생산 현황을 한눈에 파악할 수 있도록 핵심 정보를 시각적으로 표현하는 화면이다.

MES, ERP, SCADA, 스마트팩토리 시스템에서 가장 많이 사용하는 화면이며,
관리자는 대시보드를 통해 공장의 현재 상태를 빠르게 확인할 수 있다.

이번 장을 마치면 다음과 같은 화면을 만들 수 있다.

- 공장 KPI Dashboard
- 생산 현황 Dashboard
- 품질 Dashboard
- 설비 Dashboard
- Mini MES Dashboard

이번 장에서 학습할 내용

- KPI
- Metric
- Gauge
- Pie Chart
- Trend

---

# 1. 대시보드(Dashboard)란?

대시보드는 업무에서 가장 중요한 정보를 한 화면에 모아 보여주는 화면이다.

자동차 계기판(Dashboard)처럼

- 속도
- 연료
- RPM

을 한눈에 확인하는 것과 같은 개념이다.

공장에서는 다음과 같은 정보를 표시한다.

```
생산량

불량률

설비 가동률

OEE

LOT 진행상태

작업지시

재고 현황
```

---

# 2. KPI(Key Performance Indicator)

KPI는 **핵심 성과 지표**를 의미한다.

회사의 목표 달성 정도를 측정하는 가장 중요한 지표이다.

스마트팩토리에서는 다음과 같은 KPI를 자주 사용한다.

|KPI|설명|
|---|----|
|생산량|오늘 생산한 수량|
|불량률|전체 생산 중 불량 비율|
|설비 가동률|설비가 실제 가동된 비율|
|OEE|설비 종합 효율|
|납기 준수율|정해진 날짜에 출하한 비율|
|재고 수량|현재 보유 재고|

---

# 3. Metric

Metric은 하나의 핵심 수치를 강조하여 보여주는 컴포넌트이다.

Streamlit에서는 `st.metric()`을 사용한다.

```python
import streamlit as st

st.metric(

    label="생산량",

    value="1,250 EA"

)
```

---

## 변화량 표시

```python
st.metric(

    label="생산량",

    value="1,250 EA",

    delta="+120"

)
```

실행 결과

```
생산량

1,250 EA

+120
```

---

## 여러 KPI 표시

```python
col1, col2, col3 = st.columns(3)

with col1:

    st.metric(

        "생산량",

        "1250 EA",

        "+80"

    )

with col2:

    st.metric(

        "불량률",

        "1.4%",

        "-0.2%"

    )

with col3:

    st.metric(

        "가동률",

        "97%",

        "+1%"

    )
```

---

# 4. Gauge(게이지)

게이지는 목표 대비 현재 상태를 표현하는 차트이다.

예)

```
가동률

0%

---------------▶

100%
```

Streamlit에는 기본 Gauge가 없으므로

다음과 같은 방법을 사용한다.

- Plotly Gauge
- Plotly Indicator

---

## Plotly Gauge 예제

```python
import plotly.graph_objects as go

import streamlit as st

fig = go.Figure(

    go.Indicator(

        mode="gauge+number",

        value=92,

        title={"text":"설비 가동률"}

    )

)

st.plotly_chart(fig)
```

---

## 사용 사례

- 설비 가동률

- 목표 달성률

- 배터리 상태

- 생산 진행률

---

# 5. Pie Chart

Pie Chart는 전체에서 각 항목이 차지하는 비율을 표현한다.

예)

```
양품

불량

재작업
```

---

## Plotly Pie Chart

```python
import plotly.express as px

import pandas as pd

import streamlit as st

df = pd.DataFrame(

    {

        "Status":[

            "양품",

            "불량",

            "재작업"

        ],

        "Count":[

            900,

            70,

            30

        ]

    }

)

fig = px.pie(

    df,

    names="Status",

    values="Count"

)

st.plotly_chart(fig)
```

---

## 사용 사례

- 품질 비율

- 제품별 생산 비율

- 부서별 인원

- 재고 구성

---

# 6. Trend(추세)

Trend는 시간에 따른 변화를 의미한다.

대표적으로 Line Chart를 사용한다.

```python
import pandas as pd
import streamlit as st

df = pd.DataFrame(

    {

        "Day":[1,2,3,4,5],

        "Production":[

            120,

            135,

            140,

            150,

            170

        ]

    }

)

st.line_chart(

    df,

    x="Day",

    y="Production"

)
```

---

# Dashboard 구성 예

```
--------------------------------------------

생산량

불량률

가동률

OEE

--------------------------------------------

생산 추세(Line)

--------------------------------------------

품질 비율(Pie)

--------------------------------------------

설비 가동률(Gauge)

--------------------------------------------
```

---

# 실습 1. KPI 만들기

```python
import streamlit as st

col1, col2, col3, col4 = st.columns(4)

with col1:

    st.metric(

        "생산량",

        "1,250 EA",

        "+50"

    )

with col2:

    st.metric(

        "불량률",

        "1.6%",

        "-0.2%"

    )

with col3:

    st.metric(

        "가동률",

        "96%",

        "+1%"

    )

with col4:

    st.metric(

        "OEE",

        "88%",

        "+2%"

    )
```

---

# 실습 2. Pie Chart 만들기

```python
import pandas as pd
import plotly.express as px
import streamlit as st

df = pd.DataFrame(

    {

        "Status":[

            "양품",

            "불량"

        ],

        "Count":[

            980,

            20

        ]

    }

)

fig = px.pie(

    df,

    names="Status",

    values="Count"

)

st.plotly_chart(fig)
```

---

# 실습 3. 생산 추세

```python
import pandas as pd
import streamlit as st

df = pd.DataFrame(

    {

        "Day":[1,2,3,4,5,6,7],

        "Production":[

            110,

            120,

            135,

            150,

            155,

            170,

            180

        ]

    }

)

st.line_chart(

    df,

    x="Day",

    y="Production"

)
```

---

# 종합 실습 : 공장 Dashboard

구성

```
KPI

↓

생산 추세

↓

품질 비율

↓

설비 가동률
```

```python
import streamlit as st
import pandas as pd
import plotly.express as px

st.title("Mini MES Dashboard")

# KPI
col1, col2, col3, col4 = st.columns(4)

with col1:
    st.metric("생산량", "1,250 EA", "+80")

with col2:
    st.metric("불량률", "1.5%", "-0.2%")

with col3:
    st.metric("가동률", "96%", "+1%")

with col4:
    st.metric("OEE", "88%", "+2%")

st.markdown("---")

# 생산 추세
production = pd.DataFrame({

    "Day":[1,2,3,4,5,6,7],

    "Production":[120,135,145,150,160,175,185]

})

st.subheader("생산량 추세")

st.line_chart(

    production,

    x="Day",

    y="Production"

)

# 품질 비율
quality = pd.DataFrame({

    "Status":["양품","불량"],

    "Count":[980,20]

})

fig = px.pie(

    quality,

    names="Status",

    values="Count"

)

st.subheader("품질 현황")

st.plotly_chart(fig)
```

---

# 도전 실습 1

오늘의 KPI를 만들어 보자.

```
생산량

불량률

가동률

OEE
```

---

# 도전 실습 2

CSV 파일을 읽어 생산량 추세를 Line Chart로 출력해 보자.

---

# 도전 실습 3

SQLite에서 품질 데이터를 읽어 Pie Chart를 작성해 보자.

---

# 도전 실습 4

Plotly Gauge를 이용하여 설비 가동률을 표시해 보자.

---

# 실무 활용 사례

공장 Dashboard에서는 다음과 같은 화면을 자주 구성한다.

- 생산 KPI
- 실시간 생산량
- 설비 상태
- LOT 진행 현황
- 작업 지시 현황
- 품질 현황
- 재고 현황
- OEE 분석
- 공정별 생산량

대부분의 MES와 스마트팩토리 시스템은 이러한 정보를 하나의 Dashboard에서 실시간으로 제공한다.

---

# 핵심 정리

✔ KPI는 공장의 핵심 성과를 측정하는 지표이다.

✔ `st.metric()`은 핵심 수치를 강조하여 표시하는 컴포넌트이다.

✔ Gauge는 목표 대비 현재 상태를 직관적으로 표현하는 시각화 방식이다.

✔ Pie Chart는 전체 대비 비율을 표현할 때 적합하다.

✔ Trend는 시간에 따른 데이터 변화를 분석하는 데 사용한다.

✔ 여러 시각화 요소를 조합하면 실무에서 사용하는 MES Dashboard를 구현할 수 있다.

---

# 연습 문제

## 문제 1

`st.metric()`을 이용하여 생산량 KPI를 출력하시오.

---

## 문제 2

양품과 불량의 비율을 Pie Chart로 출력하시오.

---

## 문제 3

7일간 생산량을 Line Chart로 출력하시오.

---

## 문제 4

Plotly Gauge를 이용하여 설비 가동률을 표시하시오.

---

## 문제 5

다음 항목을 포함하는 Mini MES Dashboard를 작성하시오.

- 생산량 KPI
- 불량률 KPI
- 가동률 KPI
- OEE KPI
- 생산량 추세
- 품질 비율
- 설비 가동률 Gauge

---

# 다음 장 예고

다음 장에서는 **Mini MES 프로젝트**를 구현한다.

학습 내용

- 로그인
- 고객 관리
- 제품 관리
- 작업 지시
- 생산 실적
- LOT 추적
- Dashboard 통합

지금까지 학습한 **Streamlit, Pandas, SQLite, Plotly**를 하나의 프로젝트로 통합하여 실제 스마트팩토리 업무 시스템과 유사한 Mini MES를 완성한다.
