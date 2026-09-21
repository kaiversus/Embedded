# 📋 CHECKLIST DỰ ÁN: HỆ THỐNG ĐIỂM DANH THÔNG MINH 2 LỚP
### RFID + Edge AI Face Verification + Cloud Sync

> **Thời gian:** 4 tuần | **Nhóm:** Bảo · Đạt · Đăng
> **Cập nhật lần cuối:** 27/08/2026

---

## 👥 PHÂN CÔNG TỔNG QUAN

| Thành viên | Phạm vi phụ trách | Phụ trách chính |
|:---:|:---|:---|
| 🔧 **BẢO** | **Tất cả phần cứng + code nhúng** | ESP32 (RFID/OLED/Buzzer/WiFi), Raspberry Pi 4 (OS/Webcam/Flask/Face AI) |
| 🎨 **ĐẠT** | **UI/UX + Bot + hỗ trợ Bảo phần phi cứng** | Web Dashboard, Telegram Bot, chuẩn bị dữ liệu khuôn mặt, Python Telegram service |
| 📊 **ĐĂNG** | **Cloud backend + Kiểm thử + Báo cáo** | Firebase Python, Offline Cache SQLite, toàn bộ báo cáo & slide |

> ⚠️ **Nguyên tắc phân công:** Bất kỳ task nào liên quan đến phần cứng hoặc code chạy trực tiếp trên thiết bị (ESP32, Raspberry Pi) → **BẢO** phụ trách. Bảo load nặng nên Đạt hỗ trợ phần data prep và Python service không liên quan phần cứng.

---

## 🗺️ SƠ ĐỒ KIẾN TRÚC (ai làm gì)

```
[Sinh viên quẹt thẻ]
        │
        ▼
   [RC522] ──(SPI)──► [ESP32 Node (OLED + Buzzer)]  ◄── 🔧 BẢO
                             │
                      (HTTP POST UID)
                             ▼
   [Webcam 1080p] ──► [Raspberry Pi 4]  ◄────────────── 🔧 BẢO
                         │  (Flask Server + Face Recognition)
                         │
          ┌──────────────┴──────────────┐
          ▼                             ▼
 [Firebase DB & Storage]       [Telegram Bot API]
    ◄── 📊 ĐĂNG (Python)         ◄── 🎨 ĐẠT (Python service)
          │
          ▼
 [Web Dashboard]  ◄──────────────────── 🎨 ĐẠT
```

---
---

# 🔧 BẢO — PHẦN CỨNG & CODE NHÚNG
### (ESP32 Node + Raspberry Pi 4 + Edge AI + Flask Server)

> **Nhiệm vụ:** Toàn bộ tầng thiết bị — từ lắp mạch ESP32, lập trình firmware, cài đặt Raspberry Pi, đến viết server AI nhận diện khuôn mặt và Flask API. Đây là backbone của hệ thống.

---

## 📚 KNOWLEDGE BẢO CẦN NẮM

### Nhóm ESP32
- [ ] **C/C++ cơ bản:** Pointer, struct, vòng lặp, hàm callback
- [ ] **Arduino IDE / PlatformIO:** Cài đặt, nạp firmware, Serial Monitor
- [ ] **ESP32 Pinout:** GPIO, SPI (MOSI/MISO/SCK/SS), I2C (SDA/SCL), PWM
- [ ] **Giao thức SPI:** RC522 giao tiếp qua 4 dây, thư viện `MFRC522`
- [ ] **Giao thức I2C:** OLED SSD1306, thư viện `Adafruit SSD1306`
- [ ] **WiFi + HTTP trên ESP32:** `WiFi.h`, `HTTPClient.h`, `ArduinoJson`

### Nhóm Raspberry Pi
- [ ] **Linux cơ bản:** `apt`, `pip`, `nano`, `ssh`, `systemctl`
- [ ] **Python 3:** Hàm, class, import, try/except, threading
- [ ] **OpenCV Python:** `cv2.VideoCapture()`, `cv2.imwrite()`, `cv2.cvtColor()`
- [ ] **face_recognition:** `face_encodings()`, `compare_faces()`, `face_distance()`
- [ ] **Flask:** Route, `request.json`, `jsonify`, `app.run(host, port)`

**Tài nguyên:**
- ESP32 Pinout: https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
- RC522 + ESP32: https://randomnerdtutorials.com/esp32-rfid-reader-rc522/
- OLED + ESP32: https://randomnerdtutorials.com/esp32-ssd1306-oled-display-arduino-ide/
- face_recognition: https://github.com/ageitgey/face_recognition
- Flask Quickstart: https://flask.palletsprojects.com/quickstart/

---

## 📅 TUẦN 1 — ESP32: Lắp mạch + RFID + OLED + Buzzer

### 🎯 Mục tiêu: Quẹt thẻ → Serial in UID → OLED hiển thị → Buzzer bíp

#### Lắp ráp phần cứng ESP32
- [ ] **[BẢO] HW-01:** Kết nối RC522 với ESP32 qua SPI
  - Chân: SDA→GPIO5, SCK→GPIO18, MOSI→GPIO23, MISO→GPIO19, RST→GPIO4
  - ✔ *Đạt khi:* Đèn LED RC522 sáng, không cắm ngược
- [ ] **[BẢO] HW-02:** Kết nối OLED 0.96" qua I2C
  - Chân: SDA→GPIO21, SCL→GPIO22, VCC→3.3V, GND→GND
  - ✔ *Đạt khi:* I2C Scanner tìm thấy địa chỉ 0x3C
- [ ] **[BẢO] HW-03:** Kết nối Buzzer (GPIO25) và LED báo (GPIO26)
  - ✔ *Đạt khi:* Buzzer kêu, LED sáng khi test thủ công

#### Lập trình RFID
- [ ] **[BẢO] SW-01:** Cài thư viện `MFRC522` trong Arduino IDE
- [ ] **[BẢO] SW-02:** Upload sketch `DumpInfo` kiểm tra RC522
  - ✔ *Đạt khi:* Serial in UID khi đưa thẻ lại gần, VD: `4A 3B 2C 1D`
- [ ] **[BẢO] SW-03:** Viết hàm `String getCardUID()` trả về chuỗi UID

#### Lập trình OLED
- [ ] **[BẢO] SW-04:** Cài thư viện `Adafruit SSD1306` + `Adafruit GFX`
- [ ] **[BẢO] SW-05:** Hàm `displayStatus(name, uid, status)` hiển thị lên OLED
  - Layout: Dòng 1 = Tên | Dòng 2 = MSSV | Dòng 3 = ✓ OK / ✗ FAIL
  - ✔ *Đạt khi:* Hiển thị rõ, không tràn màn hình
- [ ] **[BẢO] SW-06:** Hàm `displayWaiting()` — hiện "Mời quẹt thẻ..."

#### Buzzer & LED
- [ ] **[BẢO] SW-07:** Hàm `beepSuccess()` — 1 tiếng bíp dài (700ms)
- [ ] **[BẢO] SW-08:** Hàm `beepFail()` — 3 tiếng bíp ngắn (200ms/tiếng)

### ✅ HOÀN THÀNH TUẦN 1 KHI:
> Đưa thẻ vào → Serial in UID → OLED hiển thị UID → Buzzer bíp 1 tiếng. Demo cho cả nhóm.

---

## 📅 TUẦN 2 — ESP32 WiFi/HTTP + Raspberry Pi Setup + Flask Server

### 🎯 Mục tiêu: ESP32 gửi UID → Pi → nhận kết quả MATCH/MISMATCH → OLED hiển thị, toàn chu trình ≤ 2 giây

#### Phần A: ESP32 WiFi + HTTP + 2FA
- [ ] **[BẢO] NET-01:** Lập trình kết nối WiFi nội bộ
  - ✔ *Đạt khi:* Serial in IP của ESP32, OLED hiện "WiFi OK"
- [ ] **[BẢO] NET-02:** Hàm `sendUID(String uid)` — HTTP POST `{"uid":"4A3B2C1D"}` lên Pi 4
  - ✔ *Đạt khi:* Nhận về JSON `{"result":"MATCH","name":"..."}`
- [ ] **[BẢO] NET-03:** Parse JSON bằng `ArduinoJson` — trích xuất `result` và `name`
- [ ] **[BẢO] FA-01:** Tích hợp toàn bộ flow 2FA:
  1. Quẹt thẻ → lấy UID
  2. OLED hiện "Đang xác thực..."
  3. HTTP POST → Pi (timeout 3 giây)
  4. Nhận response → OLED hiện tên + MATCH/MISMATCH
  5. Phát beep đúng loại
- [ ] **[BẢO] FA-02:** Xử lý timeout 3 giây → "Lỗi kết nối" + beep fail
- [ ] **[BẢO] FA-03:** Debounce: lockout 2 giây sau mỗi lần quét

#### Phần B: Raspberry Pi 4 Setup + Face Recognition
- [ ] **[BẢO] RPI-01:** Flash Raspberry Pi OS 64-bit vào thẻ microSD (bật SSH trong Imager)
  - ✔ *Đạt khi:* SSH được: `ssh pi@raspberrypi.local`
- [ ] **[BẢO] RPI-02:** Update hệ thống: `sudo apt update && sudo apt upgrade -y`
- [ ] **[BẢO] RPI-03:** Cài Python dependencies:
  ```bash
  pip3 install opencv-python-headless face_recognition flask requests python-dotenv
  ```
  - ✔ *Đạt khi:* `python3 -c "import face_recognition; print('OK')"` không lỗi
  - ⚠️ Mất 15-30 phút do compile dlib
- [ ] **[BẢO] RPI-04:** Kiểm tra Webcam: `python3 -c "import cv2; cap=cv2.VideoCapture(0); print(cap.isOpened())"` → in `True`
- [ ] **[BẢO] AI-01:** Load ảnh đã chuẩn bị từ `known_faces/` (Đạt chuẩn bị ảnh)
         → Viết `encode_faces.py` tạo file `encodings.pkl`
  - ✔ *Đạt khi:* File `encodings.pkl` được tạo không lỗi
- [ ] **[BẢO] AI-02:** Viết `test_recognition.py` — chụp frame webcam → so sánh encodings → in kết quả
  - ✔ *Đạt khi:* In đúng tên người đứng trước camera, độ chính xác > 90%

#### Phần C: Flask Server trên Pi 4
- [ ] **[BẢO] SRV-01:** Viết `server.py` Flask:
  - `POST /verify` → nhận uid → chụp webcam → face recognition → trả JSON
  - `GET /health` → trả `{"status":"ok"}`
  - ✔ *Đạt khi:* `curl http://localhost:5000/health` → `{"status":"ok"}`
- [ ] **[BẢO] SRV-02:** Pre-initialize `VideoCapture` (không mở lại mỗi request để tăng tốc)
  - ✔ *Đạt khi:* Thời gian xử lý 1 request ≤ 1.5 giây
- [ ] **[BẢO] SRV-03:** Log mỗi request ra `attendance.log` (timestamp, uid, result, ms)
- [ ] **[BẢO] SRV-04:** Cấu hình systemd — server tự chạy khi Pi boot
  - ✔ *Đạt khi:* Reboot Pi → `systemctl status attendance` = active
- [ ] **[BẢO] INT-01:** Cùng cả nhóm test: ESP32 POST → Pi → OLED hiện kết quả
  - ✔ *Đạt khi:* Chu trình end-to-end ≤ 2.5 giây

### ✅ HOÀN THÀNH TUẦN 2 KHI:
> Quẹt thẻ → ESP32 gửi → Pi nhận diện khuôn mặt → OLED hiện tên + MATCH/MISMATCH → beep. Đo ≤ 2 giây. Demo cả nhóm.

---

## 📅 TUẦN 3 — Tối ưu firmware + Tích hợp module từ Đăng/Đạt

### 🎯 Mục tiêu: Hệ thống ổn định, tích hợp được Firebase và Telegram vào pipeline Pi

- [ ] **[BẢO] OPT-01:** Chế độ "WiFi OFFLINE" — OLED báo lỗi, không nhận thẻ
- [ ] **[BẢO] OPT-02:** Lưu SSID/IP Pi vào Preferences (không nạp lại firmware khi đổi mạng)
- [ ] **[BẢO] OPT-03:** Log Serial: timestamp, UID, kết quả, thời gian phản hồi
- [ ] **[BẢO] OPT-04:** Tích hợp `firebase_service.py` và `telegram_service.py` (do Đăng + Đạt viết) vào `server.py` — gọi sau mỗi lần verify
  - ✔ *Đạt khi:* Quẹt thẻ → Pi tự gọi Firebase log + Telegram gửi tin, không cần can thiệp thủ công
- [ ] **[BẢO] OPT-05:** Test toàn pipeline hoàn chỉnh cùng Đăng + Đạt

---

## 📅 TUẦN 4 — Kiểm thử + Đóng gói phần cứng

### 🎯 Mục tiêu: Sản phẩm phần cứng hoàn chỉnh, pass hết test

- [ ] **[BẢO] TEST-01:** Stress test 20 lượt liên tiếp, đo + ghi thời gian từng lần
  - ✔ *Mục tiêu:* Trung bình < 2 giây, không lần nào timeout
- [ ] **[BẢO] TEST-02:** Test chống quẹt thẻ hộ: thẻ A + mặt B → bắt buộc ra MISMATCH
- [ ] **[BẢO] TEST-03:** Test ngắt WiFi → ESP32 không crash, hiển thị lỗi đúng
- [ ] **[BẢO] HW-FINAL:** Cố định mạch, dán nhãn dây, đặt vào hộp/khung mica
- [ ] **[BẢO] DOC-01:** Chụp ảnh mạch hoàn chỉnh + vẽ sơ đồ kết nối chân (Fritzing/tay)
- [ ] **[BẢO] DOC-02:** Gửi ảnh mạch + bảng số liệu thời gian phản hồi cho Đăng làm báo cáo

---
---

# 🎨 ĐẠT — UI/UX + BOT + HỖ TRỢ DATA & PYTHON SERVICE

> **Nhiệm vụ:** Web Dashboard realtime + Telegram Bot + chuẩn bị dữ liệu khuôn mặt (hỗ trợ Bảo) + viết Python Telegram service (để Bảo tích hợp vào Pi).

---

## 📚 KNOWLEDGE ĐẠT CẦN NẮM

- [ ] **HTML/CSS/JavaScript:** DOM, Event listener, Fetch API
- [ ] **Firebase Web SDK:** `onValue()` realtime listener, `get()`, khởi tạo app
- [ ] **Firebase Console:** Tạo project, Realtime Database, Storage, Hosting
- [ ] **Telegram Bot API:** Tạo bot qua @BotFather, `sendMessage`, `sendPhoto`
- [ ] **Python `requests` cơ bản:** `requests.post()`, `json` param — để gọi Telegram API
- [ ] **Responsive Design:** Flexbox / CSS Grid

**Tài nguyên:**
- Firebase Web: https://firebase.google.com/docs/web/setup
- Chart.js: https://www.chartjs.org/docs/
- Telegram Bot API: https://core.telegram.org/bots/api#sendphoto
- Python requests: https://requests.readthedocs.io/

---

## 📅 TUẦN 1 — Firebase Setup + Wireframe + Chuẩn bị dữ liệu khuôn mặt

### 🎯 Mục tiêu: Firebase sẵn sàng, wireframe duyệt xong, ảnh khuôn mặt đã chụp đủ cho Bảo dùng

#### Firebase Setup (Đăng cần `firebaseConfig` sớm)
- [ ] **[ĐẠT] FB-01:** Tạo Firebase project → chia sẻ `firebaseConfig` cho Đăng ngay Ngày 3
- [ ] **[ĐẠT] FB-02:** Bật Realtime Database (vùng asia-southeast1)
- [ ] **[ĐẠT] FB-03:** Bật Firebase Storage
- [ ] **[ĐẠT] FB-04:** Set Database Rules tạm thời (read/write = true cho development)
- [ ] **[ĐẠT] DB-01:** Thiết kế schema Firebase (thống nhất với Đăng):
  ```
  /students/{uid}/        → name, mssv, face_registered
  /attendance_logs/{date}/{id}/  → uid, name, timestamp, result, snapshot_url
  ```
  - ✔ *Đạt khi:* Schema được cả nhóm duyệt
- [ ] **[ĐẠT] DB-02:** Tạo mock data thủ công trên Firebase Console để test Dashboard

#### Thiết kế UI Mockup
- [ ] **[ĐẠT] UI-01:** Phác thảo wireframe (Figma hoặc tay):
  - Màn hình chính: Bảng điểm danh hôm nay (tên, giờ, trạng thái, ảnh)
  - Bộ lọc: Theo ngày, Có mặt / Vắng
  - Thống kê: Biểu đồ tỷ lệ theo ngày
  - ✔ *Đạt khi:* Wireframe được nhóm OK trước khi code

#### Chuẩn bị dữ liệu khuôn mặt (hỗ trợ Bảo — không cần code phần cứng)
- [ ] **[ĐẠT] DATA-01:** Chụp ảnh khuôn mặt cho từng thành viên/người dùng thử nghiệm
  - 5-10 ảnh/người, điều kiện ánh sáng đa dạng (thẳng mặt, góc nghiêng, sáng/tối)
  - ✔ *Đạt khi:* Thư mục `known_faces/ten_nguoi/` có đủ ảnh cho tất cả mọi người
- [ ] **[ĐẠT] DATA-02:** Đặt tên file ảnh đúng convention Bảo yêu cầu (VD: `nguyen_van_a_1.jpg`)
  - Gửi thư mục `known_faces/` cho Bảo trước Ngày 5 Tuần 1

### ✅ HOÀN THÀNH TUẦN 1 KHI:
> Firebase project tạo xong, Đăng có `firebaseConfig`, wireframe được nhóm duyệt, Bảo có thư mục `known_faces/` đủ ảnh.

---

## 📅 TUẦN 2 — Phát triển Web Dashboard

### 🎯 Mục tiêu: Dashboard kết nối Firebase, hiển thị dữ liệu realtime, không cần F5

- [ ] **[ĐẠT] WEB-01:** Tạo project HTML/CSS/JS (hoặc React + Vite)
- [ ] **[ĐẠT] WEB-02:** Tích hợp Firebase SDK (console không lỗi)
- [ ] **[ĐẠT] WEB-03:** Lắng nghe realtime `/attendance_logs/{hôm nay}`
  - ✔ *Đạt khi:* Đăng insert record → bảng tự cập nhật, không cần F5
- [ ] **[ĐẠT] WEB-04:** Bảng: STT | Tên | MSSV | Giờ vào | Trạng thái | Ảnh thumbnail
- [ ] **[ĐẠT] WEB-05:** Badge màu: 🟢 MATCH | 🔴 MISMATCH | ⚫ UNKNOWN
- [ ] **[ĐẠT] WEB-06:** Tổng số: "Có mặt: X | Vắng: Y | Tổng: Z"
- [ ] **[ĐẠT] WEB-07:** Date picker xem lịch sử theo ngày
- [ ] **[ĐẠT] WEB-08:** Tìm kiếm theo Tên / MSSV
- [ ] **[ĐẠT] WEB-09:** Dropdown lọc: Tất cả / Có mặt / Vắng
- [ ] **[ĐẠT] WEB-10:** Chart.js — biểu đồ cột tỷ lệ có mặt theo ngày trong tuần
- [ ] **[ĐẠT] WEB-11:** Card số liệu: Tổng buổi | Tổng SV | Tỷ lệ TB
- [ ] **[ĐẠT] WEB-12:** Bảng danh sách SV đọc từ `/students`
- [ ] **[ĐẠT] WEB-13:** Hiển thị trạng thái "Đã / Chưa đăng ký khuôn mặt"

### ✅ HOÀN THÀNH TUẦN 2 KHI:
> Dashboard hiển thị đúng mock data từ Firebase, tự cập nhật khi Đăng thêm record.

---

## 📅 TUẦN 3 — Telegram Bot + Python Service + Dashboard Hoàn Thiện

### 🎯 Mục tiêu: Python Telegram service sẵn sàng cho Bảo tích hợp, Dashboard deploy được

#### Telegram Bot Setup & Template
- [ ] **[ĐẠT] BOT-01:** Tạo bot qua @BotFather, lưu `BOT_TOKEN`
  - ✔ *Đạt khi:* Bot có username VD: @SmartAttendanceBot
- [ ] **[ĐẠT] BOT-02:** Tạo nhóm Telegram test, lấy `CHAT_ID` bằng `getUpdates`
- [ ] **[ĐẠT] BOT-03:** Gửi `BOT_TOKEN` + `CHAT_ID` cho Bảo (để tích hợp vào Pi) và Đăng (để lưu config)

#### Viết Python Telegram Service (hỗ trợ Bảo — file chạy trên Pi nhưng không cần hiểu phần cứng)
- [ ] **[ĐẠT] BOT-04:** Viết `telegram_service.py`:
  - Hàm `send_success(name, mssv, timestamp, image_path)` — gửi tin + ảnh khi MATCH
  - Hàm `send_warning(card_owner, timestamp, image_path)` — gửi cảnh báo khi MISMATCH
  - Dùng `requests.post()` gọi Telegram API trực tiếp (không cần thư viện nặng)
  - ✔ *Đạt khi:* Chạy `python3 telegram_service.py` test → tin nhắn + ảnh xuất hiện nhóm Telegram
  - ✔ *Đạt khi:* Xử lý được timeout/lỗi mạng mà không crash
- [ ] **[ĐẠT] BOT-05:** Template tin nhắn THÀNH CÔNG:
  ```
  ✅ ĐIỂM DANH THÀNH CÔNG
  👤 Tên: Nguyen Van A  |  🪪 MSSV: 21110001
  🕐 07:30:15 - 27/08/2026
  📸 [Ảnh xác thực đính kèm]
  ```
- [ ] **[ĐẠT] BOT-06:** Template CẢNH BÁO quẹt thẻ hộ:
  ```
  ⚠️ CẢNH BÁO: QUẸT THẺ HỘ!
  🪪 Thẻ: 4A3B2C1D (thuộc: Nguyen Van A)
  ❌ Khuôn mặt: KHÔNG KHỚP  |  🕐 07:35:22
  📸 [Ảnh nghi phạm đính kèm]
  ```
- [ ] **[ĐẠT] BOT-07:** Gửi file `telegram_service.py` cho Bảo để tích hợp vào `server.py`

#### Hoàn thiện Dashboard
- [ ] **[ĐẠT] WEB-14:** Responsive — test trên 1080p và điện thoại
- [ ] **[ĐẠT] WEB-15:** Favicon, tiêu đề, loading spinner
- [ ] **[ĐẠT] WEB-16:** Deploy lên Firebase Hosting
  - ✔ *Đạt khi:* Có URL `https://ten-project.web.app` từ mọi nơi

### ✅ HOÀN THÀNH TUẦN 3 KHI:
> Bảo nhận được `telegram_service.py` chạy được. Dashboard live trên hosting.

---

## 📅 TUẦN 4 — Kiểm thử UX + Hỗ trợ báo cáo

- [ ] **[ĐẠT] UX-01:** Kiểm tra toàn luồng: quẹt thẻ → Bot gửi tin → Dashboard cập nhật
- [ ] **[ĐẠT] UX-02:** Test trên Chrome, Edge, Safari mobile
- [ ] **[ĐẠT] UX-03:** Chụp screenshot Dashboard + quay video demo UI → gửi Đăng
- [ ] **[ĐẠT] UX-04:** Viết hướng dẫn sử dụng Dashboard (1 trang A4) cho báo cáo

---
---

# 📊 ĐĂNG — CLOUD BACKEND + KIỂM THỬ + BÁO CÁO

> **Nhiệm vụ:** Toàn bộ phần cloud/backend không liên quan phần cứng — Firebase Python integration, Offline Cache SQLite, điều phối kiểm thử hệ thống và viết báo cáo đồ án đầy đủ.

---

## 📚 KNOWLEDGE ĐĂNG CẦN NẮM

- [ ] **Python 3:** Biến, hàm, class, import, try/except, threading
- [ ] **Firebase Admin SDK (Python):** `db.reference().push()`, `bucket.blob().upload_from_filename()`
- [ ] **SQLite Python:** `sqlite3.connect()`, `cursor.execute()`, cơ bản CRUD
- [ ] **Python requests:** `requests.get()` để kiểm tra kết nối mạng
- [ ] **Cấu trúc JSON Firebase:** Đọc/ghi dữ liệu dạng cây JSON
- [ ] **Kỹ năng viết báo cáo kỹ thuật:** Sơ đồ khối, lưu đồ thuật toán, bảng số liệu

**Tài nguyên:**
- Firebase Admin Python: https://firebase.google.com/docs/database/admin/retrieve-data
- Firebase Storage Python: https://firebase.google.com/docs/storage/admin/start
- SQLite Python: https://docs.python.org/3/library/sqlite3.html

---

## 📅 TUẦN 1 — Thiết kế DB Schema + Nghiên cứu kiến trúc

### 🎯 Mục tiêu: Hiểu rõ toàn bộ kiến trúc, cùng Đạt duyệt schema Firebase

- [ ] **[ĐĂNG] ARCH-01:** Đọc kỹ đề cương, vẽ lại sơ đồ kiến trúc hệ thống bằng tay
  - ✔ *Đạt khi:* Giải thích được cho cả nhóm: dữ liệu chạy qua đâu, lưu ở đâu
- [ ] **[ĐĂNG] ARCH-02:** Cùng Đạt review và duyệt Firebase schema (DB-01)
  - Đảm bảo schema đủ field cho báo cáo cần (timestamp, result, snapshot_url, ...)
- [ ] **[ĐĂNG] ARCH-03:** Lấy `firebaseConfig` từ Đạt, kiểm tra kết nối Firebase Admin SDK từ laptop
  - ✔ *Đạt khi:* `python3 -c "import firebase_admin; print('OK')"` không lỗi

### ✅ HOÀN THÀNH TUẦN 1 KHI:
> Giải thích được toàn bộ kiến trúc cho nhóm, Firebase SDK cài được trên laptop.

---

## 📅 TUẦN 2 — Chuẩn bị module Firebase + Hỗ trợ tích hợp

### 🎯 Mục tiêu: Module Firebase Python sẵn sàng, Bảo có thể import và gọi ngay

- [ ] **[ĐĂNG] FB-01:** Lấy `serviceAccountKey.json` từ Firebase Console → Project Settings → Service Accounts
- [ ] **[ĐĂNG] FB-02:** Viết `firebase_service.py`:
  - Hàm `log_attendance(uid, name, mssv, result, snapshot_url)` → push vào `/attendance_logs/{date}/`
  - Hàm `upload_snapshot(image_path)` → upload lên Firebase Storage, trả về public URL
  - ✔ *Đạt khi:* Test bằng script độc lập — record xuất hiện Firebase Console + Đạt thấy trên Dashboard
- [ ] **[ĐĂNG] FB-03:** Gửi `firebase_service.py` cho Bảo kèm hướng dẫn import

### ✅ HOÀN THÀNH TUẦN 2 KHI:
> `firebase_service.py` test độc lập được — Bảo nhận file và tích hợp vào server.py.

---

## 📅 TUẦN 3 — Offline Cache + Kiểm thử chuẩn bị

### 🎯 Mục tiêu: Hệ thống không mất dữ liệu khi mất mạng, chuẩn bị kịch bản test

- [ ] **[ĐĂNG] CACHE-01:** Viết `offline_queue.py` — SQLite lưu log khi mất mạng:
  - Schema: `id | uid | name | result | timestamp | image_path | synced`
  - Hàm `enqueue(uid, name, result, timestamp, image_path)` — thêm bản ghi
  - Hàm `sync_to_firebase()` — đẩy tất cả `synced=0` lên Firebase
  - ✔ *Đạt khi:* Test độc lập: thêm 3 bản ghi → gọi sync → 3 record xuất hiện Firebase
- [ ] **[ĐĂNG] CACHE-02:** Background thread kiểm tra mạng mỗi 30 giây, tự gọi `sync_to_firebase()`
  - ✔ *Đạt khi:* Ngắt WiFi → enqueue → cắm WiFi → tự đồng bộ lên Firebase
- [ ] **[ĐĂNG] CACHE-03:** Gửi `offline_queue.py` cho Bảo để tích hợp vào `server.py`
- [ ] **[ĐĂNG] PREP-01:** Soạn kịch bản kiểm thử chi tiết cho Tuần 4:
  - Danh sách test case, điều kiện, kết quả kỳ vọng, cách đo

### ✅ HOÀN THÀNH TUẦN 3 KHI:
> `offline_queue.py` test được độc lập. Bảo nhận và tích hợp vào server.py.

---

## 📅 TUẦN 4 — Kiểm thử hệ thống + Viết báo cáo đầy đủ

### 🎯 Mục tiêu: Hệ thống pass toàn bộ test, báo cáo 5 chương hoàn chỉnh, slide sẵn sàng

#### Kiểm thử toàn diện
- [ ] **[ĐĂNG] TEST-01:** Stress test 20 lượt liên tiếp — ghi thời gian từng lần vào bảng
  - ✔ *Mục tiêu:* Trung bình < 2 giây, không lần nào timeout/crash
- [ ] **[ĐĂNG] TEST-02:** Test độ chính xác Face AI:
  - Đúng người đúng thẻ → MATCH ✅
  - Sai người đúng thẻ → MISMATCH ❌
  - ✔ *Mục tiêu:* Độ chính xác ≥ 95% (ít nhất 19/20 lượt đúng)
- [ ] **[ĐĂNG] TEST-03:** Test offline 5 phút → quẹt 5 thẻ → cắm mạng → 5 bản ghi lên Firebase
- [ ] **[ĐĂNG] TEST-04:** Tổng hợp tất cả kết quả vào bảng số liệu → đưa vào báo cáo Chương 4

#### Viết báo cáo đồ án
- [ ] **[ĐĂNG] RPT-01:** **Chương 1 — Tổng quan:** Đặt vấn đề, mục tiêu, phạm vi đề tài
- [ ] **[ĐĂNG] RPT-02:** **Chương 2 — Cơ sở lý thuyết:**
  - RFID RC522 + SPI protocol
  - Face Recognition (dlib ResNet-34, vector 128 chiều, Euclidean distance)
  - HTTP REST API
  - Firebase Realtime Database + Storage
  - Telegram Bot API
- [ ] **[ĐĂNG] RPT-03:** **Chương 3 — Thiết kế hệ thống:**
  - Sơ đồ khối tổng thể
  - Sơ đồ kết nối phần cứng *(lấy ảnh từ Bảo — DOC-01)*
  - Lưu đồ thuật toán 2FA
  - Lưu đồ đồng bộ Firebase
  - Thiết kế cơ sở dữ liệu *(lấy schema từ Đạt — DB-01)*
- [ ] **[ĐĂNG] RPT-04:** **Chương 4 — Thực nghiệm & Kết quả:**
  - Ảnh sản phẩm thực tế *(từ Bảo — DOC-01)*
  - Video/screenshot Dashboard *(từ Đạt — UX-03)*
  - Bảng số liệu kiểm thử *(từ TEST-01~04)*
  - Phân tích và nhận xét kết quả
- [ ] **[ĐĂNG] RPT-05:** **Chương 5 — Kết luận & Hướng phát triển**
- [ ] **[ĐĂNG] RPT-06:** **Tài liệu tham khảo** (IEEE format)
- [ ] **[ĐĂNG] SLIDE-01:** Slide thuyết trình 12-15 slide:
  - Slide 1: Tiêu đề, nhóm
  - Slide 2-3: Đặt vấn đề + Mục tiêu
  - Slide 4-5: Kiến trúc hệ thống + Sơ đồ
  - Slide 6-8: Demo từng phần (ảnh + video clip)
  - Slide 9-10: Kết quả thực nghiệm (bảng số liệu, biểu đồ)
  - Slide 11-12: Kết luận + Hướng phát triển
  - Slide 13: Q&A

---
---

# 📊 BẢNG TRACKING TIẾN ĐỘ NHÓM

> **Ký hiệu:** ⬜ Chưa làm | 🔄 Đang làm | ✅ Xong | — Không phụ trách

## Tuần 1

| Task | Bảo 🔧 | Đạt 🎨 | Đăng 📊 | Deadline |
|:---|:---:|:---:|:---:|:---:|
| Lắp mạch ESP32 (HW-01~03) | ⬜ | — | — | Ngày 3 |
| Firmware RFID + OLED + Buzzer (SW-01~08) | ⬜ | — | — | Ngày 5 |
| Firebase project + DB schema (FB-01~04, DB-01~02) | — | ⬜ | — | Ngày 3 |
| Wireframe Dashboard (UI-01) | — | ⬜ | — | Ngày 5 |
| **⚡ Chụp ảnh khuôn mặt gửi cho Bảo (DATA-01~02)** | — | ⬜ | — | **Ngày 5** |
| Nghiên cứu kiến trúc + Firebase SDK (ARCH-01~03) | — | — | ⬜ | Ngày 7 |

## Tuần 2

| Task | Bảo 🔧 | Đạt 🎨 | Đăng 📊 | Deadline |
|:---|:---:|:---:|:---:|:---:|
| ESP32 WiFi + HTTP + 2FA (NET-01~03, FA-01~03) | ⬜ | — | — | Ngày 3 |
| Pi setup + Webcam + Face AI (RPI-01~04, AI-01~02) | ⬜ | — | — | Ngày 4 |
| Flask Server trên Pi (SRV-01~04) | ⬜ | — | — | Ngày 5 |
| Dashboard realtime (WEB-01~13) | — | ⬜ | — | Ngày 5 |
| **⚡ `firebase_service.py` gửi cho Bảo (FB-01~03)** | — | — | ⬜ | **Ngày 5** |
| Test tích hợp ESP32 ↔ Pi (INT-01) | ⬜ | — | ⬜ | Ngày 7 |

## Tuần 3

| Task | Bảo 🔧 | Đạt 🎨 | Đăng 📊 | Deadline |
|:---|:---:|:---:|:---:|:---:|
| Tối ưu firmware (OPT-01~03) | ⬜ | — | — | Ngày 2 |
| Telegram Bot setup + template (BOT-01~03) | — | ⬜ | — | Ngày 2 |
| **⚡ `telegram_service.py` gửi cho Bảo (BOT-04~07)** | — | ⬜ | — | **Ngày 3** |
| **⚡ `offline_queue.py` gửi cho Bảo (CACHE-01~03)** | — | — | ⬜ | **Ngày 3** |
| Tích hợp firebase + telegram vào server.py (OPT-04) | ⬜ | — | — | Ngày 5 |
| Dashboard hoàn thiện + chart (WEB-14~16) | — | ⬜ | — | Ngày 5 |
| Deploy Firebase Hosting (WEB-16) | — | ⬜ | — | Ngày 7 |
| Test pipeline hoàn chỉnh (OPT-05) | ⬜ | ⬜ | ⬜ | Ngày 7 |

## Tuần 4

| Task | Bảo 🔧 | Đạt 🎨 | Đăng 📊 | Deadline |
|:---|:---:|:---:|:---:|:---:|
| Stress test 20 lượt (TEST-01) | ⬜ | — | ⬜ | Ngày 2 |
| Test chống quẹt thẻ hộ (TEST-02) | ⬜ | — | ⬜ | Ngày 2 |
| Test offline (TEST-03) | ⬜ | — | ⬜ | Ngày 2 |
| Đóng gói phần cứng (HW-FINAL) | ⬜ | — | — | Ngày 3 |
| **⚡ Bảo + Đạt gửi ảnh/video demo cho Đăng** | ⬜ | ⬜ | — | **Ngày 3** |
| Báo cáo 5 chương (RPT-01~06) | — | — | ⬜ | Ngày 5 |
| Slide thuyết trình (SLIDE-01) | — | — | ⬜ | Ngày 6 |
| **🏁 Full demo cuối — CẢ NHÓM** | ⬜ | ⬜ | ⬜ | **Ngày 7** |

---

## ⚠️ PHỐI HỢP — AI CẦN GÌ TỪ AI?

| Người nhận | Thứ cần | Người giao | Hạn chót |
|:---:|:---|:---:|:---:|
| **Bảo** | Thư mục `known_faces/` đủ ảnh khuôn mặt | Đạt | Ngày 5 Tuần 1 |
| **Đăng** | `firebaseConfig` object | Đạt | Ngày 3 Tuần 1 |
| **Bảo** | File `firebase_service.py` | Đăng | Ngày 5 Tuần 2 |
| **Bảo** | File `telegram_service.py` | Đạt | Ngày 3 Tuần 3 |
| **Bảo** | File `offline_queue.py` | Đăng | Ngày 3 Tuần 3 |
| **Đăng** | Ảnh mạch + số liệu thời gian (DOC-01~02) | Bảo | Ngày 3 Tuần 4 |
| **Đăng** | Screenshot + video demo Dashboard (UX-03) | Đạt | Ngày 3 Tuần 4 |

### 5 cuộc họp bắt buộc

| Cuộc họp | Khi nào | Ai tham dự | Nội dung |
|:---|:---:|:---:|:---|
| **Kickoff** | Ngày 1 T1 | Cả nhóm | Phân công, WiFi dùng chung, IP Pi |
| **Schema Review** | Ngày 3 T1 | Đạt + Đăng | Duyệt Firebase schema |
| **Integration Test** | Ngày 7 T2 | Bảo + Đăng | Test ESP32↔Pi, Đạt demo Dashboard |
| **Full System Demo** | Ngày 3 T4 | Cả nhóm | Chạy full demo, quay video |
| **Final Review** | Ngày 6 T4 | Cả nhóm | Review báo cáo + slide, tập thuyết trình |

---

## 🎯 TIÊU CHÍ THÀNH CÔNG CUỐI DỰ ÁN

| Tiêu chí | Mục tiêu | Ai verify |
|:---|:---:|:---:|
| Thời gian toàn chu trình 2FA | ≤ 2 giây | Bảo + Đăng đo |
| Độ chính xác nhận diện khuôn mặt | ≥ 95% | Đăng thống kê |
| Phát hiện quẹt thẻ hộ | 100% | Cả nhóm test |
| Dashboard realtime lag | ≤ 1 giây | Đạt kiểm tra |
| Telegram Bot gửi tin | ≤ 3 giây sau quẹt | Cả nhóm |
| Offline fallback | 5 phút mất mạng → sync được | Đăng test |
| Báo cáo đầy đủ | 5 chương + tài liệu tham khảo | Đăng |

---

## 📈 TÓM TẮT KHỐI LƯỢNG CÔNG VIỆC

| | Bảo 🔧 | Đạt 🎨 | Đăng 📊 |
|:---|:---:|:---:|:---:|
| Tuần 1 | Nặng (HW + firmware) | Vừa (Firebase + mockup + chụp ảnh) | Nhẹ (nghiên cứu + schema) |
| Tuần 2 | **Rất nặng** (ESP32 + Pi + Flask) | Nặng (Dashboard) | Vừa (firebase_service.py) |
| Tuần 3 | Vừa (tối ưu + tích hợp) | Nặng (Bot + Dashboard + telegram_service.py) | Vừa (offline cache) |
| Tuần 4 | Vừa (test + đóng gói) | Nhẹ (UX test + docs) | **Nặng** (báo cáo + slide) |

---

*📌 Update sau mỗi buổi làm: ⬜ → 🔄 khi bắt đầu, 🔄 → ✅ khi xong. Ghi ngày hoàn thành bên cạnh task.*