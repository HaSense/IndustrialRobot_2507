import sqlite3

path=path = '/mnt/c/Users/shhawork/Code/sql/sqlite/sample1/'
conn = sqlite3.connect(path + "addressbook.db")
cur = conn.cursor()

sql = """
INSERT INTO person (name, pnumber)
VALUES (?, ?);
"""

data_list = [
    ("이순신", "010-2222-2222"),
    ("강감찬", "010-3333-3333")
]

cur.executemany(sql, data_list)

conn.commit()
conn.close()

print("데이터 2건이 추가되었습니다.")
