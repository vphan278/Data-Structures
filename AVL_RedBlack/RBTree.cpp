#include <iostream>
using namespace std;

class Node {
public:
    int data;
    bool isRed;     // true = RED, false = BLACK
    Node* left;
    Node* right;
    Node* parent;

    Node(int value) {
        data = value;
        isRed = true;   // new nodes start RED
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

class RedBlackTree {
private:
    Node* root;

    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;

        if (y->left != nullptr)
            y->left->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;

        if (x->right != nullptr)
            x->right->parent = y;

        x->parent = y->parent;

        if (y->parent == nullptr)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;

        x->right = y;
        y->parent = x;
    }

    void fixInsert(Node* node) {
        while (node != root && node->parent->isRed) {
            Node* parent = node->parent;
            Node* grandparent = parent->parent;

            // Parent is left child of grandparent
            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;

                // Case 1: uncle is RED
                if (uncle != nullptr && uncle->isRed) {
                    parent->isRed = false;
                    uncle->isRed = false;
                    grandparent->isRed = true;
                    node = grandparent;
                }
                else {
                    // Case 2: Left-Right
                    if (node == parent->right) {
                        node = parent;
                        rotateLeft(node);
                        parent = node->parent;
                        grandparent = parent->parent;
                    }

                    // Case 3: Left-Left
                    parent->isRed = false;
                    grandparent->isRed = true;
                    rotateRight(grandparent);
                }
            }
            // Parent is right child of grandparent
            else {
                Node* uncle = grandparent->left;

                // Case 1: uncle is RED
                if (uncle != nullptr && uncle->isRed) {
                    parent->isRed = false;
                    uncle->isRed = false;
                    grandparent->isRed = true;
                    node = grandparent;
                }
                else {
                    // Case 2: Right-Left
                    if (node == parent->left) {
                        node = parent;
                        rotateRight(node);
                        parent = node->parent;
                        grandparent = parent->parent;
                    }

                    // Case 3: Right-Right
                    parent->isRed = false;
                    grandparent->isRed = true;
                    rotateLeft(grandparent);
                }
            }
        }

        root->isRed = false; // root must always be BLACK
    }

    void inorder(Node* node) {
        if (node == nullptr)
            return;

        inorder(node->left);
        cout << node->data << (node->isRed ? "(R) " : "(B) ");
        inorder(node->right);
    }

    void printTree(Node* node, int space) {
        if (node == nullptr)
            return;

        space += 8;
        printTree(node->right, space);

        cout << endl;
        for (int i = 8; i < space; i++)
            cout << " ";
        cout << node->data << (node->isRed ? "(R)" : "(B)") << endl;

        printTree(node->left, space);
    }

public:
    RedBlackTree() {
        root = nullptr;
    }

    void insert(int value) {
        Node* newNode = new Node(value);

        Node* y = nullptr;
        Node* x = root;

        // Normal BST insert
        while (x != nullptr) {
            y = x;
            if (newNode->data < x->data)
                x = x->left;
            else
                x = x->right;
        }

        newNode->parent = y;

        if (y == nullptr)
            root = newNode;
        else if (newNode->data < y->data)
            y->left = newNode;
        else
            y->right = newNode;

        // If root, make it black
        if (newNode == root) {
            newNode->isRed = false;
            return;
        }

        // Fix red-black violations
        fixInsert(newNode);
    }

    void inorderPrint() {
        inorder(root);
        cout << endl;
    }

    void display() {
        printTree(root, 0);
        cout << endl;
    }
};

int main() {
    RedBlackTree tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(15);
    tree.insert(25);
    tree.insert(5);

    cout << "Inorder: ";
    tree.inorderPrint();

    cout << "\nTree structure:\n";
    tree.display();

    return 0;
}