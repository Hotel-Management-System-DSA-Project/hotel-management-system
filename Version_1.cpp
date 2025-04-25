#include<stdio.h>
#include<iostream>
using namespace std;

enum Status {availble, inavailble};
enum Sexsual {man, felman};
struct Room {
    int ID;
    int Price;
    Status stt;
};

struct Custom {
    char name[20];
    int age;
    Sexsual sx;
    int telNumber;
    int identify;
    int time;
};

struct Service {
    int a;
    int b;
};

int main () {
    cout << "Du an quan ly khach san";
    return 0;
}