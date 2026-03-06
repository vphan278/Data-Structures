#include <iostream>
using namespace std;

// Function to perform Selection Sort
//O(n²) - is a Quadratic time algorithm(nested loops)
void SelectionSort(int arr[] , int n){

    for(int i = 0; i < n -1; i++){
        int minIndex = i;

        for(int j = i + 1; j < n; j++){
            if(arr[j] < arr[minIndex]){
                minIndex = j;
            }
        }
        swap(arr[i], arr[minIndex]);

    }
}

int main(){

    int arr[] = {64, 25, 12, 22, 11};
    int n = 5;

    SelectionSort(arr, n);

    //Print sorted array
    for(int i = 0; i < n; i++){
        cout << arr[i] << " , ";   // 11 , 12 , 22 , 25 , 64 ,
    }
    return 0;

}