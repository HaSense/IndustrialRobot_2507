CREATE TABLE IF NOT EXISTS sensor_data (
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    -- 온도 (섭씨)
    temperature REAL NOT NULL,

    -- 상대습도 (%)
    humidity REAL NOT NULL,

    -- 데이터가 DB에 저장된 시간
    -- Python에서 넣지 않고 SQLite가 자동 생성
    created_at TEXT NOT NULL DEFAULT (
        strftime('%Y-%m-%d %H:%M:%S', 'now', 'localtime')
    )
);
