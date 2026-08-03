from datetime import date

import streamlit as st

from src import queries
from src.services import ShipmentInput, create_shipment
from src.ui import metric_row, page_title, row_options, setup_page, show_dataframe


setup_page("출하")
page_title("출하", "완제품 LOT를 고객에게 출하하고 LOT별 재고를 차감합니다.")

shipments_all = queries.shipments()
fg_lots_all = queries.lots(item_type="PRODUCT", status="AVAILABLE")
fg_lots_all = fg_lots_all[fg_lots_all["current_qty"] > 0]
planned_shipments = shipments_all[shipments_all["status"] == "PLANNED"] if not shipments_all.empty else shipments_all
shipped_qty = shipments_all["shipment_qty"].sum() if not shipments_all.empty else 0
metric_row(
    [
        ("출하 문서", len(shipments_all)),
        ("출하 대기", len(planned_shipments)),
        ("출하 가능 LOT", len(fg_lots_all)),
        ("누적 출하수량", f"{shipped_qty:,.0f}"),
    ]
)

if not planned_shipments.empty:
    st.info(f"계획 상태 출하 {len(planned_shipments)}건이 있습니다.")

tab_list, tab_create = st.tabs(["출하 이력", "출하 등록"])

with tab_list:
    show_dataframe(shipments_all)
    if not shipments_all.empty:
        selected = st.selectbox("출하 상세", shipments_all["shipment_no"].tolist())
        shipment_id = int(shipments_all[shipments_all["shipment_no"] == selected].iloc[0]["shipment_id"])
        show_dataframe(queries.shipment_details(shipment_id))

with tab_create:
    customers = row_options(queries.partners("CUSTOMER"), ("partner_code", "partner_name"), "partner_id")
    lot_options = {
        f"{row['lot_no']} | {row['item_name']} | 가능 {row['current_qty']:,.0f}": row
        for _, row in fg_lots_all.iterrows()
    }

    form_col, lot_col = st.columns([0.95, 1.05])
    with form_col:
        with st.form("shipment_form"):
            customer_label = st.selectbox("고객", list(customers.keys()))
            shipment_date = st.date_input("출하일", value=date.today())
            shipment_no = st.text_input("출하번호", value=f"SHP-{date.today().strftime('%Y%m%d')}-NEW")
            selected_lots = st.multiselect("출하 LOT", list(lot_options.keys()), default=list(lot_options.keys())[:1])
            detail_rows = []
            for label in selected_lots:
                row = lot_options[label]
                qty = st.number_input(
                    f"{row['lot_no']} 출하수량",
                    min_value=0.0,
                    max_value=float(row["current_qty"]),
                    value=min(100.0, float(row["current_qty"])),
                    step=10.0,
                    key=f"ship_{row['lot_id']}",
                )
                detail_rows.append({"lot_id": int(row["lot_id"]), "item_id": int(row["item_id"]), "qty": qty})
            submitted = st.form_submit_button("출하 저장")

    with lot_col:
        st.subheader("출하 가능 완제품 LOT")
        show_dataframe(fg_lots_all[["lot_no", "item_name", "current_qty", "status", "produced_date", "expire_date"]])

    if detail_rows:
        total_qty = sum(row["qty"] for row in detail_rows)
        st.metric("이번 출하 예정 수량", f"{total_qty:,.0f}")

    if submitted:
        try:
            result = create_shipment(
                ShipmentInput(
                    shipment_no=shipment_no,
                    customer_id=customers[customer_label],
                    shipment_date=shipment_date,
                    detail_rows=detail_rows,
                )
            )
            st.success("출하 등록 완료")
            st.write(result)
        except ValueError as exc:
            st.error(str(exc))
