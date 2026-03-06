#include <iostream>
#include <stdexcept>
using namespace std;

enum ListOrder { ORDER_MIN, ORDER_MAX };

class Node {
public:
    int value;
    Node* prev;
    Node* next;
    Node(int v) : value(v), prev(nullptr), next(nullptr) {}
};

class HeapList {
private:
    Node* head;
    Node* tail;
    int sz;
    ListOrder order;

public:
    // DEFAULT constructor (so HeapList h; works)
    HeapList(ListOrder mode = ORDER_MAX)
        : head(nullptr), tail(nullptr), sz(0), order(mode) {}

    ~HeapList() {
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

    void insertFirstFromMiddle(const int* arr, int n, int mid) {
        if (n <= 0) throw runtime_error("empty array");
        if (mid < 0 || mid >= n) throw runtime_error("mid out of range");
        if (sz != 0) throw runtime_error("list already started");

        Node* first = new Node(arr[mid]);
        head = tail = first;
        sz = 1;

        cout << "Inserted FIRST node from mid index " << mid
             << " value " << arr[mid] << "\n";
    }

    void printLine() const {
        Node* curr = head;
        while (curr) {
            cout << curr->value;
            if (curr->next) cout << " <-> ";
            curr = curr->next;
        }
        cout << "\n";
    }
};

int main() {
    int arr[] = {50, 30, 40, 10, 20, 60, 5, 70, 15};
    int n = sizeof(arr) / sizeof(arr[0]);
    int mid = 4;

    HeapList h(ORDER_MAX);              // <-- you still have ListOrder
    h.insertFirstFromMiddle(arr, n, mid);
    h.printLine();
}