#include <iostream>
#include <queue>
#include <vector>

using namespace std;


int main(){

    //Starting array
    int arr[] {10,20,30};
    int n = 3;

    //Create a Queue
    queue<int> q;

    //Load array into the queue(enqueue)
    for (int i = 0; i < n; i++) {
        q.push(arr[i]);
    }

    cout << "Initial queue laoded from array" << endl;
    cout << "Front: " << q.front() <<endl; //Front: 10
    cout << "Back: " << q.back() <<endl; //Back: 30

    //enqueue(add to back)
    q.push(45);
    cout << "After enqueue, back: " <<q.back()<<endl; // After enqueue, back: 45

    ////dequeue(remove from front
    q.pop();

     // 🔹 PRINT ENTIRE QUEUE
    queue<int> temp = q;   // copy queue

    cout << "Entire queue: ";
    while (!temp.empty()) {
        cout << temp.front() << ", "; // Entire queue: 20, 30, 45,
        temp.pop();
    }
    cout << endl;
    
    return 0;


}