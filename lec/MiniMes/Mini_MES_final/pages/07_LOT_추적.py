import streamlit as st

from src import queries
from src.ui import page_title, setup_page, show_dataframe


setup_page("LOT 추적")
page_title("LOT 추적", "원자재 LOT에서 출하까지, 완제품 LOT에서 원자재까지 추적합니다.")

tab_forward, tab_reverse = st.tabs(["정방향 추적", "역방향 추적"])

with tab_forward:
    materials = queries.lots(item_type="MATERIAL")
    options = {
        f"{row['lot_no']} | {row['item_name']}": int(row["lot_id"])
        for _, row in materials.iterrows()
    }
    if not options:
        st.warning("원자재 LOT가 없습니다.")
    else:
        selected = st.selectbox("원자재 LOT", list(options.keys()))
        show_dataframe(queries.forward_trace(options[selected]))

with tab_reverse:
    products = queries.lots(item_type="PRODUCT")
    options = {
        f"{row['lot_no']} | {row['item_name']}": int(row["lot_id"])
        for _, row in products.iterrows()
    }
    if not options:
        st.warning("완제품 LOT가 없습니다.")
    else:
        selected = st.selectbox("완제품 LOT", list(options.keys()))
        show_dataframe(queries.reverse_trace(options[selected]))
