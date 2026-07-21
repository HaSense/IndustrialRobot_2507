import streamlit as st

from src import queries
from src.ui import metric_row, page_title, setup_page, show_dataframe


setup_page("LOT 조회")

page_title(
    "LOT 조회",
    "품목과 LOT의 차이, 원자재 LOT와 완제품 LOT의 차이를 확인합니다.",
    "lot JOIN item",
    "LOT 번호, LOT 유형, 품목 기준으로 데이터를 좁혀 봅니다.",
)

items = queries.active_items_for_select()
item_options = {"전체": None}
for item in items:
    item_options[f"{item['item_code']} | {item['item_name']}"] = item["item_id"]

col1, col2, col3 = st.columns(3)
keyword = col1.text_input("LOT 번호 검색")
lot_type = col2.selectbox("LOT 유형", ["전체", "RECEIPT", "PRODUCTION"])
item_label = col3.selectbox("품목", list(item_options.keys()))

df = queries.lots(keyword=keyword, lot_type=lot_type, item_id=item_options[item_label])
if not df.empty:
    metric_row(
        [
            ("LOT 수", len(df)),
            ("총 수량", f"{df['qty'].sum():,.0f}"),
            ("품목 수", df["item_code"].nunique()),
        ]
    )

st.subheader("LOT와 품목 JOIN 결과")
show_dataframe(df)

if not df.empty:
    selected_lot = st.selectbox("상세 확인 LOT", df["lot_no"].tolist())
    selected_row = df[df["lot_no"] == selected_lot].iloc[0]
    st.write(
        {
            "LOT ID": int(selected_row["lot_id"]),
            "품목": selected_row["item_name"],
            "품목 유형": selected_row["item_type"],
            "LOT 유형": selected_row["lot_type"],
            "수량": float(selected_row["qty"]),
            "입고일": selected_row["received_date"],
            "생산일": selected_row["produced_date"],
            "유효기한": selected_row["expire_date"],
        }
    )

st.caption(
    "`lot_type = 'RECEIPT'`는 원자재 입고 LOT이고, `lot_type = 'PRODUCTION'`은 생산 결과 완제품 LOT다."
)
