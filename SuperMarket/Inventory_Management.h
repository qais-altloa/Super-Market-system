#pragma once
#include <iostream>
#include "Templates.h"
#include <string>
using namespace std;

class TrieNode {
public:
    TrieNode* children[26];
    char c;
    bool isEnd;
    string originalWord;

    TrieNode(char c = ' ') {
        isEnd = false;
        this->c = c;
        originalWord = "";
        for (int i = 0; i < 26; i++)
            children[i] = nullptr;
    }
};
class Trie {
private:
    TrieNode* root;

    void dfs(TrieNode* node, string results[], int& count, int max_results) {
        if (count >= max_results) return;
        if (node->isEnd) {
            results[count++] = node->originalWord;
        }
        for (int i = 0; i < 26; i++) {
            if (node->children[i] != nullptr) {
                dfs(node->children[i], results, count, max_results);
            }
        }
    }

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const string& word) {
        TrieNode* curr = root;
        for (char c : word) {
            if (c == ' ') continue;
            int idx = tolower(c) - 'a';
            if (curr->children[idx] == nullptr)
                curr->children[idx] = new TrieNode(tolower(c));
            curr = curr->children[idx];
        }
        curr->isEnd = true;
        curr->originalWord = word;
    }

    int autocomplete(const string& prefix, string results[], int max_results) {
        TrieNode* curr = root;
        for (char c : prefix) {
            if (c == ' ') continue;
            int idx = tolower(c) - 'a';
            if (curr->children[idx] == nullptr)
                return 0;
            curr = curr->children[idx];
        }
        int count = 0;
        dfs(curr, results, count, max_results);
        return count;
    }
};

class Product {
private:
    int id;
    string name;
    string category;
    float price;
    int stock;
    int demand;
public:
    Product() {}
    Product(int id, string name, string category, float price, int stock) {
        this->id = id;
        this->category = category;
        this->name = name;
        this->price = price;
        this->stock = stock;
        this->demand = 0;
    }
    int get_id() { return id; }
    string get_name() { return name; }
    string get_category() { return category; }
    float get_price() { return price; }
    int get_stock() { return stock; }
    int get_demand() { return demand; }


    bool update_stock(int amount) {
        if (amount < 0) {
            int removeQ = -amount;
            if (removeQ > stock) {
                cout << "-- Not enough stock availble!" << endl;
                return false;
            }
        }
        stock += amount;
        return true;
    }

    void set_price(float i) { price = i; }

    void update_demand(int demand) {
        if (demand < 0) {
            cout << "-- Demand can not be negative!" << endl;
            return;
        }
        this->demand += demand;
    }

};
class InventoryManager {
private:
    DoublyLinkedList <Product> product_list;
    Tree <string> categories;
    Queue <Product> low_stock;
    Trie pt;
    void print_spaces(int n) {
        for (int i = 0; i < n; i++) cout << " ";
    }
public:

    void print_table_header() {
        cout << "ID      Name              Category            Price       Stock       Demand\n";
        cout << "--------------------------------------------------------------------------\n";
    }

    void print_product_row(Product& p) {

        cout << p.get_id();
        print_spaces(8 - to_string(p.get_id()).length());


        cout << p.get_name();
        int name_spaces = 18 - p.get_name().length();
        if (name_spaces < 1) name_spaces = 1;
        print_spaces(name_spaces);


        cout << p.get_category();
        int cat_spaces = 20 - p.get_category().length();
        if (cat_spaces < 1) cat_spaces = 1;
        print_spaces(cat_spaces);


        string priceStr = to_string(p.get_price());
        if (priceStr.length() > 6) priceStr = priceStr.substr(0, 6);
        cout << priceStr;
        print_spaces(12 - priceStr.length());


        string stockStr = to_string(p.get_stock());
        cout << p.get_stock();
        print_spaces(12 - stockStr.length());


        cout << p.get_demand() << "\n";
    }


    void searchProductByPrefix(const string& prefix) {
        const int MAX_RESULTS = 50;
        string results[MAX_RESULTS];
        int count = pt.autocomplete(prefix, results, MAX_RESULTS);
        if (count == 0) {
            cout << "not found\n";
            return;
        }
        bool x = false;


        for (int i = 0; i < count; i++) {
            Product* p = search_product(results[i]);
            if (p) {
                if (!x) {
                    print_table_header();
                    x = true;
                }
                print_product_row(*p);
            }
        }
        if (!x)cout << "not found\n";
    }



    void add_product(Product p) {
        product_list.insert_back(p);
        categories.insert(p.get_category());
        pt.insert(p.get_name());
    }
    bool remove_by_id(int id) {
        ListNode<Product>* temp = product_list.getHead();
        while (temp != NULL) {
            if (temp->data.get_id() == id) {
                product_list.remove_node(temp);
                cout << "\n-- Product removed successfully\n";
                return true;
            }
            temp = temp->next;
        }
        cout << "\n-- Product not found\n";
        return false;

    }
    Product* search_product(string name) {
        ListNode<Product>* temp = product_list.getHead();
        while (temp != NULL) {
            if (temp->data.get_name() == name) {
                return &(temp->data);
            }
            temp = temp->next;
        }
        return NULL;
    }
    Product* find_product(int id) {
        ListNode<Product>* temp = product_list.getHead();
        while (temp) {
            if (temp->data.get_id() == id) {
                return &(temp->data);
            }
            temp = temp->next;
        }
        cout << "\nProduct Not Found \n";
        return NULL;
    }
    void list_all_products() {
        ListNode<Product>* temp = product_list.getHead();
        while (temp) {
            print_product_row(temp->data);
            temp = temp->next;
        }
    }


    void print_products_in_category(const string& category) {
        ListNode<Product>* temp = product_list.getHead();
        bool printed_header = false;

        while (temp) {
            if (temp->data.get_category() == category) {
                if (!printed_header) { print_table_header(); printed_header = true; }
                print_product_row(temp->data);
            }
            temp = temp->next;
        }

        if (!printed_header) cout << "No products in this category.\n";
    }

    void list_products_by_category(Node<string>* root) {
        if (!root) return;

        list_products_by_category(root->left);

        cout << "\nCategory: " << root->data << "\n";
        cout << "-----------------------------\n";

        print_products_in_category(root->data);
        cout << "\t";

        list_products_by_category(root->right);
    }

    void list_products_by_category() {
        cout << "\n--- Products by Category ---\n";
        list_products_by_category(categories.get_root());
    }

    void generate_low_stock(int threshold = 10) {
        ListNode<Product>* temp = product_list.getHead();
        while (temp) {
            int s = temp->data.get_stock();
            if (s < threshold)
                low_stock.enqueuePriority(temp->data, s);
            temp = temp->next;
        }
    }
    void print_low_stock_info() {
        if (low_stock.isEmpty()) {
            cout << "\n No low_stock items.\n";
            return;
        }
        cout << "\n--- Low_Stock Items --- \n";
        while (!low_stock.isEmpty()) {
            Product p = low_stock.front();
            print_product_row(p);
            low_stock.dequeue();
        }
    }
    void Update_Stock(int product_id, int change) {
        ListNode<Product>* temp = product_list.getHead();
        while (temp) {
            if (temp->data.get_id() == product_id) {
                temp->data.update_stock(change);
                return;
            }
            temp = temp->next;
        }
    }

    int get_stock_of(int product_id) {
        ListNode<Product>* temp = product_list.getHead();
        while (temp) {
            if (temp->data.get_id() == product_id)
                return temp->data.get_stock();
            temp = temp->next;
        }
        return -1; // product not found
    }

    void search_by_category(const string& category) {
        ListNode<Product>* temp = product_list.getHead();
        bool found = false;
        while (temp) {
            if (temp->data.get_category() == category) {
                print_products_in_category(temp->data.get_category());
                found = true;
                break;
            }
            temp = temp->next;
        }
        if (!found)  cout << "\nNo products found in category: " << category << endl;


    }


    Node<string>* get_categories_root() { return categories.get_root(); }
    DoublyLinkedList<Product>& Product_list() { return product_list; }
};