#include <iostream>
#include <string>
using namespace std;

// Vinh Phan
// CIS-22C: C++ Data Structures and Algorithms
// CRN 33571

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

int main() {
    // Build the tree (done by Tree constructor)
    Tree t;
    TreeNode* root = t.getRoot();   // P

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

    return 0;
}