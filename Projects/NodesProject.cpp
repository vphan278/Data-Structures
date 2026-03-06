#include <iostream>
using namespace std;

//Vinh Phan
//CIS-22C: C++ Data Structures and Algorithms
//CRN 33571

//Single Linked List
class Node {
    public:
        int data;
        Node* next;

        Node(int value) : data(value), next(nullptr) {}
};


// Singly Linked List Class
class SingleLinkedList {
    private:
        Node* head;

    public:
        // Constructor
        SingleLinkedList() : head(nullptr) {}

        // Destructor 
        ~SingleLinkedList() {
            clear();
        }

        // Insert at front
        void insertFront(int value) {
            Node* newNode = new Node(value);
            newNode->next = head;
            head = newNode;
        }

        // Insert at back (
        void insertBack(int value) {
            Node* newNode = new Node(value);

            if (!head) {
                head = newNode;
                return;
            }

            Node* curr = head;
            while (curr->next) {
                curr = curr->next;
            }
            curr->next = newNode;
        }

        // Swap two adjacent nodes at index i and i+1
        // Example: swapAdjacent(1) swaps nodes at index 1 and 2
        void swap(int index) {
            if (!head || !head->next || index < 0) return;

            if (index == 0) {
                Node* first = head;
                Node* second = head->next;

                first->next = second->next;
                second->next = first;
                head = second;
                return;
            }

            Node* prev = head;
            for (int i = 0; i < index - 1 && prev->next; i++) {
                prev = prev->next;
            }

            if (!prev->next || !prev->next->next) return;

            Node* first = prev->next; //index at 1(B)
            Node* second = first->next; //index at 2(C)

            prev->next = second;
            first->next = second->next;
            second->next = first;
        }

        
        void print() const {
            Node* curr = head;
            while (curr) {
                cout << curr->data << " -> ";
                curr = curr->next;
            }
            cout << "null\n";
        }

        // Clear entire list
        void clear() {
            while (head) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
        }

};

//Double Linked List
class DNode {
    public:
        char data;
        DNode* prev;
        DNode* next;
        DNode(char v) : data(v), prev(nullptr), next(nullptr) {}
    };

class DoubleLinkedList {

    private:
        DNode* head;

    public:
        DoubleLinkedList() : head(nullptr) {}

         // Destructor 
        ~DoubleLinkedList() {
             clear();
        }

        void insertFront(char v) {
            DNode* n = new DNode(v);
            if (head) {
                n->next = head;
                head->prev = n;
            }
            head = n;
        }

        void insertBack(char v) {
            DNode* n = new DNode(v);
            if (!head) { head = n; return; }
            DNode* c = head;
            while (c->next) c = c->next;
            c->next = n;
            n->prev = c;
        }

        void swap() {
            int count = 0;
            for (DNode* c = head; c; c = c->next) count++;

            DNode* c = head;
            for (int i = 0; i < count / 2; i++) c = c->next;

            if (!c || !c->prev) return;

            DNode* left = c->prev;
            DNode* a = left->prev;
            DNode* d = c->next;

            if (a) a->next = c;
            c->prev = a;

            c->next = left;
            left->prev = c;

            left->next = d;
            if (d) d->prev = left;

            if (left == head) head = c;
        }

        void print() {
            for (DNode* c = head; c; c = c->next)
                cout << c->data << " <-> ";
            cout << "null\n";
        }

        // Clear entire list
        void clear() {
            while (head) {
                DNode* temp = head;
                head = head->next;
                delete temp;
            }
        }
};

//Tri-Node Rotations
class TreeNode {
    public:
        char data;
        TreeNode* parent;
        TreeNode* left;
        TreeNode* right;

        TreeNode(char val) : data(val), parent(nullptr), left(nullptr), right(nullptr) {}
};

class Tree {
    private:
        TreeNode* root;

        void destroy(TreeNode* node) {
            if (!node) return;
            destroy(node->left);
            destroy(node->right);
            delete node;
        }

    public:
        // Constructor builds the example tree (setup logic inside class)
        Tree() : root(nullptr) {
            // Role-based variable names (not hardcoded to A/B/C names)
            TreeNode* rootNode  = new TreeNode('P');
            TreeNode* pivot     = new TreeNode('X');
            TreeNode* leftNode  = new TreeNode('A');
            TreeNode* rightNode = new TreeNode('Y');
            TreeNode* midLeft   = new TreeNode('B');
            TreeNode* midRight  = new TreeNode('C');

            // Build structure:
            //       P
            //       |
            //       X
            //      / \
            //     A   Y
            //        / \
            //       B   C
            rootNode->left = pivot;        pivot->parent = rootNode;
            pivot->left = leftNode;        leftNode->parent = pivot;
            pivot->right = rightNode;      rightNode->parent = pivot;
            rightNode->left = midLeft;     midLeft->parent = rightNode;
            rightNode->right = midRight;   midRight->parent = rightNode;

            root = rootNode;
        }

        ~Tree() {
            destroy(root);
            root = nullptr;
        }

        TreeNode* getRoot() const { return root; }

        void printTree(TreeNode* node, int indent = 0) const {
            if (!node) return;
            printTree(node->right, indent + 4);
            cout << string(indent, ' ') << node->data << "\n";
            printTree(node->left, indent + 4);
        }

        // rotate left at X
        void rotateLeft(TreeNode* X) {
            if (!X || !X->right) return;

            TreeNode* Y = X->right;     // Y moves up
            TreeNode* B = Y->left;      // B moves to X->right
            TreeNode* P = X->parent;    // parent of X

            // Step 1: move B to X's right
            X->right = B;
            if (B) B->parent = X;

            // Step 2: move X under Y
            Y->left = X;
            X->parent = Y;

            // Step 3: connect Y to P (or update root)
            Y->parent = P;
            if (!P) {
                root = Y;
            } else if (P->left == X) {
                P->left = Y;
            } else if (P->right == X) {
                P->right = Y;
            }
        }

        // rotate right at Y
        void rotateRight(TreeNode* Y) {
            if (!Y || !Y->left) return;

            TreeNode* X = Y->left;      // X moves up
            TreeNode* B = X->right;     // B moves to Y->left
            TreeNode* P = Y->parent;    // parent of Y

            // Step 1: move B to Y's left
            Y->left = B;
            if (B) B->parent = Y;

            // Step 2: move Y under X
            X->right = Y;
            Y->parent = X;

            // Step 3: connect X to P (or update root)
            X->parent = P;
            if (!P) {
                root = X;
            } else if (P->left == Y) {
                P->left = X;
            } else if (P->right == Y) {
                P->right = X;
            }
        }
};

//Quad Node Rotation
class Rotor {

    private: 
        char north;
        char east;
        char south;
        char west;
        int degrees;

   
    public:
        //Constructor- initial layout
        Rotor() : north('A'), east('B'), south('C'), west('D'), degrees(0) {}
    
    char rotateClockwise() {
        //initial layout
        char temp = north;
        north = west;
        west  = south;
        south = east;
        east  = temp;

        degrees = (degrees + 90) % 360;
        return north;
    }
    
    void printRotor() {
        for (int i = 0; i < 4; i++) {
            cout << "After 90 degree rotation, north =  " << rotateClockwise() << " :: "; //After rotation each 90 degree rotation, north: D
        }
        cout << endl;
    }

};


 //First clockwise rotation

    //       [D]
    //        ↑
    //  [C] ← X → [A]
    //        ↓
    //       [B]

int main() {
    SingleLinkedList list;

    cout<< "------SingleLinkList-----------------"<<endl;
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);
    list.insertBack(40);

    list.print();

    list.swap(1); // swaps 20 and 30
    list.print();

    DoubleLinkedList Dlist;

    cout << "------DoubleLinkedList-------------"<<endl;
    Dlist.insertBack('A');
    Dlist.insertBack('B');
    Dlist.insertBack('C');
    Dlist.insertBack('D');
    Dlist.insertBack('E');

    Dlist.print();       // A <-> B <-> C <-> D <-> E
    Dlist.swap();        // swap C with B
    Dlist.print();       // A <-> C <-> B <-> D <-> E


    //Build the tree(done by Tree constructor)
    Tree t;
    TreeNode* root = t.getRoot();   // P

    cout <<"-----TriNode Rotation-------------------------------"<<endl;
    // Print tree sideways
    cout << "Tree structure:\n";
    t.printTree(root);

    // -----------------------------------------------------------------------------------------
    // Rotate LEFT at X (X is P->left)
    TreeNode* X = root->left;
    t.rotateLeft(X);

    cout << "\nAfter LEFT rotation at X:\n";
    t.printTree(t.getRoot());

    // Checks - after the left rotation
    cout << "\nChecks after left rotation:\n";
    cout << "P->left = " << t.getRoot()->left->data << "\n";                 // Y
    cout << "Y->left = " << t.getRoot()->left->left->data << "\n";           // X
    cout << "Y->right = " << t.getRoot()->left->right->data << "\n";         // C
    cout << "X->left = " << t.getRoot()->left->left->left->data << "\n";     // A
    cout << "X->right = " << t.getRoot()->left->left->right->data << "\n";   // B

    // -----------------------------------------------------------------------------------------
    // Rotate RIGHT at Y back to original before left rotation
    TreeNode* Y = t.getRoot()->left;   // Y is now P->left
    t.rotateRight(Y);

    cout << "\nAfter RIGHT rotation at Y (restored):\n";
    t.printTree(t.getRoot());

    // Checks - after the right rotation back to original
    cout << "\nChecks after right rotation:\n";
    cout << "P->left = " << t.getRoot()->left->data << "\n";                  // X
    cout << "X->left = " << t.getRoot()->left->left->data << "\n";            // A
    cout << "X->right = " << t.getRoot()->left->right->data << "\n";          // Y
    cout << "Y->left = " << t.getRoot()->left->right->left->data << "\n";     // B
    cout << "Y->right = " << t.getRoot()->left->right->right->data << "\n";   // C

    //Quad Node Rotation
    //initial layout

    cout <<"--------Quad Node Rotation----------------"<<endl;
    Rotor r;
    r.printRotor();


    return 0;
}