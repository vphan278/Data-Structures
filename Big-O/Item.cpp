#include <iostream>
using namespace std;

//O(1) is a constant time algorithm
//Access Arrays index, push/pop stack, Queue enqueue/dequeue

void printItem(){

   int x = 41;
   cout << "x: " << x <<endl; // x: 41

}

int main(){

    printItem();

    //----
    int arr[] = {10, 20, 30, 40, 50};

    cout <<  "arr[2]: " << arr[2] << endl;  // arr[2]: 30

    return 0;
}