import pandas as pd
import streamlit as st

from src import queries
from src.ui import metric_row, page_title, setup_page, show_dataframe


setup_page("기준정보")
page_title("기준정보", "품목, BOM, 설비, 창고, 거래처 기준정보를 조회합니다.")

items_all = queries.items()
bom_all = queries.bom()
work_centers = [dict(row) for row in queries.work_centers()]
equipment = [dict(row) for row in queries.equipment()]
warehouses = [dict(row) for row in queries.warehouses()]
partners = [dict(row) for row in queries.partners()]

product_count = int((items_all["item_type"] == "PRODUCT").sum()) if not items_all.empty else 0
material_count = int((items_all["item_type"] == "MATERIAL").sum()) if not items_all.empty else 0
active_count = int((items_all["is_active"] == "Y").sum()) if not items_all.empty else 0
equipment_status = pd.DataFrame(equipment)["status"].value_counts().to_dict() if equipment else {}

metric_row(
    [
        ("제품", product_count),
        ("원자재", material_count),
        ("BOM 행", len(bom_all)),
        ("설비", len(equipment)),
        ("거래처", len(partners)),
        ("활성 품목", active_count),
    ]
)

if equipment_status.get("DOWN", 0):
    st.warning("비가동 설비가 있습니다. 설비 상태를 확인하세요.")
else:
    st.info("기준 설비는 비가동 상태 없이 등록되어 있습니다.")

tab_items, tab_bom, tab_site = st.tabs(["품목", "BOM", "공장 기준정보"])

with tab_items:
    col1, col2 = st.columns([0.65, 0.35])
    with col1:
        keyword = st.text_input("품목 검색")
    with col2:
        item_type = st.selectbox("품목 유형", ["전체", "PRODUCT", "MATERIAL"])

    df = queries.items(keyword=keyword, item_type=item_type)
    if not df.empty:
        metric_row(
            [
                ("조회 품목", len(df)),
                ("안전재고 합계", f"{df['safety_stock'].sum():,.0f}"),
                ("비활성 품목", int((df["is_active"] == "N").sum())),
            ]
        )
    show_dataframe(df)

with tab_bom:
    products = queries.active_items("PRODUCT")
    options = {"전체": None}
    for item in products:
        options[f"{item['item_code']} | {item['item_name']}"] = item["item_id"]
    selected = st.selectbox("제품", list(options.keys()))
    bom_df = queries.bom(options[selected])

    if not bom_df.empty:
        summary = (
            bom_df.groupby(["product_code", "product_name", "version"], as_index=False)
            .agg(material_count=("material_code", "count"), total_qty_per=("qty_per", "sum"))
            .sort_values(["product_code", "version"])
        )
        st.subheader("제품별 BOM 요약")
        show_dataframe(summary)

    st.subheader("BOM 상세")
    show_dataframe(bom_df)

with tab_site:
    col1, col2 = st.columns(2)
    with col1:
        st.subheader("작업장")
        st.dataframe(work_centers, use_container_width=True, hide_index=True)

        st.subheader("설비")
        if equipment:
            eq_df = pd.DataFrame(equipment)
            metric_row(
                [
                    ("RUN", equipment_status.get("RUN", 0)),
                    ("IDLE", equipment_status.get("IDLE", 0)),
                    ("DOWN", equipment_status.get("DOWN", 0)),
                    ("PM", equipment_status.get("PM", 0)),
                ]
            )
            st.dataframe(eq_df, use_container_width=True, hide_index=True)
        else:
            st.warning("등록된 설비가 없습니다.")

    with col2:
        st.subheader("창고")
        st.dataframe(warehouses, use_container_width=True, hide_index=True)

        st.subheader("거래처")
        if partners:
            partner_df = pd.DataFrame(partners)
            partner_counts = partner_df["partner_type"].value_counts().to_dict()
            metric_row(
                [
                    ("공급사", partner_counts.get("SUPPLIER", 0)),
                    ("고객", partner_counts.get("CUSTOMER", 0)),
                ]
            )
            st.dataframe(partner_df, use_container_width=True, hide_index=True)
        else:
            st.warning("등록된 거래처가 없습니다.")
