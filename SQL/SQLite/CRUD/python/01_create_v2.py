"""
Ubuntu 24.04 / WSL

실행
python3 create_student.py
"""

import sqlite3

conn = None

try:
    # student.db 열기 (없으면 자동 생성)
    conn = sqlite3.connect("student.db")

    # SQL 실행을 위한 커서 생성
    cursor = conn.cursor()

    # student 테이블 생성
    sql = """
    CREATE TABLE IF NOT EXISTS student (
        id INTEGER PRIMARY KEY,
        name TEXT NOT NULL,
        phone TEXT
    );
    """

    # SQL 실행
    cursor.execute(sql)

    # 변경사항 저장
    conn.commit()

    print("student 테이블 생성 완료")

except sqlite3.Error as e:
    print("테이블 생성 실패 :", e)

finally:
    # 데이터베이스 닫기
    if conn:
        conn.close()
