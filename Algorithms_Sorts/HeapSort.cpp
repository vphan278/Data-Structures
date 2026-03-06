#include <iostream>
using namespace std;

// Heapify function (sift down)
void heapify(int arr[], int n, int i) {
    int largest = i;       // assume root is largest
    int left = 2 * i + 1;   // left child index
    int right = 2 * i + 2;  // right child index

    // If left child is larger
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // If right child is larger
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // If largest is not root
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest); // recursive sift down
    }
}

// Heap Sort function
void heapSort(int arr[], int n) {

    // 1) Build max heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // 2) Extract elements one by one
    for (int end = n - 1; end > 0; end--) {
        swap(arr[0], arr[end]);   // move max to end
        heapify(arr, end, 0);     // restore heap
    }
}

// Print array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}

// MAIN
int main() {
    int arr[] = {11, 5, 3, 8, 2, 6, 1, 4, 9, 7};
    int n = sizeof(arr) / sizeof(arr[0]);

    cout << "Original array:\n";
    printArray(arr, n);  // 11 5 3 8 2 6 1 4 9 7 

    heapSort(arr, n);

    cout << "Sorted array:\n";
    printArray(arr, n);  // 1 2 3 4 5 6 7 8 9 11

    return 0;
}