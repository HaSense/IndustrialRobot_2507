# app.py 위치에 두고 실행하면 됩니다.

import streamlit as st

from src import queries
from src.ui import metric_row, page_title, setup_page, show_database_status, show_dataframe
from src.queries import fetch_dataframe

setup_page("소개")
show_database_status()

def item2(id: str = ""):
    return fetch_dataframe(
        """
        SELECT *
        FROM item
        WHERE ? = ''
           OR item_id = ?
        ORDER BY item_id
        """,
        (id, id),
    )

def item3(keyword: str = "라면"):
    return fetch_dataframe(
        """
        SELECT *
        FROM item
        WHERE ? = ''
           OR item_name LIKE ?
        ORDER BY item_id
        """,
        (keyword, f"%{keyword}%"),
    )

try:
    id = st.text_input("id로 검색")
    df = item2(id ='')
    st.subheader("조회 결과")
    show_dataframe(df)

    keyword = st.text_input("제품 이름으로 검색")
    df2 = item3(keyword)
    st.subheader("조회 결과2")
    show_dataframe(df2)

except Exception as exc:
    st.error("데이터베이스 구조를 확인하는 중 오류가 발생했습니다.")
    st.exception(exc)
