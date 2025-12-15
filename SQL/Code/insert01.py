import sqlite3

path = '/mnt/c/Users/[UserID]/Code/sql/sqlite/sample1/'
conn = sqlite3.connect(path + "addressbook.db")
cur = conn.cursor()

sql = """
INSERT INTO person (name, pnumber)
VALUES (?, ?);
"""

cur.execute(sql, ("홍길동", "010-1111-1111"))

conn.commit()
conn.close()

print("데이터 1건이 추가되었습니다.")
