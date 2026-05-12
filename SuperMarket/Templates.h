#pragma once
#include <iostream>
using namespace std;

// -------------------------
//   List Node
// -------------------------
template <class T>
struct ListNode {
    T data;
    ListNode<T>* next;
    ListNode<T>* prev;
    int Priority;
    ListNode(T val) : data(val), next(nullptr), prev(nullptr) {}

};

// -------------------------
//   Doubly Linked List
// -------------------------
template <class T>
class DoublyLinkedList {
private:
    ListNode<T>* head;
    ListNode<T>* tail;
    int count;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    bool isEmpty() { return head == nullptr; }
    int size() { return count; }

    void insert_front(T val) {
        ListNode<T>* temp = new ListNode<T>(val);
        if (isEmpty()) {
            head = tail = temp;
        }
        else {
            temp->next = head;
            head->prev = temp;
            head = temp;
        }
        count++;
    }

    void insert_back(T val) {
        ListNode<T>* temp = new ListNode<T>(val);
        if (isEmpty()) {
            head = tail = temp;
        }
        else {
            tail->next = temp;
            temp->prev = tail;
            tail = temp;
        }
        count++;
    }

    void delete_front() {
        if (isEmpty()) {
            cout << "List is empty\n";
            return;
        }
        ListNode<T>* temp = head;

        if (head == tail)
            head = tail = nullptr;
        else {
            head = head->next;
            head->prev = nullptr;
        }

        delete temp;
        count--;
    }

    void delete_back() {
        if (isEmpty()) {
            cout << "List is empty\n";
            return;
        }

        ListNode<T>* temp = tail;

        if (head == tail)
            head = tail = nullptr;
        else {
            tail = tail->prev;
            tail->next = nullptr;
        }

        delete temp;
        count--;
    }

    bool delete_value(T val) {
        if (isEmpty()) return false;

        ListNode<T>* cur = head;
        while (cur && cur->data != val)
            cur = cur->next;

        if (!cur) return false;

        if (cur == head) delete_front();
        else if (cur == tail) delete_back();
        else {
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;
            delete cur;
            count--;
        }
        return true;
    }

    void clear() {
        ListNode<T>* temp = head;
        while (temp != NULL) {
            ListNode<T>* nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }
        head = NULL;
        tail = NULL;
        count = 0;
    }

    bool search(T val) {
        ListNode<T>* cur = head;
        while (cur) {
            if (cur->data == val) return true;
            cur = cur->next;
        }
        return false;
    }

    void print_forward() {
        ListNode<T>* cur = head;
        while (cur) {
            cout << cur->data << " ";
            cur = cur->next;
        }
        cout << endl;
    }

    void print_backward() {
        ListNode<T>* cur = tail;
        while (cur) {
            cout << cur->data << " ";
            cur = cur->prev;
        }
        cout << endl;
    }

    void remove_node(ListNode<T>* node) {
        if (!node) return;

        // Removing head
        if (node == head) {
            head = head->next;
            if (head) head->prev = nullptr;
            delete node;
            return;
        }

        // Removing tail
        if (node == tail) {
            tail = tail->prev;
            tail->next = nullptr;
            delete node;
            return;
        }

        // Removing middle
        node->prev->next = node->next;
        node->next->prev = node->prev;
        delete node;
    }

    // For Stack/Queue access
    ListNode<T>* getHead() { return head; }
    ListNode<T>* getTail() { return tail; }
};

// -------------------------
//            Stack
// -------------------------
template <class T>
class Stack {
private:
    DoublyLinkedList<T> list;

public:
    void push(T val) {
        list.insert_back(val);
    }

    void pop() {
        if (list.isEmpty()) {
            cout << "Stack is empty\n";
            return;
        }
        list.delete_back();
    }

    T top() {
        if (list.isEmpty()) {
            cout << "Stack is empty\n";
            return T();
        }
        return list.getTail()->data;
    }

    bool isEmpty() { return list.isEmpty(); }
    int size() { return list.size(); }
};

// -------------------------
//            Queue
// -------------------------
template <class T>
class Queue {
private:
    DoublyLinkedList<T> list;

public:
    void enqueue(T val) {
        list.insert_back(val);
    }
    
    void dequeue() {
        if (list.isEmpty()) {
            cout << "Queue is empty\n";
            return;
        }
        list.delete_front();
    }

    T front() {
        if (list.isEmpty()) {
            cout << "Queue is empty\n";
            return T();
        }
        return list.getHead()->data;
    }

    bool isEmpty() { return list.isEmpty(); }
    int size() { return list.size(); }
    
    void enqueuePriority(T val, int priority) {
        // Create new node
        ListNode<T>* newNode = new ListNode<T>(val);
        newNode->Priority = priority;

        // Case 1: empty list
        if (list.isEmpty()) {
            list.insert_back(val);
            list.getTail()->Priority = priority;
            return;
        }

        // Case 2: find correct position
        ListNode<T>* current = list.getHead();
        while (current != nullptr && current->Priority <= priority) {
            current = current->next;
        }

        if (current == nullptr) {
            // Insert at the end
            list.insert_back(val);
            list.getTail()->Priority = priority;
        }
        else if (current == list.getHead()) {
            // Insert at the front
            list.insert_front(val);
            list.getHead()->Priority = priority;
        }
        else {
            // Insert before current manually
            ListNode<T>* prevNode = current->prev;
            prevNode->next = newNode;
            newNode->prev = prevNode;
            newNode->next = current;
            current->prev = newNode;

            // Manually increment count since we bypassed insert_front/back
            // (your DoublyLinkedList has private count, so you may add a helper if needed)
        }
    }

};

// Tree
template<class T>
struct Node {
    Node<T>* right;
    Node<T>* left;
    T data;
    Node() :right(NULL), left(NULL) {}
    Node(T data) :data(data), right(NULL), left(NULL) {}
    Node(T data, Node<T>* right, Node<T>* left) :data(data), right(right), left(left) {}
};
template<class T>
class Tree {
private:
    Node<T>* root;
    void get_insert(Node<T>*& root, T val) {
        if (!root) {
            root = new Node<T>(val);
            return;
        }
        else if (root->data > val) get_insert(root->left, val);
        else if (root->data < val) get_insert(root->right, val);
    }
    void get_inorder(Node<T>* root) {
        if (!root)return;
        get_inorder(root->left);
        cout << root->data << " ";
        get_inorder(root->right);
    }
    Node<T>* find_min(Node<T>* root) {
        if (!root) return NULL;
        if (root->left == NULL)return root;
        return find_min(root->left);
    }
    void get_deleted(Node<T>*& root, T val) {
        if (!root) return;
        else if (val < root->data) get_deleted(root->left, val);
        else if (val > root->data) get_deleted(root->right, val);
        else {
            if (!root->left && !root->right) {
                delete root;
                root = NULL;
            }
            else if (root->left == NULL) {
                Node<T>* temp = root;
                root = root->right;
                delete temp;
            }
            else if (root->right == NULL) {
                Node<T>* temp = root;
                root = root->left;
                delete temp;
            }
            else {
                Node<T>* temp = find_min(root->right);
                root->data = temp->data;
                get_deleted(root->right, temp->data);
            }
        }
    }
    Node<T>* search(Node<T>* root, T val) {
        if (!root) return NULL;

        if (val == root->data) return root;
        else if (val < root->data) return search(root->left, val);
        else return search(root->right, val);
    }
public:
    Tree() :root(NULL) {}
    Node<T>* get_root(){return root; }
    void insert(T val) {
        get_insert(root, val);
    }
    void Breadth_print() {
        if (!root) return;

        Queue<Node<T>*> my_queue;
        my_queue.enqueue(root);

        while (!my_queue.isEmpty()) {
            Node<T>* current = my_queue.front();
            my_queue.dequeue();

            cout << current->data << " ";

            if (current->left != NULL)
                my_queue.enqueue(current->left);
            if (current->right != NULL)
                my_queue.enqueue(current->right);
        }
    }
    void Inorder_print() {
        get_inorder(root);
    }
    void Delete(T val) {
        get_deleted(root, val);
    }
    
    Node<T>* search(T val) {
        return search(root, val);
    }
};
