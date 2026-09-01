import serial
from datetime import datetime

# Arduino IDE의 포트 번호로 변경하세요.
SERIAL_PORT = "COM5"
BAUD_RATE = 115200


def serial_check():
    try:
        with serial.Serial(port=SERIAL_PORT, baudrate=BAUD_RATE, timeout=1) as arduino:
            print(f"{SERIAL_PORT}에서 Arduino 신호 수집을 시작합니다.")
            print("종료하려면 Ctrl+C를 누르세요.")

            while True:
                # Arduino가 보낸 한 줄을 읽음
                message = arduino.readline().decode("utf-8", errors="ignore").strip()

                if message == "Pushed":
                    received_at = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                    print(f"[{received_at}] 버튼 입력: {message}")

    except serial.SerialException as error:
        print(f"시리얼 포트를 열 수 없습니다: {error}")
        print("포트 번호와 Arduino IDE 시리얼 모니터 종료 여부를 확인하세요.")

    except KeyboardInterrupt:
        print("\n수집 프로그램을 종료합니다.")


if __name__ == "__main__":
    serial_check()
