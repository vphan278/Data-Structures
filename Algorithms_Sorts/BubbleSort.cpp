#include <iostream>
using namespace std;

// 0   1   2   3   4
// 10  20  30  40  50
//O(n²) - is a Quadratic time algorithm(nested loops)
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {           // outer loop
        for (int j = 0; j < n - i - 1; j++) {   // inner loop
            if (arr[j] > arr[j + 1]) {          // adjacent comparison
                swap(arr[j], arr[j + 1]);      // adjacent swap
            }
        }
    }
}

int main() {
    int arr[] = {8, 1, 5, 2, 4};
    int n = 5;

    bubbleSort(arr, n);

    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }

    //After pass 1: largest is always at the end: { 1, 5, 2, 4, 8}
    //After pass 2: next largest is second to the last:  { 1, 2, 4 , 5, 8}

    return 0;
}