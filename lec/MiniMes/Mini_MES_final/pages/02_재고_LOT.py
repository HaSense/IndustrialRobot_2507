from datetime import date, timedelta

import streamlit as st

from src import queries
from src.services import MaterialReceipt, receive_material
from src.ui import metric_row, page_title, row_options, setup_page, show_dataframe


setup_page("재고 LOT")
page_title("재고 LOT", "원자재 입고, LOT 재고, 재고 이동 이력을 관리합니다.")

lots_all = queries.lots()
inventory = queries.inventory_summary()
txn_recent = queries.inventory_txns().head(8)
shortage = inventory[inventory["stock_status"] == "부족"]
hold_lots = lots_all[lots_all["status"] == "HOLD"]
available_lots = lots_all[(lots_all["status"] == "AVAILABLE") & (lots_all["current_qty"] > 0)]

metric_row(
    [
        ("가용 LOT", len(available_lots)),
        ("재고 부족 품목", len(shortage)),
        ("품질 보류 LOT", len(hold_lots)),
        ("최근 이동 건수", len(txn_recent)),
    ]
)

if not shortage.empty:
    st.warning("안전재고 미만 품목이 있습니다. 품목별 재고 탭에서 확인하세요.")
if not hold_lots.empty:
    st.info("품질 보류 LOT는 출하/투입 대상에서 제외됩니다.")

tab_lot, tab_receipt, tab_txn = st.tabs(["LOT 재고", "원자재 입고", "이동 이력"])

with tab_lot:
    col1, col2, col3 = st.columns(3)
    keyword = col1.text_input("LOT/품목 검색")
    item_type = col2.selectbox("품목 유형", ["전체", "MATERIAL", "PRODUCT"])
    status = col3.selectbox("상태", ["전체", "AVAILABLE", "QUARANTINE", "HOLD", "CONSUMED", "SHIPPED"])
    df = queries.lots(keyword=keyword, item_type=item_type, status=status)
    if not df.empty:
        metric_row(
            [
                ("조회 LOT", len(df)),
                ("현재고 합계", f"{df['current_qty'].sum():,.0f}"),
                ("품목 수", df["item_code"].nunique()),
            ]
        )
    show_dataframe(df)

    st.subheader("품목별 재고")
    show_dataframe(inventory)

with tab_receipt:
    materials = row_options(queries.active_items("MATERIAL"), ("item_code", "item_name"), "item_id")
    suppliers = row_options(queries.partners("SUPPLIER"), ("partner_code", "partner_name"), "partner_id")
    raw_wh = row_options(queries.warehouses("RAW"), ("warehouse_code", "warehouse_name"), "warehouse_id")

    form_col, recent_col = st.columns([0.95, 1.05])
    with form_col:
        with st.form("receipt_form"):
            item_label = st.selectbox("원자재", list(materials.keys()))
            supplier_label = st.selectbox("공급사", list(suppliers.keys()))
            warehouse_label = st.selectbox("입고 창고", list(raw_wh.keys()))
            received_date = st.date_input("입고일", value=date.today())
            lot_no = st.text_input("입고 LOT 번호", value=f"RM-{date.today().strftime('%Y%m%d')}-NEW")
            qty = st.number_input("입고수량", min_value=0.0, value=1000.0, step=100.0)
            use_expire = st.checkbox("유효기한 입력", value=True)
            expire_date = None
            if use_expire:
                expire_date = st.date_input("유효기한", value=date.today() + timedelta(days=120))
            submitted = st.form_submit_button("입고 등록")

    with recent_col:
        st.subheader("최근 재고 이동")
        show_dataframe(txn_recent)

    if submitted:
        try:
            result = receive_material(
                MaterialReceipt(
                    lot_no=lot_no,
                    item_id=materials[item_label],
                    warehouse_id=raw_wh[warehouse_label],
                    supplier_id=suppliers[supplier_label],
                    qty=qty,
                    received_date=received_date,
                    expire_date=expire_date,
                )
            )
            st.success("입고 등록 완료")
            st.write(result)
        except ValueError as exc:
            st.error(str(exc))

with tab_txn:
    lot_options = {"전체": None}
    for _, row in lots_all.iterrows():
        lot_options[f"{row['lot_no']} | {row['item_name']}"] = int(row["lot_id"])
    selected = st.selectbox("LOT", list(lot_options.keys()))
    show_dataframe(queries.inventory_txns(lot_options[selected]))
