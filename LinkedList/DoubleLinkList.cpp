#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

       
class Node{

    public: 
        int value;     // stored value
        Node *prev = nullptr;  // points to a smaller value
        Node *next = nullptr;  // points to a larger value

        Node(int val) : value(val){}


};

//OrderedList class that manages Node objects
class OrderedList {
    private:
        Node* center = nullptr;

    public:
        OrderedList() = default;

        ~OrderedList() {
            clear();
        }

        void setCenter(int val){
            clear();
            center = new Node(val);
        }

        Node *getCenter()const{
            return center;
        }

        void insert(int val) {
            if (!center) {
                center = new Node(val);
                return;
            }

            Node* cur = center;

            while (true) {
                if (val < cur->value) {
                    if (cur->prev == nullptr || val >= cur->prev->value) {
                        Node* node = new Node(val);
                        Node* Left = cur->prev;

                        node->next = cur;
                        node->prev = Left;
                        cur->prev = node;
                        if (Left) Left->next = node;

                        return;
                    }
                    cur = cur->prev;
                } 
                else {
                    if (cur->next == nullptr || val < cur->next->value) {
                        Node* node = new Node(val);
                        Node* Right = cur->next;

                        node->prev = cur;
                        node->next = Right;
                        cur->next = node;
                        if (Right) Right->prev = node;

                        return;
                    }
                    cur = cur->next;
                }
            }
        }

        void print() const {
            if (!center) {
                cout << "nullptr\n";
                return;
            }

            Node* cur = center;
            while (cur->prev) cur = cur->prev;

            cout << "nullptr <-> ";
            while (cur) {
                cout << cur->value << " <-> ";
                cur = cur->next;
            }
            cout << "nullptr\n";
        }

        void clear() {
            if (!center) return;

            Node* cur = center;
            while (cur->prev) cur = cur->prev;

            while (cur) {
                Node* nxt = cur->next;
                delete cur;
                cur = nxt;
            }

            center = nullptr;
        }
};


void printLayer(Node *node, string prefix = "", bool isLast = true, string label = "Root"){

        if(!node) return;
        cout << prefix << (isLast ? "+--" : "|-- ");
        cout << (label == "Center" ? "Center: " : label == "Prev" ? "<-- " : "--> ");
        cout << node->value << endl;

        prefix +=isLast ? "   " : "|  ";
        bool hasPrev = node->prev !=nullptr;
        bool hasNext = node->next !=nullptr;
        if(hasPrev) printLayer(node->prev, prefix, !hasNext, "Prev");
        if(hasNext) printLayer(node->next, prefix, true, "Next");



}



int main(){

    OrderedList list;

    list.insert(10);
    list.print();  // nullptr <-> 10 <-> nullptr
    list.insert(20);
    list.print();  // nullptr <-> 10 <-> 20 <-> nullptr
    list.insert(7);
    list.print();  // nullptr <-> 7 <-> 10 <-> 20 <-> nullptr
    list.insert(15);
    list.print();  // nullptr <-> 7 <-> 10 <-> 20 <-> 15 <-> nullptr
    list.insert(9);
    list.print();  // nullptr <-> 9 <-> 7 <-> 10 <-> 20 <-> 15 <-> nullptr
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //Part 1
    //Generate 10 random numbers
    srand(time(nullptr));

    vector<int> values;
    for(int i=0; i < 10; i++){
        values.push_back(rand() % 100); // will give 0-99
    }

    // //Print the random numbers
    cout << "Random Numbers: ";
    for(int v: values){
        cout << v << " ";
    }
    cout <<endl;

    return 0;

}