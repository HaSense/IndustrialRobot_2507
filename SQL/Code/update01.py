import sqlite3

path = "/mnt/c/Users/[UserId]/Code/sql/sqlite/sample1/"
db_file = path + "addressbook.db"

conn = sqlite3.connect(db_file)
cur = conn.cursor()

sql = """
UPDATE person
SET name = ?, pnumber = ?
WHERE id = ?;
"""

cur.execute(sql, ("홍길동(수정)", "010-9999-9999", 1))

conn.commit()
print(f"수정된 행 수: {cur.rowcount}")

conn.close()
