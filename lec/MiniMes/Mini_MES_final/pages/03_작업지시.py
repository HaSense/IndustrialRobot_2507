from datetime import date

import streamlit as st

from src import queries
from src.services import ProductionOrderInput, create_production_order
from src.ui import metric_row, page_title, row_options, setup_page, show_dataframe


setup_page("작업지시")
page_title("작업지시", "제품별 생산계획을 작업지시로 발행하고 진행 상태를 봅니다.")

orders_all = queries.orders()
status_counts = orders_all["status"].value_counts().to_dict() if not orders_all.empty else {}
open_orders = orders_all[orders_all["status"].isin(["PLANNED", "RELEASED"])] if not orders_all.empty else orders_all
metric_row(
    [
        ("계획", status_counts.get("PLANNED", 0)),
        ("발행", status_counts.get("RELEASED", 0)),
        ("완료", status_counts.get("COMPLETED", 0)),
        ("취소", status_counts.get("CANCELED", 0)),
    ]
)

if not open_orders.empty:
    st.info(f"진행 대상 작업지시 {len(open_orders)}건이 있습니다.")

tab_list, tab_create = st.tabs(["작업지시 조회", "작업지시 발행"])

with tab_list:
    status = st.selectbox("상태", ["전체", "PLANNED", "RELEASED", "COMPLETED", "CANCELED"])
    df = queries.orders(status=status)
    show_dataframe(df)

    if not open_orders.empty:
        st.subheader("진행 대상 우선순위")
        show_dataframe(open_orders.sort_values(["priority", "due_date", "order_no"]))

with tab_create:
    products = row_options(queries.active_items("PRODUCT"), ("item_code", "item_name"), "item_id")
    centers = row_options(queries.work_centers(), ("work_center_code", "work_center_name"), "work_center_id")

    form_col, ref_col = st.columns([0.95, 1.05])
    with form_col:
        with st.form("order_form"):
            product_label = st.selectbox("생산 제품", list(products.keys()))
            center_label = st.selectbox("작업장", list(centers.keys()))
            plan_date = st.date_input("계획일", value=date.today())
            due_date = st.date_input("납기일", value=date.today())
            order_no = st.text_input("작업지시 번호", value=f"WO-{date.today().strftime('%Y%m%d')}-NEW")
            order_qty = st.number_input("지시수량", min_value=0.0, value=1000.0, step=100.0)
            priority = st.slider("우선순위", min_value=1, max_value=5, value=3)
            submitted = st.form_submit_button("작업지시 발행")

    with ref_col:
        st.subheader("제품 BOM 참고")
        selected_product_id = products[product_label]
        show_dataframe(queries.bom(selected_product_id))

    if submitted:
        try:
            result = create_production_order(
                ProductionOrderInput(
                    order_no=order_no,
                    product_item_id=products[product_label],
                    work_center_id=centers[center_label],
                    plan_date=plan_date,
                    due_date=due_date,
                    order_qty=order_qty,
                    priority=priority,
                )
            )
            st.success("작업지시 발행 완료")
            st.write(result)
        except ValueError as exc:
            st.error(str(exc))
