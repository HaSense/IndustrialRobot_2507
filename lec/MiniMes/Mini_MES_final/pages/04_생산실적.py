from datetime import date

import pandas as pd
import streamlit as st

from src import queries
from src.services import ProductionCompletion, bom_requirements, complete_production
from src.ui import metric_row, page_title, row_options, setup_page, show_dataframe


setup_page("생산실적")
page_title("생산실적", "작업지시를 완료 처리하고 원자재 투입, 완제품 LOT, 재고 이력을 동시에 저장합니다.")

productions_all = queries.productions()
open_orders = queries.releasable_orders()
total_good = productions_all["good_qty"].sum() if not productions_all.empty else 0
total_defect = productions_all["defect_qty"].sum() if not productions_all.empty else 0
avg_yield = productions_all["yield_rate"].mean() if not productions_all.empty else 0
metric_row(
    [
        ("생산실적", len(productions_all)),
        ("미완료 지시", len(open_orders)),
        ("누적 양품", f"{total_good:,.0f}"),
        ("평균 수율", f"{avg_yield:.2f}%"),
        ("누적 불량", f"{total_defect:,.0f}"),
    ]
)

tab_list, tab_complete = st.tabs(["생산실적 조회", "생산 완료 등록"])

with tab_list:
    show_dataframe(productions_all)
    if not productions_all.empty:
        selected = st.selectbox("투입 원자재 확인", productions_all["production_no"].tolist())
        production_id = int(productions_all[productions_all["production_no"] == selected].iloc[0]["production_id"])
        st.subheader("선택 생산의 원자재 투입")
        show_dataframe(queries.production_materials(production_id))

with tab_complete:
    orders = open_orders
    products = queries.active_items("PRODUCT")
    order_options = {"작업지시 없이 등록": None}
    for row in orders:
        order_options[f"{row['order_no']} | {row['item_name']} | {row['order_qty']:,.0f}"] = row
    product_options = row_options(products, ("item_code", "item_name"), "item_id")
    equipment_options = row_options(queries.equipment(), ("equipment_code", "equipment_name"), "equipment_id")

    input_col, material_col = st.columns([0.9, 1.1])
    with input_col:
        order_label = st.selectbox("작업지시", list(order_options.keys()))
        selected_order = order_options[order_label]
        if selected_order:
            product_item_id = int(selected_order["product_item_id"])
            default_qty = float(selected_order["order_qty"])
            st.info(f"선택 지시 제품: {selected_order['item_name']} / 지시수량 {default_qty:,.0f}")
        else:
            product_label = st.selectbox("제품", list(product_options.keys()))
            product_item_id = product_options[product_label]
            default_qty = 1000.0

        production_date = st.date_input("생산일", value=date.today())
        good_qty = st.number_input("양품수량", min_value=0.0, value=default_qty, step=100.0)
        defect_qty = st.number_input("불량수량", min_value=0.0, value=0.0, step=10.0)
        if good_qty + defect_qty > 0:
            st.metric("예상 수율", f"{good_qty * 100 / (good_qty + defect_qty):.2f}%")

    requirements = bom_requirements(product_item_id, good_qty)
    material_rows = []
    preview_rows = []
    with material_col:
        st.subheader("BOM 소요량과 투입 LOT")
        for req in requirements:
            lots = queries.available_lots_for_item(req["material_item_id"])
            labels = {
                f"{lot['lot_no']} | 현재고 {lot['current_qty']:,.0f} | 유효 {lot['expire_date']}": lot
                for lot in lots
            }
            st.write(f"{req['item_code']} | {req['item_name']} 필요수량: {req['required_qty']:,.3f}")
            if not labels:
                st.error("사용 가능한 LOT가 없습니다.")
                continue
            lot_label = st.selectbox("투입 LOT", list(labels.keys()), key=f"lot_{req['material_item_id']}")
            issued_qty = st.number_input(
                "투입수량",
                min_value=0.0,
                value=float(req["required_qty"]),
                step=10.0,
                key=f"qty_{req['material_item_id']}",
            )
            lot = labels[lot_label]
            material_rows.append(
                {
                    "material_item_id": req["material_item_id"],
                    "material_lot_id": int(lot["lot_id"]),
                    "required_qty": float(req["required_qty"]),
                    "issued_qty": issued_qty,
                }
            )
            preview_rows.append(
                {
                    "원자재": req["item_name"],
                    "LOT": lot["lot_no"],
                    "필요수량": float(req["required_qty"]),
                    "투입수량": issued_qty,
                    "투입후 예상재고": float(lot["current_qty"]) - issued_qty,
                }
            )
        if preview_rows:
            show_dataframe(pd.DataFrame(preview_rows))

    with st.form("complete_form"):
        equipment_label = st.selectbox("사용 설비", list(equipment_options.keys()))
        production_no = st.text_input("생산실적 번호", value=f"PRD-{date.today().strftime('%Y%m%d')}-NEW")
        output_lot_no = st.text_input("완제품 LOT 번호", value=f"FG-{date.today().strftime('%Y%m%d')}-NEW")
        submitted = st.form_submit_button("생산 완료 저장")

    if submitted:
        try:
            result = complete_production(
                ProductionCompletion(
                    order_id=int(selected_order["order_id"]) if selected_order else None,
                    production_no=production_no,
                    product_item_id=product_item_id,
                    output_lot_no=output_lot_no,
                    equipment_id=equipment_options[equipment_label],
                    production_date=production_date,
                    good_qty=good_qty,
                    defect_qty=defect_qty,
                    material_rows=material_rows,
                )
            )
            st.success("생산 완료 등록 완료")
            st.write(result)
        except ValueError as exc:
            st.error(str(exc))
