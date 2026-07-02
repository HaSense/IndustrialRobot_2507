"""
Ubuntu 24.04 / WSL

실행
python3 insert_student.py
"""

import sqlite3


def main():

    sql = """
    INSERT INTO student (id, name, phone)
    VALUES (1, 'Hong Gil Dong', '010-1234-5678');
    """

    # student.db 열기
    with sqlite3.connect("student.db") as conn:

        # 커서 생성
        cursor = conn.cursor()

        # SQL 실행
        cursor.execute(sql)

        # 저장
        conn.commit()

        print("데이터 삽입 완료")


if __name__ == "__main__":
    main()
