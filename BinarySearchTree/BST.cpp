#include <iostream>
using namespace std;

//Show and INSERT

class Node{

    public:
        int value;
        Node *parent;
        Node *left;
        Node *right;

        Node(int value){
            this->value = value;
            left = nullptr;
            right = nullptr;
        }

};


//Traversal function ..putting the tree in order  ex. 5 10 15
void showinorder(Node *root){

    if(root ==nullptr)
        return;

    showinorder(root->left);
    cout << root->value << " "; // 5 10 15
    showinorder(root->right);
}

//INSERT function
Node *insert(Node *root2, int value){

    if(root2 ==nullptr)
        return new Node(value);

    if(value <root2->value)
        root2->left = insert(root2->left, value);
    else
        root2->right = insert(root2->right, value);

    return root2;

}




//     10  - root 
//    /   \ 
//   5     15

int main(){

    //Manual Tree Construction
    Node *root = new Node(10);

    root->left = new Node(5);
    root->right = new Node(15);

    cout << "root = " << root->value <<endl; // root = 10
    cout << "root->left =  " << root->left->value <<endl; //root->left = 5
    cout << "root->right = " << root->right->value <<endl; //root->right = 15

    //Inorder Command
    cout << "Inorder traversal: " ; //Inorder traversal: 5 10 15
    showinorder(root);
    cout <<endl;


    //INSERT COMMAND
    Node *root2 = nullptr;

    root2 = insert(root2, 20);
    root2 = insert(root2,30);
    root2 = insert(root2, 40);

    cout << "Inorder traversal: ";
    showinorder(root2);
    
    return 0;


}