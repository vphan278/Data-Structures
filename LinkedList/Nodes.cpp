#include <iostream>
using namespace std;

//Step 1: Define a Node
struct Node{
    int data;
    Node *next;

};

int main(){

    //Step 2: Create nodes
    Node *head = new Node{40, nullptr};
    head->next = new Node{50, nullptr};
    head->next->next = new Node{60, nullptr};
    head->next->next->next = new Node{70, nullptr};

    //--------------------------------------------------------------//
    //Traverse the list
    cout << "Linked List: " <<endl;

    Node *curr = head;

    while(curr !=nullptr){
        cout << curr->data << ", ";  //Linked List: 40,50,60,70
        curr = curr->next;
    }
    cout <<endl;
    

    //-------------------------------------------------------------//
    //INSERT AT HEAD-after while loop
    Node *newNode = new Node{30, head};
    head = newNode;

    cout << "After insert at head: " <<endl;
    curr = head;

    while(curr !=nullptr){
        cout << curr->data << ", ";  //After insert at head: 30, 40,50,60,70
        curr = curr->next;
    }
    cout <<endl;


    //------------------------------------------------------------//
    //INSERT AT TAIL
    Node* newTail = new Node{80, nullptr};
    curr = head;

    while (curr->next != nullptr) {
        curr = curr->next;
    }

    curr->next = newTail;

    cout << "After insert at tail:" << endl;
    curr = head;

    while (curr != nullptr) {
        cout << curr->data << ", "; //After insert at tail: 30, 40, 50, 60, 70, 80
        curr = curr->next;
    }
    cout << endl;

    //------------------------------------------------------------//
    //Remove at head
    
    if(head !=nullptr){
        Node *temp = head; //save old head
        head = head->next; // move head forward
        delete temp;  //free memory
    }

    cout << "After remove at head: " <<endl;
    curr = head;

    while(curr !=nullptr){
        cout << curr->data << "; ";//After remove at head: 40, 50, 60, 70, 80,
        curr = curr->next;
    }
    cout <<endl;


    //-------------------------------------------------------------//
    //Remove at tail
    curr = head;

    while(curr->next->next !=nullptr){
        curr = curr->next;
    }
    delete curr->next; // delete last node
    curr->next = nullptr;
    
    cout << "After remove at tail: "<<endl;
    curr = head;

    while(curr != nullptr){
        cout << curr->data << ", "; //After remove at tail: 40, 50, 60, 70,
        curr = curr->next;
    }
    cout <<endl;


    //------------------------------------------------------------------------------//
    //INSERT at index 1(after head)
    Node *mid = new Node{45, nullptr};

    mid->next = head->next;
    head->next = mid;

    cout << "After insert at index 1: " <<endl;
    curr  = head;

    while(curr !=nullptr){
        cout << curr->data << ", "; //After insert at index 1: 40, 45, 50, 60, 70
        curr = curr->next;
    }
    cout <<endl;

    //---------------------------------------------------------------------//
    //INSERT before tail(second to last position)

    Node *beforeTail = new Node{65, nullptr};
    curr = head;

    //stop at second to last
    while(curr->next->next !=nullptr){
        curr = curr->next;
    }

    //insert new node
    beforeTail->next = curr->next;
    curr->next = beforeTail;

    cout << "After insert before tail: " << endl;
    curr = head;

    while (curr != nullptr) {
        cout << curr->data << ", "; // After insert before tail: 40,45,50,60,65,70
        curr = curr->next;
    }
    cout << endl;



    return 0;

}