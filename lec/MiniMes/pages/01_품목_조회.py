import streamlit as st

from src import queries
from src.ui import metric_row, page_title, setup_page, show_dataframe


setup_page("품목 조회")

page_title(
    "품목 조회",
    "품목 기준정보가 제품과 원자재를 함께 관리하는 방식을 학습합니다.",
    "item -> lot, item -> production_material",
    "품목 유형과 검색어로 품목을 조회하고 LOT 연결 건수를 확인합니다.",
)

keyword = st.text_input("품목 코드 또는 품목명 검색")
item_type = st.selectbox("품목 유형", ["전체", "PRODUCT", "MATERIAL"])

df = queries.items(keyword=keyword, item_type=item_type)
type_counts = queries.item_type_counts()

if not type_counts.empty:
    count_map = dict(zip(type_counts["item_type"], type_counts["item_count"]))
    metric_row(
        [
            ("전체 품목", int(type_counts["item_count"].sum())),
            ("제품", count_map.get("PRODUCT", 0)),
            ("원자재", count_map.get("MATERIAL", 0)),
        ]
    )

st.subheader("조회 결과")
show_dataframe(df)

if not df.empty:
    selected_item = st.selectbox("상세 확인 품목", df["item_name"].tolist())
    selected_row = df[df["item_name"] == selected_item].iloc[0]
    st.write(
        {
            "품목 ID": int(selected_row["item_id"]),
            "품목 코드": selected_row["item_code"],
            "품목 유형": selected_row["item_type"],
            "단위": selected_row["unit"],
            "연결 LOT 수": int(selected_row["lot_count"]),
        }
    )

st.caption(
    "`item`은 기준정보다. 실제 입고 또는 생산으로 생긴 묶음과 수량은 `lot`에서 확인한다."
)
