#pragma once
#include<iostream>
#include "Templates.h"
#include"Inventory_Management.h"
using namespace std;

struct PromoCode {
    string code;
    float discount;
    PromoCode* next;
    PromoCode(string c = "", float d = 0) {
        code = c;
        discount = d;
        next = NULL;
    }
};
class promotion {
    InventoryManager* inv;
    PromoCode* P_head;
    Queue<Product> dynamicpricing;
public:
    promotion(InventoryManager* iinn) {
        inv = iinn;
        P_head = NULL;
    }

    void addPromocode(string c, int d) {
        PromoCode* p = new PromoCode(c, d);
        if (!P_head)P_head = p;
        else {
            p->next = P_head;
            P_head = p;
        }
    }

    float applyPromocode(string c, float subtotal) {
        PromoCode* temp = P_head;
        float discount = 0.0;
        while (temp) {
            if (temp->code == c) {
                discount = subtotal * (temp->discount / 100.0f);
                return discount;

            }
            temp = temp->next;
        }
        cout << "Promocode is not found\n";
        return discount;
    }

    void dynamicPricing() {
        ListNode<Product>* temp = inv->Product_list().getHead();
        while (temp != NULL) {
            int x = temp->data.get_demand();
            dynamicpricing.enqueuePriority(temp->data, x);
            temp = temp->next;

        }

        while (!dynamicpricing.isEmpty()) {
            Product p = dynamicpricing.front();
            Product* pp = inv->find_product(p.get_id());
            dynamicpricing.dequeue();
            int demand = pp->get_demand();

            float increase = 0;
            if (demand >= 20) increase = 0.20f;
            else if (demand >= 10) increase = 0.10f;
            else if (demand >= 5) increase = -0.05f;
            else increase = -0.01f;


            float price = pp->get_price();
            float newPrice = price * (1 + increase);
            pp->set_price(newPrice);

        }
    }


    // ---------------------------------------
    // Seasonal Discounts using BST  in all  |
    // ---------------------------------------
    void applySeasonalDiscountBST(float discountPercent) {
        applySeasonalDiscountBST(inv->get_categories_root(), discountPercent);
    }

    void applySeasonalDiscountBST(Node<string>* root, float discountPercent) {
        if (!root) return;

        ListNode<Product>* temp = inv->Product_list().getHead();
        while (temp) {
            if (temp->data.get_category() == root->data) {
                float price = temp->data.get_price();
                float newPrice = price * (1 - discountPercent);
                temp->data.set_price(newPrice);
            }
            temp = temp->next;
        }
        applySeasonalDiscountBST(root->left, discountPercent);
        applySeasonalDiscountBST(root->right, discountPercent);
    }

    void applySeasonalDiscountToCategory(const string& category, float discountPercent) {
        ListNode<Product>* temp = inv->Product_list().getHead();
        while (temp) {
            if (temp->data.get_category() == category) {
                float price = temp->data.get_price();
                float newPrice = price * (1 - discountPercent);
                temp->data.set_price(newPrice);
            }
            temp = temp->next;
        }
    }


};