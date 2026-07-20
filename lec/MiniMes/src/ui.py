from __future__ import annotations

import pandas as pd
import streamlit as st

from src.db import DB_PATH, database_exists


def setup_page(title: str):
    st.set_page_config(page_title=f"Mini MES - {title}", layout="wide")


def page_title(title: str, description: str, tables: str, task: str):
    st.title(title)
    st.info(
        f"""
        이 화면에서 배우는 내용: {description}

        관련 테이블: {tables}

        학생이 수행할 작업: {task}
        """
    )


def show_database_status():
    if database_exists():
        st.success(f"데이터베이스 연결 대상: {DB_PATH}")
    else:
        st.error(f"데이터베이스 파일을 찾을 수 없습니다: {DB_PATH}")


def show_dataframe(df: pd.DataFrame, empty_message: str = "조건에 해당하는 데이터가 없습니다."):
    if df.empty:
        st.warning(empty_message)
        return
    st.dataframe(df, use_container_width=True, hide_index=True)


def metric_row(values: list[tuple[str, object]]):
    columns = st.columns(len(values))
    for column, (label, value) in zip(columns, values):
        column.metric(label, value)


def row_to_dict(row):
    if row is None:
        return {}
    return {key: row[key] for key in row.keys()}
