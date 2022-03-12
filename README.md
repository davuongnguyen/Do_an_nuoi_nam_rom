# Đồ án nuôi nấm rơm

### Tổng quan:

​	Mô hình nuôi nấm rơm 2 chế độ tự động và bằng tay.

​	Mã được viết bằng editor "Visual studio code" kết hợp với extensions"PlatformIO IDE".

​	Phần code chính trong thư mục "src".

​	Phần minh họa và mô phỏng Proteus trong thư mục "Schematic".

​	Các thư viện chính của code đã được tích hợp sẵn.

​	Các thư viện dành cho mô phỏng Proteus được đặt trong thư mục "Proteus library".

### Mô tả hoạt động:

- Chế độ bằng tay:
  - Mỗi khi ấn một nút thì đầu ra tương ứng sẽ thay đổi trạng thái theo.
  - Chống dội mặc định của nút bấm là 500ms.
- Chế độ tự động:
  - Khi nhiệt độ thấp, chân 8 arduino sẽ được kích hoạt và đợi đến khi nhiệt độ lên cao đến mức thiết đặt trước sẽ dừng.
  - Khi nhiệt độ quá cao, chân 9 arduino sẽ kích hoạt và đợi đến khi nhiệt độ giảm đến mức nhất định sẽ chuyển về mức thấp.
  - Tương tự với độ ẩm đất, mô hình sẽ khống chế độ ẩm đất trong một mức nhất định bằng đầu ra chân 10 arduino.

### Mô tả phần cứng:

- Về phần cứng của mô hình, khối xử lý chính ở đây sẽ là Arduino Uno R3.

- Các cảm biến được sử dụng là cảm biến nhiệt độ độ ẩm DHT 11, cảm biến độ ẩm đất.

- Các thiết bị ngoại vi gồm 4 nút nhấn, 3 rơ le đầu ra, động cơ quạt, máy bơm mini, đèn.

- Về phần hiển thị, mô hình sử dụng module LCD + I2C (tối thiểu 16x2).

  ![Picture](D:\User\Source code\Embedded\Project\Do_an_nuoi_nam_rom\Schematic\Picture.png)

### Các vấn đề lỗi có thể gặp:

- Màn hình LCD không hiển thị: có thể do địa chỉ giao tiếp I2C sai, sửa trong code.
- Mạch phản hồi chậm: Do thiết lập cảm biến 1s mới đọc 1 lần nên phải ít nhất 1s mới có sự thay đổi lên màn hình.
- Mạch chạy sai: Do chỉ mô phỏng trên proteus, nên không thể đúng hoàn toàn với các mô hình thực tế. Để khắc phục, cần căn chỉnh lại dải hoạt động của từng thiết bị sao cho phù hợp.



### Các phần mềm cần thiết:

- Visual studio code: https://code.visualstudio.com/download
- PlatformIO IDE: https://platformio.org/platformio-ide
- Git: https://git-scm.com/downloads
- Proteus (Tối thiểu 8.10 SP3): https://www.labcenter.com/
