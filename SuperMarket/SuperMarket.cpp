
#include <iostream>
#include "Inventory_Management.h"
#include "Customer_Cart.h"
#include "User_Admin.h"
#include "Analytics.h"
#include <string>
#include"OrderA.h"
#include "Promotion.h"
#include "Loyalty.h"

using namespace std;

/*
========================================================
             SUPERMARKET MANAGEMENT SYSTEM
========================================================

=== INITIAL DATA ===

-- Admin Accounts --
1) Username: Ahmed Tamer   Password: ahmed1574
2) Username: Omar Tamer    Password: ahmed1574
3) Username: Mohamed Tamer Password: ahmed1574

-- Loyalty Customers --
ID: 101  Name: AhmedTamer       Points: 2000  Level: PLATINUM
ID: 102  Name: OmarAli          Points: 500   Level: SILVER
ID: 103  Name: YousefAhmed      Points: 300   Level: NORMAL

-- Promo Codes (Discount Amounts) --
1) SAVE20       -> 20.0$
2) Ahmed        -> 50.0$
3) BlackFriday  -> 30.0$
4) NEWYEAR      -> 25.0$

=== USAGE INSTRUCTIONS ===

1) Admins can login using their username & password.
2) Customers must use their ID to login if already registered.
3) New customers will be assigned a new unique ID automatically.
4) Loyalty points:
   - PLATINUM: 30% discount
   - GOLD: 20% discount
   - SILVER: 10% discount
   - NORMAL: 0% discount
5) Minimum discount applied per order is 60% of subtotal (if promo + loyalty < 60%).
6) Promo codes can be applied up to 3 times per checkout attempt.
7) Customers can view products, add/remove items, view cart, and checkout.
8) On checkout confirmation, loyalty points are added, and order is processed.
9) All customer IDs are unique and must be used next time for login.


========================================================
*/

admin admins[3];
void initializeSystemData(InventoryManager& inv, LoyaltyProgram& loyalty, promotion& promo) {
    // ===== Add Products =====
    inv.add_product(Product(101, "Milk", "Dairy", 2.50, 5));
    inv.add_product(Product(102, "Bread", "Bakery", 1.75, 12));
    inv.add_product(Product(103, "Eggs", "Dairy", 3.20, 20));
    inv.add_product(Product(104, "Sugar", "Grocery", 1.10, 8));
    inv.add_product(Product(105, "Rice", "Grocery", 4.50, 3));
    inv.add_product(Product(106, "Chicken", "Meat", 6.99, 15));
    inv.add_product(Product(107, "Apple Juice", "Beverages", 2.20, 2));
    inv.add_product(Product(108, "Chocolate", "Snacks", 1.00, 25));
    inv.add_product(Product(109, "Tea", "Beverages", 1.40, 6));
    inv.add_product(Product(110, "Chips", "Snacks", 0.80, 30));
    inv.add_product(Product(135, "Frozen Pizza", "Frozen", 44.50, 10));
    inv.add_product(Product(136, "Ice Cream", "Frozen", 10.75, 16));
    inv.add_product(Product(137, "Frozen Fries", "Frozen", 85.90, 14));
    inv.add_product(Product(132, "Tomatoes", "Vegetables", 20.90, 25));
    inv.add_product(Product(133, "Potatoes", "Vegetables", 15.40, 30));
    inv.add_product(Product(134, "Onions", "Vegetables", 6.20, 28));
    inv.add_product(Product(111, "Cheese", "Dairy", 30.75, 10));
    inv.add_product(Product(112, "Butter", "Dairy", 60.60, 8));
    inv.add_product(Product(120, "Beef", "Meat", 190.50, 7));
    inv.add_product(Product(121, "Fish", "Meat", 77.80, 9));
    inv.add_product(Product(124, "Cola", "Beverages", 17.10, 50));
    inv.add_product(Product(125, "Water", "Beverages", 0.50, 100));


    // ===== Add Admins =====
    admins[0].setusername("Ahmed Tamer"); admins[0].setpassword("ahmed1574");
    admins[1].setusername("Omar Tamer"); admins[1].setpassword("ahmed1574");
    admins[2].setusername("Mohamed Tamer"); admins[2].setpassword("ahmed1574");

    // ===== Add Customers =====
    Customer* c1 = loyalty.addCustomer("Ahmed Tamer");
    c1->points = 2000;
    Customer* c2 = loyalty.addCustomer("Omar Ali");
    c2->points = 500;
    Customer* c3 = loyalty.addCustomer("Yousef Ahmed");
    c3->points = 300;

    // ===== Add Promo Codes =====
    promo.addPromocode("SAVE20", 20);
    promo.addPromocode("Ahmed", 50);
    promo.addPromocode("BlackFriday", 30);
    promo.addPromocode("NEWYEAR", 25);
}

void cleanword(string& cat) {
    for (int i = 0; i < cat.size(); i++) {
        if (i == 0 || cat[i - 1] == ' ') {
            cat[i] = toupper(cat[i]);
        }
        else {
            cat[i] = tolower(cat[i]);
        }
    }
}
int main() {
    InventoryManager inv;
    Checkout checkout;
    LoyaltyProgram loyalty;
    Analytics report(&inv, &checkout);
    promotion p(&inv);

    initializeSystemData(inv, loyalty, p);
    int choice;
    do {
        cout << "\n=====================================" << endl;
        cout << " SUPERMARKET MANAGEMENT SYSTEM" << endl;
        cout << "=====================================" << endl;
        cout << "Login as:\n1) Admin\n2) Customer\n3) Exit\n";
        cout << "Choose: ";
        cin >> choice;

        if (choice == 1) { // Admin Login
            cin.ignore();
            string username, password;
            cout << "Enter username: ";
            getline(cin, username);
            cout << "Enter password: ";
            cin >> password;

            bool found = false;
            int index = -1;
            for (int i = 0; i < 3; i++) {
                if (admins[i].logain(username, password)) {
                    found = true;
                    index = i;
                    break;
                }
            }

            if (!found) {
                cout << "Wrong username or password!\n";
                continue;
            }

            cout << "Welcome, ";
            admins[index].displayrole();

            int option;
            do {
                cout << "\n===== ADMIN MENU =====\n";
                cout << "1) Add Product\n";
                cout << "2) Update Stock\n";
                cout << "3) List All Products\n";
                cout << "4) Check Low Stock\n";
                cout << "5) Search Product by Name\n";
                cout << "6) Search Product by ID\n";
                cout << "7) Remove Product by ID\n";
                cout << "8) List Categories\n";
                cout << "9) Process Next Order\n";
                cout << "10) Show Pending Orders\n";
                cout << "11) Show Analytics Report\n";
                cout << "12) add promo code \n";
                cout << "13) apply seasonal discount to all categorys\n";
                cout << "14) apply seasonal discount to specific category\n";
                cout << "15) Search by categorys\n";
                cout << "16) search Product By Prefix\n";

                cout << "0) Logout\n";
                cout << "Choose: ";
                cin >> option;

                if (option == 1) {
                    cin.ignore();
                    string name, category;
                    int id, stock;
                    float price;
                    cout << "Enter product name: "; getline(cin, name);
                    cleanword(name);
                    cout << "Enter category: "; getline(cin, category);
                    cleanword(category);
                    cout << "Enter id: "; cin >> id;
                    cout << "Enter price: "; cin >> price;
                    cout << "Enter quantity: "; cin >> stock;
                    Product p(id, name, category, price, stock);
                    inv.add_product(p);
                    cout << "\n-- Product added successfully.\n";
                }
                else if (option == 2) {
                    int id, stock;
                    cout << "Enter product ID: "; cin >> id;
                    cout << "Enter new stock quantity: "; cin >> stock;
                    Product* p = inv.find_product(id);
                    if (p) {
                        p->update_stock(stock - p->get_stock());
                        cout << "Stock updated.\n";
                    }
                    else {
                        cout << "Product not found.\n";
                    }
                }
                else if (option == 3) {
                    inv.print_table_header();
                    inv.list_all_products();
                }
                else if (option == 4) {
                    inv.generate_low_stock();
                    inv.print_low_stock_info();
                }
                else if (option == 5) {
                    cin.ignore();
                    string name; cout << "Enter product name: "; getline(cin, name);
                    cleanword(name);

                    Product* p = inv.search_product(name);
                    if (p)inv.print_product_row(*p);
                    else cout << "Product not found.\n";
                }
                else if (option == 6) {
                    int id; cout << "Enter product ID: "; cin >> id;
                    Product* p = inv.find_product(id);
                    if (p) inv.print_product_row(*p);

                }
                else if (option == 7) {
                    int id; cout << "Enter product ID to remove: "; cin >> id;
                    inv.remove_by_id(id);
                }
                else if (option == 8) inv.list_products_by_category();
                else if (option == 9) checkout.process_next_order(inv);
                else if (option == 10) checkout.show_pending_orders();
                // Analytics to work use customer checkout and then pending 10  9 process then report
                else if (option == 11) {
                    report.print_report();
                }
                else if (option == 12) {
                    cin.ignore();
                    string code; float discount;
                    cout << "Enter promocode: "; getline(cin, code);
                    cout << "Enter discount percent: "; cin >> discount;
                    p.addPromocode(code, discount);
                    cout << "Promocode added successfully!\n";
                }

                else if (option == 13) {
                    float discountPercent;
                    cout << "Enter seasonal discount percent: ";
                    cin >> discountPercent;
                    float discountRate;
                    if (discountPercent > 1)discountRate = discountPercent / 100.0f;
                    else discountRate = discountPercent;
                    p.applySeasonalDiscountBST(discountRate);
                    cout << "Seasonal discount applied successfully!\n";
                }

                else if (option == 14) {
                    cin.ignore();
                    string category;
                    float discountPercent;
                    cout << "Enter category name: "; getline(cin, category);
                    cout << "Enter discount percent: "; cin >> discountPercent;
                    cleanword(category);
                    float discountRate;
                    if (discountPercent > 1)discountRate = discountPercent / 100.0f;
                    else discountRate = discountPercent;

                    p.applySeasonalDiscountToCategory(category, discountRate);
                    cout << "Discount applied to category " << category << " successfully!\n";
                }

                else if (option == 15) {
                    cin.ignore();
                    string cat;
                    cout << "Enter category name: ";
                    getline(cin, cat);
                    cleanword(cat);

                    inv.search_by_category(cat);

                }
                else if (option == 16) {
                    cin.ignore();
                    string pre;
                    cout << "Enter prefix name: ";
                    getline(cin, pre);

                    inv.searchProductByPrefix(pre);

                }



                else if (option == 0) cout << "Logging out...\n";
                else cout << "Invalid option!\n";

            } while (option != 0);
        }

        else if (choice == 2) {
            // ===== Customer Mode =====
            p.dynamicPricing();
            cin.ignore();

            Customer* currentCustomer = nullptr;
            int custChoice;


            do {
                cout << "\nAre you an existing customer or a new customer?\n";
                cout << "1) Existing Customer\n2) New Customer\nChoose: ";
                cin >> custChoice;
                cin.ignore();

                if (custChoice == 1) {
                    int id;
                    cout << "Enter your Customer ID: ";
                    cin >> id;
                    cin.ignore();
                    currentCustomer = loyalty.findCustomer(id);

                    if (!currentCustomer) {
                        cout << "Customer ID not found! Please try again.\n";
                    }
                    else {
                        currentCustomer->displayInfo();
                    }
                }
                else if (custChoice == 2) {
                    string name;
                    cout << "Enter your name: ";
                    getline(cin, name);
                    currentCustomer = loyalty.addCustomer(name);
                    cout << "Welcome " << name << "! Your Customer ID is: " << currentCustomer->id << endl;
                    currentCustomer->displayInfo();
                }
                else {
                    cout << "Invalid choice. Please choose 1 or 2.\n";
                }
            } while (!currentCustomer);

            ShoppingCart cart(&inv);
            int option;
            do {
                cout << "\n===== CUSTOMER MENU =====\n";
                cout << "1) View Products\n";
                cout << "2) Add Item to Cart\n";
                cout << "3) Remove Last Item\n";
                cout << "4) View Cart\n";
                cout << "5) Checkout\n";
                cout << "6)Search by categorys\n";
                cout << "7)search by prefix\n";
                cout << "0) Exit\n";
                cout << "Choose: ";
                cin >> option;

                if (option == 1) {
                    inv.print_table_header();
                    inv.list_all_products();
                }
                else if (option == 2) {
                    cin.ignore();
                    string name; int qty;
                    cout << "Enter product name: "; getline(cin, name);
                    cout << "Enter quantity: "; cin >> qty;
                    cleanword(name);

                    Product* p = inv.search_product(name);
                    if (p) cart.add_item(p, qty);
                    else cout << "Product not found!\n";
                }
                else if (option == 3) cart.undo_last();
                else if (option == 4) cart.print_cart();
                else if (option == 5) {
                    cart.print_cart();
                    if (!cart.get_items()) {
                        cout << "Cart is empty! Cannot checkout.\n";
                        continue;
                    }

                    int confirm;
                    cout << "\nProceed with checkout? (1=Yes, 0=No): ";
                    cin >> confirm;
                    cin.ignore();

                    if (confirm == 1) {
                        checkout.add_order(currentCustomer, cart, &p, loyalty, inv);
                    }
                }
                else if (option == 6) {
                    cin.ignore();
                    string cat;
                    cout << "Enter category name: ";
                    getline(cin, cat);
                    cleanword(cat);

                    inv.search_by_category(cat);
                }
                else if (option == 7) {
                    cin.ignore();
                    string pre;
                    cout << "Enter prefix name: ";
                    getline(cin, pre);

                    inv.searchProductByPrefix(pre);

                }
                else if (option == 0) cout << "Thank you for visiting!\n";
                else cout << "Invalid option!\n";

            } while (option != 0);
        }

        else if (choice == 3) {
            cout << "Thank you for using Supermarket Management System!\n";
            break;
        }

        else cout << "Invalid choice. Please try again.\n";

    } while (choice != 3);



    return 0;
}