# ĐỀ CƯƠNG ĐỒ ÁN: HỆ THỐNG ĐIỂM DANH THÔNG MINH BẰNG RFID SỬ DỤNG VÀ MÔ PHỎNG VỚI STM32

## 1. Tên đề tài & Mục tiêu
**Tên đề tài:** Nghiên cứu, mô phỏng và chế tạo hệ thống điểm danh tự động bằng công nghệ RFID sử dụng vi điều khiển STM32.

**Mục tiêu:** Xây dựng một hệ thống nhúng cục bộ quản lý chuyên cần. Dự án tuân thủ nghiêm ngặt quy trình phát triển hệ thống nhúng: thiết kế mạch mô phỏng trên Proteus, cấu hình phần cứng bằng STM32CubeMX và lập trình vi điều khiển bằng ngôn ngữ C trên môi trường Keil C (MDK-ARM) sử dụng thư viện chuẩn HAL.

---

## 2. Kiến trúc hệ thống
Hệ thống loại bỏ hoàn toàn các yếu tố kết nối mạng (Internet/IoT) để tập trung vào xử lý nhúng cục bộ và giao tiếp ngoại vi cấp thấp.

*   **Khối xử lý trung tâm (MCU):** Vi điều khiển họ STM32 (thường dùng STM32F103C8T6 vì được hỗ trợ tốt và phổ biến) làm nhiệm vụ điều phối toàn bộ hệ thống.
*   **Khối thu thập dữ liệu (Input):** Module thẻ từ RFID RC522 giao tiếp với MCU thông qua chuẩn truyền thông SPI (Serial Peripheral Interface).
*   **Khối hiển thị (Output):** Màn hình LCD 16x2 (giao tiếp song song hoặc qua module I2C) để hiển thị thông tin như "Mời quẹt thẻ", "Hợp lệ", "Sai thẻ".
*   **Khối báo hiệu (Output):** Còi báo (Buzzer) và đèn LED để phát tín hiệu trực quan/âm thanh khi đọc thẻ.
*   **Khối giao tiếp máy tính (Tùy chọn nâng cao):** Sử dụng chuẩn giao tiếp UART (kết hợp mạch chuyển đổi UART-to-USB) để đẩy log mã thẻ quét được lên màn hình Terminal của máy tính (thay thế cho Cloud/Web).

---

## 3. Thiết bị, Phần cứng và Phần mềm
Thay vì sử dụng ESP32 và Raspberry Pi, dự án sẽ sử dụng bộ công cụ tiêu chuẩn cho ngành Kỹ thuật Điện tử/Hệ thống nhúng.

### Phần mềm (Software Toolchain)
1.  **Proteus Design Suite:** Vẽ sơ đồ nguyên lý mạch điện và chạy mô phỏng ảo trước khi ráp mạch thật.
2.  **STM32CubeMX:** Công cụ đồ họa để khởi tạo dự án, thiết lập hệ thống xung nhịp (Clock Tree) và cấu hình các chân chức năng (GPIO, SPI, UART). Sinh code tự động cho Keil C.
3.  **Keil C (MDK-ARM):** Môi trường phát triển tích hợp (IDE) và trình biên dịch để viết mã nguồn C, tích hợp thư viện HAL do STM32CubeMX sinh ra, biên dịch ra file hex nạp vào Proteus/Chip thật.

### Phần cứng (Hardware)
| STT | Thiết bị / Linh kiện | Giao tiếp | Vai trò |
|:---:|:---|:---:|:---|
| 1 | **Kit STM32F103C8T6 (BluePill)** | Core | Vi điều khiển trung tâm thực thi chương trình. |
| 2 | **Mạch nạp ST-Link V2** | SWD | Dùng để nạp code từ máy tính (Keil C) thẳng xuống kit STM32 thật. |
| 3 | **Module RFID RC522 (13.56MHz)** | SPI | Đọc/Ghi dữ liệu từ thẻ từ sinh viên. |
| 4 | **Màn hình LCD 16x2** | 4-bit/I2C | Hiển thị chữ cái, thông báo cho người dùng. |
| 5 | **Module I2C cho LCD (Tùy chọn)** | I2C | Giúp LCD 16x2 chỉ cần dùng 4 dây thay vì 16 dây, tiết kiệm chân STM32. |
| 6 | **Module UART to USB (CH340/PL2303)**| UART | Chuyển đổi tín hiệu UART từ STM32 sang USB để kết nối với máy tính. |
| 7 | **Buzzer 5V & Đèn LED** | GPIO | Phát âm thanh bíp và nháy đèn khi có người quẹt thẻ. |
| 8 | **Breadboard & Dây nối (Jumper)** | N/A | Đế cắm mạch và dây truyền tín hiệu. |

---

## 4. Lộ trình triển khai (4 tuần)

### Tuần 1: Thiết kế mạch, Mô phỏng Proteus và Cấu hình ngoại vi
*   Khởi tạo dự án Proteus: Lấy linh kiện STM32F103C8, LCD, Button, LED... và nối dây sơ đồ nguyên lý sơ bộ.
*   Sử dụng phần mềm STM32CubeMX:
    *   Cấu hình Clock (xung nhịp).
    *   Cấu hình ngoại vi SPI1 (cho RC522), I2C1 (cho LCD), UART1 (để giao tiếp máy tính).
    *   Cấu hình các chân GPIO Output cho Buzzer/LED.
*   Generate (sinh) code sang định dạng cho Keil C.

### Tuần 2: Lập trình điều khiển ngoại vi với thư viện HAL
*   Tìm kiếm và tích hợp thư viện MFRC522 (bản port cho bộ thư viện HAL STM32) vào project Keil C.
*   Lập trình khởi tạo LCD 16x2, viết hàm in chuỗi ra màn hình.
*   Viết logic cơ bản: Vòng lặp liên tục phát hiện thẻ $\rightarrow$ Đọc mã UID $\rightarrow$ In mã UID đó ra màn hình LCD và kêu bíp.
*   Biên dịch file `.hex` từ Keil C và trỏ vào con vi điều khiển trong Proteus để chạy mô phỏng ảo.

### Tuần 3: Hoàn thiện logic điểm danh và Giao tiếp PC
*   Lập trình giao tiếp UART: Chuyển đoạn mã thẻ đọc được thành dạng chuỗi kí tự (String) và gửi qua UART.
*   Sử dụng phần mềm trên PC (như Hercules, PuTTY, hoặc tự viết tool C#) để hứng chuỗi kí tự này hiển thị (đóng vai trò lưu log điểm danh trên máy).
*   Lập trình thuật toán xác thực: Hard-code (lưu cứng) vài đoạn mã thẻ UID của thành viên nhóm vào mảng trong code. So sánh thẻ vừa quẹt với mảng này $\rightarrow$ Đúng thì LCD báo "Hop le", sai báo "Sai the".

### Tuần 4: Ráp mạch thật & Hoàn thiện báo cáo
*   Chuyển từ môi trường mô phỏng sang phần cứng thật.
*   Cắm linh kiện lên breadboard theo đúng sơ đồ Proteus.
*   Sử dụng ST-Link cắm vào cổng nạp SWD trên BluePill để nạp code.
*   Kiểm tra thực tế với thẻ RFID vật lý.
*   Vẽ lưu đồ thuật toán, hoàn chỉnh báo cáo Word/Slide và chuẩn bị bảo vệ đồ án.
