import sqlite3

DB_FILE = "sensor_data.db"
SCHEMA_FILE = "schema.sql"


with sqlite3.connect(DB_FILE) as conn:

    # schema.sql 파일 읽기
    with open(SCHEMA_FILE, "r", encoding="utf-8") as file:
        schema = file.read()

    # SQL 실행
    conn.executescript(schema)

    conn.commit()


print(f"{DB_FILE} 데이터베이스 생성 완료")
