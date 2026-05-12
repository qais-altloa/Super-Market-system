#pragma once
#include "Templates.h"
#include <iostream>
#include "Inventory_Management.h"

using namespace std;

class CartItem {
public:
	Product *product;
	int quantity;
	CartItem() :quantity(0){}
	CartItem(Product *p, int q = 1):product(p),quantity(q){}
	float get_total(){ return product->get_price() * quantity; }
	void print() {
		cout << endl << product->get_name() << " X " << quantity  << " = " << get_total()<< endl;
	}
};
class ShoppingCart {
private:
	DoublyLinkedList<CartItem> items;  // Stores Cart Items
	Stack<CartItem> undo_stack;       // Last actions
	InventoryManager* inv;
public:
	ShoppingCart(){}
	ShoppingCart(InventoryManager * inventory): inv(inventory){}
	void add_item(Product *p, int qty = 1) {
		int available = inv->get_stock_of(p->get_id());
		if (available < qty) {
			cout << "\nERROR: Only " << available << " items available in inventory. \n";
			return;
		}
		ListNode<CartItem>* temp = items.getHead();
		// check first if it exits;
		while (temp) {
			if (temp->data.product->get_id() == p->get_id()) {
				if (temp->data.quantity + qty > available) {
					cout << "\nERROR: Only " << available << " items available in inventory. \n";
					return;
				}
				temp->data.quantity += qty;
				
				undo_stack.push(CartItem(p, qty));
				cout << "\nUpdated quantity of " << p->get_name() << ".\n";
				return;
			}
			temp = temp->next;
		}
		// otherwise
		CartItem c(p, qty);
		items.insert_back(c);
		inv->Update_Stock(p->get_id(),-qty);
		undo_stack.push(c);
		cout << "\n--Added " << p->get_name() << " to cart. \n";
		return;
	}
	void undo_last() {
		if (undo_stack.isEmpty()) 
		{
			cout << "\nNothing To Undo \n";
			return;
		}
		CartItem last = undo_stack.top();
		undo_stack.pop();
		int id = last.product->get_id();
		int qty = last.quantity;
		inv->Update_Stock(id, qty);
		ListNode<CartItem>* temp = items.getHead();
		while (temp) {
			if (temp->data.product->get_id() == last.product->get_id()) {
				if (temp->data.quantity == last.quantity) items.remove_node(temp);
				else temp->data.quantity -= last.quantity;
				cout << "\nUndo successful \n";
				return;
			}
			temp = temp->next;
		}

	}
	void print_cart() {
		cout << "\n=== Your Cart ===\n";
		
		ListNode<CartItem>* temp = items.getHead();

		if (!temp) {
			cout << "\nCart is Empty \n";
			return;
		}
		float total = 0;
		while (temp) {
			temp->data.print();
			total += temp->data.get_total();
			temp = temp->next;
		}
		cout << "----------------------------\n";
		cout << "Subtotal: " << total << endl;
		float tax = total * 0.05;
		cout << "Tax (5%): " << tax << endl;
	}
	

	// OrderA
	ListNode<CartItem>* get_items() {
		return items.getHead();
	}

	void clear_cart() {
		while (!items.isEmpty())items.delete_front();
		while (!undo_stack.isEmpty())undo_stack.pop();
	}
};
