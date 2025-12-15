import sqlite3

# 1. 데이터베이스 연결 (없으면 자동 생성)
conn = sqlite3.connect("addressbook.db")

# 2. 커서 생성
cur = conn.cursor()

# 3. 테이블 생성 SQL
create_table_sql = """
CREATE TABLE IF NOT EXISTS person (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    pnumber TEXT NOT NULL
);
"""

# 4. SQL 실행
cur.execute(create_table_sql)

# 5. 커밋
conn.commit()

# 6. 연결 종료
conn.close()

print("person 테이블이 생성되었습니다.")
