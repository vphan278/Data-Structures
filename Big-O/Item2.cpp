#include <iostream>
using namespace std;

//O(n)- means is  a linear time algorithm
// Traverse array or linked list


void printItems(int a, int b){

    for(int i =0; i<a; i++){
        cout << i <<endl; //0 1 2 3 4 5
    }

    for(int j=0; j<b; j++){
        cout << j <<endl; //0 1 2 3 4 5 
    }

}

int main(){
    printItems(5,5);

    return 0;
}