import sqlite3

path = '/mnt/c/Users/shhawork/Code/sql/sqlite/sample1/'
conn = sqlite3.connect("addressbook.db")
cur = conn.cursor()

sql = "SELECT id, name, pnumber FROM person;"
cur.execute(sql)

rows = cur.fetchall()

for row in rows:
    print(row)

conn.close()
