import streamlit as st

st.set_page_config(
    page_title="Mini MES",
    layout="wide", #centered
    initial_sidebar_state='auto' #collapsed, expanded
)

st.title("Mini MES Dashboard")
st.sidebar.title("사이드 메뉴")

menu = st.sidebar.selectbox(
    "선택",
    [
        "Dashboard",
        "생산관리",
        "품질관리",
        "설비관리",
        "고객관리"
    ]
)

col1, col2, col3 = st.columns(3)

with col1:
    st.metric("오늘 생산량", "1,250 EA",border=True)

with col2:
    st.metric("불량률", "1.8 %", label_visibility="visible", border=True)

with col3:
    st.metric("설비 가동률", "96 %", border=True)

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
