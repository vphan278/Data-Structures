#include <iostream>
#include <queue>
using namespace std;

class HeapNode {
private:
    int data;
    HeapNode* left;
    HeapNode* right;
    HeapNode* parent;

public:
    HeapNode(int value)
        : data(value), left(nullptr), right(nullptr), parent(nullptr) {}

    int getData() const { return data; }
    void setData(int value) { data = value; }

    HeapNode* getLeft() const { return left; }
    HeapNode* getRight() const { return right; }
    HeapNode* getParent() const { return parent; }

    void setLeft(HeapNode* node) { left = node; }
    void setRight(HeapNode* node) { right = node; }
    void setParent(HeapNode* node) { parent = node; }
};

class Heap {
private:
    HeapNode* root;
    int sz;

    // Max-heap: bubble bigger values upward
    void heapifyUp(HeapNode* node) {
        while (node && node->getParent() &&
               node->getData() > node->getParent()->getData()) {
            // swap VALUES (easy version)
            int tmp = node->getData();
            node->setData(node->getParent()->getData());
            node->getParent()->setData(tmp);

            node = node->getParent();
        }
    }

    // Max-heap: push smaller values downward
    void heapifyDown(HeapNode* node) {
        while (node) {
            HeapNode* left = node->getLeft();
            HeapNode* right = node->getRight();

            if (!left && !right) break; // leaf

            // choose larger child (for max heap)
            HeapNode* best = left;
            if (right && (!left || right->getData() > left->getData())) {
                best = right;
            }

            if (best && best->getData() > node->getData()) {
                int tmp = node->getData();
                node->setData(best->getData());
                best->setData(tmp);
                node = best;
            } else {
                break;
            }
        }
    }

    // Find the first node in level-order that has an empty child spot
    HeapNode* findInsertionParent() const {
        if (!root) return nullptr;

        queue<HeapNode*> q;
        q.push(root);

        while (!q.empty()) {
            HeapNode* cur = q.front(); q.pop();

            if (!cur->getLeft() || !cur->getRight())
                return cur;

            q.push(cur->getLeft());
            q.push(cur->getRight());
        }
        return nullptr; // shouldn't happen
    }

    // Get last node in level-order (rightmost node on last level)
    HeapNode* getLastNode() const {
        if (!root) return nullptr;

        queue<HeapNode*> q;
        q.push(root);
        HeapNode* last = nullptr;

        while (!q.empty()) {
            last = q.front(); q.pop();
            if (last->getLeft()) q.push(last->getLeft());
            if (last->getRight()) q.push(last->getRight());
        }
        return last;
    }

    // Delete entire tree
    void clearTree(HeapNode* node) {
        if (!node) return;
        clearTree(node->getLeft());
        clearTree(node->getRight());
        delete node;
    }

public:
    Heap() : root(nullptr), sz(0) {}
    ~Heap() { clearTree(root); }

    int size() const { return sz; }
    bool empty() const { return sz == 0; }

    int top() const {
        if (!root) throw runtime_error("Heap is empty");
        return root->getData();
    }

    // Insert while maintaining COMPLETE tree + heap property
    void insert(int value) {
        HeapNode* node = new HeapNode(value);

        if (!root) {
            root = node;
            sz = 1;
            return;
        }

        HeapNode* p = findInsertionParent();

        if (!p->getLeft()) {
            p->setLeft(node);
        } else {
            p->setRight(node);
        }
        node->setParent(p);

        sz++;
        heapifyUp(node);
    }

    // Remove root (max), keep complete, then heapify down
    void pop() {
        if (!root) return;

        if (sz == 1) {
            delete root;
            root = nullptr;
            sz = 0;
            return;
        }

        HeapNode* last = getLastNode();

        // Move last value to root
        root->setData(last->getData());

        // Detach last node from its parent
        HeapNode* p = last->getParent();
        if (p->getLeft() == last) p->setLeft(nullptr);
        else if (p->getRight() == last) p->setRight(nullptr);

        delete last;
        sz--;

        // Restore heap property
        heapifyDown(root);
    }

    // Print level-order (shows completeness)
    void printLevelOrder() const {
        if (!root) {
            cout << "(empty)\n";
            return;
        }

        queue<HeapNode*> q;
        q.push(root);

        while (!q.empty()) {
            HeapNode* cur = q.front(); q.pop();
            cout << cur->getData() << " ";

            if (cur->getLeft()) q.push(cur->getLeft());
            if (cur->getRight()) q.push(cur->getRight());
        }
        cout << "\n";
    }
};

int main() {
    Heap h;

    h.insert(10);
    h.insert(50);
    h.insert(30);
    h.insert(20);
    h.insert(40);

    cout << "Level-order: ";
    h.printLevelOrder();      // should represent a valid max heap
    cout << "Top: " << h.top() << "\n";

    h.pop();
    cout << "After pop: ";
    h.printLevelOrder();
    cout << "Top: " << h.top() << "\n";

    return 0;
}