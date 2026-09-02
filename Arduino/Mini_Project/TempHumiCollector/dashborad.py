import sqlite3
import pandas as pd
import streamlit as st
import plotly.express as px


# ============================================================
# 기본 설정
# ============================================================

DB_FILE = "sensor_data.db"

st.set_page_config(
    page_title="Temperature & Humidity Dashboard",
    page_icon="🌡️",
    layout="wide"
)

st.title("🌡️ Temperature & Humidity Dashboard")
st.caption("Arduino DHT11 Sensor Monitoring")


# ============================================================
# SQLite 데이터 읽기
# ============================================================

def load_data():

    conn = sqlite3.connect(DB_FILE)

    query = """
        SELECT
            id,
            temperature,
            humidity,
            created_at
        FROM sensor_data
        ORDER BY created_at
    """

    df = pd.read_sql_query(query, conn)

    conn.close()

    # 문자열 형태의 시간을 datetime으로 변환
    df["created_at"] = pd.to_datetime(df["created_at"])

    return df


# ============================================================
# 데이터 로드
# ============================================================

df = load_data()


# 데이터가 없는 경우
if df.empty:

    st.warning("수집된 센서 데이터가 없습니다.")
    st.stop()


# ============================================================
# 사이드바
# ============================================================

st.sidebar.header("Dashboard 설정")

# 최근 N개 데이터 선택
data_count = st.sidebar.slider(
    "표시할 데이터 개수",
    min_value=10,
    max_value=min(500, len(df)),
    value=min(100, len(df))
)

df_view = df.tail(data_count)


# ============================================================
# 최신 데이터
# ============================================================

latest = df.iloc[-1]

latest_temp = latest["temperature"]
latest_humi = latest["humidity"]
latest_time = latest["created_at"]


# 이전 데이터와 비교하기 위한 값
if len(df) >= 2:

    previous = df.iloc[-2]

    temp_delta = latest_temp - previous["temperature"]
    humi_delta = latest_humi - previous["humidity"]

else:

    temp_delta = 0
    humi_delta = 0


# ============================================================
# 현재 상태 KPI
# ============================================================

st.subheader("현재 센서 상태")

col1, col2, col3, col4 = st.columns(4)


col1.metric(
    "현재 온도",
    f"{latest_temp:.1f} °C",
    f"{temp_delta:+.1f} °C"
)


col2.metric(
    "현재 습도",
    f"{latest_humi:.1f} %",
    f"{humi_delta:+.1f} %"
)


col3.metric(
    "수집 데이터",
    f"{len(df):,} 건"
)


col4.metric(
    "최근 측정 시간",
    latest_time.strftime("%H:%M:%S")
)


st.divider()


# ============================================================
# 통계 정보
# ============================================================

st.subheader("📊 센서 통계")

col1, col2, col3, col4 = st.columns(4)


col1.metric(
    "평균 온도",
    f"{df_view['temperature'].mean():.1f} °C"
)

col2.metric(
    "최고 온도",
    f"{df_view['temperature'].max():.1f} °C"
)

col3.metric(
    "평균 습도",
    f"{df_view['humidity'].mean():.1f} %"
)

col4.metric(
    "최고 습도",
    f"{df_view['humidity'].max():.1f} %"
)


# 추가 통계
col1, col2, col3, col4 = st.columns(4)

col1.metric(
    "최저 온도",
    f"{df_view['temperature'].min():.1f} °C"
)

col2.metric(
    "온도 표준편차",
    f"{df_view['temperature'].std():.2f}"
)

col3.metric(
    "최저 습도",
    f"{df_view['humidity'].min():.1f} %"
)

col4.metric(
    "습도 표준편차",
    f"{df_view['humidity'].std():.2f}"
)


st.divider()


# ============================================================
# 온도 변화
# ============================================================

st.subheader("🌡️ 온도 변화")

fig_temp = px.line(
    df_view,
    x="created_at",
    y="temperature",
    markers=True,
    labels={
        "created_at": "시간",
        "temperature": "온도 (°C)"
    }
)

st.plotly_chart(
    fig_temp,
    use_container_width=True
)


# ============================================================
# 습도 변화
# ============================================================

st.subheader("💧 습도 변화")

fig_humi = px.line(
    df_view,
    x="created_at",
    y="humidity",
    markers=True,
    labels={
        "created_at": "시간",
        "humidity": "습도 (%)"
    }
)

st.plotly_chart(
    fig_humi,
    use_container_width=True
)


# ============================================================
# 온도 + 습도 동시 표시
# ============================================================

st.subheader("📈 온도 / 습도 변화 비교")


chart_data = df_view.set_index("created_at")[
    ["temperature", "humidity"]
]

st.line_chart(chart_data)


# ============================================================
# 온도와 습도의 관계
# ============================================================

st.subheader("🔬 온도와 습도의 관계")

fig_scatter = px.scatter(
    df_view,
    x="temperature",
    y="humidity",
    hover_data=["created_at"],
    labels={
        "temperature": "온도 (°C)",
        "humidity": "습도 (%)"
    }
)

st.plotly_chart(
    fig_scatter,
    use_container_width=True
)


# ============================================================
# 데이터 분포
# ============================================================

col1, col2 = st.columns(2)


with col1:

    st.subheader("온도 분포")

    fig_temp_hist = px.histogram(
        df_view,
        x="temperature",
        nbins=20,
        labels={
            "temperature": "온도 (°C)"
        }
    )

    st.plotly_chart(
        fig_temp_hist,
        use_container_width=True
    )


with col2:

    st.subheader("습도 분포")

    fig_humi_hist = px.histogram(
        df_view,
        x="humidity",
        nbins=20,
        labels={
            "humidity": "습도 (%)"
        }
    )

    st.plotly_chart(
        fig_humi_hist,
        use_container_width=True
    )


# ============================================================
# 상세 데이터
# ============================================================

st.divider()

st.subheader("📋 센서 데이터")


# 최신 데이터가 위로 오도록 정렬
display_df = df_view.sort_values(
    "created_at",
    ascending=False
)

st.dataframe(
    display_df,
    use_container_width=True,
    hide_index=True
)


# ============================================================
# CSV 다운로드
# ============================================================

csv = display_df.to_csv(
    index=False
).encode("utf-8-sig")


st.download_button(
    label="CSV 다운로드",
    data=csv,
    file_name="sensor_data.csv",
    mime="text/csv"
)
