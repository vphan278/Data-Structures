#include <iostream>
using namespace std;

//Vinh Phan
//CIS-22C: C++ Data Structures and Algorithms
//CRN 33571


//Doubly Linked List
class DNode{

    public:
        char data;
        DNode *prev;
        DNode *next;

        DNode(char val) : data(val), prev(nullptr), next(nullptr){}

};

DNode *buildDoublyList(){

    DNode *A = new DNode('A');
    DNode *B = new DNode('B');
    DNode *C = new DNode('C');
    DNode *D = new DNode('D');
    DNode *E = new DNode('E');

    A->next = C;
    B->prev = C; B->next = D;
    C->prev = A; C->next = B;
    D->prev = B; D->next = E;
    E->prev = D;

    return A; // head


}


void printDoublyList(DNode *head2){
    while(head2){
        cout << head2->data << " <-> ";
        head2 = head2->next;
    }
    cout << "null\n";

}




int main(){

    

    //Doubly Linked List
    DNode* head2 = buildDoublyList();  // build list
    printDoublyList(head2);            // print list
    
    return 0;
}