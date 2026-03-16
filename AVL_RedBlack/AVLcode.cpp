
#include <iostream>
#include <algorithm>

struct Node {
    int key;
    Node* left;
    Node* right;
    int height;
    Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

int h(Node* n) {
    return n ? n->height : 0;
}

int bf(Node* n) {
    return n ? h(n->left) - h(n->right) : 0;
}

Node* rr(Node* y) {
    Node* x = y->left;
    Node* t = x->right;
    x->right = y;
    y->left = t;
    y->height = std::max(h(y->left), h(y->right)) + 1;
    x->height = std::max(h(x->left), h(x->right)) + 1;
    return x;
}

Node* lr(Node* x) {
    Node* y = x->right;
    Node* t = y->left;
    y->left = x;
    x->right = t;
    x->height = std::max(h(x->left), h(x->right)) + 1;
    y->height = std::max(h(y->left), h(y->right)) + 1;
    return y;
}

Node* insert(Node* n, int k) {
    if (!n) return new Node(k);
    if (k < n->key) n->left = insert(n->left, k);
    else if (k > n->key) n->right = insert(n->right, k);
    else return n;

    n->height = std::max(h(n->left), h(n->right)) + 1;

    int b = bf(n);

    if (b > 1 && k < n->left->key) return rr(n);
    if (b < -1 && k > n->right->key) return lr(n);
    if (b > 1 && k > n->left->key) {
        n->left = lr(n->left);
        return rr(n);
    }
    if (b < -1 && k < n->right->key) {
        n->right = rr(n->right);
        return lr(n);
    }

    return n;
}

int main() {
    Node* root = nullptr;
    int x;
    while (std::cin >> x) root = insert(root, x);
    return 0;
}