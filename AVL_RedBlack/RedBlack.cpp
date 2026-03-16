#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int rotationCount = 0;
int recolorCount = 0;
int maxHeight = 0;
vector<int> heightsAfterEachInsertion;

vector<string> datasetNames;
vector<int> tableRotations;
vector<int> tableRecolors;
vector<int> tableFinalHeight;
vector<int> tableMaxHeight;

class Node{

    public: 
        int data;
        bool isRed;     //true = RED   , false = BLACK
        Node *left;
        Node *right;
        Node *parent;

        Node(int val){
            data = val;
            isRed  = true;   // new nodes start RED
            left = nullptr;
            right = nullptr;
            parent = nullptr;
        }

};

class RedBlackTree{

    private:
        Node *root;

        //Height
        int getHeight(Node* node) {
            if (node == nullptr)
                return 0;

            int leftHeight = getHeight(node->left);
            int rightHeight = getHeight(node->right);

            return 1 + max(leftHeight, rightHeight);
        }

        //Color
        void setColor(Node* node, bool red) {
            if (node != nullptr && node->isRed != red) {
                node->isRed = red;
                recolorCount++;
            }
        }

        //Right Rotate
        void rotateRight(Node* y) {
            rotationCount++;

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

        //Left Rotate
        void rotateLeft(Node* x) {
            rotationCount++;

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

        

        void fixInsert(Node* node) {
            while (node != root && node->parent->isRed) {
                Node* parent = node->parent;
                Node* grandparent = parent->parent;

                // parent is left child
                if (parent == grandparent->left) {
                    Node* uncle = grandparent->right;

                    // Case 1: uncle is red
                    if (uncle != nullptr && uncle->isRed) {
                        setColor(parent, false);
                        setColor(uncle, false);
                        setColor(grandparent, true);

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
                        setColor(parent, false);
                        setColor(grandparent, true);
                        rotateRight(grandparent);
                    }
                }
                // parent is right child
                else {
                    Node* uncle = grandparent->left;

                    // Case 1: uncle is red
                    if (uncle != nullptr && uncle->isRed) {
                        setColor(parent, false);
                        setColor(uncle, false);
                        setColor(grandparent, true);

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
                        setColor(parent, false);
                        setColor(grandparent, true);
                        rotateLeft(grandparent);
                    }
                }
            }

           setColor(root, false);
        }

        void inorder(Node* node) {
            if (node == nullptr)
                return;

            inorder(node->left);
            cout << node->data << (node->isRed ? "(R) " : "(B) ");
            inorder(node->right);
        }

        void printTree(Node* root, int space = 0) {
            if (root == nullptr)
                return;

            space += 3;

            printTree(root->right, space);

            cout << endl;
            for (int i = 3; i < space; i++)
                cout << " ";

            cout << root->data << (root->isRed ? "(R)" : "(B)") << endl;

            printTree(root->left, space);
        }

    public:

        RedBlackTree(){
            root = nullptr;
        }

        //Final Height
        int getFinalHeight() {
            return getHeight(root);
        }

        void insert(int value){
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
                setColor(newNode, false);

                int currentHeight = getHeight(root);
                heightsAfterEachInsertion.push_back(currentHeight);

                if (currentHeight > maxHeight)
                    maxHeight = currentHeight;
                    
                return;
            }

            // Fix red-black violations
            fixInsert(newNode);

            int currentHeight = getHeight(root);
            heightsAfterEachInsertion.push_back(currentHeight);

            if (currentHeight > maxHeight)
                maxHeight = currentHeight;
            
        }

        void printInorder() {
            inorder(root);
            cout << endl;
        }

        void displayTree() {
            cout << "Red-Black Tree (Sideways View):" << endl;
            printTree(root);
            cout << endl;
        }

        void printStats() {
            cout << "Rotations: " << rotationCount << endl;
            cout << "Recolors: " << recolorCount << endl;
            cout << "Final Height: " << getHeight(root) << endl;
            cout << "Maximum Height Reached: " << maxHeight << endl;

            cout << "Height After Each Insertion: ";
            for (int h : heightsAfterEachInsertion)
                cout << h << " ";
            cout << endl;
        }

       
                
};

void resetCounters() {
    rotationCount = 0;
    recolorCount = 0;
    maxHeight = 0;
    heightsAfterEachInsertion.clear();
}

void runDataset(string filename, bool showTree) {
    RedBlackTree tree;
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
            tree.insert(x);
        }
    }

    if (showTree) {
        tree.displayTree();

        cout << "\nInorder Traversal: ";
        tree.printInorder();
        cout << endl;
    }

    cout << "Dataset: " << filename << endl;
    tree.printStats();


    datasetNames.push_back(filename);
    tableRotations.push_back(rotationCount);
    tableRecolors.push_back(recolorCount);
    tableFinalHeight.push_back(tree.getFinalHeight());
    tableMaxHeight.push_back(maxHeight);

    cout << "------------------------------------------------------------------------------" << endl;
}



int main(){

    RedBlackTree tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(15);
    tree.insert(5);

    tree.displayTree();
// SideWay View
//           30(B)

// 20(B)

//             15(R)

//       10(B)

//             5(R)

    cout << "\nInorder: ";
    tree.printInorder(); // Inorder: 5(R) 10(B) 15(R) 20(B) 30(B)
    cout << endl;

    tree.printStats();
// Rotations: 1
// Recolors: 7
// Final Height: 3
// Maximum Height Reached: 3
// Height After Each Insertion: 1 2 2 3 3


//----------------Datasets---------------------------
    cout << "---------------------Datasets-------------------------"<<endl;
    resetCounters();
    runDataset("DatasetA.csv", true);

    resetCounters();
    runDataset("DatasetB.csv", false);

    resetCounters();
    runDataset("DatasetC.csv", false);
    
    resetCounters();
    runDataset("DatasetD.csv", false);

    resetCounters();
    runDataset("DatasetE.csv", false);

    cout << "-------------------- RED-BLACK SUMMARY TABLE --------------------" << endl;
    cout << "Dataset\t\tRotations\tRecolors\tFinalHeight\tMaxHeight" << endl;

    for (int i = 0; i < datasetNames.size(); i++) {
        cout << datasetNames[i] << "\t"
            << tableRotations[i] << "\t\t"
            << tableRecolors[i] << "\t\t"
            << tableFinalHeight[i] << "\t\t"
            << tableMaxHeight[i] << endl;
    }


    return 0;
}