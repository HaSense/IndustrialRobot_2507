import streamlit as st

from src import queries
from src.ui import page_title, setup_page, show_dataframe


setup_page("역방향 추적")

page_title(
    "역방향 LOT 추적",
    "완제품 LOT에서 시작해 생산에 사용된 원자재 LOT를 찾습니다.",
    "완제품 lot -> production -> production_material -> 원자재 lot -> item",
    "완제품 LOT를 선택하고 사용 원자재 LOT와 투입수량을 확인합니다.",
)

output_lots = queries.lots_for_select("PRODUCTION")
if not output_lots:
    st.warning("완제품 LOT가 없습니다.")
    st.stop()

options = {
    f"{lot['lot_no']} | {lot['item_name']} | 수량 {lot['qty']:,.0f}": lot["lot_id"]
    for lot in output_lots
}
selected_label = st.selectbox("완제품 LOT", list(options.keys()))

df = queries.reverse_trace(options[selected_label])

st.subheader("추적 결과")
show_dataframe(df, "이 완제품 LOT와 연결된 원자재 투입 이력이 없습니다.")

if not df.empty:
    first = df.iloc[0]
    st.subheader("추적 경로")
    st.markdown(
        f"""
        `{first['output_lot_no']}`
        -> 생산실적 `{first['production_no']}`
        -> `production_material`
        -> 원자재 LOT {', '.join(df['material_lot_no'].tolist())}
        """
    )

st.caption(
    "역방향 추적은 완제품 품질 문제가 발생했을 때 생산에 사용된 원자재 LOT를 확인하는 데 사용한다."
)
