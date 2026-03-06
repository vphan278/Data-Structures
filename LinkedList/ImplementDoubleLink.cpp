#include <iostream>
#include <stdexcept> 
#include <cassert>
using namespace std;

//Vinh Phan
//CIS-22C: C++ Data Structures and Algorithms
//CRN 33571

class Node{

    public:
        int data;
        Node *prev;
        Node *next;

        Node(int v) : data(v), prev(nullptr), next(nullptr) {}
};



class DoublyLinkedList{

    private: 
        Node *head;
        Node *tail;
        int count;

        Node* getNodeAt(int index) const {
        if (index < 0 || index >= count) {
            throw out_of_range("getNodeAt: index out of range");
        }

        // Optimize: start from head or tail depending on which is closer
        if (index <= count / 2) {
            Node* cur = head;
            for (int i = 0; i < index; i++) cur = cur->next;
            return cur;
        } else {
            Node* cur = tail;
            for (int i = count - 1; i > index; i--) cur = cur->prev;
            return cur;
        }
    }

    public:
        //Constructor
        DoublyLinkedList() : head(nullptr), tail(nullptr), count(0){}

        //Destructor
        ~DoublyLinkedList(){
            clear();
        }

        //Insert at front
        void insertFront(int value){
            Node * n = new Node(value);

            if(isEmpty()){
                head = tail = n;
            }
            else{
                n->next = head;
                head->prev = n;
                head = n;
            }
            count++;
        }

        //Insert at back
        void insertBack(int value) {
            Node* n = new Node(value);

            if (isEmpty()) {
                head = tail = n;
            } else {
                n->prev = tail;
                tail->next = n;
                tail = n;
            }
            count++;
        }

        //Insert At
        void insertAt(int index, int value) {
            if (index < 0 || index > count) {
                throw out_of_range("insertAt: index out of range");
            }
            if (index == 0) {
                insertFront(value);
                return;
            }
            if (index == count) {
                insertBack(value);
                return;
            }

            Node* cur = getNodeAt(index);      // node currently at 'index'
            Node* before = cur->prev;

            Node* n = new Node(value);
            n->prev = before;
            n->next = cur;

            before->next = n;
            cur->prev = n;

            count++;
        }

        //Remove Front
        void removeFront() {
            if (isEmpty()) return;

            Node* doomed = head;

            if (count == 1) {
                head = tail = nullptr;
            } else {
                head = head->next;
                head->prev = nullptr;
            }

            delete doomed;
            count--;
        }

        //Remove Back
        void removeBack() {
            if (isEmpty()) return;

            Node* doomed = tail;

            if (count == 1) {
                head = tail = nullptr;
            } else {
                tail = tail->prev;
                tail->next = nullptr;
            }

            delete doomed;
            count--;
        }

        //Remove At
        void removeAt(int index) {
            if (index < 0 || index >= count) {
                throw out_of_range("removeAt: index out of range");
            }
            if (index == 0) {
                removeFront();
                return;
            }
            if (index == count - 1) {
                removeBack();
                return;
            }

            Node* cur = getNodeAt(index);
            Node* before = cur->prev;
            Node* after = cur->next;

            before->next = after;
            after->prev = before;

            delete cur;
            count--;
        }

        //getAt
        int getAt(int index) const {
            return getNodeAt(index)->data;;
        }

        //index Of
        int indexOf(int value) const {
            int i = 0;
            for (Node* cur = head; cur != nullptr; cur = cur->next, i++) {
                if (cur->data == value) return i;
            }
            return -1;
        }

        //int size
        int size() const {
            return count;
        }

        //IsEmpty
        bool isEmpty() const {
            return count == 0;
        }

        //printForward
        void printForward() const {
            Node* cur = head;
            cout << "[";
            while (cur != nullptr) {
                cout << cur->data;
                cur = cur->next;
                if (cur != nullptr) cout << " <-> ";
            }
            cout << "]\n";
        }

        //printBackward
        void printBackward() const {
            Node* cur = tail;
            cout << "[";
            while (cur != nullptr) {
                cout << cur->data;
                cur = cur->prev;
                if (cur != nullptr) cout << " <-> ";
            }
            cout << "]\n";
        }
        
        //Clear entire list
        void clear() {
            Node* cur = head;
            while (cur != nullptr) {
                Node* nxt = cur->next;
                delete cur;
                cur = nxt;
            }
            head = tail = nullptr;
            count = 0;
        }
};


//Stack
class Stack : public DoublyLinkedList {
    public:
        //Constructor
        Stack() : DoublyLinkedList() {}

        //Destructor
        // ~Stack() {
        //     clear();
        // }

        void push(int value) {
            insertFront(value);     // top = front
        }

        void pop() {
            if (isEmpty()) {
                throw runtime_error("pop: Stack is empty");
            }
            removeFront();
        }

        int top() const {
            if (isEmpty()) {
                throw runtime_error("top: Stack is empty");
            }
            return getAt(0);
        }

        bool isEmpty() const {
            return DoublyLinkedList::isEmpty();
        }

        int size() const {
            return DoublyLinkedList::size();
        }

        void clear() {
            DoublyLinkedList::clear();
        }
};

//class Queue
class Queue : public DoublyLinkedList{

    public: 
        //Constructor
        Queue() : DoublyLinkedList(){}

        //Destructor
        // ~Queue(){
        //     clear();
        // }

        //Add to back of queue
        void enqueue(int value){
            insertBack(value);
        }

        //Remove from front of queue
        void dequeue(){
            if(isEmpty()){
                throw runtime_error("dequeue: Queue is empty");
            }
            removeFront();
        }

        //Peek at front value
        int front() const{
            if(isEmpty()){
                throw runtime_error("front: Queue is empty");
            }
            return getAt(0);
        }

        //For clarity
        bool isEmpty() const{
            return DoublyLinkedList::isEmpty();
        }

        //Current queue size
        int size() const{
            return DoublyLinkedList::size();
        }


        //Clear all items
        void clear(){
            DoublyLinkedList::clear();
        }



};

//Validation
bool validate(){

    // ----Stack Test----------//
    cout << "Stack Test" <<endl;
    Stack s;
    s.push(10);
    s.push(20);
    s.push(30);
    s.push(40);
    s.push(50);
    s.push(60);

    //s.printForward(); // [60 <-> 50 <-> 40 <-> 30 <-> 20 <-> 10]

    //Top should be last pushed
    assert(s.top() == 60);

    s.pop();
    assert(s.top() == 50);
    //s.printForward(); //[50 <-> 40 <-> 30 <-> 20 <-> 10]

    // After clear, stack must be empty
    s.clear();
    assert(s.isEmpty() == true);


    //--------QUEUE Test---------------------------
    Queue q;

    q.enqueue(100);
    q.enqueue(200);
    q.enqueue(300);
    q.enqueue(400);
    q.enqueue(500);
    q.enqueue(600);

    //q.printForward(); //[100 <-> 200 <-> 300 <-> 400 <-> 500 <-> 600]

    // Front should be first enqueued
    assert(q.front() == 100);

    q.dequeue();
    assert(q.front() == 200);

    // After clear, queue must be empty
    q.clear();
    assert(q.isEmpty() == true);

    return true;

};


int main(){

    DoublyLinkedList Dl;

    Dl.insertFront(5);
    Dl.insertFront(10);
    Dl.printForward(); //[10 <-> 5]
    Dl.printBackward(); //[5 <-> 10]

    Dl.insertAt(1,7); 
    Dl.printForward(); //[10 <-> 7 <-> 5]

    Dl.insertBack(20);
    Dl.insertBack(30); 
    Dl.printForward(); // [10 <-> 7 <-> 5 <-> 20 <-> 30]
    Dl.printBackward(); //[30 <->20 <-> 5 <-> 7 <-> 10]

    Dl.insertAt(3,28);
    Dl.printForward(); // [10 <-> 7 <-> 5 <-> 28 <-> 20 <-> 30]


    Dl.removeFront(); 
    Dl.printForward(); //[7 <-> 5 <-> 28 <-> 20 <-> 30]

    Dl.removeBack();
    Dl.printForward(); //[7 <-> 5 <-> 28 <-> 20]

    Dl.removeAt(1);
    Dl.printForward(); //[7 <-> 28 <-> 20]


    cout << "getAt(1) = " << Dl.getAt(1) << endl; //getAt(1) = 28
    cout << "indexOf(20) = " << Dl.indexOf(20) <<endl; // indexOf(20) = 2

    //Dl.clear();
    //Dl.printForward(); //[ ]

    //Stack--------------------------------------------------------
    cout << " -------Beginning of class Stack ------------" <<endl;
    Stack s;

    s.push(10);
    s.push(20);
    s.push(30);
    s.printForward(); //[30 <-> 20 <-> 10]

    cout << "Top: " << s.top() <<endl; // Top :30
    cout << "Size: " << s.size() << endl; // 3

    s.pop();
    s.printForward(); // [20 <-> 10]
    cout << "Top after pop: " << s.top() << endl; // Top after pop: 20

    s.clear();
    cout << "Empty? " << (s.isEmpty() ? "yes" : "no") << endl;

    //Stack Test Again ///
    cout << "----------------Stack Test Again----------------- "<<endl;
    assert(validate()); // [60 <-> 50 <-> 40 <-> 30 <-> 20 <-> 10]
    cout << "All tests passed! \n";

    

    return 0;

}