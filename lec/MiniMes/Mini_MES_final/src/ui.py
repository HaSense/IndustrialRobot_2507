from __future__ import annotations

import pandas as pd
import streamlit as st

from src.db import DB_PATH, database_exists


def setup_page(title: str) -> None:
    st.set_page_config(page_title=f"Mini MES Final - {title}", layout="wide")
    inject_global_style()


def inject_global_style() -> None:
    st.markdown(
        """
        <style>
        section[data-testid="stSidebar"] {
            background:
                linear-gradient(180deg, rgba(19, 32, 47, 0.98) 0%, rgba(24, 46, 66, 0.98) 54%, rgba(29, 73, 69, 0.98) 100%);
            border-right: 1px solid rgba(255,255,255,0.08);
        }
        section[data-testid="stSidebar"] > div {
            padding-top: 1.1rem;
        }
        section[data-testid="stSidebar"] [data-testid="stSidebarNav"] {
            padding-top: 0.4rem;
        }
        section[data-testid="stSidebar"] [data-testid="stSidebarNav"]::before {
            content: "라면공장 Mini MES";
            display: block;
            margin: 0.2rem 0.55rem 0.8rem;
            padding: 16px 14px 14px;
            border-radius: 8px;
            color: #f8fafc;
            font-weight: 850;
            font-size: 1.04rem;
            letter-spacing: 0;
            background:
                linear-gradient(135deg, rgba(201, 52, 43, 0.88), rgba(208, 136, 24, 0.86)),
                repeating-linear-gradient(90deg, rgba(255,255,255,0.12) 0 1px, transparent 1px 38px);
            box-shadow: 0 12px 28px rgba(0,0,0,0.22);
        }
        section[data-testid="stSidebar"] [data-testid="stSidebarNav"]::after {
            content: "운영 대시보드 · 생산 · 품질 · 추적";
            display: block;
            margin: -0.45rem 0.75rem 0.8rem;
            color: #bdd4e8;
            font-size: 0.75rem;
            font-weight: 700;
        }
        section[data-testid="stSidebar"] a {
            border-radius: 8px;
            margin: 0.18rem 0.45rem;
            padding: 0.2rem 0.25rem;
            color: #dbe7f5;
            transition: background 140ms ease, transform 140ms ease, color 140ms ease;
        }
        section[data-testid="stSidebar"] a:hover {
            background: rgba(255,255,255,0.10);
            color: #ffffff;
            transform: translateX(2px);
        }
        section[data-testid="stSidebar"] a[aria-current="page"] {
            background: linear-gradient(90deg, rgba(255,255,255,0.18), rgba(255,255,255,0.06));
            border-left: 4px solid #f59e0b;
            color: #ffffff;
            font-weight: 800;
            box-shadow: inset 0 0 0 1px rgba(255,255,255,0.08);
        }
        section[data-testid="stSidebar"] a span {
            color: inherit;
            font-weight: 700;
        }
        section[data-testid="stSidebar"] div[data-testid="stMarkdownContainer"] p,
        section[data-testid="stSidebar"] label,
        section[data-testid="stSidebar"] .st-emotion-cache-1y4p8pa {
            color: #dbe7f5;
        }
        button[data-testid="baseButton-header"] {
            border-radius: 8px;
        }
        </style>
        """,
        unsafe_allow_html=True,
    )


def page_title(title: str, caption: str) -> None:
    st.title(title)
    st.caption(caption)


def show_database_status() -> None:
    if database_exists():
        st.success(f"DB: {DB_PATH}")
    else:
        st.error(f"DB 파일이 없습니다: {DB_PATH}")


def show_dataframe(df: pd.DataFrame, empty_message: str = "조회 결과가 없습니다.") -> None:
    if df.empty:
        st.warning(empty_message)
        return
    st.dataframe(df, use_container_width=True, hide_index=True)


def metric_row(values: list[tuple[str, object]]) -> None:
    cols = st.columns(len(values))
    for col, (label, value) in zip(cols, values):
        col.metric(label, value)


def row_options(rows: list, label_fields: tuple[str, ...], value_field: str) -> dict[str, int]:
    result = {}
    for row in rows:
        label = " | ".join(str(row[field]) for field in label_fields)
        result[label] = int(row[value_field])
    return result
