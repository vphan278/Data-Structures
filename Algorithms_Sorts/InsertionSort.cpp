#include <iostream>
using namespace std;


//O(n²) - is a Quadratic time algorithm(nested loops)
//Always start at index 1, therefor int i = 1 
void InsertionSort(int arr[], int n){

    for(int i = 1; i < n; i++){
        int j = i - 1;

        //Shift elements to the right to make space
        while(j >= 0 && arr[j] > arr[j + 1]){
            swap(arr[j], arr[j + 1]);
            j--;
        }
    }
}

int main(){

    int arr[] = {5,2,4,6,1,3};
    int n = 6;

    InsertionSort(arr, n);

    //Print sorted array
    for(int i = 0; i < n; i++){
        cout << arr[i] << " , ";  // 1 , 2, 3 , 4, 5, 6 ,
    }

    return 0;

}