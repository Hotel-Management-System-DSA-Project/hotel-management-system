# Hotel Management System - DSA Project

## 📋 Mô tả
Dự án xây dựng hệ thống quản lý khách sạn bằng ngôn ngữ C++.  
Đây là bài tập lớn của môn **Cấu trúc Dữ liệu và Giải thuật** tại Đại học Bách Khoa Hà Nội.

## 👥 Thành viên nhóm
- Phạm Triệu Minh
- Lê Tiến Hiếu
- Nguyễn Việt Trường
- Vũ Hương Thảo

## 🎯 Tính năng dự kiến
- Thêm/sửa/xóa phòng khách sạn.
- Thêm/sửa/xóa thông tin khách hàng.
- Đặt phòng và quản lý đơn đặt.
- Tìm kiếm thông tin phòng/trạng thái đặt phòng.
- Xử lý trả phòng và thanh toán
- Quản lý hóa đơn.
- Đánh giá dịch vụ & quản lý lịch sử

## 🛠 Công nghệ sử dụng
- Ngôn ngữ: C++
- IDE: Visual Studio Code / Code::Blocks / Dev C++

## 🗂 Phân chia công việc
- Phạm Triệu Minh:    +Tạo struct phòng và khách
                      +Quản lý danh sách phòng và khách (dùng linkedlist)
                      +Viết các hàm: -Hiển thị danh sách phòng
                                     -Tìm phòng theo số phòng
                                     -Hiển thị thông tin phòng
                                     -Sắp xếp phòng theo giá
                                     

- Lê Tiến Hiếu:       +Hàm thêm khách hàng vào hàng đợi
                      +Hàm lấy khách hàng ra khỏi hàng đợi
                      +Hàm đặt phòng
                      +Hàm hiển thị danh sách khách hàng ( Duyệt )
                      +Hàm tìm kiếm khách hàng theo ID rồi in ra thông tin ứng với ID đấy

- Nguyễn Việt Trường: +Xây dựng hàm trả phòng (Đầu vào là ID khách hàng)
                                             - Duyệt danh sách khách hàng
                                             - In hóa đơn
                                             - Cập nhật lại trạng thái phòng
                                             - Xóa khách hàng 
                                             - Kiểm tra danh sách chờ
                      

- Vũ Hương Thảo:      +Đánh giá khách hàng
                      +Lưu và hiển thị lịch sử của các thao tác (Thêm vào stack)
                      +Undo thao tác
                      +Giải phóng bộ nhớ
                      +Viết báo cáo
