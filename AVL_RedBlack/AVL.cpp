#include <iostream>
using namespace std;

class Node{

    public:
        int data;
        Node *left;
        Node *right;
        int height;

        Node(int val){
            data = val;
            left = NULL;
            right = NULL;
            height = 1;
        }

};



//Height
int getHeight(Node *root){
    if(root == NULL)
        return 0;

    return root->height;
}

int getBalance(Node *root){
    if(root == NULL)
        return 0;

    return getHeight(root->left) - getHeight(root->right);
}

//Right Rotate
Node *rotateRight(Node *y){
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + max(getHeight(y->left), getHeight(y->right));
    x->height = 1 + max(getHeight(x->left), getHeight(x->right));

    return x;
}

//Left Rotate
Node *rotateLeft(Node *x){
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + max(getHeight(x->left), getHeight(x->right));
    y->height = 1 + max(getHeight(y->left), getHeight(y->right));

    return y;
}

//  10   (x)
//    \ 
//     20  
//    /  \ 
//  (T2)   30


Node* insert(Node* root, int value){
    if(root == NULL)
        return new Node(value);

    if(value < root->data)
        root->left = insert(root->left, value);

    else if(value > root->data)
        root->right = insert(root->right, value);
    else    
        return root;

    // update height
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    // get balance factor
    int balance = getBalance(root);

    // LL case
    if(balance > 1 && value < root->left->data)
        return rotateRight(root);

    // RR case
    if(balance < -1 && value > root->right->data)
        return rotateLeft(root);

    // LR case
    if(balance > 1 && value > root->left->data){
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // RL case
    if(balance < -1 && value < root->right->data){
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
};


void printInOrder(Node *root){
    if(root == NULL)
        return;

    printInOrder(root->left);
    cout << root->data << " ";
    printInOrder(root->right);
};

void printTree(Node* root, int space = 0){
    if(root == NULL)
        return;

    space += 2;

    printTree(root->right, space);

    cout << endl;
    for(int i = 2; i < space; i++)
        cout << " ";

    cout << root->data << endl;

    printTree(root->left, space);
}

int main(){

    //----------------------------------Right Rotation Test-----------------------------------------
    Node *root = NULL;

    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 10);

    printInOrder(root); //10 20 30
    cout << endl;

    cout << "Before right rotation: " << endl;
    printTree(root);// see tree sideways
    cout << endl;

    // Right Rotation
    cout << "Right Rotation: ";
    root = rotateRight(root);
    printTree(root);

    //     20        or                     Sideways is   30
    //    /  \                                          20
   //    10   30                                          10 

   //-----------------------------Left Rotation Test---------------------------------------------

    Node *root2 = NULL;

    root2 = insert(root2, 10);
    root2 = insert(root2, 20);
    root2 = insert(root2, 30);

    cout << "Before Left Rotation: " <<endl;
    printTree(root2); // see tree sideways
    cout <<endl;


   //Left Rotation
    root2 = rotateLeft(root2);                   // Sideway view
    cout << "After Left Rotation: " << endl;    //    30
    printTree(root2);                           //  20
    cout <<endl;                                //    10


    //------------------Left-Right Test-------------------------------------------------
    Node *root3 = NULL;
    root3 = insert(root3, 30);                     //    30
    root3 = insert(root3, 10);                     //    /
    root3 = insert(root3, 20);                     //   10
                                                   //     \     
                                                   //      20

    cout << "Before Left-Right Fix: " << endl;
    printTree(root3); //sideway view
    cout <<endl;

    //Step 1: left rotate the left child
    root3->left = rotateLeft(root3->left);

    cout << "After left rotate at 10: " << endl;
    printTree(root3);
    cout <<endl;

    //Step 2: right rotate the root
    root3 = rotateRight(root3);

    cout<< "After right rotate at 30: " <<endl;  //     30
    printTree(root3);                            //   20
    cout<<endl;                                  //     10



    //----------------------------Right-Left Test----------------------------
    Node *root4 = NULL;
    root4 = insert(root4, 10);
    root4 = insert(root4, 30);
    root4 = insert(root4, 20);

    cout << "Before Right-Left fix:" << endl;
    printTree(root4);
    cout << endl;

    // Step 1: right rotate the right child
    root4->right = rotateRight(root4->right);               // 10
    cout << "After right rotate at 30:" << endl;            //   \  
    printTree(root4);                                       //    20 
    cout << endl;                                           //     \  
                                                            //      30


    // Step 2: left rotate the root
    root4 = rotateLeft(root4);                              //    20
    cout << "After left rotate at 10:" << endl;             //   /  \  
    printTree(root4);                                       //  10   30
    cout << endl;

    return 0;
}



//   printTree: Before right rotation
//      y (30)
//     /
//    x (20)
//   /
//  10



// printTree: Before left rotation(sideway view)
//     30                                    10
//                                             \ 
//   20                                         20
      
//  10                                              30      