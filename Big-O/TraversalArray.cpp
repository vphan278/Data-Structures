#include <iostream>
using namespace std;


//O(n)
int main(){

    int arr[] = {10,20,30,40,50};
    int n = 5;

    cout << "Array traversal: "; 

    for(int i = 0; i < n; i++){
        cout << arr[i] <<  ", ";
    }

    cout <<endl;

    return 0;

}