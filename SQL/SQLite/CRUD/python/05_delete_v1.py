"""
Ubuntu 24.04 / WSL

실행
python3 delete_student.py
"""

import sqlite3

# student.db 열기
conn = sqlite3.connect("student.db")

# 커서 생성
cursor = conn.cursor()

# 삭제 SQL
sql = """
DELETE FROM student
WHERE id = 1;
"""

# SQL 실행
cursor.execute(sql)

# 저장
conn.commit()

print("데이터 삭제 완료")

# 데이터베이스 닫기
conn.close()
