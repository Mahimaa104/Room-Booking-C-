#include<iostream>
#include<string>
using namespace std;

class Room {
private:
    string name;
    bool ac;
    double price;
    bool occupied;
    string room_in;

public:
    Room(string name, bool ac, double price,bool occupied)
        : name(name), ac(ac), price(price), occupied(occupied) {}

    friend class BookingSystem;
};

// Customer class
class Customer {
private:
    string username;
    string password;
    string startTime;
    int days;
    string checkinDate;
    string roomname;


public:
 static int objectCount;
    Customer(string username, string password, string startTime, int days)
        : username(username), password(password), startTime(startTime), days(days) {}

    friend class BookingSystem;
};
