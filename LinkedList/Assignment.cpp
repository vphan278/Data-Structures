#include <iostream>
using namespace std;

//Vinh Phan
//CIS-22C: C++ Data Structures and Algorithms
//CRN 33571


//Singly Linked List
class Node {

    public:
        char data;
        Node* next;
        Node(char val) : data(val), next(nullptr) {}
    
};

Node * buildList(){

    Node *A = new Node('A');
    Node *B = new Node('B');
    Node *C = new Node('C');
    Node *D = new Node('D');
    Node *E = new Node('E');

    A->next = B;
    B->next = C;
    C->next = D;
    D->next = E;

    return A;
}

void printList(Node *head){
    
    while(head){
        cout << head->data << " -> ";
        head = head->next;
    }
    cout << "null\n";

}

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

    //Singly Linked List
    Node *head = buildList();

    //Swap B and C   // 
    // (prev)A, prev->next(B), first->next(C), second->next(D)
    Node *prev = head;  // index at 0 (A)
    Node *first = prev->next; // index at 1(B)
    Node *second = first->next; //index at 2(C)
    Node *third = second->next; //index at 3(D)

    prev->next = second; //A -> C
    first->next = second->next;  // B -> D
    second->next = first; // C -> B

    printList(head);

    //Doubly Linked List
    DNode* head2 = buildDoublyList();  // build list
    printDoublyList(head2);            // print list
    
    return 0;
}