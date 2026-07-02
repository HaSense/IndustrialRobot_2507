"""
Ubuntu 24.04 / WSL

실행
python3 select_student.py
"""

import sqlite3

# student.db 열기
conn = sqlite3.connect("student.db")

# 커서 생성
cursor = conn.cursor()

# 조회 SQL
sql = """
SELECT id, name, phone
FROM student;
"""

# SQL 실행
cursor.execute(sql)

# 조회 결과 출력
rows = cursor.fetchall()

for row in rows:
    print(row)

# 데이터베이스 닫기
conn.close()
