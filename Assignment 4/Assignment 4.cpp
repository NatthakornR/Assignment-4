#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <nlohmann/json.hpp> // (I installed this from https://github.com/nlohmann/json)

using namespace std;
using json = nlohmann::json;

class Person {
public:
    string name;
    string email;
    string phone;
    string address;
    int age;

    Person(string name, string email, string phone, string address, int age)
        : name(name), email(email), phone(phone), address(address), age(age) {}

    json toJson() const {
        return json{ {"name", name}, {"email", email}, {"phone", phone}, {"address", address}, {"age", age} };
    }

    static Person fromJson(const json& j) {
        return Person(j.at("name"), j.at("email"), j.at("phone"), j.at("address"), j.at("age"));
    }
};

class ContactList {
private:
    vector<Person> contacts;

public:
    void addContact(const Person& person) {
        contacts.push_back(person);
    }

    void printAllContacts() const {
        if (contacts.empty()) {
            cout << "No contacts available.\n";
            return;
        }

        for (const auto& contact : contacts) {
            cout << "Name: " << contact.name
                << "\nEmail: " << contact.email
                << "\nPhone: " << contact.phone
                << "\nAddress: " << contact.address
                << "\nAge: " << contact.age
                << "\n-----------------------------\n";
        }
    }

    void searchContacts(const string& parameter, const string& value) const {
        vector<Person> results;
        for (const auto& contact : contacts) {
            if ((parameter == "name" && contact.name == value) ||
                (parameter == "email" && contact.email == value) ||
                (parameter == "phone" && contact.phone == value) ||
                (parameter == "address" && contact.address == value)) {
                results.push_back(contact);
            }
        }

        if (results.empty()) {
            cout << "No matching contacts found.\n";
        }
        else {
            for (const auto& result : results) {
                cout << "Name: " << result.name
                    << "\nEmail: " << result.email
                    << "\nPhone: " << result.phone
                    << "\nAddress: " << result.address
                    << "\nAge: " << result.age
                    << "\n-----------------------------\n";
            }
        }
    }

    void saveToFile(const string& filename) const {
        json j;
        for (const auto& contact : contacts) {
            j.push_back(contact.toJson());
        }

        ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile << j.dump(4);
            outFile.close();
            cout << "Contacts saved to " << filename << "\n";
        }
        else {
            cerr << "Failed to open file for writing.\n";
        }
    }

    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            json j;
            inFile >> j;
            contacts.clear();
            for (const auto& item : j) {
                contacts.push_back(Person::fromJson(item));
            }
            cout << "Contacts loaded from " << filename << "\n";
        }
        else {
            cerr << "Failed to open file for reading.\n";
        }
    }
};

int main() {
    ContactList contactList;
    int choice;

    while (true) {
        cout << "\nMenu:\n"
            << "1. Add Contact\n"
            << "2. Print All Contacts\n"
            << "3. Search Contacts\n"
            << "4. Save Contacts to File\n"
            << "5. Load Contacts from File\n"
            << "6. Exit\n"
            << "Enter number: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string name, email, phone, address;
            int age;

            cout << "Enter Name: ";
            getline(cin, name);
            cout << "Enter Email: ";
            getline(cin, email);
            cout << "Enter Phone: ";
            getline(cin, phone);
            cout << "Enter Address: ";
            getline(cin, address);
            cout << "Enter Age: ";
            cin >> age;
            cin.ignore();

            contactList.addContact(Person(name, email, phone, address, age));
        }
        else if (choice == 2) {
            contactList.printAllContacts();
        }
        else if (choice == 3) {
            string parameter, value;
            cout << "Search by (name/email/phone/address): ";
            getline(cin, parameter);
            cout << "Enter value: ";
            getline(cin, value);

            contactList.searchContacts(parameter, value);
        }
        else if (choice == 4) {
            string filename;
            cout << "Enter filename to save contacts (e.g., contacts.json): ";
            getline(cin, filename);
            contactList.saveToFile(filename);
        }
        else if (choice == 5) {
            string filename;
            cout << "Enter filename to load contacts (e.g., contacts.json): ";
            getline(cin, filename);
            contactList.loadFromFile(filename);
        }
        else if (choice == 6) {
            cout << "Exiting program.\n";
            break;
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}