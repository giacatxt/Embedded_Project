# Bài tập lớn: HỆ THỐNG NHÚNG

## Sinh viên thực hiện
- Trương Đăng Biển 20200063
- Phạm Xuân Trường 20200655

## Thiết bị sử dụng
- ESP32
- MRFC522
- SSD1306
- Servo 9g 
## Mạch demo
![Sơ đồ ghép nối các thiết bị](image.png)

## Thư viện
- Adafruit_SSD1306
- ESP32Servo
- MFRC522
- SPI

## Code

### Khởi tạo Module
- RFID: Sử dụng thư viện MFRC522 để giao tiếp với module RFID.
- OLED: Sử dụng thư viện Adafruit_SSD1306 để điều khiển màn hình OLED.

### Khởi tạo Biến và Đối Tượng
- `Servo MyServo;`: Đối tượng servo motor để điều khiển vị trí và góc quay.
- `Adafruit_SSD1306 display;`: Đối tượng để điều khiển màn hình OLED.

### Khởi Tạo Các Chân Kết Nối
- `#define RST_PIN 4` và `#define SS_PIN 5`: Chân kết nối của module RFID (Reset và Slave Select).
- `#define SCREEN_WIDTH 128` và `#define SCREEN_HEIGHT 64`: Kích thước màn hình OLED.
- `#define OLED_RESET -1`: Chân kết nối Reset của màn hình OLED.

### Hàm `displayData`
Hiển thị thông tin lên màn hình OLED.

### Hàm `setup`
Khởi tạo các thiết bị và hiển thị thông báo trên màn hình OLED.

### Hàm `loop`
- Kiểm tra nếu có thẻ mới được đặt lên RFID.
- Đọc thông tin từ thẻ RFID, bao gồm cả xác thực và đọc dữ liệu từ các khối trên thẻ.
- Hiển thị thông tin lên Serial Monitor.
- Nếu UID của thẻ trùng với ID đã được đặt sẵn (ID1), thì thực hiện xoay servo motor góc 90 độ.
- Hiển thị tên từ thẻ lên màn hình OLED, chỉ cập nhật nếu tên thay đổi.
