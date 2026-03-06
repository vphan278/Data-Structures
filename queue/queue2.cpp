#include <iostream>
#include <queue>

using namespace std;

//Helper function to print entire queue
void printQueue(queue<int> q){
    cout << "[ ";
    while(!q.empty()){
        cout <<q.front() << ",";
        q.pop();
    }
    cout <<"]"<<endl;
}


int main(){

    //starting array
    int arr[] ={50, 60, 70};
    int n =3;

    //Create a queue
    queue<int> q;

    //Load array into the queue
    for(int i =0; i<n; i++){
        q.push(arr[i]);
    }

    cout << "After loading array: ";
    printQueue(q);  //[50,60,70,]

    cout << "Front: " << q.front() << endl; //Front: 50
    cout << "Back: " << q.back() << endl; //Back: 70
    cout <<"------------" <<endl;

    //Enqueue(add)
    q.push(85);
    cout << "After enqueue: ";
    printQueue(q); // After enqueue: [50,60,70,85]

    cout << "--------------"<<endl;

    // Check empty AFTER enqueue
    if (q.empty()) {
        cout << "Queue is empty" << endl;
    } else {
        cout << "Queue is NOT empty" << endl;
    }

    //Dequeue( remove from the front)
    q.pop();
    cout << "After dequeue: ";
    printQueue(q); //After dequeue: [60,70,85]

    cout << "Front: " << q.front() << endl; // Front: 60
    cout << "Back: " << q.back() << endl; // Back: 85

    return 0;

}