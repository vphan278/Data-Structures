#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

//Vinh Phan
//CIS-22C: C++ Data Structures and Algorithms
//CRN 33571
       
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

        void clearRec(Node * node) {
            if(!node) return;
            clearRec(node->prev);
            clearRec(node->next);
            delete node;
        }

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

        void insert(int val){
            if(!center){
                center = new Node(val);
                return;
            }
            Node *curr = center;

            while(true){
                if(val <curr->value){
                    if(curr->prev == nullptr){ // Only stop at nullptr
                        curr->prev = new Node(val);
                        return;
                    } 
                    curr = curr->prev;  // move left
                }
                else{
                    if(curr->next == nullptr){ // Only stop at nullptr
                        curr->next = new Node(val);
                        return;
                    }
                    curr = curr->next;  // move right
                }

            }
        }

        void insertSorted(int val){
            if(!center){
                center = new Node(val);
                return;
            }

            Node *curr = center;

            while(true){
                if(val < curr->value){
                    if(curr->prev == nullptr){  // Only stop at nullptr
                        curr->prev = new Node(val);
                        return;
                    }
                    curr = curr->prev; // move left
                } 
                else {
                    if(curr->next == nullptr){ //Only stop at nullptr
                        curr->next = new Node(val);
                        return;
                    }
                    curr = curr->next; //move right
                }
            }
        }
        

        void clear() {
            clearRec(center);
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

    //Unsorted Centered Insertion
    OrderedList part1;

    part1.setCenter(values[5]);

    //Insert remaining random numbers
    for(int i = 0; i < 10; i++){
        if( i == 5) continue;
        part1.insert(values[i]);
    }

    //Visualization 
    cout << "Part 1- Unsorted List: \n";
    printLayer(part1.getCenter(), "", true, "Center");

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Part 2
    vector<int> sortedValues = values;
    sort(sortedValues.begin(), sortedValues.end());

    //Print sorted values
    cout << "\nSorted Numbers: ";
    for(int v: sortedValues) cout << v << " ";
    cout << endl;

    //Sorted Centered Insertion
    OrderedList part2;
    part2.setCenter(sortedValues[5]);

    //Insert Numbers
    for(int i=0; i < 10; i++){
        if(i == 5) continue;
        part2.insertSorted(sortedValues[i]);
    }

    //Visualization
    cout << "Part 2 - Sorted List:\n";
    printLayer(part2.getCenter(), "", true, "Center");


    return 0;


}


//Technical Comparison ( 2-3 sentences)
//Even with the Random numbers generated, it stil followed the insertion rule of small on the left and larger on the right.
//But, the insertion order affects where the number could be place. For example, in Random, 90 is the center at index 5. 64 and 42 will go to the left. But, 46 and 80 will also
//go to the left. Space is taken up, so 80 won't go before the 62 and 42. The "Ordered List" won't follow the traditional list of descending order. 


 