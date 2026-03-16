#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int rotationCount = 0;
int balanceChecks = 0;
int maxHeight = 0;
vector<int> heightsAfterEachInsertion;

vector<string> datasetNames;
vector<int> tableRotations;
vector<int> tableBalanceChecks;
vector<int> tableFinalHeight;
vector<int> tableMaxHeight;

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

    //Count every balance check
    balanceChecks++;

    if(root == NULL)
        return 0;

    return getHeight(root->left) - getHeight(root->right);
}

//Right Rotate
Node *rotateRight(Node *y){
    
    //Count rotation
    rotationCount++;

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

    //Count rotation
    rotationCount++;

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

    space += 6;

    printTree(root->right, space);

    cout << endl;
    for(int i = 6; i < space; i++)
        cout << " ";

    cout << root->data << endl;

    printTree(root->left, space);
};

void runDataset(string filename, bool showTree){

    Node *root = NULL;
    ifstream fin(filename);


    if (!fin) {
        cout << "Could not open " << filename << endl;
        return;
    }

    char ch;
    int x;

    while (fin >> ch) {
        if ((ch >= '0' && ch <= '9') || ch == '-') {
            fin.putback(ch);
            fin >> x;

            root = insert(root, x);

            int currentHeight = getHeight(root);
            heightsAfterEachInsertion.push_back(currentHeight);

            if(currentHeight > maxHeight)
                maxHeight = currentHeight;
        }
    }

    if(showTree){
    cout << "AVL Tree (Sideways view):" << endl;
    printTree(root);

    cout << endl << "Inorder Traversal: ";
    printInOrder(root);
    cout << endl;
    }


    cout << "Dataset: " << filename << endl;
    cout << "Final Height: " << getHeight(root) << endl;
    cout << "Number of Rotations: " << rotationCount << endl;
    cout << "Number of Balance Checks: " << balanceChecks << endl;

    cout << "Height After Each Insertion: ";
        for(int h : heightsAfterEachInsertion){
            cout << h << " ";
    }
    cout <<endl;

    cout << "Maximum Height Reached: " << maxHeight << endl;

    datasetNames.push_back(filename);
    tableRotations.push_back(rotationCount);
    tableBalanceChecks.push_back(balanceChecks);
    tableFinalHeight.push_back(getHeight(root));
    tableMaxHeight.push_back(maxHeight);

    cout << "------------------------------------------------------------------------" << endl;

    

};

void resetCounters(){
    rotationCount = 0;
    balanceChecks = 0;
    maxHeight = 0;
    heightsAfterEachInsertion.clear();
}



int main(){

    resetCounters();
    runDataset("DatasetA.csv",true);

    resetCounters();
    runDataset("DatasetB.csv", false);

    resetCounters();
    runDataset("DatasetC.csv", false);

    resetCounters();
    runDataset("DatasetD.csv", false);

    resetCounters();
    runDataset("DatasetE.csv", false);

    cout << endl;
    cout << "----------------------- AVL SUMMARY TABLE ---------------------" << endl;

    cout << "Dataset\t\tRotations\tBalanceChecks\tFinalHeight\tMaxHeight" << endl;

    for(int i = 0; i < datasetNames.size(); i++)
    {
        cout << datasetNames[i] << "\t"
            << tableRotations[i] << "\t\t"
            << tableBalanceChecks[i] << "\t\t"
            << tableFinalHeight[i] << "\t\t"
            << tableMaxHeight[i] << endl;
    }


    // Node *root = NULL;

    // root = insert(root, 30);
    // root = insert(root, 20);
    // root = insert(root, 10);

    // cout << "AVL Tree (Sideways view): " << endl;     //       30
    // printTree(root);                                  //     20
    //                                                   //       10

    // cout << endl << "Inorder Traversal: ";
    // printInOrder(root);   // Inorder Traversal: 10 20 30
    // cout << endl;

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