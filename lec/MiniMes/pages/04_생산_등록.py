from datetime import date, timedelta

import streamlit as st

from src import queries
from src.services import ProductionRegistration, register_production
from src.ui import page_title, setup_page


setup_page("생산 등록")

page_title(
    "생산 등록",
    "완제품 LOT, 생산실적, 원자재 투입 이력을 하나의 트랜잭션으로 저장하는 흐름을 실습합니다.",
    "lot, production, production_material",
    "생산할 제품과 투입 원자재 LOT를 선택하고 저장 결과를 확인합니다.",
)

products = queries.active_items_for_select("PRODUCT")
material_lots = queries.lots_for_select("RECEIPT")

if not products or not material_lots:
    st.warning("생산 등록에 필요한 제품 품목 또는 원자재 LOT가 없습니다.")
    st.stop()

product_options = {
    f"{item['item_code']} | {item['item_name']}": item["item_id"]
    for item in products
}
material_options = {
    f"{lot['lot_no']} | {lot['item_name']} | 보유 {lot['qty']:,.0f}": lot
    for lot in material_lots
}

with st.form("production_form"):
    product_label = st.selectbox("생산할 완제품 품목", list(product_options.keys()))
    production_date = st.date_input("생산일자", value=date.today())
    production_no = st.text_input("생산번호", value=f"PRD-{date.today().strftime('%Y%m%d')}-NEW")
    output_lot_no = st.text_input("생성할 완제품 LOT 번호", value=f"FG-NEW-{date.today().strftime('%Y%m%d')}-001")
    qty = st.number_input("생산수량", min_value=0.0, value=1000.0, step=100.0)
    expire_date = st.date_input("완제품 유효기한", value=date.today() + timedelta(days=180))

    st.subheader("투입 원자재")
    selected_material_labels = st.multiselect(
        "원자재 LOT 선택",
        list(material_options.keys()),
        default=list(material_options.keys())[:3],
    )

    material_rows = []
    for label in selected_material_labels:
        lot = material_options[label]
        used_qty = st.number_input(
            f"{lot['lot_no']} 투입수량",
            min_value=0.0,
            value=float(qty),
            step=100.0,
            key=f"material_qty_{lot['lot_id']}",
        )
        material_rows.append(
            {
                "material_item_id": lot["item_id"],
                "material_lot_id": lot["lot_id"],
                "qty": used_qty,
            }
        )

    submitted = st.form_submit_button("생산실적 저장")

if submitted:
    data = ProductionRegistration(
        product_item_id=product_options[product_label],
        output_lot_no=output_lot_no,
        production_no=production_no,
        production_date=production_date,
        qty=qty,
        expire_date=expire_date,
        material_rows=material_rows,
    )
    try:
        result = register_production(data)
        st.success("생산실적이 정상적으로 등록되었습니다.")
        st.write(result)
        st.info(
            """
            저장된 작업:
            1. 완제품 LOT 1건 생성
            2. 생산실적 1건 생성
            3. 선택한 원자재 LOT별 투입 이력 생성
            """
        )
    except ValueError as exc:
        st.error(str(exc))

st.caption(
    "현재 스키마에는 재고 이동 테이블이 없으므로 원자재 LOT 수량 차감은 하지 않는다."
)
