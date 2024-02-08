#include<iostream>
#include<string>
using namespace std;

class Hall {
private:
    string name;
    bool ac;
    bool stage;
    bool speakers;
    double price;
    bool occupied;
     string hall_in;

public:
    Hall(string name, bool ac, bool stage, bool speakers, double price,bool occupied)
        : name(name), ac(ac), stage(stage), speakers(speakers), price(price), occupied(occupied) {}

    friend class BookingSystem;
};

// Customer class
class Customer {
private:
    string username;
    string password;
    string startTime;
    string endTime;
    string meetingDate;
    string hallname;
    

public:
 static int objectCount;
    Customer(string username, string password, string startTime, string endTime)
        : username(username), password(password), startTime(startTime), endTime(endTime) {}

    friend class BookingSystem;
};