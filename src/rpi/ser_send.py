import subprocess
import serial

# シリアルポートの設定
ser = serial.Serial('/dev/ttyS0', 115200, timeout=1)

def system_check():
    try:
        log = subprocess.check_output(['journalctl -p 4 -b'])
        warning_count = log.lower().count("warning")
        message = "boot"
    except Exception as e:
        message = e
    return warning_count, message


if __name__ == "__main__":
    result = system_check()
    ser.write(result.encode())