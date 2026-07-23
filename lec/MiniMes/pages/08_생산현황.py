import streamlit as st

from src import queries
from src.ui import metric_row, page_title, setup_page, show_dataframe


setup_page("생산현황")

page_title(
    "생산현황 대시보드",
    "스키마에서 계산 가능한 범위의 생산 수량, LOT 수, 사용 횟수를 집계합니다.",
    "item, lot, production, production_material",
    "집계 결과를 표와 간단한 차트로 확인합니다.",
)

counts = queries.table_counts()
count_map = dict(zip(counts["table_name"], counts["row_count"]))
metric_row(
    [
        ("품목 수", count_map.get("item", 0)),
        ("LOT 수", count_map.get("lot", 0)),
        ("생산실적 수", count_map.get("production", 0)),
        ("투입 이력 행 수", count_map.get("production_material", 0)),
    ]
)

by_date = queries.production_by_date()
by_item = queries.production_by_item()
lot_use = queries.lot_use_counts()
recent = queries.productions().sort_values("production_date", ascending=False).head(5)

st.subheader("일자별 생산량")
show_dataframe(by_date)
if not by_date.empty:
    st.bar_chart(by_date.set_index("production_date")["production_qty"])

st.subheader("품목별 생산량")
show_dataframe(by_item)
if not by_item.empty:
    st.bar_chart(by_item.set_index("item_name")["production_qty"])

st.subheader("최근 생산실적")
show_dataframe(recent)

st.subheader("LOT별 원자재 사용 횟수")
show_dataframe(lot_use)

st.caption(
    "현재 스키마에 없는 설비, 작업자, 불량 수량 통계는 임의로 만들지 않는다."
)
