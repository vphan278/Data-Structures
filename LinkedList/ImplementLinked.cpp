#include <iostream>
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


int main(){

    SingleLinkedList list;

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

    SingleLinkedList test;
    cout << test.sizeSingle() << endl;   // 0

    test.insertBackSingle(10);
    cout << test.sizeSingle() << endl;   // 1

    test.insertFrontSingle(20);
    cout << test.sizeSingle() << endl; // 2
    cout << "Size: " << list.sizeSingle() << endl;
    test.printSingle(); // 20 -> 10 -> null

    return 0;

}