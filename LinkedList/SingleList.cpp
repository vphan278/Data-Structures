#include <iostream>
using namespace std;

//Vinh Phan
//CIS-22C: C++ Data Structures and Algorithms
//CRN 33571


class Node {
    public:
        int data;
        Node* next;

        Node(int value) : data(value), next(nullptr) {}
};


// Singly Linked List Class
class SingleLinkedList {
    private:
        Node *head;
        Node *tail;

    public:
        // Constructor
        SingleLinkedList() : head(nullptr), tail(nullptr) {}

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
                head = tail = newNode;
                return;
            }
            else{
                tail->next = newNode;
                tail = newNode;
            }

    
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


int main() {
    SingleLinkedList list;

    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);
    list.insertBack(40);

    list.print();

    list.swap(1); // swaps 20 and 30
    list.print();

    return 0;
}