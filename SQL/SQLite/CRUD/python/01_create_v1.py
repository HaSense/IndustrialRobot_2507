#예외처리가 없는 버전 - 에러 시 대처를 못함 하지만 성공시 직관적

import sqlite3

# student.db 열기 (없으면 자동 생성)
conn = sqlite3.connect("student.db")

# 커서 생성
cursor = conn.cursor()

# student 테이블 생성
sql = """
CREATE TABLE IF NOT EXISTS student (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    phone TEXT
);
"""

cursor.execute(sql)

# 저장
conn.commit()

print("student 테이블 생성 완료")

# 데이터베이스 닫기
conn.close()
