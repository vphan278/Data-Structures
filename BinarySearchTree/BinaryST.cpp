#include <iostream>
using namespace std;

class Node{

    private:
        int value;
        Node *parent;
        Node *left;
        Node *right;


    public:
        //Constructor
        explicit Node(int val): value(val), parent(nullptr), left(nullptr), right(nullptr){}

        //Getters
        int getValue() const{
            return value;
        }
        Node *getParent() const{
            return parent;
        }
        Node *getLeft() const{
            return left;
        }
        Node *getRight() const{
            return right;
        }

        //Setters
        void setParent(Node *p){
            parent = p;
        }
        void setLeft(Node *l){
            left = l;
        }
        void setRight(Node *r){
            right = r;
        }
};

class BST {
    private:
        Node* root;

        void inorder(Node* node) const {
            if (node == nullptr) return;
            inorder(node->getLeft());
            cout << node->getValue() << " ";
            inorder(node->getRight());
        }

    public:
        BST() : root(nullptr) {}

        Node* getRoot() const { 
            return root; 
        }

        void insert(int value) {
            Node* newNode = new Node(value);

            // empty tree
            if (root == nullptr) {
                root = newNode;
                return;
            }

            Node* current = root;
            Node* parent  = nullptr;

            // walk down until we hit a null child
            while (current != nullptr) {
                parent = current;

                if (value < current->getValue())
                    current = current->getLeft();
                else if (value > current->getValue())
                    current = current->getRight();
                else {
                    // duplicate: do nothing (avoid memory leak)
                    delete newNode;
                    return;
                }
            }

            // attach
            newNode->setParent(parent);

            if (value < parent->getValue())
                parent->setLeft(newNode);
            else
                parent->setRight(newNode);
        }

    void printInOrder() const {
        inorder(root);
        cout << endl;
    }
};

int main(){
    BST tree;

    int values[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};
    for(int v: values) tree.insert(v);

    cout << "In-order traversal: ";
    tree.printInOrder();

    return 0;
}

//Binary Search Tree
//         8
//       /   \ 
//      3     10
//     / \      \ 
//    1   6      14
//       / \     /
//      4   7   13


// But the In-order traversal: 1 3 4 6 7 8 10 13 14