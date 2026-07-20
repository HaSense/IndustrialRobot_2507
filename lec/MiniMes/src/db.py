
from pathlib import Path
import sqlite3

import pandas as pd


BASE_DIR = Path(__file__).resolve().parent.parent
DB_PATH = BASE_DIR / "sql" / "mini_mes.db"


def database_exists() -> bool:
    return DB_PATH.exists()


def get_connection() -> sqlite3.Connection:
    if not database_exists():
        raise FileNotFoundError(f"SQLite 데이터베이스 파일을 찾을 수 없습니다: {DB_PATH}")

    connection = sqlite3.connect(DB_PATH)
    connection.row_factory = sqlite3.Row
    connection.execute("PRAGMA foreign_keys = ON")
    return connection


def fetch_dataframe(sql: str, params: tuple = ()) -> pd.DataFrame:
    with get_connection() as connection:
        return pd.read_sql_query(sql, connection, params=params)


def fetch_one(sql: str, params: tuple = ()) -> sqlite3.Row | None:
    with get_connection() as connection:
        cursor = connection.execute(sql, params)
        return cursor.fetchone()


def fetch_all(sql: str, params: tuple = ()) -> list[sqlite3.Row]:
    with get_connection() as connection:
        cursor = connection.execute(sql, params)
        return cursor.fetchall()
