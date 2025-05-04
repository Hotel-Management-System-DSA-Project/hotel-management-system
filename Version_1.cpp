#include <stdio.h>
#include <iostream>
using namespace std;

enum roomStatus {ready, wait, inUse};
enum Type {vip, single, doubles};

struct Room {
    int ID;
    int Price;
    roomStatus stt;
    Type type;
};

struct roomNode {
    Room room;
    roomNode *next, *prev;
};

void initRoom(roomNode& R) {
    R.next = NULL;

}

struct Custom {
    char name[20];
    int age;
    int phoneNumber;
    int identify;
    int timeUse;
};

int main() {
    cout << "Test code";
    return 0;
}
