from datetime import date

import streamlit as st

from src import queries
from src.services import InspectionInput, register_inspection
from src.ui import metric_row, page_title, setup_page, show_dataframe


setup_page("품질검사")
page_title("품질검사", "입고, 공정, 최종검사 결과를 LOT 기준으로 등록하고 보류 상태를 관리합니다.")

quality = queries.quality_inspections()
result_counts = quality["result"].value_counts().to_dict() if not quality.empty else {}
metric_row(
    [
        ("PASS", result_counts.get("PASS", 0)),
        ("HOLD", result_counts.get("HOLD", 0)),
        ("FAIL", result_counts.get("FAIL", 0)),
        ("검사 이력", len(quality)),
    ]
)

if result_counts.get("HOLD", 0) or result_counts.get("FAIL", 0):
    st.warning("보류 또는 불합격 검사 결과가 있습니다. 해당 LOT 상태를 확인하세요.")

tab_list, tab_create = st.tabs(["검사 이력", "검사 등록"])

with tab_list:
    col1, col2 = st.columns(2)
    result_filter = col1.selectbox("판정", ["전체", "PASS", "HOLD", "FAIL"])
    type_filter = col2.selectbox("검사 유형", ["전체", "INCOMING", "PROCESS", "FINAL"])
    df = quality.copy()
    if result_filter != "전체":
        df = df[df["result"] == result_filter]
    if type_filter != "전체":
        df = df[df["inspection_type"] == type_filter]
    show_dataframe(df)

with tab_create:
    lots = queries.lots()
    lot_options = {
        f"{row['lot_no']} | {row['item_name']} | {row['status']}": int(row["lot_id"])
        for _, row in lots.iterrows()
    }
    productions = queries.productions()
    production_options = {"없음": None}
    for _, row in productions.iterrows():
        production_options[f"{row['production_no']} | {row['item_name']}"] = int(row["production_id"])

    form_col, recent_col = st.columns([0.95, 1.05])
    with form_col:
        with st.form("inspection_form"):
            inspection_type = st.selectbox("검사 유형", ["INCOMING", "PROCESS", "FINAL"])
            lot_label = st.selectbox("검사 LOT", list(lot_options.keys()))
            production_label = st.selectbox("연결 생산실적", list(production_options.keys()))
            inspection_date = st.date_input("검사일", value=date.today())
            inspection_no = st.text_input("검사번호", value=f"QC-{date.today().strftime('%Y%m%d')}-NEW")
            sample_qty = st.number_input("샘플수량", min_value=0.0, value=100.0, step=10.0)
            defect_qty = st.number_input("불량수량", min_value=0.0, value=0.0, step=1.0)
            result = st.selectbox("판정", ["PASS", "HOLD", "FAIL"])
            memo = st.text_area("메모")
            submitted = st.form_submit_button("검사 저장")

    with recent_col:
        st.subheader("최근 검사")
        show_dataframe(quality.head(6))

    if submitted:
        try:
            result_data = register_inspection(
                InspectionInput(
                    inspection_no=inspection_no,
                    inspection_type=inspection_type,
                    lot_id=lot_options[lot_label],
                    production_id=production_options[production_label],
                    inspection_date=inspection_date,
                    sample_qty=sample_qty,
                    defect_qty=defect_qty,
                    result=result,
                    memo=memo,
                )
            )
            st.success("검사 등록 완료")
            st.write(result_data)
        except ValueError as exc:
            st.error(str(exc))
