#include <iostream>
using namespace std;

//O(n²) - is a Quadratic time algorithm(nested loops)
// Bubble sort, Selection sort, Insertion sort

void printItems(int n){
    for(int i=0; i<n; i++){
        for(int j =0; j <n; j++){
            cout << i << "," << j <<endl;
        }
    }   
}

int main(){
    printItems(3);

    return 0;
}

//0,0
//0,1
//0,2
//1.0
//1,1
//1,2
//2,0
//2,1
//2,2