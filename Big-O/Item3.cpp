#include <iostream>
using namespace std;

//O(n log n) - a Log Linear algorithm : cuts the problem in half AND process all n items
//Merge sort, Heap sort, average Quick sort

void printItems(int n) {

    for (int i = 0; i < n; i++) {   // O(n)
        int j = n;

        while (j > 1) {             // O(log n)
            cout << "i = " << i << ", j = " << j << endl;
            j = j / 2;
        }
    }
}


int main(){

    printItems(3); 

    //Output 
    //i = 0, j = 3
    //i = 1, j = 3
    //i = 2, j = 3

    return 0;
}