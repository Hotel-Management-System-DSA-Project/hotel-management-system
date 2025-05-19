#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
using namespace std;

// Typedef cho struct Room
typedef struct Room {
    int roomNumber;
    char roomType[20];
    float pricePerDay;
    int status; // 0: trống, 1: đã có khách
} Room;

// Typedef cho struct Customer
typedef struct Customer {
    int id;
    char name[50];
    char phone[12];
    int roomNumber;
    char roomType[20];
    int days;
    float totalCost;
} Customer;

// Node cho Linked List quản lý phòng
typedef struct RoomNode {
    Room room;
    RoomNode* next;
} RoomNode;

// Node cho Linked List quản lý khách hàng
typedef struct CustomerNode {
    Customer customer;
    CustomerNode* next;
} CustomerNode;

// Node cho Queue quản lý danh sách chờ
typedef struct WaitNode {
    Customer customer;
    WaitNode* next;
} WaitNode;

// Typedef cho struct Hotel
typedef struct Hotel {
    RoomNode* roomList; // Linked List cho phòng
    int roomCount; // Số phòng
    CustomerNode* customerList; // Linked List cho khách hàng
    WaitNode* waitQueue; // Queue cho danh sách chờ
} Hotel;

// Khởi tạo node phòng
RoomNode* createRoomNode(int roomNumber, const char* roomType, float pricePerDay) {
    RoomNode* node = new RoomNode;
    node->room.roomNumber = roomNumber;
    strcpy(node->room.roomType, roomType);
    node->room.pricePerDay = pricePerDay;
    node->room.status = 0;
    node->next = NULL;
    return node;
}

// Khởi tạo node khách hàng
CustomerNode* createCustomerNode(int id, const char* name, const char* phone, int roomNumber, int days, float totalCost) {
    CustomerNode* node = new CustomerNode;
    node->customer.id = id;
    strcpy(node->customer.name, name);
    strcpy(node->customer.phone, phone);
    node->customer.roomNumber = roomNumber;
    node->customer.days = days;
    node->customer.totalCost = totalCost;
    node->next = NULL;
    return node;
}

// Khởi tạo khách sạn
Hotel* initHotel() {
    Hotel* hotel = new Hotel;
    hotel->roomList = NULL;
    hotel->roomCount = 0;
    hotel->customerList = NULL;
    hotel->waitQueue = NULL;

    // Khởi tạo 10 phòng: 4 Standard, 4 Deluxe, 2 Suite
    struct {
        int roomNumber;
        const char* roomType;
        float pricePerDay;
    } roomData[] = {
        {101, "Standard", 45.0}, // Standard
        {102, "Standard", 50.0},
        {103, "Standard", 48.0},
        {104, "Standard", 52.0},
        {201, "Deluxe", 75.0},  // Deluxe
        {202, "Deluxe", 80.0},
        {203, "Deluxe", 78.0},
        {204, "Deluxe", 82.0},
        {301, "Suite", 120.0},  // Suite
        {302, "Suite", 130.0}
    };

    for (int i = 0; i < 10; i++) {
        RoomNode* node = createRoomNode(roomData[i].roomNumber, 
                                       roomData[i].roomType, 
                                       roomData[i].pricePerDay);
        node->next = hotel->roomList;
        hotel->roomList = node;
        hotel->roomCount++;
    }

    return hotel;
}

// Duyệt danh sách phòng (Linked List)
void displayRooms(Hotel* hotel) {
    cout << "\nDanh sach phong:\n";

    cout << "+------------+-----------------+------------+--------------+\n";
    cout << "| " << left << setw(10) << "So phong"
         << "| " << left << setw(15) << "Loai phong"
         << "| " << left << setw(10) << "Gia / ngay"
         << "| " << left << setw(15) << "Trang thai" << "|\n";
    cout << "+------------+-----------------+------------+--------------+\n";

    RoomNode* current = hotel->roomList;
    while (current != NULL) {
        cout << "| " << left << setw(10) << current->room.roomNumber
             << "| " << left << setw(15) << current->room.roomType
             << "| " << left << setw(10) << current->room.pricePerDay
             << "| " << left << setw(15) << (current->room.status ? "Da co khach" : "Trong") << "|\n";
        current = current->next;
    }

    cout << "+------------+-----------------+------------+--------------+\n";
}

// Tìm kiếm phòng theo số phòng (Linked List)
RoomNode* searchRoomByNumber(Hotel* hotel, int roomNumber) {
    RoomNode* current = hotel->roomList;
    while (current != NULL) {
        if (current->room.roomNumber == roomNumber) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Tìm phòng trống theo loại phòng
RoomNode* findAvailableRoomByType(Hotel* hotel, const char* roomType) {
    RoomNode* current = hotel->roomList;
    while (current != NULL) {
        if (strcmp(current->room.roomType, roomType) == 0 && current->room.status == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Hiển thị thông tin phòng
void displayRoomInfo(RoomNode* node) {
    if (node == NULL) {
        cout << "Khong tim thay phong!\n";
        return;
    }
    cout << "\nThong tin phong:\n";
    cout << "So phong: " << node->room.roomNumber << endl;
    cout << "Loai phong: " << node->room.roomType << endl;
    cout << "Gia/ngay: " << node->room.pricePerDay << endl;
    cout << "Trang thai: " << (node->room.status ? "Da co khach" : "Trong") << endl;
}

// Hàm main
int main() {
    Hotel* hotel = initHotel();
    int choice, roomNumber;

    do {
        cout << "\n=== QUAN LY KHACH SAN ===\n";
        cout << "1. Hien thi danh sach phong\n";
        cout << "2. Tim kiem phong theo so phong\n";
        cout << "3. Sap xep phong theo gia\n";
        cout << "4. Them khach hang\n";
        cout << "5. Dat phong\n";
        cout << "6. Hien thi danh sach khach hang\n";
        cout << "7. Tim kiem khach hang\n";
        cout << "8. Tra phong\n";
        cout << "9. Hoan tac thao tac\n";
        cout << "10. Thoat\n";
        cout << "Nhap lua chon: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                displayRooms(hotel);
                break;
            case 2:
                cout << "Nhap so phong can tim: ";
                cin >> roomNumber;
                displayRoomInfo(searchRoomByNumber(hotel, roomNumber));
                break;
            case 3:
                cout << "hello" << endl;
                break;
            case 4:
                cout << "hellohello" << endl;
                break;
            case 5:
                cout << "hello" << endl;
                break;
            case 6:
                cout << "hello" << endl;
                break;
            case 7:
                cout << "hello" << endl;
                break;
            case 8:
                cout << "hello" << endl;
                break;
            case 9:
                cout << "hello" << endl;
                break;
            case 10:
                cout << "Tam biet!\n";
                break;
            default:
                cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 10);
    return 0;
}