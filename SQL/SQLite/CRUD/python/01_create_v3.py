"""
Ubuntu 24.04 / WSL

실행
python3 create_student.py
"""

import sqlite3


def main():

    sql = """
    CREATE TABLE IF NOT EXISTS student (
        id INTEGER PRIMARY KEY,
        name TEXT NOT NULL,
        phone TEXT
    );
    """

    try:
        # student.db 열기 (없으면 자동 생성)
        with sqlite3.connect("student.db") as conn:

            # 커서 생성
            cursor = conn.cursor()

            # SQL 실행
            cursor.execute(sql)

            # 저장
            conn.commit()

            print("student 테이블 생성 완료")

    except sqlite3.Error as e:
        print("테이블 생성 실패 :", e)


if __name__ == "__main__":
    main()
