import streamlit as st

from src import queries
from src.ui import page_title, setup_page, show_dataframe


setup_page("정방향 추적")

page_title(
    "정방향 LOT 추적",
    "특정 원자재 LOT가 어떤 완제품 LOT 생산에 사용되었는지 추적합니다.",
    "원자재 lot -> production_material -> production -> 완제품 lot -> item",
    "원자재 LOT를 선택하고 영향받는 완제품 LOT 목록을 확인합니다.",
)

material_lots = queries.lots_for_select("RECEIPT")
if not material_lots:
    st.warning("원자재 LOT가 없습니다.")
    st.stop()

options = {
    f"{lot['lot_no']} | {lot['item_name']} | 보유 {lot['qty']:,.0f}": lot["lot_id"]
    for lot in material_lots
}
selected_label = st.selectbox("원자재 LOT", list(options.keys()))

df = queries.forward_trace(options[selected_label])

st.subheader("추적 결과")
show_dataframe(df, "이 원자재 LOT를 사용한 생산실적이 없습니다.")

if not df.empty:
    first = df.iloc[0]
    st.subheader("추적 경로")
    st.markdown(
        f"""
        `{first['material_lot_no']}`
        -> `production_material`
        -> 생산실적 {', '.join(df['production_no'].tolist())}
        -> 완제품 LOT {', '.join(df['output_lot_no'].tolist())}
        """
    )

st.caption(
    "정방향 추적은 원자재 문제가 발생했을 때 해당 원자재를 사용한 완제품 LOT를 찾는 데 사용한다."
)
