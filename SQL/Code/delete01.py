import sqlite3

path = '/mnt/c/Users/[UserID]/Code/sql/sqlite/sample1/'
conn = sqlite3.connect(path + "addressbook.db")
cur = conn.cursor()

sql = "DELETE FROM person WHERE id = ?;"
cur.execute(sql, (1,))   # id = 1 삭제

conn.commit()
print(f"삭제된 행 수: {cur.rowcount}")

conn.close()
