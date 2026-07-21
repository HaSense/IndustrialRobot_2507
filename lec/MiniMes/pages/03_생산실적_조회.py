import streamlit as st

from src import queries
from src.ui import metric_row, page_title, setup_page, show_dataframe


setup_page("생산실적 조회")

page_title(
    "생산실적 조회",
    "생산 이벤트와 생산 결과 완제품 LOT가 어떻게 연결되는지 확인합니다.",
    "production JOIN item JOIN lot",
    "생산번호, 품목명, 완제품 LOT 번호를 검색하고 생산수량을 확인합니다.",
)

col1, col2, col3 = st.columns(3)
keyword = col1.text_input("생산번호, 품목명, 완제품 LOT 검색")
use_date_filter = col2.checkbox("생산일자 필터 사용")
date_from = None
date_to = None
if use_date_filter:
    date_from = col2.date_input("시작일")
    date_to = col3.date_input("종료일")

df = queries.productions(keyword=keyword, date_from=date_from, date_to=date_to)
if not df.empty:
    metric_row(
        [
            ("생산실적 수", len(df)),
            ("총 생산수량", f"{df['production_qty'].sum():,.0f}"),
            ("완제품 LOT 수", df["output_lot_no"].nunique()),
        ]
    )

st.subheader("생산 이벤트와 결과 LOT")
show_dataframe(df)

if not df.empty:
    selected_no = st.selectbox("원자재 투입 확인 생산번호", df["production_no"].tolist())
    production_id = int(df[df["production_no"] == selected_no].iloc[0]["production_id"])
    st.subheader("선택한 생산의 원자재 투입")
    show_dataframe(queries.production_materials(production_id))

st.caption(
    "`production.qty`는 생산 수량이고, `production.output_lot_id`는 생산 결과로 만들어진 완제품 LOT를 가리킨다."
)
