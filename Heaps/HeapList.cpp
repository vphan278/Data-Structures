#include <iostream>
#include <stdexcept>
using namespace std;


//Vinh Phan
//CIS-22C: C++ Data Structures and Algorithms
//CRN 33571

enum ListOrder { 
    ORDER_MIN, 
    ORDER_MAX 
};

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
        int size;
        ListOrder order;

    public:
        HeapList(const int* arr, int n, ListOrder mode)
            : head(nullptr), tail(nullptr), size(0), order(mode) {

            // 1) build DLL in input order
            for(int i = 0; i < n; i++){
                Node *node = new Node(arr[i]);

                if(!head){
                    head = tail = node;
                }
                else{
                    tail->next = node;
                    node->prev = tail;
                    tail = node;
                }
                size++;
            }
            reorderList();   
        }

        ~HeapList() {
            Node* cur = head;
            while (cur) {
                Node* nxt = cur->next;
                delete cur;
                cur = nxt;
            }
        }

        // Add a new value 
        void add(int value) {
            Node* node = new Node(value);
            appendNode(node);
    
            // bubble up from the last position
            bubbleUp(size -1);
        }

        int front() const {
            if (!head) throw runtime_error("HeapList is empty");
            return head->value;
        }

        int extractFront() {
            if (!head) throw runtime_error("HeapList is empty");

            int out = head->value;

            if (size == 1) {
                delete head;
                head = tail = nullptr;
                size = 0;
                return out;
            }

            // swap head node with tail node by POINTERS (not values)
            swapNodesByPointer(head, tail);

            // now the old head is at the tail position; delete tail
            Node* toDelete = tail;
            tail = tail->prev;
            tail->next = nullptr;

            delete toDelete;
            size--;

            // restore order by bubbling down from the head
            bubbleDown(0);

            return out;
        }

        void printList() const{
            Node *curr = head;

            cout << "[";
            while(curr){
                cout << curr->value;
                if(curr->next) cout << ", ";
                curr = curr->next;
            }
            cout << "]\n";
        }


        void printGroups() const{
            Node *curr = head;
            int groupSize = 1;
            while (curr) {
                for (int i = 0; i < groupSize && curr; i++) {
                    cout << curr->value;
                    curr = curr->next;
                    if (i < groupSize - 1 && curr) cout << " ";
                }
                cout << "\n";
                groupSize *= 2;
            }
        }


    private: //Heaper Functions

        //Return node at offset from the base node
        Node *getNodeAtOffset(int offset) const{
            Node *curr = head;
            while(curr && offset > 0){
                curr = curr->next;
                offset--;
            }
            return curr;
        }

        //Compare twp values according to ORDER_MIN OR ORDER_MAX
        bool inCorrectOrder(int a, int b) const{
            if(order == ORDER_MAX) return a >= b;
            return a<= b; //ORDER_MIN
        }

        void swapNodesByPointer(Node *a, Node *b){
            if(!a || !b || a == b) return;

            //Ensure a comes before b in the list
            Node *curr = a;
            bool aBeforeB = false;
            while (curr) {
                if (curr == b) { aBeforeB = true; break; }
                curr = curr->next;
            }
            if(!aBeforeB){
                Node *temp = a; a = b; b = temp;
            }

            //adjacent: a <-> b
            if(a->next == b){
                Node *ap = a->prev;
                Node *bn = b->next;

                if(ap) ap->next = b; else head = b;
                if(bn) bn->prev = a; else tail = a;

                b->prev = ap;
                b->next = a;
                a->prev = b;
                a->next = bn;
                return;
            }

            //non-adjacent
            Node *ap = a->prev;
            Node *an = a->next;
            Node *bp = b->prev;
            Node *bn = b->next;

            if(ap) ap->next = b; else head = b;
            if(an) an->prev = b;

            if(bp) bp->next = a; else head = a;
            if(bn) bn->prev = a; else tail = a;

            b->prev = ap; b->next = an;
            a->prev = bp; a->next = bn;

            //Fix tail
            if(!b->next) tail = b;
            if(!a->next) tail = a;
        }

        //Append
        void appendNode(Node* node) {
            if (!head) {
                head = tail = node;
            } 
            else {
                tail->next = node;
                node->prev = tail;
                tail = node;
            }
            size++;
        }

        //Move a node upward based on ordering rules
        void bubbleUp(int index) {
            Node* curr = getNodeAtOffset(index);

            while (curr && curr->prev) {
                if (inCorrectOrder(curr->prev->value, curr->value)) break;
                swapNodesByPointer(curr->prev, curr);
                // curr pointer remains the same node, now earlier in list
            }
        }

        //Move a node downward based on ordering rules
        void bubbleDown(int index) {
            Node* curr = getNodeAtOffset(index);

            while (curr) {
                Node* c1 = curr->next;
                if (!c1) break;
                Node* c2 = c1->next;

                Node* best = c1;
                if (c2 && !inCorrectOrder(c1->value, c2->value)) best = c2;

                if (inCorrectOrder(curr->value, best->value)) break;

                swapNodesByPointer(curr, best);
                // curr is same node, now later; continue pushing down
            }
        }

        //Reorder the entire list after construction
        void reorderList(){
            if (!head || !head->next) return;

                // Find middle and its index
                Node* slow = head;
                Node* fast = head;
                int midIndex = 0;

                while (fast && fast->next) {
                    fast = fast->next->next;
                    slow = slow->next;
                    midIndex++;
                }

                // Heapify from middle index down to 0
                for (int i = midIndex; i >= 0; --i) {
                    bubbleDown(i);
                }
        }
};


int main(){

    //Task 1 - Build HeapList
    int arr[] = {50, 30, 40, 10, 20, 60, 5, 70, 15};

    //ORDER_MAX
    HeapList maxList(arr, 9, ORDER_MAX);

    cout << "ORDER_MAX List: ";
    maxList.printList(); //ORDER_MAX List: [70, 60, 50, 40, 20, 30, 5, 15, 10]

    cout << "Groups:\n";
    maxList.printGroups();  
    //Groups:
    //70
    //60 50
    //40 3 2 5
    //15 10

    cout << "Front: " << maxList.front() << "\n"; // Front: 70
    cout << "ExtractFront: " << maxList.extractFront() << "\n"; //ExtractFront: 70

    cout << "After extract: ";
    maxList.printList(); //After extract: [60, 50, 40, 30, 20, 15, 5, 10]

    //ORDER_MIN
    HeapList minList(arr, 9, ORDER_MIN);
    cout << "ORDER_MIN List: ";
    minList.printList(); //ORDER_MIN List: [5, 10, 15, 20, 30, 60, 40, 70, 50]

    //Task 2 - Insert
    //ORDER_MAX
    cout << "------INSERT (ORDER_MAX)------\n";
    maxList.add(65);
    maxList.printList(); //[65, 60, 50, 40, 30, 20, 15, 5, 10]
    cout << "Groups:\n";
    maxList.printGroups();
    //65,
    //60 50
    //40 30 20 15
    //5 10
    cout << "Front now: " << maxList.front() << "\n"; //Front now: 65

    //ORDER_MIN
    cout << "------INSERT (ORDER_MIN)------\n";
    minList.add(1);
    minList.printList(); //[1, 5, 10, 15, 20, 30, 60, 40, 70, 50]
    cout << "Groups:\n";
    minList.printGroups();
    //1
    //5 10
    //15 20 30 60
    //40 70 50
    cout << "Front now: " << minList.front()<< "\n"; //Front now: 1


    return 0;

}