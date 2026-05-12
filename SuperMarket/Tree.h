
#include<iostream>
using namespace std;
#include "Templates.h"
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
public:
    Tree() :root(NULL) {}
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
};
