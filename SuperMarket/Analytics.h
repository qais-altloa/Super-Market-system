#pragma once
#include<iostream>
using namespace std;
#include"Templates.h"
#include"Inventory_Management.h"
#include"OrderA.h"
#include "Customer_Cart.h"
#include "Promotion.h"
class Analytics {
private:
	InventoryManager* inv;
	Checkout* checkout;
public:
	Analytics(InventoryManager* i,Checkout * c):inv(i),checkout(c){}
	float total_revenue() {
		float total = 0;
		Queue<Order> temp;
		Queue<Order>& hist = checkout->get_history();
		while (!hist.isEmpty()) {
			Order o = hist.front();
			hist.dequeue();
			total += o.get_total();
			temp.enqueue(o);
		}
		while (!temp.isEmpty()) {
			hist.enqueue(temp.front());
			temp.dequeue();
		}
		return total;
	}
	int orders_count() {
		return checkout->get_history().size();
	}
	void best_selling_products() {
		cout << "\n=== BEST SELLING PRODUCTS ===\n";

		const int MAX = 500;
		int ids[MAX];
		int qty[MAX];
		int used = 0;
		Queue<Order> temp;
		Queue<Order>& hist = checkout->get_history();
		while (!hist.isEmpty()) {
			Order o = hist.front();
			hist.dequeue();
			temp.enqueue(o);
			ListNode<CartItem>* it = o.get_items();
			while (it) {
				int id = it->data.product->get_id();
				int q = it->data.quantity;
				bool found = false;
				for (int i = 0; i < used; i++) {
					if (ids[i] == id) {
						qty[i] += q;
						found = true;
						break;
					}
					
				}
				if (!found) {
					ids[used] = id;
					qty[used] = q;
					used++;
				}
				it = it->next;
			}
			
		}
		while (!temp.isEmpty()) {
			hist.enqueue(temp.front());
			temp.dequeue();
		}
		for (int i = 0; i < used; i++) {
			cout << "Product ID " << ids[i] << " sold " << qty[i] << " units.\n";
		}
	}
	void popular_categories() {
		cout << "\n=== POPULAR CATEGORIES === \n";
		const int MAX = 100;
		string cat[MAX];
		int count[MAX];
		int used = 0;
		Queue<Order>temp;
		Queue<Order>& hist = checkout->get_history();
		while (!hist.isEmpty()) {
			Order o = hist.front();
			hist.dequeue();
			temp.enqueue(o);
			ListNode<CartItem>* it = o.get_items();
			string c = it->data.product->get_category();
			int q = it->data.quantity;
			bool found = false;
			for (int i = 0; i < used; i++) {
				if (cat[i] == c) {
					count[i] += q;
					found = true;
					break;
				}
			}
			if (!found) {
				cat[used] = c;
				count[used] = q;
				used++;
			}
		}
		while (!temp.isEmpty()) {
			hist.enqueue(temp.front());
			temp.dequeue();
		}
		for (int i = 0; i < used; i++) {
			cout << cat[i] << " sold " << count[i] << " units. \n";
		}
	}
	float inventory_value() {
		float sum = 0;
		ListNode<Product>* temp = inv->Product_list().getHead();
		while (temp) {
			sum += temp->data.get_stock();
			temp = temp->next;
		}
		return sum;
	}
	void print_report() {
		cout << "\n========== ANALYTICS REPORT ==========\n";
		cout << "Total Orders: " << orders_count() << "\n";
		cout << "Total Revenue: " << total_revenue() << "$\n";
		cout << "Inventory Value: " << inventory_value() << "$\n";
		
		best_selling_products();
		popular_categories();
		cout << "==========================================\n";

	}
};