import json
from datetime import datetime

import serial


SERIAL_PORT = "COM3"  # Arduino가 연결된 포트로 변경
BAUD_RATE = 115200


def serial_get():
    try:
        with serial.Serial(
            port=SERIAL_PORT,
            baudrate=BAUD_RATE,
            timeout=2,
        ) as arduino:
            print(f"Arduino 수집기 시작: {SERIAL_PORT}")
            print("종료하려면 Ctrl+C를 누르세요.\n")

            while True:
                raw_data = arduino.readline()

                # timeout 동안 데이터가 없으면 다시 읽기
                if not raw_data:
                    continue

                try:
                    line = raw_data.decode(
                        "utf-8",
                        errors="ignore",
                    ).strip()

                    if not line:
                        continue

                    # JSON 문자열을 Python 딕셔너리로 변환
                    sensor_data = json.loads(line)

                    device_id = sensor_data["device_id"]
                    temperature_c = float(
                        sensor_data["temperature_c"]
                    )
                    collected_at = datetime.now().strftime(
                        "%Y-%m-%d %H:%M:%S"
                    )

                    print(
                        f"[{collected_at}] "
                        f"장치={device_id}, "
                        f"온도={temperature_c:.2f}℃"
                    )

                except json.JSONDecodeError:
                    print(f"잘못된 JSON 데이터: {line}")

                except KeyError as error:
                    print(f"필수 항목이 없습니다: {error}, 데이터={line}")

                except ValueError as error:
                    print(f"온도값 변환 오류: {error}, 데이터={line}")

    except serial.SerialException as error:
        print(f"시리얼 포트 연결 실패: {error}")
        print("COM 포트 번호와 Arduino IDE 시리얼 모니터를 확인하세요.")

    except KeyboardInterrupt:
        print("\nArduino 수집기를 종료합니다.")


if __name__ == "__main__":
    serial_get()
