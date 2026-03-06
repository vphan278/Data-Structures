#include <iostream>
#include <ostream>
#include <string>
#include <thread>
#include <vector>
#include <cmath>

//#include "ConsoleColor.h"

#ifndef CONSOLECOLOR_H
#define CONSOLECOLOR_H

#include <ostream>

// Minimal color tokens that work with: cout << yellow << "text";
inline std::ostream& yellow(std::ostream& os)   { return os; }
inline std::ostream& cyan(std::ostream& os)     { return os; }
inline std::ostream& white(std::ostream& os)    { return os; }
inline std::ostream& lavender(std::ostream& os) { return os; }
inline std::ostream& green(std::ostream& os)    { return os; }
inline std::ostream& red(std::ostream& os)      { return os; }
inline std::ostream& grey(std::ostream& os)     { return os; }
inline std::ostream& orange(std::ostream& os)   { return os; }

#endif

using namespace std;

int swaps = 0;
int passes = 0;
int compares = 0;

template <typename T>
void reset(vector<T>& theArray)
{
    ::swaps = 0;
    ::passes = 0;
    ::compares = 0;

    string animals[] =
    {
        "Zebra", "Xray", "Turtle", "Rabbit", "Monkey",
        "Kangaroo", "Giraffe", "Fox", "Elephant", "Bison"
    };

    theArray.clear();

    for (int i = 0; i < 10; i++)
        theArray.push_back(animals[i]);
}

template <typename T>
void Statistics(vector<T>& arr)
{
    for (int i = 0; i < arr.size(); i++)
        cout << yellow << '[' << arr[i] << ']' << " ";

    cout << endl;
    this_thread::sleep_for(350ms);

    cout << cyan << "Number of passes: " << white << ::passes
         << cyan << "\nNumber of swaps: " << white << ::swaps
         << cyan << "\nNumber of compares: " << white << ::compares
         << endl;

    int cpuExchanges = 40;
    int cpuCompares = 10;

    cout << lavender << "Weight: " << white
         << ::passes + (cpuExchanges * ::swaps) + (cpuCompares * ::compares)
         << endl;

    cout << white;
}

template <typename T>
void Swap(vector<T>& arr, int a, int b)
{
    T temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
    ::swaps++;
}

template <typename T>
void Exchange(vector<T>& theArray, int a, int b)
{
    Swap(theArray, a, b);

    for (int i = 0; i < theArray.size(); i++)
    {
        if (theArray[i] == theArray[a])
            cout << green << '[' << theArray[i] << ']' << " ";
        else if (theArray[i] == theArray[b])
            cout << red << '[' << theArray[i] << ']' << " ";
        else
            cout << grey << '[' << theArray[i] << ']' << " ";
    }

    cout << endl;
    this_thread::sleep_for(500ms);
}

template <typename T>
bool CompareGT(vector<T>& theArray, int a, int b)
{
    ::compares++;
    return theArray[a] > theArray[b];
}

template <typename T>
bool CompareLT(vector<T>& theArray, int a, int b)
{
    ::compares++;
    return theArray[a] < theArray[b];
}

/* ================= Bubble Sort ================= */

template <typename T>
void BubbleSort(vector<T>& arr)
{
    for (int i = 0; i < arr.size() - 1; i++)
    {
        for (int j = 0; j < arr.size() - i - 1; j++)
        {
            if (CompareGT(arr, j, j + 1))
                Exchange(arr, j, j + 1);
        }
        ::passes++;
    }
}

/* ================= Insertion Sort ================= */

template <typename T>
void InsertionSort(vector<T>& arr)
{
    for (int i = 1; i < arr.size(); i++)
    {
        int j = i - 1;

        while (j >= 0 && CompareGT(arr, j, j + 1))
        {
            Exchange(arr, j, j + 1);
            j--;
        }

        ::passes++;
    }
}

/* ================= Selection Sort ================= */

template <typename T>
void SelectionSort(vector<T>& arr)
{
    for (int i = 0; i < arr.size() - 1; i++)
    {
        int minIndex = i;

        for (int j = i + 1; j < arr.size(); j++)
        {
            if (CompareLT(arr, j, minIndex))
                minIndex = j;
        }

        if (i != minIndex)
            Exchange(arr, i, minIndex);

        ::passes++;
    }
}

/* ================= Shell Sort ================= */

template <typename T>
void ShellSort(vector<T>& arr)
{
    int n = arr.size();

    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            int j = i;

            while (j >= gap && CompareGT(arr, j - gap, j))
            {
                Exchange(arr, j - gap, j);
                j -= gap;
            }
        }

        ::passes++;
    }
}

/* ================= Quick Sort ================= */

template <typename T>
int Partition(vector<T>& arr, int low, int high)
{
    int pivotIndex = high;
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (CompareLT(arr, j, pivotIndex))
        {
            i++;
            Exchange(arr, i, j);
        }
    }

    Exchange(arr, i + 1, pivotIndex);
    return i + 1;
}

template <typename T>
void QuickSort(vector<T>& arr, int low, int high)
{
    if (low < high)
    {
        int pivot = Partition(arr, low, high);

        QuickSort(arr, low, pivot - 1);
        QuickSort(arr, pivot + 1, high);

        ::passes++;
    }
}

template <typename T>
void QuickSort(vector<T>& arr)
{
    QuickSort(arr, 0, arr.size() - 1);
}

/* ================= Merge Sort ================= */

template <typename T>
void Merge(vector<T>& arr, int left, int mid, int right)
{
    int i = left, j = mid + 1;
    vector<int> tempIndices;

    while (i <= mid && j <= right)
    {
        if (CompareLT(arr, i, j))
            tempIndices.push_back(i++);
        else
            tempIndices.push_back(j++);
    }

    while (i <= mid) tempIndices.push_back(i++);
    while (j <= right) tempIndices.push_back(j++);

    for (int k = 0; k < tempIndices.size(); k++)
        Exchange(arr, left + k, tempIndices[k]);
}

template <typename T>
void MergeSort(vector<T>& arr, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        MergeSort(arr, left, mid);
        MergeSort(arr, mid + 1, right);
        Merge(arr, left, mid, right);

        ::passes++;
    }
}

template <typename T>
void MergeSort(vector<T>& arr)
{
    MergeSort(arr, 0, arr.size() - 1);
}

/* ================= Heap + Intro Sort ================= */

template <typename T>
void Heapify(vector<T>& arr, int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && CompareGT(arr, left, largest)) largest = left;
    if (right < n && CompareGT(arr, right, largest)) largest = right;

    if (largest != i)
    {
        Exchange(arr, i, largest);
        Heapify(arr, n, largest);
    }
}

template <typename T>
void HeapSort(vector<T>& arr, int low, int high)
{
    int n = high - low + 1;

    for (int i = n / 2 - 1; i >= 0; i--)
        Heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--)
    {
        Exchange(arr, 0, i);
        Heapify(arr, i, 0);
    }
}

template <typename T>
void IntroSort(vector<T>& arr, int low, int high, int depthLimit)
{
    if (low < high)
    {
        if (depthLimit == 0)
        {
            HeapSort(arr, low, high);
            return;
        }

        int pivot = Partition(arr, low, high);

        IntroSort(arr, low, pivot - 1, depthLimit - 1);
        IntroSort(arr, pivot + 1, high, depthLimit - 1);
    }
}

template <typename T>
void IntroSort(vector<T>& arr)
{
    int depthLimit = 2 * log(arr.size());

    IntroSort(arr, 0, arr.size() - 1, depthLimit);
    InsertionSort(arr);
}

/* ================= MAIN ================= */

int main()
{
    vector<string> darray;

    cout << orange << "\n<BubbleSort>\n";
    reset(darray);
    BubbleSort(darray);
    Statistics(darray);

    cout << orange << "\n<InsertionSort>\n";
    reset(darray);
    InsertionSort(darray);
    Statistics(darray);

    cout << orange << "\n<SelectionSort>\n";
    reset(darray);
    SelectionSort(darray);
    Statistics(darray);

    cout << orange << "\n<ShellSort>\n";
    reset(darray);
    ShellSort(darray);
    Statistics(darray);

    cout << orange << "\n<QuickSort>\n";
    reset(darray);
    QuickSort(darray);
    Statistics(darray);

    cout << orange << "\n<MergeSort>\n";
    reset(darray);
    MergeSort(darray);   // ← fixed typo
    Statistics(darray);

    cout << orange << "\n<IntroSort>\n";
    reset(darray);
    IntroSort(darray);
    Statistics(darray);
}