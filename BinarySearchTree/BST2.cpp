#include <iostream>
using namespace std;

//SHOW, INSERT, DELETE


class Node {
public:
    int value;
    Node* left;
    Node* right;

    Node(int value) {
        this->value = value;
        left = nullptr;
        right = nullptr;
    }
};

//Display the tree
void show(Node* root) {  // sorted order (verifies BST)
    if (root == nullptr) return;
    
    cout << root->value << " ";
    show(root->left);
    show(root->right);
}

// INSERT into BST (prints insertion order)
Node* insert(Node* root, int value) {

    // Empty spot found → INSERT
    if (root == nullptr) {
        cout << value << " ";   // PRINT INSERTION ORDER
        return new Node(value);
    }

    if (value < root->value) {
        root->left = insert(root->left, value);
    }
    else if (value > root->value) {
        root->right = insert(root->right, value);
    }

    return root;
}

//Part of DELETE
Node* findMin(Node* root) {
    while (root->left != nullptr) {
        root = root->left;
    }
    return root;
}




//DELETE from BST
Node *deleteNode(Node *root, int value){

    if (root == nullptr) return root; 
    
    //Step 1: search for the node
    if(value < root->value){
        root->left = deleteNode(root->left, value);
    }
    else if(value >root->value){
        root->right = deleteNode(root->right, value);
    }
    else{

        //Step 2: Node found
        //Case 1: no left child
        if(root->left == nullptr){
            Node *temp = root->right;
            delete root;
            return temp;
        }
        //Case 2: no right child
        else if(root->right ==nullptr){
            Node *temp = root->left;
            delete root;
            return temp;
        }

        //Case 3: two children
        Node *temp = findMin(root->right); //in-order successor
        root->value = temp->value; //replace value
        root->right = deleteNode(root->right, temp->value);
    }

    return root;

}



int main() {

    //INSERT
    Node* root = nullptr;

    cout << "BST: Root + insertion order =  ";

    root = insert(root, 50);   // root prints first
    insert(root, 30);
    insert(root, 70);
    insert(root, 20);
    insert(root, 40);
    insert(root, 60);
    insert(root, 80);

    cout << endl;


    //Delete 
    root = deleteNode(root, 70);
    root = deleteNode(root,30);

    cout << "BST after delete: ";
    show(root);
    cout<<endl;


    return 0;
}