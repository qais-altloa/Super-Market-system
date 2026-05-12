
#pragma once
#include <iostream>
#include<string>
using namespace std;



struct Customer {
    int id;
    string name;
    int points;
    Customer* next;

    Customer(int i, string n) {
        id = i;
        name = n;
        points = 0;
        next = nullptr;
    }


    string getLevel() {
        if (points >= 2000) return "PLATINUM";
        else if (points >= 1000) return "GOLD";
        else if (points >= 500) return "SILVER";
        else return "NORMAL";
    }


    float getLoyaltyDiscount(float subtotal) {
        float discount = 0.0f;
        string level = getLevel();
        if (level == "SILVER") discount = subtotal * 0.10f;
        else if (level == "GOLD") discount = subtotal * 0.20f;
        else if (level == "PLATINUM")discount = subtotal * 0.30f;
        return discount;
    }

    void displayInfo() {
        cout << "=== Customer Info ===\n";
        cout << "ID: " << id << "\n";
        cout << "Name: " << name << "\n";
        cout << "Points: " << points << "\n";
        cout << "Level: " << getLevel() << "\n";
        cout << "=====================\n";
    }
};

class LoyaltyProgram {
private:
    Customer* head;
    int nextID;

public:
    LoyaltyProgram() {
        head = nullptr;
        nextID = 101;
    }


    Customer* addCustomer(string name) {
        Customer* newCust = new Customer(nextID++, name);
        newCust->next = head;
        head = newCust;
        cout << "Your Customer ID (for next login) is: " << newCust->id << endl;
        return newCust;
    }

    Customer* findCustomer(const int& Id) {
        Customer* temp = head;
        while (temp) {
            if (temp->id == Id) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    void addPoints(Customer* cust, float purchaseAmount) {
        if (!cust) return;
        int earnedPoints = int(purchaseAmount / 10);
        cust->points += earnedPoints;
        cout << cust->name << " earned " << earnedPoints << " points. Total: " << cust->points << endl;
    }


    void listCustomers() {
        Customer* temp = head;
        cout << "\n=== Loyalty Customers ===\n";
        while (temp) {
            temp->displayInfo();
            temp = temp->next;
        }
    }
};