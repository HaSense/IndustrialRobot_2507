"""
Ubuntu 24.04 / WSL

실행
python3 update_student.py
"""

import sqlite3

# student.db 열기
conn = sqlite3.connect("student.db")

# 커서 생성
cursor = conn.cursor()

# 수정 SQL
sql = """
UPDATE student
SET phone = '010-9999-9999'
WHERE id = 1;
"""

# SQL 실행
cursor.execute(sql)

# 저장
conn.commit()

print("데이터 수정 완료")

# 데이터베이스 닫기
conn.close()
