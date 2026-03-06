#include <iostream>
using namespace std;

//O(n)
//Node Structure
struct Node{
    int data;
    Node *next;

};

int main(){

    //Create Linked List: 10 -> 20 -> 30

    Node *head = new Node{10, nullptr};
    head->next = new Node{20, nullptr};
    head->next->next = new Node{30, nullptr};

    cout << "Linked list traversal: ";

    //Traverse the List
    Node *curr = head;

    while(curr !=nullptr){
        cout << curr->data << ", ";//Linked list travesal: 10, 20, 30,
        curr = curr->next;
    }

    cout <<endl;


    return 0;

}


