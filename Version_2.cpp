#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <limits>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <algorithm>
using namespace std;

// Struct Room: Quan ly thong tin phong
typedef struct Room {
    int roomNumber;
    char roomType[20];
    float pricePerDay;
    int status; // 0: trong, 1: da co khach
    int countUsing;
    int days; // So ngay thue
} Room;

// Node cho danh sach lien ket quan ly phong
typedef struct RoomNode {
    Room room;
    RoomNode* next;
} RoomNode;

// Struct Feedback: Quan ly danh gia cua khach hang
typedef struct Feedback {
    int rating;
    char comment[100];
} Feedback;

// Struct Customer: Quan ly thong tin khach hang
typedef struct Customer {
    int id;
    char name[50];
    char phone[12];
    float totalCost;
    int days;
    Room room; // Phong ma khach da dat
    Feedback feedback;
} Customer;

// Node cho danh sach lien ket quan ly khach hang
typedef struct CustomerNode {
    Customer customer;
    CustomerNode* next;
} CustomerNode;

// Node cho hang doi danh sach cho
typedef struct WaitNode {
    Customer customer;
    WaitNode* next;
} WaitNode;

// Node cho danh sach lich su khach hang
typedef struct HistoryNode {
    Customer customer;
    HistoryNode* next;
} HistoryNode;

// Struct Hotel: Quan ly khach san
typedef struct Hotel {
    RoomNode* roomList; // Danh sach lien ket phong
    int roomCount; // So luong phong
    CustomerNode* customerList; // Danh sach lien ket khach hang
    WaitNode* waitQueue; // Hang doi danh sach cho
    HistoryNode* historyList; // Danh sach lien ket lich su
} Hotel;

// Tao node phong
RoomNode* createRoomNode(int roomNumber, const char* roomType, float pricePerDay) {
    RoomNode* node = new RoomNode;
    node->room.roomNumber = roomNumber;
    strcpy(node->room.roomType, roomType);
    node->room.pricePerDay = pricePerDay;
    node->room.status = 0;
    node->room.countUsing = 0;
    node->room.days = 0;
    node->next = NULL;
    return node;
}

// Tao node khach hang
CustomerNode* createCustomerNode(int id, const char* name, const char* phone, int days, float totalCost, Room room) {
    CustomerNode* node = new CustomerNode;
    node->customer.id = id;
    strcpy(node->customer.name, name);
    strcpy(node->customer.phone, phone);
    node->customer.days = days;
    node->customer.totalCost = totalCost;
    node->customer.room = room;
    node->customer.feedback.rating = 0;
    node->customer.feedback.comment[0] = '\0';
    node->next = NULL;
    return node;
}

// Tao node lich su khach hang
HistoryNode* createHistoryNode(Customer& customer) {
    HistoryNode* node = new HistoryNode;
    node->customer = customer;
    node->next = NULL;
    return node;
}

// Khoi tao khach san
Hotel* initHotel() {
    Hotel* hotel = new Hotel;
    hotel->roomList = NULL;
    hotel->roomCount = 0;
    hotel->customerList = NULL;
    hotel->waitQueue = NULL;
    hotel->historyList = NULL;

    // Khoi tao 10 phong: 4 Standard, 4 Deluxe, 2 Suite
    struct {
        int roomNumber;
        const char* roomType;
        float pricePerDay;
    } roomData[] = {
        {101, "Standard", 45.0},
        {102, "Standard", 50.0},
        {103, "Standard", 48.0},
        {104, "Standard", 52.0},
        {201, "Deluxe", 75.0},
        {202, "Deluxe", 80.0},
        {203, "Deluxe", 78.0},
        {204, "Deluxe", 82.0},
        {301, "Suite", 120.0},
        {302, "Suite", 130.0}
    };

    for (int i = 0; i < 10; i++) {
        RoomNode* node = createRoomNode(roomData[i].roomNumber, roomData[i].roomType, roomData[i].pricePerDay);
        node->next = hotel->roomList;
        hotel->roomList = node;
        hotel->roomCount++;
    }

    return hotel;
}

// Hien thi danh sach phong
void displayRooms(Hotel* hotel) {
    cout << "\n+------------------------------------------------------------+\n";
    cout << "|                   DANH SACH PHONG                          |\n";
    cout << "+------------+-----------------+------------+---------------+\n";
    cout << "| " << left << setw(10) << "So phong"
         << "| " << left << setw(15) << "Loai phong"
         << "| " << left << setw(10) << "Gia/ngay"
         << "| " << left << setw(13) << "Trang thai" << "|\n";
    cout << "+------------+-----------------+------------+---------------+\n";

    RoomNode* current = hotel->roomList;
    while (current != NULL) {
        cout << "| " << left << setw(10) << current->room.roomNumber
             << "| " << left << setw(15) << current->room.roomType
             << "| " << left << setw(10) << fixed << setprecision(2) << current->room.pricePerDay
             << "| " << left << setw(13) << (current->room.status ? "Da co khach" : "Trong") << "|\n";
        current = current->next;
    }
    cout << "+------------+-----------------+------------+---------------+\n";
}

// Them phong moi
void addNewRoom(Hotel* hotel) {
    int roomNumber;
    char roomType[20];
    float pricePerDay;
    cout << "\n+------------------------------------------------------------+\n";
    cout << "|                   THEM PHONG MOI                           |\n";
    cout << "+------------------------------------------------------------+\n";
    displayRooms(hotel);
    cout << "Nhap so phong muon them: "; cin >> roomNumber;
    cin.ignore();
    cout << "Nhap kieu phong: "; cin.getline(roomType, 20);
    cout << "Nhap gia thue/ngay: "; cin >> pricePerDay;
    RoomNode* newRoom = createRoomNode(roomNumber, roomType, pricePerDay);
    newRoom->next = hotel->roomList;
    hotel->roomList = newRoom;
    hotel->roomCount++;
    cout << "Da them thanh cong phong " << roomNumber << " vao khach san!\n";
}

// Tim kiem phong theo so phong
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

// Tim phong trong theo loai phong
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

// Hien thi thong tin phong
void displayRoomInfo(Room* room) {
    if (room == NULL) {
        cout << "Khong tim thay phong!\n";
        return;
    }
    cout << "\n+------------------------------------------------------------+\n";
    cout << "|                   THONG TIN PHONG                          |\n";
    cout << "+------------------------------------------------------------+\n";
    cout << "| So phong: " << left << setw(46) << room->roomNumber << "|\n";
    cout << "| Loai phong: " << left << setw(44) << room->roomType << "|\n";
    cout << "| Gia/ngay: " << left << setw(46) << fixed << setprecision(2) << room->pricePerDay << "|\n";
    cout << "| Trang thai: " << left << setw(44) << (room->status ? "Da co khach" : "Trong") << "|\n";
    cout << "+------------------------------------------------------------+\n";
}

// Them khach hang vao hang doi
void addWaitNode(Hotel* hotel, Customer customer) {
    WaitNode* newNode = new WaitNode;
    newNode->customer = customer;
    newNode->next = NULL;

    if (hotel->waitQueue == NULL) {
        hotel->waitQueue = newNode;
    } else {
        WaitNode* current = hotel->waitQueue;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    cout << "Khach hang " << customer.name << " da duoc them vao hang cho!\n";
}

// Hoan tac dat phong
void undoReservation(Hotel* hotel) {
    if (hotel->customerList == NULL) {
        cout << "Khong co khach hang nao trong danh sach!\n";
        return;
    }

    CustomerNode* firstCustomer = hotel->customerList;

    hotel->customerList = hotel->customerList->next;

    RoomNode* roomToFree = searchRoomByNumber(hotel, firstCustomer->customer.room.roomNumber);
    if (roomToFree != NULL) {
        roomToFree->room.status = 0;
        roomToFree->room.days = 0;
    }

    delete firstCustomer;
    cout << "Hoan tac dat phong thanh cong!\n";
}

// hàm undo trả phòng
void undoTraPhong (Hotel* hotel){
    if (hotel->historyList == NULL){
        cout <<"Khong co khach hang nao da tra phong";
        return;
    }
    HistoryNode* current = hotel -> historyList;
    hotel-> historyList = hotel -> historyList -> next;

    RoomNode* roomGetBack = searchRoomByNumber (hotel, current->customer.room.roomNumber);
    if (roomGetBack != NULL){
        roomGetBack -> room.status = 1;
        roomGetBack -> room.countUsing--;
        // Thêm vào danh sách khách hàng
        CustomerNode* newCustomer = createCustomerNode(
            current->customer.id, current->customer.name, current->customer.phone,
            current->customer.days, current->customer.totalCost, roomGetBack->room);
        newCustomer->next = hotel->customerList;
        hotel->customerList = newCustomer;
        // cap nhat tong tien cu cua khach hang
        // Xóa lịch sử
        delete current;
        cout << "\nBan da hoan tac thanh cong !";
    } else {
        cout << "error!";
    }
}

// Nhap chuoi hop le
string getValidInputString(const string& prompt, const function<bool(string)>& validator, const string& errorMessage) {
    string input;
    bool valid = false;
    while (!valid) {
        cout << prompt;
        try {
            getline(cin >> ws, input);
            if (input.empty()) {
                throw invalid_argument("Du lieu khong duoc de trong");
            }
            if (!validator(input)) {
                throw invalid_argument(errorMessage);
            }
            valid = true;
        } catch (const exception& e) {
            cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return input;
}

// Nhap du lieu khong phai chuoi
template <typename T>
T getValidInput(const string& prompt, const function<bool(T)>& validator, const string& errorMessage) {
    T input;
    bool valid = false;
    while (!valid) {
        cout << prompt;
        try {
            if (!(cin >> input)) {
                throw invalid_argument("Dinh dang khong hop le");
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (!validator(input)) {
                throw invalid_argument(errorMessage);
            }
            valid = true;
        } catch (const exception& e) {
            cout << "Loi: " << e.what() << " Vui long nhap lai.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return input;
}

// Dat phong
void Reservation_Room(Hotel* hotel) {
    int id;
    string name;
    string phone;
    string roomType;
    int roomNumber;
    int days;
    float totalCost = 0;
    Room bookedRoom;

    cout << "\n+------------------------------------------------------------+\n";
    cout << "|                   DAT PHONG                                |\n";
    cout << "+------------------------------------------------------------+\n";
    displayRooms(hotel);

    cout << "\nXin Quy Khach Cung Cap Thong Tin\n";

    id = getValidInput<int>(
        "So Can Cuoc Cong Dan: ",
        [](int x) { return x > 0; },
        "ID phai la so duong!"
    );

    name = getValidInputString(
        "Ten Cua Quy Khach: ",
        [](string s) { return !s.empty() && s.length() <= 49; },
        "Ten khong duoc rong va toi da 49 ky tu!"
    );

    phone = getValidInputString(
        "So Dien Thoai: ",
        [](string s) { return s.length() >= 10 && s.length() <= 11 && all_of(s.begin(), s.end(), ::isdigit); },
        "So dien thoai phai co 10-11 chu so!"
    );

    roomType = getValidInputString(
        "Loai Phong Muon Dat: ",
        [hotel](string s) {
            RoomNode* current = hotel->roomList;
            while (current) {
                if (strcmp(current->room.roomType, s.c_str()) == 0) return true;
                current = current->next;
            }
            return false;
        },
        "Loai phong khong ton tai!"
    );
    days = getValidInput<int>(
        "So ngay thue phong: ",
        [](int x) { return x >= 1; },
        "So ngay thue phai lon hon hoac bang 1!"
    );

    int flag;
   do
   {
     do {
        cout << "\nNhap so phong ban muon them : ";
        if (cin >> roomNumber) {
            // Kiểm tra xem số có dương hay không
            if (roomNumber > 0) {
                break; 
            } else {
                cout << "Vui long nhap mot so nguyen duong!" << endl;
            }
        } else {
            
            cout << "Dau vao khong hop le! Vui long nhap mot so nguyen." << endl;
            cin.clear(); // Xóa trạng thái lỗi của cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa bộ đệm
        }
    } while (true);
    RoomNode* hotelRoom = searchRoomByNumber(hotel, roomNumber);
    if (hotelRoom != NULL && hotelRoom->room.status == 0 && hotelRoom->room.roomType == roomType) {
        hotelRoom->room.status = 1;
        hotelRoom->room.countUsing++;
        hotelRoom->room.days = days;
        totalCost = hotelRoom->room.pricePerDay * days;
        bookedRoom = hotelRoom->room;
        flag = 0;
    } else if(hotelRoom != NULL && hotelRoom->room.status == 1 && hotelRoom->room.roomType == roomType) {
        cout << "Phong  da co khach!\n";
        Customer waitCustomer;
        waitCustomer.id = id;
        strncpy(waitCustomer.name, name.c_str(), sizeof(waitCustomer.name) - 1);
        waitCustomer.name[sizeof(waitCustomer.name) - 1] = '\0';
        strncpy(waitCustomer.phone, phone.c_str(), sizeof(waitCustomer.phone) - 1);
        waitCustomer.phone[sizeof(waitCustomer.phone) - 1] = '\0';
        waitCustomer.days = days;
        waitCustomer.totalCost = 0;
        if (hotelRoom != NULL) {
            waitCustomer.room = hotelRoom->room;
        } else {
            waitCustomer.room.roomNumber = roomNumber;
            strcpy(waitCustomer.room.roomType, roomType.c_str());
            waitCustomer.room.pricePerDay = 0;
            waitCustomer.room.status = 1;
            waitCustomer.room.countUsing = 0;
            waitCustomer.room.days = 0;
        }

        char choice;
        cout << "Phong khong kha dung. Ban co muon them khach vao hang cho khong? (y/n): ";
        cin >> choice;
        cin.ignore();
        if (choice == 'y' || choice == 'Y') {
            addWaitNode(hotel, waitCustomer);
        }
        return;
        flag = 0;
    }else{
        cout << "\nPhong so " <<  roomNumber << " khong co trong danh sach phong";
        cout << "\nHay nhap lai so phong!";
        flag = 1;
    }

   } while (flag);
   
    
   

    CustomerNode* customerNode = createCustomerNode(id, name.c_str(), phone.c_str(), days, totalCost, bookedRoom);
    customerNode->next = hotel->customerList;
    hotel->customerList = customerNode;

    cout << "\nDat phong thanh cong!\n";

    cout << "Ban co muon hoan tac dat phong khong? (1: Co, 0: Khong): ";
    int undoChoice;
    cin >> undoChoice;
    cin.ignore();
    if (undoChoice == 1) {
        undoReservation(hotel);
        cout << "Hoan tac dat phong thanh cong!\n";
    } else {
        cout << "Dat phong da duoc luu lai.\n";
    }
}

// Tim kiem khach hang theo ID
CustomerNode* searchCustomerById(Hotel* hotel, int id) {
    CustomerNode* current = hotel->customerList;
    while (current != NULL) {
        if (current->customer.id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Hàm sửa đổi thông tin khách hàng
void changeInforCustomer(Hotel* hotel) {
    cout << "\n+------------------------------------------------------------+\n";
    cout << "|               SUA DOI THONG TIN KHACH HANG                 |\n";
    cout << "+------------------------------------------------------------+\n";

    // Nhập ID khách hàng cần sửa
    int id = getValidInput<int>(
        "Nhap ID khach hang can sua: ",
        [](int x) { return x > 0; },
        "ID phai la so duong!"
    );

    // Tìm khách hàng theo ID
    CustomerNode* customerNode = searchCustomerById(hotel, id);
    if (customerNode == NULL) {
        cout << "Khong tim thay khach hang voi ID: " << id << "\n";
        return;
    }

    // Hiển thị thông tin hiện tại của khách hàng
    cout << "\nThong tin hien tai cua khach hang:\n";
    cout << "| Ten khach hang: " << left << setw(42) << customerNode->customer.name << "|\n";
    cout << "| So dien thoai: " << left << setw(43) << customerNode->customer.phone << "|\n";
    cout << "| Phong da dat:                                              |\n";
    cout << "|  - So phong: " << left << setw(43) << customerNode->customer.room.roomNumber << "|\n";
    cout << "|  - Loai phong: " << left << setw(41) << customerNode->customer.room.roomType << "|\n";
    cout << "|  - Gia/ngay: " << left << setw(43) << fixed << setprecision(2) << customerNode->customer.room.pricePerDay << "|\n";
    cout << "|  - So ngay thue: " << left << setw(39) << customerNode->customer.room.days << "|\n";
    cout << "+------------------------------------------------------------+\n";

    string newName, newPhone;
    cout << "\nBan muon thay doi ten khong ?(y/n)";
    char choice; cin >> choice;
    if (choice == 'y') {
        // Nhập thông tin mới
        newName = getValidInputString(
        "Nhap ten moi (nhan Enter de giu nguyen): ",
        [](string s) { return s.empty() || (s.length() <= 49); },
        "Ten khong duoc vuot qua 49 ky tu!"
        );
    } else {
        newName = customerNode->customer.name;
    }
    
    cout << "\nBan co muon thay doi so dien thoai khong ?(y/n)";
    char choice1; cin >> choice1;
    if (choice == 'y') {
        newPhone = getValidInputString(
        "Nhap so dien thoai moi (nhan Enter de giu nguyen): ",
        [](string s) { return s.empty() || (s.length() >= 10 && s.length() <= 11 && all_of(s.begin(), s.end(), ::isdigit)); },
        "So dien thoai phai co 10-11 chu so!"
        );
    } else {
        newPhone = customerNode->customer.phone;
    }

    // Cập nhật thông tin khách hàng
    if (!newName.empty()) {
        strncpy(customerNode->customer.name, newName.c_str(), sizeof(customerNode->customer.name) - 1);
        customerNode->customer.name[sizeof(customerNode->customer.name) - 1] = '\0';
    }

    if (!newPhone.empty()) {
        strncpy(customerNode->customer.phone, newPhone.c_str(), sizeof(customerNode->customer.phone) - 1);
        customerNode->customer.phone[sizeof(customerNode->customer.phone) - 1] = '\0';
    }


    // Hiển thị thông tin sau khi cập nhật
    cout << "\nCap nhat thong tin khach hang thanh cong!\n";
    cout << "\nThong tin sau khi cap nhat:\n";
    cout << "| Ten khach hang: " << left << setw(42) << customerNode->customer.name << "|\n";
    cout << "| So dien thoai: " << left << setw(43) << customerNode->customer.phone << "|\n";
    
}

// Hien thi thong tin khach hang
void display_infor_Customer(Hotel* hotel) {
    CustomerNode* customerNode = hotel->customerList;
    if (customerNode == NULL) {
        cout << "\n+------------------------------------------------------------+\n";
        cout << "| Danh sach khach hang: TRONG                                |\n";
        cout << "+------------------------------------------------------------+\n";
        return;
    }

    cout << "\n+------------------------------------------------------------+\n";
    cout << "|                   DANH SACH KHACH HANG                     |\n";
    cout << "+------------+----------------------+--------------+------------+\n";
    cout << "| " << left << setw(10) << "ID"
         << "| " << left << setw(20) << "Ten khach"
         << "| " << left << setw(12) << "So dien thoai"
         << "| " << left << setw(10) << "So phong" << "|\n";
    cout << "+------------+----------------------+--------------+------------+\n";

    while (customerNode != NULL) {
        cout << "| " << left << setw(10) << customerNode->customer.id
             << "| " << left << setw(20) << customerNode->customer.name
             << "| " << left << setw(12) << customerNode->customer.phone
             << "| " << left << setw(10) << customerNode->customer.room.roomNumber << "|\n";
        customerNode = customerNode->next;
    }
    cout << "+------------+----------------------+--------------+------------+\n";

    customerNode = hotel->customerList;
    while (customerNode != NULL) {
        cout << "\nChi tiet khach hang " << customerNode->customer.name << ":\n";
        displayRoomInfo(&customerNode->customer.room);
        customerNode = customerNode->next;
    }
}

// Hiển thị thông tin khách hàng trong hàng chờ
void displayWaitCustomer(Hotel *hotel)
{
    WaitNode *waitNode = hotel->waitQueue ;
    if (waitNode == NULL)
    {
        cout << "\n+------------------------------------------------------------+\n";
        cout << "| Danh sach khach hang: TRONG                                |\n";
        cout << "+------------------------------------------------------------+\n";
        return ;
    }

    cout << "\n+------------------------------------------------------------+\n";
    cout << "|                   DANH SACH KHACH HANG CHO                    |\n";
    cout << "+------------+----------------------+--------------+------------+\n";
    cout << "| " << left << setw(10) << "ID"
         << "| " << left << setw(20) << "Ten khach"
         << "| " << left << setw(12) << "So dien thoai"
         << "| " << left << setw(10) << "So phong" << "|\n";
    cout << "+------------+----------------------+--------------+------------+\n";

    while (waitNode != NULL) {
        cout << "| " << left << setw(10) << waitNode->customer.id
             << "| " << left << setw(20) << waitNode->customer.name
             << "| " << left << setw(12) << waitNode->customer.phone
             << "| " << left << setw(10) << waitNode->customer.room.roomNumber << "|\n";
        waitNode = waitNode->next;
    }
    cout << "+------------+----------------------+--------------+------------+\n";

    while (waitNode != NULL) {
        cout << "\nChi tiet khach hang " << waitNode->customer.name << ":\n";
        displayRoomInfo(&waitNode->customer.room);
        waitNode = waitNode->next;
    }
}


// Nhap danh gia khach hang
void danhGiaKhachHang(Customer* c) {
    cout << "\n+------------------------------------------------------------+\n";
    cout << "|                   DANH GIA DICH VU                         |\n";
    cout << "+------------------------------------------------------------+\n";
    c->feedback.rating = getValidInput<int>(
        "Nhap rating (1-5): ",
        [](int x) { return x >= 1 && x <= 5; },
        "Rating phai tu 1 den 5!"
    );
    cin.ignore();
    cout << "Nhap nhan xet: ";
    cin.getline(c->feedback.comment, 100);
}

// Tra phong
void returnRoom(Hotel* hotel, int ID) {
    CustomerNode* consumer = hotel->customerList;
    CustomerNode* prev = NULL;
    bool found = false;

    while (consumer != NULL) {
        if (consumer->customer.id == ID) {
            found = true;
            float total = consumer->customer.room.pricePerDay * consumer->customer.room.days;

            cout << "\n+------------------------------------------------------------+\n";
            cout << "|                   HOA DON KHACH HANG                       |\n";
            cout << "+------------------------------------------------------------+\n";
            cout << "| ID khach hang: " << left << setw(43) << consumer->customer.id << "|\n";
            cout << "| Ten khach hang: " << left << setw(42) << consumer->customer.name << "|\n";
            cout << "| So dien thoai: " << left << setw(43) << consumer->customer.phone << "|\n";
            cout << "| Phong da dat:                                              |\n";
            cout << "|  - So phong: " << left << setw(43) << consumer->customer.room.roomNumber << "|\n";
            cout << "|  - Loai phong: " << left << setw(41) << consumer->customer.room.roomType << "|\n";
            cout << "|  - Gia/ngay: " << left << setw(43) << fixed << setprecision(2) << consumer->customer.room.pricePerDay << "|\n";
            cout << "|  - So ngay thue: " << left << setw(39) << consumer->customer.room.days << "|\n";
            cout << "| Tong gia tien: " << left << setw(43) << fixed << setprecision(2) << total << "|\n";
            cout << "+------------------------------------------------------------+\n";

            RoomNode* space = searchRoomByNumber(hotel, consumer->customer.room.roomNumber);
            if (space != NULL) {
                space->room.status = 0;
                space->room.days = 0;
                cout << "Tra phong " << consumer->customer.room.roomNumber << " thanh cong!\n";
            } else {
                cout << "Khong tim thay phong " << consumer->customer.room.roomNumber << " trong danh sach phong!\n";
            }

            consumer->customer.totalCost = total;
            cout << "\nXin quy khach danh gia dich vu cua chung toi!\n";
            danhGiaKhachHang(&consumer->customer);
            cout << "Cam on quy khach da su dung dich vu cua chung toi!\n";

            HistoryNode* historyNode = createHistoryNode(consumer->customer);
            historyNode->next = hotel->historyList;
            hotel->historyList = historyNode;

            if (prev == NULL) {
                hotel->customerList = consumer->next;
            } else {
                prev->next = consumer->next;
            }
            CustomerNode* temp = consumer->next ;
            delete temp ;
            consumer = temp ;

            WaitNode* wait = hotel->waitQueue;
            WaitNode* prevWait = NULL;
            while (wait != NULL) {
                if ((strcmp(wait->customer.room.roomType, space->room.roomType) == 0)
                    && (wait->customer.room.roomNumber == space->room.roomNumber)) {
                    CustomerNode* newCustomer = createCustomerNode(
                        wait->customer.id, wait->customer.name, wait->customer.phone,
                        wait->customer.days, space->room.pricePerDay * wait->customer.days, space->room);
                    newCustomer->next = hotel->customerList;
                    hotel->customerList = newCustomer;
                    space->room.status = 1;
                    space->room.days = wait->customer.days;
                    cout << "Khach hang " << wait->customer.name << " da dat duoc phong "
                         << space->room.roomNumber << " voi loai phong "
                         << wait->customer.room.roomType << " thanh cong!\n";

                    if (prevWait == NULL) {
                        hotel->waitQueue = wait->next;
                    } else {
                        prevWait->next = wait->next;
                    }
                    delete wait;
                    break;
                }
                prevWait = wait;
                wait = wait->next;
            }

            break ;
        }
        prev = consumer;
        consumer = consumer->next;
    }

    if (!found) {
        cout << "Khong tim thay khach hang voi ID: " << ID << "\n";
    }
}

// Doi du lieu hai phong
void swapRoomData(RoomNode* a, RoomNode* b) {
    Room temp = a->room;
    a->room = b->room;
    b->room = temp;
}

// Sap xep phong theo gia
void sortByPrice(Hotel* hotel) {
    RoomNode* current = hotel->roomList;
    while (current != NULL) {
        RoomNode* minNode = current;
        RoomNode* nextNode = current->next;
        while (nextNode != NULL) {
            if (nextNode->room.pricePerDay < minNode->room.pricePerDay) {
                minNode = nextNode;
            }
            nextNode = nextNode->next;
        }
        if (minNode != current) {
            swapRoomData(current, minNode);
        }
        current = current->next;
    }
    displayRooms(hotel);
}

// Bao cao thong ke
void baoCaoThongKe(Hotel* hotel) {
    float tongDoanhThu = 0;
    int soKhach = 0;
    float totalRating = 0.0;

    cout << "\n+---------------------------------------------------------------------------+\n";
    cout << "|                   LICH SU KHACH HANG VA DANH GIA                          |\n";
    cout << "+------------+-----------------+--------------+--------------+-------+---------------+\n";
    cout << "| " << left << setw(10) << "ID Khach"
         << "| " << left << setw(15) << "Ten Khach"
         << "| " << left << setw(12) << "So Dien Thoai"
         << "| " << left << setw(12) << "Tong Tien"
         << "| " << left << setw(5) << "Rating"
         << "| " << left << setw(13) << "Nhan Xet" << "|\n";
    cout << "+------------+-----------------+--------------+--------------+-------+---------------+\n";

    HistoryNode* current = hotel->historyList;
    if (current == NULL) {
        cout << "| " << left << setw(77) << "Chua co du lieu lich su khach hang" << "|\n";
    } else {
        while (current != NULL) {
            soKhach++;
            tongDoanhThu += current->customer.totalCost;
            totalRating += current->customer.feedback.rating;
            cout << "| " << left << setw(10) << current->customer.id
                 << "| " << left << setw(15) << current->customer.name
                 << "| " << left << setw(12) << current->customer.phone
                 << "| " << left << setw(12) << fixed << setprecision(2) << current->customer.totalCost
                 << "| " << left << setw(5) << current->customer.feedback.rating
                 << "| " << left << setw(13) << current->customer.feedback.comment << "|\n";
            current = current->next;
        }
    }
    cout << "+------------+-----------------+--------------+--------------+-------+---------------+\n";

    cout << "\nTong So Khach: " << soKhach << endl;
    cout << "Tong Doanh Thu: " << fixed << setprecision(2) << tongDoanhThu << endl;
    cout << "Rating Trung Binh: ";
    if (soKhach == 0) {
        cout << "Chua co khach hang nao" << endl;
    } else {
        cout << fixed << setprecision(2) << totalRating / soKhach << endl;
    }

    cout << "\n+------------------------------------------------------------+\n";
    cout << "|           DANH SACH PHONG VA SO LAN DAT PHONG              |\n";
    cout << "+------------+-----------------+--------------+\n";
    cout << "| " << left << setw(10) << "So Phong"
         << "| " << left << setw(15) << "Loai Phong"
         << "| " << left << setw(12) << "So Lan Dat" << "|\n";
    cout << "+------------+-----------------+--------------+\n";

    RoomNode* roomCurrent = hotel->roomList;
    while (roomCurrent != NULL) {
        cout << "| " << left << setw(10) << roomCurrent->room.roomNumber
             << "| " << left << setw(15) << roomCurrent->room.roomType
             << "| " << left << setw(12) << roomCurrent->room.countUsing << "|\n";
        roomCurrent = roomCurrent->next;
    }
    cout << "+------------+-----------------+--------------+\n";
}

// Struct User: Quan ly thong tin nguoi dung
typedef struct User {
    string username;
    string password;
    bool isAdmin;
} User;

User admin = {"admin", "********", true};

// Dang nhap
bool login(User& user) {
    string username, password;
    cout << "\n+------------------------------------------------------------+\n";
    cout << "|                   DANG NHAP HE THONG                       |\n";
    cout << "+------------------------------------------------------------+\n";
    cout << "Nhap ten dang nhap: ";
    cin >> username;
    cout << "Nhap mat khau: ";
    cin >> password;
    cin.ignore();

    if (username == user.username && password == user.password) {
        cout << "Dang nhap thanh cong!\n";
        return true;
    } else {
        cout << "Ten dang nhap hoac mat khau khong dung!\n";
        return false;
    }
}

// Dang xuat
void logout() {
    cout << "\nDang xuat thanh cong!\n";
}

// Doi mat khau
void changePassword(User& user) {
    string currentPassword;
    cout << "\n+------------------------------------------------------------+\n";
    cout << "|                   DOI MAT KHAU                             |\n";
    cout << "+------------------------------------------------------------+\n";
    cout << "Nhap mat khau hien tai: ";
    cin >> currentPassword;
    cin.ignore();
    if (currentPassword != user.password) {
        cout << "Mat khau hien tai khong dung!\n";
        return;
    }
    string newPassword;
    cout << "Nhap mat khau moi: ";
    cin >> newPassword;
    cin.ignore();
    user.password = newPassword;
    cout << "Mat khau da duoc doi thanh cong!\n";
}

// Hien thi menu quan ly phong
void displayRoomMenu() {
    cout << "\n+------------------------------------------------------------+\n";
    cout << "|                   QUAN LY PHONG                            |\n";
    cout << "+------------------------------------------------------------+\n";
    cout << "| 1. Hien thi danh sach phong                                |\n";
    cout << "| 2. Tim kiem phong theo so phong                            |\n";
    cout << "| 3. Sap xep phong theo gia                                  |\n";
    cout << "| 4. Them phong moi                                          |\n";
    cout << "| 5. Quay lai menu chinh                                     |\n";
    cout << "+------------------------------------------------------------+\n";
    cout << "Nhap lua chon: ";
}

// Hien thi menu quan ly khach
void displayCustomerMenu() {
    cout << "\n+------------------------------------------------------------+\n";
    cout << "|                   QUAN LY KHACH                            |\n";
    cout << "+------------------------------------------------------------+\n";
    cout << "| 1. Dat phong                                               |\n";
    cout << "| 2. Hien thi danh sach khach hang                           |\n";
    cout << "| 3. Hien thi danh sach khach dang cho                       |\n";
    cout << "| 4. Thay doi thong tin khach hang                           |\n";
    cout << "| 5. Tim kiem khach hang                                     |\n";
    cout << "| 6. Tra phong                                               |\n";
    cout << "| 7. Hoan tac dat phong                                      |\n";
    cout << "| 8. Hoan tac tra phong                                      |\n";
    cout << "| 9. Quay lai menu chinh                                     |\n";
    cout << "+------------------------------------------------------------+\n";
    cout << "Nhap lua chon: ";
}

// Hien thi menu quan ly nhan su va bao cao
void displayAdminReportMenu() {
    cout << "\n+------------------------------------------------------------+\n";
    cout << "|           QUAN LY NHAN SU VA BAO CAO THONG KE              |\n";
    cout << "+------------------------------------------------------------+\n";
    cout << "| 1. Bao cao thong ke                                        |\n";
    cout << "| 2. Doi mat khau                                            |\n";
    cout << "| 3. Dang xuat                                               |\n";
    cout << "| 4. Quay lai menu chinh                                     |\n";
    cout << "+------------------------------------------------------------+\n";
    cout << "Nhap lua chon: ";
}

// Hien thi menu chinh
void displayMainMenu() {
    cout << "\n+------------------------------------------------------------+\n";
    cout << "|                   QUAN LY KHACH SAN                        |\n";
    cout << "+------------------------------------------------------------+\n";
    cout << "| 1. Quan ly phong                                           |\n";
    cout << "| 2. Quan ly khach                                           |\n";
    cout << "| 3. Quan ly nhan su va bao cao thong ke                     |\n";
    cout << "| 4. Thoat                                                   |\n";
    cout << "+------------------------------------------------------------+\n";
    cout << "Nhap lua chon: ";
}

void freeHotel(Hotel* hotel) {
    RoomNode* currentRoom = hotel->roomList;
    while (currentRoom != nullptr) {
        RoomNode* temp = currentRoom;
        currentRoom = currentRoom->next;
        delete temp;
    }
    CustomerNode* currentCustomer = hotel->customerList;
    while (currentCustomer != nullptr) {
        CustomerNode* temp = currentCustomer;
        currentCustomer = currentCustomer->next;
        delete temp;
    }
    HistoryNode* currentAction = hotel->historyList;
    while (currentAction != nullptr) {
        HistoryNode* temp = currentAction;
        currentAction = currentAction->next;
        delete temp;
    }
    WaitNode* currentWait = hotel->waitQueue;
    while (currentWait != nullptr) {
        WaitNode* temp = currentWait;
        currentWait = currentWait->next;
        delete temp;
    }
    delete hotel;
}

// Ham main
int main() {
    Hotel* hotel = initHotel();
    User user = admin;

    if (!login(user)) {
        cout << "Dang nhap that bai! Ket thuc chuong trinh.\n";
        return 0;
    }

    int mainChoice, subChoice;
    do {
        displayMainMenu();
        cin >> mainChoice;
        cin.ignore();

        switch (mainChoice) {
            case 1: // Quan ly phong
                do {
                    displayRoomMenu();
                    cin >> subChoice;
                    cin.ignore();
                    switch (subChoice) {
                        case 1:
                            displayRooms(hotel);
                            break;
                        case 2: {
                            cout << "Nhap so phong can tim: ";
                            int roomNumber;
                            cin >> roomNumber;
                            cin.ignore();
                            RoomNode* room = searchRoomByNumber(hotel, roomNumber);
                            if (room) {
                                displayRoomInfo(&room->room);
                            } else {
                                cout << "Khong tim thay phong!\n";
                            }
                            break;
                        }
                        case 3:
                            sortByPrice(hotel);
                            break;
                        case 4:
                            addNewRoom(hotel);
                            break;
                        case 5:
                            break;
                        default:
                            cout << "Lua chon khong hop le!\n";
                    }
                } while (subChoice != 5);
                break;

            case 2: // Quan ly khach
                do {
                    displayCustomerMenu();
                    cin >> subChoice;
                    cin.ignore();
                    switch (subChoice) {
                        case 1:
                            Reservation_Room(hotel);
                            break;
                        case 2:
                            display_infor_Customer(hotel);
                            break;
                        case 3:
                            displayWaitCustomer(hotel);
                            break;
                        case 4: 
                            changeInforCustomer(hotel);
                            break;
                        case 5: {
                            cout << "Nhap ID khach hang can tim: ";
                            int customerId;
                            cin >> customerId;
                            cin.ignore();
                            CustomerNode* customer = searchCustomerById(hotel, customerId);
                            if (customer) {
                                cout << "\n+------------------------------------------------------------+\n";
                                cout << "|                   THONG TIN KHACH HANG                     |\n";
                                cout << "+------------------------------------------------------------+\n";
                                cout << "| Ten khach hang: " << left << setw(42) << customer->customer.name << "|\n";
                                cout << "| So can cuoc: " << left << setw(45) << customer->customer.id << "|\n";
                                cout << "| So dien thoai: " << left << setw(43) << customer->customer.phone << "|\n";
                                cout << "+------------------------------------------------------------+\n";
                                displayRoomInfo(&customer->customer.room);
                            } else {
                                cout << "Khong tim thay khach hang voi ID: " << customerId << "\n";
                            }
                            break;
                        }
                        case 6:
                            int customerId;
                            cout << "Nhap ID cua khach hang da dat phong: ";
                            cin >> customerId;
                            cin.ignore();
                            returnRoom(hotel, customerId);
                            break;
                        case 7:
                            undoReservation(hotel);
                            break;
                        case 8:
                            undoTraPhong(hotel);
                            break;
                        case 9:
                            break;
                        default:
                            cout << "Lua chon khong hop le!\n";
                    }
                } while (subChoice != 9);
                break;

            case 3: // Quan ly nhan su va bao cao
                do {
                    displayAdminReportMenu();
                    cin >> subChoice;
                    cin.ignore();
                    switch (subChoice) {
                        case 1:
                            baoCaoThongKe(hotel);
                            break;
                        case 2:
                            changePassword(user);
                            break;
                        case 3:
                            logout();
                            if (!login(user)) {
                                cout << "Dang nhap that bai! Ket thuc chuong trinh.\n";
                                return 0;
                            }
                            break;
                        case 4:
                            break;
                        default:
                            cout << "Lua chon khong hop le!\n";
                    }
                } while (subChoice != 4);
                break;

            case 4:
                cout << "Tam biet!\n";
                break;

            default:
                cout << "Lua chon khong hop le!\n";
        }
    } while (mainChoice != 4);
    
    freeHotel(hotel);

return 0;

}
