#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <stack>
#include <string>
#include <queue>
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

class BST{

    private:
        Node *root;

        Node *insert(Node *r, int value){
            Node *newNode = new Node(value);

            //empty tree
            if(r == nullptr) return newNode;

            Node *current = r;
            Node *parent = nullptr;

            while(current !=nullptr){
                parent = current;
                if(value < current->getValue())
                current = current->getLeft();
                else
                    current = current->getRight();
            }

            newNode->setParent(parent);

            if(value < parent->getValue())
                parent->setLeft(newNode);
            else
                parent->setRight(newNode);

            return r;

        }
    
    public:
        BST() : root(nullptr) {}

        ~BST(){
            clear();
        }

        Node *getRoot() const{
            return root;
        }

        void insert(int value) {
            root = insert(root, value);
        }

        //Iterative clear/delete of entire tree
        void clear(){
            if(root == nullptr) return;

            stack<pair<Node *, bool>> st;
            st.push({root, false});

            while(!st.empty()){
                auto[node, visited] = st.top();
                st.pop();

                if(node == nullptr) continue;

                if(visited){
                    delete node;
                }
                else{
                    //Postorder: Left, right, node
                    st.push({node, true});
                    st.push({node->getRight(), false});
                    st.push({node->getLeft(), false});
                }
            }
            root = nullptr;

        }

        void display() const {
            if (!root) {
                cout << "(empty tree)\n";
                return;
            }

            queue<Node*> q;
            q.push(root);

            while (!q.empty()) {
                Node* current = q.front();
                q.pop();

                cout << "Node: " << current->getValue();

                if (current->getParent())
                    cout << " (parent: " << current->getParent()->getValue() << ")";
                else
                    cout << " (parent: null)";

                cout << "\n";

                if (current->getLeft()) {
                    cout << "  Left: " << current->getLeft()->getValue() << "\n";
                    q.push(current->getLeft());
                }

                if (current->getRight()) {
                    cout << "  Right: " << current->getRight()->getValue() << "\n";
                    q.push(current->getRight());
                }
            }
        }

        void printInOrder() const {
            stack<Node*> st;
            Node* current = root;

            while (current != nullptr || !st.empty()) {

                while (current != nullptr) {
                    st.push(current);
                    current = current->getLeft();
                }

                current = st.top();
                st.pop();

                cout << current->getValue() << " ";

                current = current->getRight();
            }

            cout << endl;
        }



};


int main(){

    srand((unsigned)time(nullptr));   

    //Task 1
    vector<int> values;
    unordered_set<int> used;

    // Generate between 9 and 15 unique random numbers
    int n = 9 + rand() % 7;   // 9..15
    while (values.size() < n) {
        int num = rand() % 100;
        if (used.insert(num).second)
            values.push_back(num);
    }

    cout << "Unsorted values: ";
    for (int v : values)
        cout << v << " ";  //Unsorted values: 76 39 73 70 38 29 99 54 97 
    cout << "\n";   

    //Task 2 and 3 
    BST tree;

    // middle index of UNSORTED vector
    int mid = values.size() / 2;

    // set initial root using the middle element
    tree.insert(values[mid]);

    // insert everything else in ORIGINAL orde
    for (int i = 0; i < (int)values.size(); i++) {
        if (i == mid) continue;
        tree.insert(values[i]);
    }

    cout << "\nBST built from UNSORTED input:\n";
    tree.display();

    cout << "In-order traversal: ";
    tree.printInOrder(); // In-order traversal: 29 38 39 54 70 73 76 97 99

    //Task 4: Rebuild Tree using Sorted input
    vector<int> sortedValues = values;
    sort(sortedValues.begin(), sortedValues.end());

    cout << "\nSorted values: "; //29 38 39 54 70 73 76 97 99
    for (int v : sortedValues)
        cout << v << " ";
    cout << "\n";

    tree.clear();

    int mid2 = sortedValues.size() / 2;

    tree.insert(sortedValues[mid2]);

    for (int i = 0; i < (int)sortedValues.size(); i++) {
        if (i == mid2) continue;
        tree.insert(sortedValues[i]);
    }

    cout << "\nBST rebuilt from SORTED input:\n";
    tree.display();

    cout << "In-order traversal: ";
    tree.printInOrder(); // In-order traversal: 29 38 39 54 70 73 76 97 99


    return 0;

}

//Task 2 and 3
// BST built from UNSORTED input: 76 39 73 70 38 29 99 54 97 
// BST built from UNSORTED input:
// Node: 38 (parent: null)
//   Left: 29
//   Right: 76
// Node: 29 (parent: 38)
// Node: 76 (parent: 38)
//   Left: 39
//   Right: 99
// Node: 39 (parent: 76)
//   Right: 73
// Node: 99 (parent: 76)
//   Left: 97
// Node: 73 (parent: 39)
//   Left: 70
// Node: 97 (parent: 99)
// Node: 70 (parent: 73)
//   Left: 54
// Node: 54 (parent: 70)

//                 38
//                /  \  
//              29    76
//                   /   \  
//                 39     99
//                   \    /
//                    73 97
//                   /
//                 70
//                /
//              54


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Task 4 Rebuild Tree Using Sorted Input
// Sorted values: 29 38 39 54 70 73 76 97 99
// BST rebuilt from SORTED input:
// Node: 70 (parent: null)
//   Left: 29
//   Right: 73
//   Right: 73
//   Right: 73
// Node: 29 (parent: 70)
//   Right: 38
// Node: 73 (parent: 70)
//   Right: 76
// Node: 38 (parent: 29)
//   Right: 39
// Node: 76 (parent: 73)
//   Right: 97
// Node: 39 (parent: 38)
//   Right: 54
// Node: 97 (parent: 76)
//   Right: 99
// Node: 54 (parent: 39)
// Node: 99 (parent: 97)

//                  70              ///
//                 /  \             ///
//               29    73           ///
//                 \     \          ///
//                  38    76        ///
//                    \     \       ///
//                     39    97     ///
//                      \     \     /// 
//                       54    99   ///


/// Task 5: Compare The Two Trees////////////////////////
// It seems the tree from the unsorted is even and the sorted one is more skewed towards the right in ascending order.
// The tree built from unsorted vector is balanced since it has branching on both sides.
// The sorted tree has a deeper path from root to leaf.
// With Insertion order, unsorted vectors, the direction of movement varies and sorted vectors, the direction of movement is in one direction, usually towards the right.
// The parent pointer made it easier to understand and debug the structure of the tree. Each node could display parent's value, confirming that left and right pointers
// were connected correctly. In the doubly linked list, there was no hierarchical structure, only linear prev/next connections. In BST, the parent pointer shows the 
// tree relationships and helps verify that insertion logic is working correctly.