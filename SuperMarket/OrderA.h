#pragma once
#include<iostream>
using namespace std;
#include"Templates.h"
#include"Customer_Cart.h"
#include"Promotion.h"
#include"Loyalty.h"
#include <string>
class Order {
private:
    int order_id;
    string customer_name;
    DoublyLinkedList<CartItem> items;
    float subtotal;
    float tax;
    float discount;
    float total;
    string status;
    int processing_time;
public:
    Order() {}
    Order(int id, string name, DoublyLinkedList<CartItem> cart_items, float dis = 0.0) {
        order_id = id;
        customer_name = name;
        status = "Pending";
        ListNode<CartItem>* temp = cart_items.getHead();
        while (temp) {
            items.insert_back(temp->data);
            temp = temp->next;
        }
        subtotal = 0;
        temp = items.getHead();
        while (temp) {
            subtotal += temp->data.get_total();
            temp = temp->next;
        }
        discount = dis;
        tax = subtotal * 0.05;

        total = subtotal + tax - discount;
        processing_time = items.size() * 3;
    }
    // for analytics
    float get_total() { return total; }

    void print_bill() {
        cout << "\n==============================\n";
        cout << "        BILL RECEIPT\n";
        cout << "==============================\n";
        cout << "Order ID: " << order_id << endl;
        cout << "Customer: " << customer_name << endl;
        cout << "Status  : " << status << endl;

        time_t now = time(0);
        char dt[26];
        ctime_s(dt, sizeof(dt), &now);
        cout << "Date: " << dt;

        cout << "\n------ Items ------\n";
        ListNode<CartItem>* temp = items.getHead();
        while (temp) {
            cout << temp->data.product->get_name()
                << " x" << temp->data.quantity
                << " = " << temp->data.get_total() << " $\n";
            temp = temp->next;
        }

        cout << "-------------------\n";
        cout << "Subtotal : " << subtotal << " $\n";
        cout << "Tax (5%) : " << tax << " $\n";
        cout << "Discount : -" << discount << " $\n";
        cout << "TOTAL    : " << total << " $\n";
        cout << "==============================\n";
    }

    int get_id() { return order_id; }
    string get_customer() { return customer_name; }
    int get_processing_time() { return processing_time; }
    string get_status() { return status; }
    void set_status(string s) { status = s; }
    ListNode<CartItem>* get_items() { return items.getHead(); }
    float get_subtotal() { return subtotal; }
};
// =====================
// CHECKOUT CLASS
// =====================
class Checkout {
private:
    Queue<Order> orders;
    int nextID = 1;
    // for Analytics
    Queue<Order> history;
public:
    Queue<Order>& get_history() { return history; }
    void add_order(Customer* cust,
        ShoppingCart& cart,
        promotion* promo,
        LoyaltyProgram& loyalty, InventoryManager& inv)
    {
        if (!cust || cart.get_items() == nullptr) return;

        DoublyLinkedList<CartItem> copied;
        ListNode<CartItem>* temp = cart.get_items();
        float subtotal = 0.0f;
        while (temp) {
            copied.insert_back(temp->data);
            subtotal += temp->data.get_total();
            temp = temp->next;
        }

        float promoDiscount = 0.0f;
        int tries = 0;
        while (tries < 3) {
            cout << "Do you have a promo code? (1=Yes, 0=No): ";
            int ans; cin >> ans; cin.ignore();
            if (ans != 1) break;
            string code;
            cout << "Enter promo code: ";
            getline(cin, code);
            promoDiscount = promo->applyPromocode(code, subtotal);
            if (promoDiscount > 0) {
                cout << "Promo code applied: " << promoDiscount << "$\n";
                break;
            }
            else {
                cout << "Invalid promo code. Try again.\n";
                tries++;
                if (tries == 3)
                    cout << "Skipping promo code after 3 attempts.\n";
            }
        }

        float loyaltyDiscount = cust->getLoyaltyDiscount(subtotal);

        float totalDiscount = promoDiscount + loyaltyDiscount;
        float maxDiscount = subtotal * 0.60f;
        if (totalDiscount > maxDiscount)
            totalDiscount = maxDiscount;

        float tax = subtotal * 0.05f;
        float total = subtotal + tax - totalDiscount;

        cout << "\n===== BILL PREVIEW =====\n";
        cout << "Subtotal: " << subtotal << "$\n";
        cout << "Promo Discount: -" << promoDiscount << "$\n";
        cout << "Loyalty Discount: -" << loyaltyDiscount << "$\n";
        if ((promoDiscount + loyaltyDiscount) > maxDiscount)
            cout << "max Discount Applied: -" << maxDiscount << "$\n";
        cout << "Tax (5%): " << tax << "$\n";
        cout << "Total: " << total << "$\n";

        int confirm;
        cout << "\nDo you want to confirm checkout? (1=Yes, 0=No): ";
        cin >> confirm; cin.ignore();

        if (confirm != 1) {
            ListNode<CartItem>* temp = cart.get_items();
            while (temp) {
                inv.Update_Stock(temp->data.product->get_id(), temp->data.quantity);
                temp = temp->next;
            }
            cart.clear_cart();
            cout << "Checkout cancelled.\n";
            return;
        }

        loyalty.addPoints(cust, subtotal);

        temp = cart.get_items();
        while (temp) {
            temp->data.product->update_demand(temp->data.quantity);
            temp = temp->next;
        }

        Order newOrder(nextID++, cust->name, copied, totalDiscount);
        orders.enqueue(newOrder);
        cart.clear_cart();
        cout << "\nOrder placed successfully!\n";

        newOrder.print_bill();
    }
    void process_next_order(InventoryManager& inv) {
        if (orders.isEmpty()) {
            cout << "No pending orders.\n";
            return;
        }
        Order current = orders.front();
        orders.dequeue();
        cout << "\n=== PROCESSING ORDER :" << current.get_id() << " ===\n";
        cout << "Customer: " << current.get_customer() << endl;
        cout << "Estimated Time: " << current.get_processing_time() << " sec\n";

        // for Analytics
        history.enqueue(current);
        cout << "\nStock updated successfully.\n";
        cout << "Order Completed Successfully!\n";
    }
    void show_pending_orders() {
        if (orders.isEmpty()) {
            cout << "\nNo pending orders.\n";
            return;
        }
        cout << "\n=== PENDING ORDERS ===\n";
        Queue<Order> temp;
        while (!orders.isEmpty()) {
            Order o = orders.front();
            cout << "Order #" << o.get_id()
                << " | Customer: " << o.get_customer()
                << " | Time: " << o.get_processing_time() << " sec\n";
            temp.enqueue(o);
            orders.dequeue();
        }
        while (!temp.isEmpty()) {
            orders.enqueue(temp.front());
            temp.dequeue();
        }
    }
};
