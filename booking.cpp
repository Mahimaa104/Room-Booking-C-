#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <limits>
#include <regex>
#include <conio.h>
#include <ctype.h>
#include <cstdlib>
#include "room.cpp"

using namespace std;


// BookingSystem class
class BookingSystem {
private:
    vector<Customer> customers;
    vector<Room> rooms;
    vector<string> waitingList;
    Customer* loggedInUser; // To keep track of the currently logged-in user

public:

    BookingSystem() {

        // Load customers from file
        loadCustomers();
        loadRooms();
        loadWaitingList();
        loggedInUser = nullptr; // Initialize the logged-in user to nullptr
    }

    void signUp() {
        string username, password;
        std::cout << "Enter Username: ";
        cin >> username;
        std::cout << "Enter Password: ";
        // cin >> password;
        char ch = _getch();
        while(ch != 13){//character 13 is enter
          password.push_back(ch);
          std::cout << '*';
          ch = _getch();
        }
        for (auto& customer : customers) {
            if (customer.username == username) {
                std::cout << "\nUser name already exists try other user name!!" << endl;
                return;
            }
        }

        /*string startTime;
        int days;
        std::cout << "\nEnter the aprroximate time of Check-in (HH:MM): ";
        startTime = getTimeInput();
        std::cout << "\nEnter the number of days you would like to stay: ";
        cin>>days;

        Customer customer(username, password, startTime, days);
        customers.push_back(customer);*/

        std::cout << "\nSign up successful!" << endl;

        // Save customers to file
        saveCustomers();
    }

    void logIn() {
        string username, password;
        std::cout << "Enter username: ";
        cin >> username;
        std::cout << "Enter password: ";
        char ch = _getch();
        while(ch != 13){//character 13 is enter
            password.push_back(ch);
            std::cout << '*';
            ch = _getch();
        }


        for (auto& customer : customers) {
            if (customer.username == username && customer.password == password) {
                std::cout <<endl<<"\nLogin successful!" << endl;
                loggedInUser = &customer; // Set the logged-in user
                return;
            }
        }

        std::cout << "\nInvalid username or password. Please try again." << endl;
    }

    void bookRoom() {
        if (loggedInUser == nullptr) {
            std::cout << "Please log in before booking a room." << endl;
            return;
        }

        std::cout << "Available Rooms:" << endl;
        displayRooms();
        int roomIndex;
        label:
        try{

        std::cout << "Enter the index of the room you want to book: ";
        cin >> roomIndex;
        if(cin.fail()){
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw runtime_error("Invalid input\n");

        }


        if (roomIndex <= 0 || roomIndex > rooms.size()) {
            std::cout << "Invalid room index." << endl;
            return;
        }
        }

        catch(...){
            cout<<"Enter a valid number\n"<<endl;
            goto label;
        }

        Room& selectedRoom = rooms[roomIndex-1];

        if (selectedRoom.occupied) {
            std::cout << "Room is already booked. Adding to waiting list." << endl;
            waitingList.push_back(loggedInUser->username);
            loggedInUser->checkinDate = getDateInput();
            loggedInUser->startTime = getTimeInput();
            std::cout<<"Enter the number of days you want to stay"<<endl;
            cin>>loggedInUser->days;
        } else {
            selectedRoom.occupied = true;
             loggedInUser->roomname=to_string(roomIndex);
             selectedRoom.room_in=to_string(roomIndex);
             loggedInUser->checkinDate = getDateInput();
        loggedInUser->startTime = getTimeInput();
        std::cout<<"Enter the number of days you want to stay"<<endl;
        cin>>loggedInUser->days;
        int hh1=std::stoi(loggedInUser->startTime .substr(0,2));
        int mm1=std::stoi(loggedInUser->startTime .substr(3,2));
        int total = (loggedInUser->days*selectedRoom.price);
        cout << "Room booked successfully!" << endl;
        cout<<"Your total bill is "<<total<<endl;

        Customer::objectCount++;

        }
//  std::cout << "Total Cost: $" << totalCost << endl;
        // Save customers and waiting list to file
        saveCustomers();
        saveWaitingList();
        saveRooms();
    }

    void cancelBooking() {
        if (loggedInUser == nullptr) {
            std::cout << "Please log in before cancelling a booking." << endl;
            return;
        }

        if (loggedInUser->startTime.empty() || !loggedInUser->days) {
            std::cout << "No booking found for the logged-in user." << endl;
            return;
        }

        Room* bookedRoom = getBookedRoom();
        if (bookedRoom != nullptr) {
            bookedRoom->occupied = false;

        }

        loggedInUser->startTime.clear();
        loggedInUser->days=0;

        std::cout << "Booking canceled successfully!" << endl;
         Customer::objectCount--;

        // Save customers to file
        saveCustomers();
        saveRooms();
    }

    void rescheduleBooking() {
        if (loggedInUser == nullptr) {
            std::cout << "Please log in before rescheduling a booking." << endl;
            return;
        }

        if (loggedInUser->startTime.empty() || !loggedInUser->days) {
            std::cout << "No booking found for the logged-in user." << endl;
            return;
        }

        std::cout << "Rescheduling Options:" << endl;
        std::cout << "1. Rebook the Room" << endl;
        std::cout << "2. Cancel the booking" << endl;
        std::cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                cancelBooking();
                bookRoom();
                break;
            case 2:
                cancelBooking();
                break;
            default:
                std::cout << "Invalid choice." << endl;
                break;
        }
    }

    void displayRooms() {
        for (int i = 0; i < rooms.size(); i++) {
            std::cout << "Room " << i+1 << ": " << rooms[i].name << endl;
            std::cout << "AC: " << (rooms[i].ac ? "Yes" : "No") << endl;
            std::cout << "Price: " << rooms[i].price << endl;
            std::cout << "Occupied: " << (rooms[i].occupied ? "Yes" : "No") << endl;
            std::cout << endl;
        }
    }

    void displayWaitingList() {
        if (waitingList.empty()) {
            std::cout << "Waiting list is empty." << endl;
            return;
        }

        std::cout << "Waiting List:" << endl;
        for (const auto& customer : waitingList) {
            std::cout << customer << endl;
        }
    }

private:
    void loadCustomers() {
        ifstream file("customers.txt");
        if (!file) {
            std::cout << "Error opening file." << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            vector<string> tokens = splitString(line, '|');
            if (tokens.size() == 4) {
                string username = tokens[0];
                string password = tokens[1];
                string startTime = tokens[2];
                int days = stoi(tokens[3]);

                Customer customer(username, password, startTime, days);
                customers.push_back(customer);
            }
        }

        file.close();
    }

    void loadRooms(){

             ifstream file("rooms.txt");
        if (!file) {
            std::cout << "Error opening file." << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            vector<string> tokens = splitString(line, '|');
            if (tokens.size() == 4) {
                string name = tokens[0];
                bool ac = (bool)stoi(tokens[1]);
                double price = stoi(tokens[2]);
                bool occupied = (bool)stoi(tokens[3]);

                Room room(name,ac,price,occupied);
                rooms.push_back(room);
            }
        }

        file.close();
        }


    void saveRooms(){
        ofstream file("rooms.txt");
        if (!file) {
            std::cout << "Error opening file." << endl;
            return;
        }

        for (const auto& room : rooms) {
            file << room.name << "|" << room.ac << "|"<<room.price<<"|"<<room.occupied<< endl;
        }

        file.close();
    }

    void saveCustomers() {
        ofstream file("customers.txt");
        if (!file) {
            std::cout << "Error opening file." << endl;
            return;
        }

        for (const auto& customer : customers) {
            file << customer.username << "|" << customer.password << "|"<< customer.startTime << "|" << customer.days << endl;
        }

        file.close();
    }

    void loadWaitingList() {
        ifstream file("waiting_list.txt");
        if (!file) {
            std::cout << "Error opening file." << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            waitingList.push_back(line);
        }

        file.close();
    }

    void saveWaitingList() {
        ofstream file("waiting_list.txt");
        if (!file) {
            std::cout << "Error opening file." << endl;
            return;
        }

        for (const auto& customer : waitingList) {
            file << customer << endl;
        }

        file.close();
    }

    string getTimeInput() {
        string time;
        while (true) {
            std::cout << "Enter the time (HH:MM): ";
            cin >> time;

            regex pattern("(0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]");
            if (regex_match(time, pattern)) {
                return time;
            } else {
                std::cout << "Invalid time format. Please enter in HH:MM format." << endl;
            }
        }
    }
string getDateInput() {
        string date;
        while (true) {
            std::cout << "Enter the date of the check-in(dd/mm/yyyy): ";
            cin >> date;

            regex pattern("([0-2][0-9]|3[0-1])/(0[1-9]|1[0-2])/\\d{4}");  // dd/mm/yyyy format
            if (regex_match(date, pattern)) {
                // Validate the entered date against the current date
                time_t currentTime = time(nullptr);
                tm* currentDate = localtime(&currentTime);
                int currentDay = currentDate->tm_mday;
                int currentMonth = currentDate->tm_mon + 1;
                int currentYear = currentDate->tm_year + 1900;

                int enteredDay = stoi(date.substr(0, 2));
                int enteredMonth = stoi(date.substr(3, 2));
                int enteredYear = stoi(date.substr(6, 4));

                if (enteredYear > currentYear ||
                    (enteredYear == currentYear && enteredMonth > currentMonth) ||
                    (enteredYear == currentYear && enteredMonth == currentMonth && enteredDay > currentDay)) {
                    return date;
                } else {
                    std::cout << "Invalid date. Please enter a future date." << endl;
                }
            } else {
                std::cout << "Invalid date format. Please enter in dd/mm/yyyy format." << endl;
            }
        }
    }

    vector<string> splitString(const string& s, char delimiter) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    Room* getBookedRoom() {
        for (auto& room : rooms) {
            if (room.room_in == loggedInUser->roomname) {
                return &room;
            }
        }
        return nullptr;
    }
};




