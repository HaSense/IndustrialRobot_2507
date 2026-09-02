import sqlite3
import serial
import time

# --------------------------------------------------
# 설정
# --------------------------------------------------

SERIAL_PORT = "COM5"
BAUD_RATE = 115200

DB_FILE = "sensor_data.db"

# --------------------------------------------------
# 데이터베이스 연결
# --------------------------------------------------

conn = sqlite3.connect(DB_FILE)

cursor = conn.cursor()

# 혹시 테이블이 없다면 자동 생성
cursor.execute("""
CREATE TABLE IF NOT EXISTS sensor_data (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    temperature REAL NOT NULL,
    humidity REAL NOT NULL,
    created_at TEXT NOT NULL DEFAULT (
        strftime('%Y-%m-%d %H:%M:%S', 'now', 'localtime')
    )
)
""")

conn.commit()


# --------------------------------------------------
# Arduino Serial 연결
# --------------------------------------------------

ser = serial.Serial(
    port=SERIAL_PORT,
    baudrate=BAUD_RATE,
    timeout=1
)

# Arduino가 Serial 연결 후 재시작할 시간을 조금 기다림
time.sleep(2)

print("Temperature / Humidity Collector Started")
print(f"Serial Port : {SERIAL_PORT}")
print(f"Database    : {DB_FILE}")
print("------------------------------------------")


# --------------------------------------------------
# 데이터 수집
# --------------------------------------------------

try:
    while True:
        # Arduino에서 한 줄 읽기
        line = ser.readline().decode(
            "utf-8",
            errors="ignore"
        ).strip()

        # 빈 데이터는 무시
        if not line:
            continue

        print(f"수신: {line}")

        try:

            # Arduino 데이터
            #
            # 24.00,45.00
            #
            # "," 기준으로 분리
            temp_text, humi_text = line.split(",")

            # 문자열 → float
            temperature = float(temp_text)
            humidity = float(humi_text)


            # --------------------------------------------------
            # SQLite 저장
            #
            # created_at은 입력하지 않는다.
            # SQLite가 현재 시간을 자동으로 입력한다.
            # --------------------------------------------------

            cursor.execute(
                """
                INSERT INTO sensor_data (
                    temperature,
                    humidity
                )
                VALUES (?, ?)
                """,
                (
                    temperature,
                    humidity
                )
            )

            conn.commit()


            print(
                f"저장 완료 -> "
                f"Temperature: {temperature:.1f} C, "
                f"Humidity: {humidity:.1f} %"
            )


        except ValueError:

            print(f"데이터 형식 오류: {line}")


except KeyboardInterrupt:

    print("\n수집기를 종료합니다.")


finally:

    # Serial 연결 종료
    ser.close()

    # DB 연결 종료
    conn.close()
