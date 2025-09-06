import asyncio
import websockets
import cv2
import numpy as np
import keyboard
import threading

# Thay bằng IP của ESP32-CAM
ESP32_WS_URL = "ws://192.168.1.100:80/ws"

async def receive_video(websocket):
    while True:
        try:
            data = await websocket.recv()
            # Nếu là binary data (JPEG frame)
            if isinstance(data, bytes):
                # Chuyển bytes thành numpy array và decode JPEG
                nparr = np.frombuffer(data, np.uint8)
                frame = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
                if frame is not None:
                    cv2.imshow("ESP32-CAM Stream", frame)
                    if cv2.waitKey(1) & 0xFF == ord('q'):
                        break
        except Exception as e:
            print(f"Error receiving frame: {e}")
            break

def send_keys(websocket):
    while True:
        try:
            key = keyboard.read_key()  # Đọc phím nhấn
            if key:
                print(f"Sending key: {key}")
                asyncio.run(websocket.send(key))  # Gửi text qua WebSocket
        except Exception as e:
            print(f"Error sending key: {e}")
            break

async def main():
    async with websockets.connect(ESP32_WS_URL) as websocket:
        print("Connected to ESP32 WebSocket")

        # Thread để gửi phím nhấn
        send_thread = threading.Thread(target=send_keys, args=(websocket,))
        send_thread.start()

        # Nhận và hiển thị video
        await receive_video(websocket)

        # Đóng khi xong
        send_thread.join()
        cv2.destroyAllWindows()

if __name__ == "__main__":
    asyncio.run(main())