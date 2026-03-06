#include <iostream>
#include <stdexcept>
using namespace std;

class DoubleNode {
    public:
        int data;
        DoubleNode* prev;
        DoubleNode* next;
        DoubleNode(int v) : data(v), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
    private:
        DoubleNode* head;
        DoubleNode* tail;
        int count;

    public:
        DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}
        ~DoublyLinkedList() { clear(); }

        bool isEmpty() const { return count == 0; }
        int sizeDouble() const { return count; }

        void insertBackDouble(int value) {
            DoubleNode* n = new DoubleNode(value);
            if (isEmpty()) {
                head = tail = n;
            } else {
                n->prev = tail;
                tail->next = n;
                tail = n;
            }
            count++;
        }

    void removeFrontDouble() {
        if (isEmpty()) return;

        DoubleNode* doomed = head;

        if (count == 1) {
            head = tail = nullptr;
        } else {
            head = head->next;
            head->prev = nullptr;
        }

        delete doomed;
        count--;
    }

    int getAtDouble(int index) const {
        if (index < 0 || index >= count) throw out_of_range("getAt: index out of range");

        // simple forward walk
        DoubleNode* cur = head;
        for (int i = 0; i < index; i++) cur = cur->next;
        return cur->data;
    }

    void printForwardDouble() const {
        DoubleNode* cur = head;
        cout << "[";
        while (cur) {
            cout << cur->data;
            cur = cur->next;
            if (cur) cout << " <-> ";
        }
        cout << "]" << endl; // endl flushes
    }

    void clear() {
        while (head) {
            DoubleNode* nxt = head->next;
            delete head;
            head = nxt;
        }
        tail = nullptr;
        count = 0;
    }
};

class Queue : public DoublyLinkedList {
    public:
        void enqueue(int value) { insertBackDouble(value); }

        void dequeue() {
            if (isEmpty()) throw runtime_error("dequeue: Queue is empty");
            removeFrontDouble();
        }

        int frontDouble() const {
            if (isEmpty()) throw runtime_error("front: Queue is empty");
            return getAtDouble(0);
        }
};

int main() {
    cout << "===== QUEUE PRINT TEST =====" << endl;

    Queue q;
    q.enqueue(100);
    q.enqueue(200);
    q.enqueue(300);
    q.enqueue(400);
    q.enqueue(500);
    q.enqueue(600);

    cout << "Queue after enqueues: ";
    q.printForwardDouble();   // [100 <-> 200 <-> 300 <-> 400 <-> 500 <-> 600]

    cout << "Front is: " << q.frontDouble() << endl;  //Front is now: 100

    q.dequeue();
    cout << "Queue after one dequeue: ";
    q.printForwardDouble();   // [200 <-> 300 <-> 400 <-> 500 <-> 600]

    cout << "Front is now: " << q.frontDouble() << endl; //Front is now: 200

    cout << "===== DONE =====" << endl;
    return 0;
}