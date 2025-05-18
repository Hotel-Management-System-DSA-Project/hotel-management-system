#include <iostream>
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
    int days;
    float totalCost;
} Customer;

// Node cho Linked List quản lý phòng
typedef struct RoomNode {
    Room room;
    RoomNode* next;
} RoomNode;

// Node cho Stack lưu lịch sử thao tác
typedef struct Action {
    char type[20]; // "add_customer", "delete_customer"
    Customer customer;
} Action;

typedef struct ActionNode {
    Action action;
    ActionNode* next;
} ActionNode;

// Node cho Queue quản lý danh sách chờ
typedef struct WaitNode {
    Customer customer;
    WaitNode* next;
} WaitNode;

// Typedef cho struct Hotel
typedef struct Hotel {
    RoomNode* roomList; // Linked List cho phòng
    Customer* customers;
    int customerCount;
    int customerCapacity;
    ActionNode* actionStack; // Stack cho lịch sử thao tác
    WaitNode* waitQueue; // Queue cho danh sách chờ
    WaitNode* waitQueueTail;
} Hotel;

// Khởi tạo node phòng
RoomNode* createRoomNode(int roomNumber, const char* roomType, float pricePerDay) {
    RoomNode* node = new RoomNode;
    node->room.roomNumber = roomNumber;
    strcpy(node->room.roomType, roomType);
    node->room.pricePerDay = pricePerDay;
    node->room.status = 0;
    node->next = nullptr;
    return node;
}

// Khởi tạo khách sạn
Hotel* initHotel(int customerCapacity) {
    Hotel* hotel = new Hotel;
    hotel->roomList = nullptr;
    hotel->customers = new Customer[customerCapacity];
    hotel->customerCount = 0;
    hotel->customerCapacity = customerCapacity;
    hotel->actionStack = nullptr;
    hotel->waitQueue = nullptr;
    hotel->waitQueueTail = nullptr;

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
    }

    return hotel;
}

// Duyệt danh sách phòng (Linked List)
void displayRooms(Hotel* hotel) {
    cout << "\nDanh sach phong:\n";
    cout << "So phong\tLoai phong\tGia/ngay\tTrang thai\n";
    cout << "------------------------------------------------\n";
    RoomNode* current = hotel->roomList;
    while (current != nullptr) {
        cout << current->room.roomNumber << "\t\t" 
             << current->room.roomType << "\t\t" 
             << current->room.pricePerDay << "\t\t" 
             << (current->room.status ? "Da co khach" : "Trong") << endl;
        current = current->next;
    }
}

// Tìm kiếm phòng theo số phòng (Linked List)
RoomNode* searchRoomByNumber(Hotel* hotel, int roomNumber) {
    RoomNode* current = hotel->roomList;
    while (current != nullptr) {
        if (current->room.roomNumber == roomNumber) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// Hiển thị thông tin phòng
void displayRoomInfo(RoomNode* node) {
    if (node == nullptr) {
        cout << "Khong tim thay phong!\n";
        return;
    }
    cout << "\nThong tin phong:\n";
    cout << "So phong: " << node->room.roomNumber << endl;
    cout << "Loai phong: " << node->room.roomType << endl;
    cout << "Gia/ngay: " << node->room.pricePerDay << endl;
    cout << "Trang thai: " << (node->room.status ? "Da co khach" : "Trong") << endl;
}

// Sắp xếp phòng theo giá (Bubble Sort trên Linked List)
void sortRoomsByPrice(Hotel* hotel) {
    if (hotel->roomList == nullptr || hotel->roomList->next == nullptr) {
        return;
    }
    bool swapped;
    do {
        swapped = false;
        RoomNode* current = hotel->roomList;
        RoomNode* prev = nullptr;
        while (current != nullptr && current->next != nullptr) {
            if (current->room.pricePerDay > current->next->room.pricePerDay) {
                RoomNode* nextNode = current->next;
                current->next = nextNode->next;
                nextNode->next = current;
                if (prev == nullptr) {
                    hotel->roomList = nextNode;
                } else {
                    prev->next = nextNode;
                }
                prev = nextNode;
                swapped = true;
            } else {
                prev = current;
                current = current->next;
            }
        }
    } while (swapped);
    cout << "Da sap xep danh sach phong theo gia tang dan!\n";
}

// Stack: Push thao tác vào lịch sử
void pushAction(Hotel* hotel, const char* type, Customer customer) {
    ActionNode* node = new ActionNode;
    strcpy(node->action.type, type);
    node->action.customer = customer;
    node->next = hotel->actionStack;
    hotel->actionStack = node;
}

// Queue: Thêm khách hàng vào danh sách chờ
void enqueueCustomer(Hotel* hotel, Customer customer) {
    WaitNode* node = new WaitNode;
    node->customer = customer;
    node->next = nullptr;
    if (hotel->waitQueue == nullptr) {
        hotel->waitQueue = node;
        hotel->waitQueueTail = node;
    } else {
        hotel->waitQueueTail->next = node;
        hotel->waitQueueTail = node;
    }
    cout << "Khach hang da duoc them vao danh sach cho!\n";
}

// Queue: Lấy khách hàng từ danh sách chờ
Customer* dequeueCustomer(Hotel* hotel) {
    if (hotel->waitQueue == nullptr) {
        return nullptr;
    }
    WaitNode* node = hotel->waitQueue;
    Customer* customer = new Customer;
    *customer = node->customer;
    hotel->waitQueue = node->next;
    if (hotel->waitQueue == nullptr) {
        hotel->waitQueueTail = nullptr;
    }
    delete node;
    return customer;
}

// Thêm khách hàng
void addCustomer(Hotel* hotel, int id, const char* name, const char* phone, int roomNumber, int days) {
    if (hotel->customerCount >= hotel->customerCapacity) {
        Customer customer;
        customer.id = id;
        strcpy(customer.name, name);
        strcpy(customer.phone, phone);
        customer.roomNumber = roomNumber;
        customer.days = days;
        customer.totalCost = 0;
        enqueueCustomer(hotel, customer);
        return;
    }

    RoomNode* room = searchRoomByNumber(hotel, roomNumber);
    if (room == nullptr) {
        cout << "Phong " << roomNumber << " khong ton tai!\n";
        return;
    }
    if (room->room.status) {
        cout << "Phong " << roomNumber << " da co khach!\n";
        return;
    }

    Customer* customer = &hotel->customers[hotel->customerCount];
    customer->id = id;
    strcpy(customer->name, name);
    strcpy(customer->phone, phone);
    customer->roomNumber = roomNumber;
    customer->days = days;
    customer->totalCost = room->room.pricePerDay * days;

    room->room.status = 1;
    hotel->customerCount++;
    pushAction(hotel, "add_customer", *customer);
    cout << "Them khach hang thanh cong!\n";
}

// Hiển thị danh sách khách hàng
void displayCustomers(Hotel* hotel) {
    if (hotel->customerCount == 0) {
        cout << "Khong co khach hang nao!\n";
        return;
    }
    cout << "\nDanh sach khach hang:\n";
    cout << "ID\tTen\t\tSDT\t\tPhong\tSo ngay\tTong tien\n";
    cout << "------------------------------------------------------------\n";
    for (int i = 0; i < hotel->customerCount; i++) {
        Customer* customer = &hotel->customers[i];
        cout << customer->id << "\t" << customer->name << "\t\t" 
             << customer->phone << "\t" << customer->roomNumber << "\t" 
             << customer->days << "\t" << customer->totalCost << endl;
    }
}

// Tìm kiếm khách hàng theo ID
void searchCustomer(Hotel* hotel, int id) {
    for (int i = 0; i < hotel->customerCount; i++) {
        Customer* customer = &hotel->customers[i];
        if (customer->id == id) {
            cout << "\nThong tin khach hang:\n";
            cout << "ID: " << customer->id << endl;
            cout << "Ten: " << customer->name << endl;
            cout << "SDT: " << customer->phone << endl;
            cout << "So phong: " << customer->roomNumber << endl;
            cout << "So ngay: " << customer->days << endl;
            cout << "Tong tien: " << customer->totalCost << endl;
            return;
        }
    }
    cout << "Khong tim thay khach hang voi ID " << id << "!\n";
}

// Xóa khách hàng
void deleteCustomer(Hotel* hotel, int id) {
    for (int i = 0; i < hotel->customerCount; i++) {
        if (hotel->customers[i].id == id) {
            Customer customer = hotel->customers[i];
            RoomNode* room = searchRoomByNumber(hotel, customer.roomNumber);
            if (room) {
                room->room.status = 0;
            }
            for (int j = i; j < hotel->customerCount - 1; j++) {
                hotel->customers[j] = hotel->customers[j + 1];
            }
            hotel->customerCount--;
            pushAction(hotel, "delete_customer", customer);
            cout << "Xoa khach hang thanh cong!\n";
            // Kiểm tra danh sách chờ
            Customer* waitingCustomer = dequeueCustomer(hotel);
            if (waitingCustomer) {
                addCustomer(hotel, waitingCustomer->id, waitingCustomer->name, 
                            waitingCustomer->phone, waitingCustomer->roomNumber, 
                            waitingCustomer->days);
                delete waitingCustomer;
            }
            return;
        }
    }
    cout << "Khong tim thay khach hang voi ID " << id << "!\n";
}

// Undo thao tác (Stack)
void undoAction(Hotel* hotel) {
    if (hotel->actionStack == nullptr) {
        cout << "Khong co thao tac de hoan tac!\n";
        return;
    }
    ActionNode* actionNode = hotel->actionStack;
    hotel->actionStack = actionNode->next;
    if (strcmp(actionNode->action.type, "add_customer") == 0) {
        deleteCustomer(hotel, actionNode->action.customer.id);
    } else if (strcmp(actionNode->action.type, "delete_customer") == 0) {
        addCustomer(hotel, actionNode->action.customer.id, 
                    actionNode->action.customer.name, 
                    actionNode->action.customer.phone, 
                    actionNode->action.customer.roomNumber, 
                    actionNode->action.customer.days);
    }
    delete actionNode;
    cout << "Hoan tac thanh cong!\n";
}

// Giải phóng bộ nhớ
void freeHotel(Hotel* hotel) {
    RoomNode* currentRoom = hotel->roomList;
    while (currentRoom != nullptr) {
        RoomNode* temp = currentRoom;
        currentRoom = currentRoom->next;
        delete temp;
    }
    ActionNode* currentAction = hotel->actionStack;
    while (currentAction != nullptr) {
        ActionNode* temp = currentAction;
        currentAction = currentAction->next;
        delete temp;
    }
    WaitNode* currentWait = hotel->waitQueue;
    while (currentWait != nullptr) {
        WaitNode* temp = currentWait;
        currentWait = currentWait->next;
        delete temp;
    }
    delete[] hotel->customers;
    delete hotel;
}

// Hàm main
int main() {
    Hotel* hotel = initHotel(10); // Dung lượng tối đa 10 khách hàng
    int choice, id, roomNumber, days;
    string name, phone;

    do {
        cout << "\n=== QUAN LY KHACH SAN ===\n";
        cout << "1. Hien thi danh sach phong\n";
        cout << "2. Tim kiem phong theo so phong\n";
        cout << "3. Sap xep phong theo gia\n";
        cout << "4. Them khach hang\n";
        cout << "5. Hien thi danh sach khach hang\n";
        cout << "6. Tim kiem khach hang\n";
        cout << "7. Xoa khach hang\n";
        cout << "8. Hoan tac thao tac\n";
        cout << "9. Thoat\n";
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
                sortRoomsByPrice(hotel);
                displayRooms(hotel);
                break;
            case 4:
                cout << "Nhap ID: ";
                cin >> id;
                cin.ignore();
                cout << "Nhap ten: ";
                getline(cin, name);
                cout << "Nhap SDT: ";
                getline(cin, phone);
                cout << "Nhap so phong: ";
                cin >> roomNumber;
                cout << "Nhap so ngay luu tru: ";
                cin >> days;
                addCustomer(hotel, id, name.c_str(), phone.c_str(), roomNumber, days);
                break;
            case 5:
                displayCustomers(hotel);
                break;
            case 6:
                cout << "Nhap ID can tim: ";
                cin >> id;
                searchCustomer(hotel, id);
                break;
            case 7:
                cout << "Nhap ID can xoa: ";
                cin >> id;
                deleteCustomer(hotel, id);
                break;
            case 8:
                undoAction(hotel);
                break;
            case 9:
                cout << "Tam biet!\n";
                break;
            default:
                cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 9);

    freeHotel(hotel);
    return 0;
}