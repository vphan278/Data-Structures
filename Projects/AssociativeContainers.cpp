#include <iostream>
#include <stdexcept> 
#include <cassert>
using namespace std;

//Vinh Phan
//CIS-22C: C++ Data Structures and Algorithms
//CRN 33571

class SingleNode {

    public:
        int data;
        SingleNode* next;

        SingleNode(int value) : data(value), next(nullptr){}

};


// SingleLinkedList class 

class SingleLinkedList{

    private:
        SingleNode *head;
        int count;

        // PRIVATE helper: returns pointer to node at index, or nullptr if invalid
        SingleNode* getNodeAtSingle(int index) const {
            if (index < 0) return nullptr;

            SingleNode* curr = head;
            for (int i = 0; i < index; i++) {
                if (!curr) return nullptr;
                curr = curr->next;
            }
            return curr;
        }

    public: 
        //Constructor
        SingleLinkedList() : head(nullptr), count(0){

        }

        //Destructor
        ~SingleLinkedList(){
            clear();
        }

        //Insert at front
        void insertFrontSingle(int value){
            SingleNode *newNode = new SingleNode(value);
            newNode->next = head;
            head = newNode;
            count++;
            
        }

        //Insert at back
        void insertBackSingle(int value) {
            SingleNode* newNode = new SingleNode(value);

            if (!head) {
                head = newNode;
            }
            else {
                SingleNode* curr = head;
                while (curr->next) curr = curr->next;
                curr->next = newNode;
            }
            count++;
            
        }

        //Insert at index
        void insertAtSingle(int index, int value) {
            if (index < 0 || index > count) {
                cerr << "insertAt: index out of range\n";
                return;
            }

            if (index == 0) { insertFrontSingle(value); return; }
            if (index == count) { insertBackSingle(value); return; }

            SingleNode* prev = getNodeAtSingle(index - 1);
            if (!prev) return;

            SingleNode* newNode = new SingleNode(value);
            newNode->next = prev->next;
            prev->next = newNode;
            count++;
        }

        //Remove at front
        void removeFrontSingle() {
            if (!head) {
                return; // list is empty
            }

            SingleNode* temp = head;
            head = head->next;
            delete temp;
            count--;
        }

        //Remove at back
        void removeBackSingle() {
            // Case 1: empty list
            if (!head) {
                return;
            }

            // Case 2: only one node
            if (!head->next) {
                delete head;
                head = nullptr;
                count--;
                return;
            }

            // Case 3: two or more nodes
            SingleNode* curr = head;
            while (curr->next->next) {
                curr = curr->next;
            }

            delete curr->next;
            curr->next = nullptr;
            count--;
        }

        //Remove at index
        void removeAtSingle(int index) {
            if (!head) return;

            // Remove front
            if (index == 0) {
                removeFrontSingle();
                return;
            }

            SingleNode* curr = head;

            // Move to node before the one we remove
            for (int i = 0; i < index - 1; i++) {
                if (!curr->next) return;  // index out of bounds
                curr = curr->next;
            }

            SingleNode* temp = curr->next;
            if (!temp) return;           // index out of bounds

            curr->next = temp->next;
            delete temp;
            count--;
        }

        // isEmpty
        bool isEmpty() const {
            return head == nullptr;
        }

        //Size of list
        int sizeSingle() const {
            // int count = 0;
            // Node* curr = head;
            // while (curr) {
            //     count++;
            //     curr = curr->next;
            // }
            return count;
        }

        //Get value at index
        int getAtSingle(int index) const {
            SingleNode* node = getNodeAtSingle(index);
            return node ? node->data : -1;
        }

        //Get index of value
        int indexOfSingle(int value) const {
            int index = 0;
            for (SingleNode* curr = head; curr; curr = curr->next, index++) {
                if (curr->data == value) return index;
            }
            return -1;
        }

        //------------------------------------------------------------//
        //printing
        void printSingle() const {
            SingleNode* curr = head;
            while (curr) {
                cout << curr->data << " -> ";
                curr = curr->next;
            }
            cout << "null\n";
        }

        void clear(){
            while (head) {
                SingleNode* temp = head;
                head = head->next;
                delete temp;
            }
            count = 0;
        }

};



//------------------------------------------------------------------//
class DoubleNode{

    public:
        int data;
        DoubleNode *prev;
        DoubleNode *next;

        DoubleNode(int v) : data(v), prev(nullptr), next(nullptr) {}
};





class DoublyLinkedList{

    private: 
        DoubleNode *head;
        DoubleNode *tail;
        int count;

        DoubleNode* getNodeAtDouble(int index) const {
        if (index < 0 || index >= count) {
            throw out_of_range("getNodeAt: index out of range");
        }

        // Optimize: start from head or tail depending on which is closer
        if (index <= count / 2) {
            DoubleNode* cur = head;
            for (int i = 0; i < index; i++) cur = cur->next;
            return cur;
        } else {
            DoubleNode* cur = tail;
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
        void insertFrontDouble(int value){
            DoubleNode * n = new DoubleNode(value);

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

        //Insert At
        void insertAtDouble(int index, int value) {
            if (index < 0 || index > count) {
                throw out_of_range("insertAt: index out of range");
            }
            if (index == 0) {
                insertFrontDouble(value);
                return;
            }
            if (index == count) {
                insertBackDouble(value);
                return;
            }

            DoubleNode* cur = getNodeAtDouble(index);      // node currently at 'index'
            DoubleNode* before = cur->prev;

            DoubleNode* n = new DoubleNode(value);
            n->prev = before;
            n->next = cur;

            before->next = n;
            cur->prev = n;

            count++;
        }

        //Remove Front
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

        //Remove Back
        void removeBackDouble() {
            if (isEmpty()) return;

            DoubleNode* doomed = tail;

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
        void removeAtDouble(int index) {
            if (index < 0 || index >= count) {
                throw out_of_range("removeAt: index out of range");
            }
            if (index == 0) {
                removeFrontDouble();
                return;
            }
            if (index == count - 1) {
                removeBackDouble();
                return;
            }

            DoubleNode* cur = getNodeAtDouble(index);
            DoubleNode* before = cur->prev;
            DoubleNode* after = cur->next;

            before->next = after;
            after->prev = before;

            delete cur;
            count--;
        }

        //getAt
        int getAtDouble(int index) const {
            return getNodeAtDouble(index)->data;
        }

        //index Of
        int indexOfDouble(int value) const {
            int i = 0;
            for (DoubleNode* cur = head; cur != nullptr; cur = cur->next, i++) {
                if (cur->data == value) return i;
            }
            return -1;
        }

        //int size
        int sizeDouble() const {
            return count;
        }

        //IsEmpty
        bool isEmpty() const {
            return count == 0;
        }

        //printForward
        void printForwardDouble() const {
            DoubleNode* cur = head;
            cout << "[";
            while (cur != nullptr) {
                cout << cur->data;
                cur = cur->next;
                if (cur != nullptr) cout << " <-> ";
            }
            cout << "]\n";
        }

        //printBackward
        void printBackwardDouble() const {
            DoubleNode* cur = tail;
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
            DoubleNode* cur = head;
            while (cur != nullptr) {
                DoubleNode* nxt = cur->next;
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
        ~Stack() {
            clear();
        }

        void push(int value) {
            insertFrontDouble(value);     // top = front
        }

        void pop() {
            if (isEmpty()) {
                throw runtime_error("pop: Stack is empty");
            }
            removeFrontDouble();
        }

        int top() const {
            if (isEmpty()) {
                throw runtime_error("top: Stack is empty");
            }
            return getAtDouble(0);
        }

        bool isEmpty() const {
            return DoublyLinkedList::isEmpty();
        }

        int sizeDouble() const {
            return DoublyLinkedList::sizeDouble();
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
        ~Queue(){
            clear();
        }

        //Add to back of queue
        void enqueue(int value){
            insertBackDouble(value);
        }

        //Remove from front of queue
        void dequeue(){
            if(isEmpty()){
                throw runtime_error("dequeue: Queue is empty");
            }
            removeFrontDouble();
        }

        //Peek at front value
        int frontDouble() const{
            if(isEmpty()){
                throw runtime_error("front: Queue is empty");
            }
            return getAtDouble(0);
        }

        //For clarity
        bool isEmpty() const{
            return DoublyLinkedList::isEmpty();
        }

        //Current queue size
        int sizeDouble() const{
            return DoublyLinkedList::sizeDouble();
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
    assert(q.frontDouble() == 100);

    q.dequeue();
    assert(q.frontDouble() == 200);

    // After clear, queue must be empty
    q.clear();
    assert(q.isEmpty() == true);

    return true;

};


int main(){
    //-----------------SingleLinkedList--------------------------
    SingleLinkedList list;
    cout << "------------SingleLinkedList--------------------"<<endl;
    list.insertFrontSingle(10);
    list.insertFrontSingle(15);
    list.printSingle();  //15 -> 10 -> null

    list.insertBackSingle(30);
    list.insertBackSingle(40);
    list.printSingle(); // 15 -> 10 -> 30 -> 40 -> null

    list.insertAtSingle(2,60);
    list.printSingle(); // 15 -> 10 -> 60 -> 30 -> 40 -> null

    list.removeFrontSingle();
    list.printSingle(); //  10 -> 60 -> 30 -> 40 -> null

    list.removeBackSingle();
    list.printSingle(); // 10 -> 60 -> 30 -> null

    list.removeAtSingle(1);
    list.printSingle(); // 10 -> 30 -> null 

    //is list Empty
    cout << list.isEmpty() <<endl; // 0(false) because it is not empty

    //Get size
    cout << "Size: " << list.sizeSingle() << endl; // Size: 2

    //Get at index
    cout << "value at index requested: " << list.getAtSingle(1) << endl; // value at index requested: 30

    //Get index
    cout << "index: " << list.indexOfSingle(10) << endl; // index: 0
    cout << "index: " << list.indexOfSingle(99) << endl; // index: -1


    //---------------DoublyLinkedList--------------------------------------------//
    cout << "------------------DoublyLinkedList ------------------------" <<endl;
    DoublyLinkedList Dl;

    Dl.insertFrontDouble(5);
    Dl.insertFrontDouble(10);
    Dl.printForwardDouble(); //[10 <-> 5]
    Dl.printBackwardDouble(); //[5 <-> 10]

    Dl.insertAtDouble(1,7); 
    Dl.printForwardDouble(); //[10 <-> 7 <-> 5]

    Dl.insertBackDouble(20);
    Dl.insertBackDouble(30); 
    Dl.printForwardDouble(); // [10 <-> 7 <-> 5 <-> 20 <-> 30]
    Dl.printBackwardDouble(); //[30 <->20 <-> 5 <-> 7 <-> 10]

    Dl.insertAtDouble(3,28);
    Dl.printForwardDouble(); // [10 <-> 7 <-> 5 <-> 28 <-> 20 <-> 30]


    Dl.removeFrontDouble(); 
    Dl.printForwardDouble(); //[7 <-> 5 <-> 28 <-> 20 <-> 30]

    Dl.removeBackDouble();
    Dl.printForwardDouble(); //[7 <-> 5 <-> 28 <-> 20]

    Dl.removeAtDouble(1);
    Dl.printForwardDouble(); //[7 <-> 28 <-> 20]


    cout << "getAt(1) = " << Dl.getAtDouble(1) << endl; //getAt(1) = 28
    cout << "indexOf(20) = " << Dl.indexOfDouble(20) <<endl; // indexOf(20) = 2

    //Dl.clear();
    //Dl.printForward(); //[ ]

    //Stack--------------------------------------------------------
    cout << " -------Beginning of class Stack ------------" <<endl;
    Stack s;

    s.push(10);
    s.push(20);
    s.push(30);
    s.printForwardDouble(); //[30 <-> 20 <-> 10]

    cout << "Top: " << s.top() <<endl; // Top :30
    cout << "Size: " << s.sizeDouble() << endl; // 3

    s.pop();
    s.printForwardDouble(); // [20 <-> 10]
    cout << "Top after pop: " << s.top() << endl; // Top after pop: 20

    s.clear();
    cout << "Empty? " << (s.isEmpty() ? "yes" : "no") << endl;

    //Stack Test Again ///
    cout << "----------------Stack Test Again----------------- "<<endl;
    assert(validate()); // [60 <-> 50 <-> 40 <-> 30 <-> 20 <-> 10]
    cout << "All tests passed! \n";

    

    return 0;

}