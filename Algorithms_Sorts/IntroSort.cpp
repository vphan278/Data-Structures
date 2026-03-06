#include <iostream>
#include <cmath>
using namespace std;

// ---------- Partition (Quick Sort part) ----------
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// ---------- Heap Sort Helper ----------
void heapify(int arr[], int n, int i, int low) {
    int largest = i;
    int left = 2 * (i - low) + 1 + low;
    int right = 2 * (i - low) + 2 + low;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest, low);
    }
}

void heapSort(int arr[], int low, int high) {
    int n = high + 1;

    for (int i = (low + high) / 2; i >= low; i--)
        heapify(arr, n, i, low);

    for (int i = high; i > low; i--) {
        swap(arr[low], arr[i]);
        heapify(arr, i, low, low);
    }
}

// ---------- Insertion Sort ----------
void insertionSort(int arr[], int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// ---------- IntroSort Recursive ----------
void introSortUtil(int arr[], int low, int high, int depthLimit) {
    int size = high - low + 1;

    // Use insertion sort for small partitions
    if (size < 16) {
        insertionSort(arr, low, high);
        return;
    }

    // If recursion too deep, use Heap Sort
    if (depthLimit == 0) {
        heapSort(arr, low, high);
        return;
    }

    // Otherwise, Quick Sort partition
    int p = partition(arr, low, high);
    introSortUtil(arr, low, p - 1, depthLimit - 1);
    introSortUtil(arr, p + 1, high, depthLimit - 1);
}

// ---------- IntroSort Main Wrapper ----------
void introSort(int arr[], int n) {
    int depthLimit = 2 * log(n);
    introSortUtil(arr, 0, n - 1, depthLimit);
}

// ---------- MAIN ----------
int main() {
    int arr[] = {10, 7, 8, 9, 1, 5, 3, 2};
    int n = 8;

    introSort(arr, n);

    cout << "Sorted array: ";
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";

    return 0;
}