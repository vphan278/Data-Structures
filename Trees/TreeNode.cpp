#include <iostream>
#include <queue>
using namespace std;

class TreeNode{

    public:
        char data;
        TreeNode *parent;
        TreeNode *left;
        TreeNode *right;

        TreeNode(char val): data(val), parent(nullptr), left(nullptr), right(nullptr){}

};

TreeNode *buildTree(){

    TreeNode *A = new TreeNode('A');
    TreeNode *B = new TreeNode('B');
    TreeNode *C = new TreeNode('C');
    TreeNode *D = new TreeNode('D');
    TreeNode *E = new TreeNode('E');

    //Build the tree structure
    A->left = B;    B->parent = A;
    A->right = C;   C->parent = A;

    B->left = D;    D->parent = B;
    B->right = E;   E->parent = B;

    return A; //root

}

//Print Tree
void printTree(TreeNode* node, int indent = 0) {
    if (!node) return;
    printTree(node->right, indent + 4);
    cout << std::string(indent, ' ') << node->data << "\n";
    printTree(node->left, indent + 4);
}





int main(){

    TreeNode *root = buildTree();

    cout << "Tree structure:\n";
    printTree(root);

   

    return 0;

}