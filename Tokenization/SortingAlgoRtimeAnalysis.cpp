#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <set>
using namespace std;

// ----------Millisecond Clock----------------------------------
class Clock : public chrono::steady_clock {
    time_point start;
    time_point stop;

    public:
        Clock() {}
        time_point Start() {
            start = chrono::steady_clock::now();
            return start;
        }
        double Stop() {
            stop = chrono::steady_clock::now();
            chrono::duration<double, std::centi> diff = stop - start; // centiseconds
            return diff.count();
        }
};

// ----------------Sorting algorithms----------------
// Bubble -  O(n^2)
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) swap(arr[j], arr[j + 1]);
        }
    }
}

// Selection -  O(n^2)
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) minIndex = j;
        }
        swap(arr[i], arr[minIndex]);
    }
}

// Insertion -  O(n^2)
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j]; //shift right
            j--;
        }
        arr[j + 1] = key;
    }
}

//ShellSort - O(n^1.5), worst O(n^2)
void shellSort(int arr[], int n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

//HeapSort  - O(n log n)
// Heapify function (sift down)
void heap(int arr[], int n, int i) {
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
        heap(arr, n, largest); // recursive sift down
    }
}

// Heap Sort function
void heapSort(int arr[], int n) {

    // 1) Build max heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heap(arr, n, i);
    }

    // 2) Extract elements one by one
    for (int end = n - 1; end > 0; end--) {
        swap(arr[0], arr[end]);   // move max to end
        heap(arr, end, 0);     // restore heap
    }
}

// MergeSort -  O(n log n)
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else              arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// QuickSort - O(n log n)
int partition(int arr[], int low, int high) {
    int pivot = arr[high];  // Choose the last element as pivot
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);  // Swap smaller element with pivot location
        }
    }
    std::swap(arr[i + 1], arr[high]); // Move pivot to correct position
    return i + 1;
}

// QuickSort function (recursive)
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        
        quickSort(arr, low, pivotIndex - 1);  // Sort left side
        quickSort(arr, pivotIndex + 1, high); // Sort right side
    }
}


//IntroSort - O(n log n)
// Partition function for QuickSort
int partitionintro(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

// QuickSort function with recursion depth tracking
void introquickSort(int arr[], int low, int high, int depthLimit) {
    if (low < high) {
        if (depthLimit == 0) {
            // Switch to HeapSort when recursion depth exceeds threshold
            std::make_heap(arr + low, arr + high + 1);
            std::sort_heap(arr + low, arr + high + 1);
            return;
        }
        
        int pivotIndex = partitionintro(arr, low, high);
        introquickSort(arr, low, pivotIndex - 1, depthLimit - 1);
        introquickSort(arr, pivotIndex + 1, high, depthLimit - 1);
    }
}


void introSort(int arr[], int size) {
    int depthLimit = 2 * log2(size); // Depth limit for QuickSort
    introquickSort(arr, 0, size - 1, depthLimit);
}

//////////////////////////////////////////////////////////////////////
vector<int>readFile(const string& filename){
    ifstream file(filename);
    vector<int> data;
    int x;

    if (!file) {
        cout << "ERROR: Cannot open " << filename << endl;
        return data;
    }

    while(file >> x){
        data.push_back(x);
    }
    return data;
}

double GapsBubble=0, GapsS=0,  GapsI=0,  GapsSh=0; 
double GapsH=0,      GapsM=0,  GapsQ=0,  GapsIntr=0;

double PartBubble=0, PartS=0,  PartI=0,  PartSh=0;
double PartH=0,      PartM=0,  PartQ=0,  PartIntr=0;

double PivBubble=0,  PivS=0,   PivI=0,   PivSh=0;
double PivH=0,       PivM=0,   PivQ=0,   PivIntr=0;

double RandBubble=0, RandS=0,  RandI=0,  RandSh=0;
double RandH=0,      RandM=0,  RandQ=0,  RandIntr=0;

double SemiBubble=0, SemiS=0,  SemiI=0,  SemiSh=0;
double SemiH=0,      SemiM=0,  SemiQ=0,  SemiIntr=0;

//////////////////////////////////////////////////////////////////////

int main() {
    vector<string> files = {"Gaps.txt","Partition.txt","Pivot.txt","Random.txt","SemiSorted.txt"};

    //---------------------Gaps.txt----------------------------------------//
    { //Hard code 
        int arr[] = {472, 108, 689, 351, 765};
        int n = 5;
        

        //Step 1
        vector<int> original(arr, arr + n);
        //Step 2
        vector<int> copy = original;

        //Sort copy
        bubbleSort(copy.data(), n);

        cout << "Bubble Sorted Array(Hard Code)- O(n^2): ";
        for(int i = 0; i < n; i++){
            cout << "[" << copy[i] << "]," ; //Bubble Sorted Array: [108],[351],[472],[689],[765],
        }
    

    }

    cout<<endl;

    //bubbleSort  : Gaps.txt (Print sorted array)
    {
        vector<int> original = readFile("Gaps.txt");
        int n = original.size();

        //Make a copy of Gaps.txt data
        vector<int> copy = original;

        bubbleSort(copy.data(), n); 

        //Print sorted output
        cout << "Bubble Sort- Gaps.txt: " <<endl;
        for(int x: copy){
            cout << x << " ";
        }
        cout << endl;

    }

    cout << "-------------Sort---Gaps.txt--------------"<<endl;
    //bubbleSort : Gaps.txt
    {
        vector<int> original = readFile("Gaps.txt");
        int n = original.size();

        //Make a copy of Gaps.txt data
        vector<int> copy = original;
    
        //Runtime Analysis of bubbleSort
        Clock clock;
        clock.Start();
        bubbleSort(copy.data(), n);
        double duration = clock.Stop();

        GapsBubble = duration;
        cout << "O(n^2)      -  Bubble Sort RunTime: " << duration << " centiseconds" <<endl; // Bubble Sort RunTime: 0.00057


    }

    
    //selectionSort : Gaps.txt
    {
        //Read from Gaps.txt
        vector<int> original = readFile("Gaps.txt");
        int n = original.size();

        //Make a copy of Gaps.txt data
        vector<int> copy = original;

        //Runtime Analysis of selectionSort
        Clock clock;
        clock.Start();
        selectionSort(copy.data(), n);
        double duration = clock.Stop();

        GapsS = duration;
        cout << "O(n^2)      -  Selection Sort Runtime: " << duration << " centiseconds" << endl; // O(n^2)- selectionSort Runtime:: 0.00051 centiseconds

    }

    //InsertionSort: Gaps.txt
    {
        //Read from Gaps.txt
        vector<int> original = readFile("Gaps.txt");
        int n = original.size();

        //Make a copy of Gaps.txt data
        vector<int> copy = original;


        //Runtime Analysis of insertionSort
        Clock clock;
        clock.Start();
        insertionSort(copy.data(), n);
        double duration = clock.Stop();

        GapsI = duration;
        cout << "O(n^2)      -  Insertion Sort Runtime: " << duration << " centiseconds" << endl; // O(n^2) - Insertion Sort Runtime: 2e-05 centiseconds

    }

    //ShellSort: Gaps.txt
    {
        //Read from Gaps.txt
        vector<int> original = readFile("Gaps.txt");
        int n = original.size();

        //Make a copy of Gaps.txt data 
        vector<int> copy = original;

        //Runtime Analysis of ShellSort 
        Clock clock;
        clock.Start();
        shellSort(copy.data(), n);
        double duration = clock.Stop();

        GapsSh = duration;
        cout << "O(n^1.5)  -  ShellSort Runtime: " << duration << " centiseconds" << endl; // ShellSort Runtime: 0.00058 centisecond

    }

    //HeapSort
    {
        //Read from Gaps.txt
        vector<int> original = readFile("Gaps.txt");
        int n = original.size();

        //Make a copy of Gaps.txt data 
        vector<int> copy = original;

        //Runtime Analysis of HeapSort 
        Clock clock;
        clock.Start();
        heapSort(copy.data(), n);
        double duration = clock.Stop();
        
        GapsH = duration;
        cout << "O(n log n)  -  HeapSort Runtime: " << duration << " centiseconds" << endl; //HeapSort Runtime: 0.00086 centiseconds

    }

    //MergeSort: Gaps.txt
    {
        //Read from Gaps.txt
        vector<int> original = readFile("Gaps.txt");
        int n = original.size();

        //Make a copy of Gaps.txt data 
        vector<int> copy = original;

        //Runtime Analysis of MergeSort 
        Clock clock;
        clock.Start();
        mergeSort(copy.data(), 0, n - 1);
        double duration = clock.Stop();

        GapsM = duration;
        cout << "O(n log n)  -  MergeSort Runtime: " << duration << " centiseconds" << endl; // MergeSort Runtime: 0.00208 centiseconds
    }

    //QuickSort: Gaps.txt
    {
        //Read from Gaps.txt
        vector<int> original = readFile("Gaps.txt");
        int n = original.size();

        //Make a copy of Gaps.txt data 
        vector<int> copy = original;

        //Runtime Analysis of QuickSort
        Clock clock;
        clock.Start();
        quickSort(copy.data(), 0, n - 1);
        double duration = clock.Stop();

        GapsQ = duration;
        cout << "O(n log n)  -  QuickSort Runtime: "  << duration << " centiseconds" << endl; // QuickSort Runtime: 0.00049 centiseconds

    }

    //IntroSort: Gaps.txt
    {
        //Read from Gaps.txt
        vector<int> original = readFile("Gaps.txt");
        int n = original.size();

        //Make a copy of Gaps.txt data 
        vector<int> copy = original;

        //Runtime Analysis of IntroSort
        Clock clock;
        clock.Start();
        introSort(copy.data(), n);
        double duration = clock.Stop();

        GapsIntr = duration;
        cout << "O(n log n)  -  introSort Runtime: "  << duration << " centiseconds" << endl; //O(n log n)  -  introSort Runtime: 0.00115 centiseconds
    }

    //----------------------Partition.txt----------------------------//
    cout << "--------------Sort---Partition.txt----------------"<<endl;

    //BubbleSort: Partition.txt
    {
        //Read from Partition.txt
        vector<int> original = readFile("Partition.txt");
        int n = original.size();

        //Make a copy of Partition.txt data
        vector<int> copy = original;

        bubbleSort(copy.data(), n); 

        //Print sorted output
        cout << "Bubble Sort- Partition.txt: " <<endl;
        for(int x: copy){
            cout << x << " ";
        }
        cout << endl;

    }

    cout << "--------------Sort---Partition.txt-------------"<<endl;
    //bubbleSort : Partition.txt
    {
        vector<int> original = readFile("Partition.txt");
        int n = original.size();

        //Make a copy of Partition.txt data
        vector<int> copy = original;
    
        //Runtime Analysis of bubbleSort
        Clock clock;
        clock.Start();
        bubbleSort(copy.data(), n);
        double duration = clock.Stop();

        PartBubble = duration;
        cout << "O(n^2)      -  Bubble Sort RunTime: " << duration << " centiseconds" <<endl; // O(n^2)      -  Bubble Sort RunTime: 0.00221 centiseconds
    }
    
    //selectionSort : Partition.txt
    {
        //Read from Partition.txt
        vector<int> original = readFile("Partition.txt");
        int n = original.size();

        //Make a copy of Partition.txt data
        vector<int> copy = original;

        //Runtime Analysis of selectionSort
        Clock clock;
        clock.Start();
        selectionSort(copy.data(), n);
        double duration = clock.Stop();

        PartS = duration;
        cout << "O(n^2)      -  Selection Sort Runtime: " << duration << " centiseconds" << endl; //O(n^2)      -  Selection Sort Runtime: 0.00095 centiseconds

    }

    //InsertionSort: Partition.txt
    {
        //Read from Partition.txt
        vector<int> original = readFile("Partition.txt");
        int n = original.size();

        //Make a copy of Partition.txt data
        vector<int> copy = original;


        //Runtime Analysis of insertionSort
        Clock clock;
        clock.Start();
        insertionSort(copy.data(), n);
        double duration = clock.Stop();

        PartI = duration;
        cout << "O(n^2)      -  Insertion Sort Runtime: " << duration << " centiseconds" << endl; // O(n^2)      -  Insertion Sort Runtime: 0.00055 centiseconds

    }

    //ShellSort: Partition.txt
    {
        //Read from Partition.txt
        vector<int> original = readFile("Partition.txt");
        int n = original.size();

        //Make a copy of Partition.txt data 
        vector<int> copy = original;

        //Runtime Analysis of ShellSort
        Clock clock;
        clock.Start();
        shellSort(copy.data(), n);
        double duration = clock.Stop();

        PartSh = duration;
        cout << "O(n^1.5)  -  ShellSort Runtime: " << duration << " centiseconds" << endl; // O(n log n)  -  ShellSort Runtime: 0.00052 centiseconds

    }

    //HeapSort
    {
        //Read from Partition.txt
        vector<int> original = readFile("Partition.txt");
        int n = original.size();

        //Make a copy of Partition.txt data 
        vector<int> copy = original;

        //Runtime Analysis of HeapSort 
        Clock clock;
        clock.Start();
        heapSort(copy.data(), n);
        double duration = clock.Stop();
        
        PartH = duration;
        cout << "O(n log n)  -  HeapSort Runtime: " << duration << " centiseconds" << endl; //O(n log n)  -  HeapSort Runtime: 0.00082 centiseconds

    }

    //MergeSort: Partition.txt
    {
        //Read from Partition.txt
        vector<int> original = readFile("Partition.txt");
        int n = original.size();

        //Make a copy of Partition.txtt data 
        vector<int> copy = original;

        //Runtime Analysis of MergeSort 
        Clock clock;
        clock.Start();
        mergeSort(copy.data(), 0, n - 1);
        double duration = clock.Stop();

        PartM = duration;
        cout << "O(n log n)  -  MergeSort Runtime: " << duration << " centiseconds" << endl; //O(n log n)  -  MergeSort Runtime: 0.00718 centiseconds
    }

     //QuickSort: Partition.txt
    {
        //Read from Partition.txt
        vector<int> original = readFile("Partition.txt");
        int n = original.size();

        //Make a copy of Partition.txt data 
        vector<int> copy = original;

        //Runtime Analysis of QuickSort 
        Clock clock;
        clock.Start();
        quickSort(copy.data(), 0, n - 1);
        double duration = clock.Stop();

        PartQ = duration;
        cout << "O(n log n)  -  QuickSort Runtime: "  << duration << " centiseconds" << endl; // O(n log n)  -  QuickSort Runtime: 0.00051 centiseconds

    }

     //IntroSort: Partition.txt
    {
        //Read from Partition.txt
        vector<int> original = readFile("Partition.txt");
        int n = original.size();

        //Make a copy of Partition.txt data 
        vector<int> copy = original;

        //Runtime Analysis of IntroSort
        Clock clock;
        clock.Start();
        introSort(copy.data(), n);
        double duration = clock.Stop();

        PartIntr = duration;
        cout << "O(n log n)  -  introSort Runtime: "  << duration << " centiseconds" << endl; //O(n log n)  -  introSort Runtime: 0.00056 centiseconds



    }

    //-----------------------Pivot.txt------------------------------//
    cout << "--------------Sort--Pivot.txt-----------------"<<endl;

     //BubbleSort: Pivot.txt
    {
        //Read from Pivot.txt
        vector<int> original = readFile("Pivot.txt");
        int n = original.size();

        //Make a copy of Pivot.txt data
        vector<int> copy = original;

        bubbleSort(copy.data(), n); 

        //Print sorted output
        cout << "Bubble Sort- Pivot.txt: " <<endl;
        for(int x: copy){
            cout << x << " ";
        }
        cout << endl;

    }

    cout << "---------------Sort--Pivot.txt----------------"<<endl;
    //bubbleSort : Pivot.txt

    {
        vector<int> original = readFile("Pivot.txt");
        int n = original.size();

        //Make a copy of Pivot.txt data
        vector<int> copy = original;
    
        //Runtime Analysis of bubbleSort
        Clock clock;
        clock.Start();
        bubbleSort(copy.data(), n);
        double duration = clock.Stop();

        PivBubble = duration;
        cout << "O(n^2)      -  Bubble Sort RunTime: " << duration << " centiseconds" <<endl; // O(n^2)      -  Bubble Sort RunTime: 0.00295 centiseconds
    
    }

    //selectionSort : Pivot.txt
    {
        //Read from Pivot.txt
        vector<int> original = readFile("Pivot.txt");
        int n = original.size();

        //Make a copy of Pivot.txt data
        vector<int> copy = original;

        //Runtime Analysis of selectionSort
        Clock clock;
        clock.Start();
        selectionSort(copy.data(), n);
        double duration = clock.Stop();

        PivS = duration;
        cout << "O(n^2)      -  Selection Sort Runtime: " << duration << " centiseconds" << endl; // O(n^2)      -  Selection Sort Runtime: 0.00092 centiseconds

    }
    
    //InsertionSort: Pivot.txt
    {
        //Read from Pivot.txt
        vector<int> original = readFile("Pivot.txt");
        int n = original.size();

        //Make a copy of Pivot.txt data
        vector<int> copy = original;


        //Runtime Analysis of insertionSort
        Clock clock;
        clock.Start();
        insertionSort(copy.data(), n);
        double duration = clock.Stop();

        PivI = duration;
        cout << "O(n^2)      -  Insertion Sort Runtime: " << duration << " centiseconds" << endl; // O(n^2)      -  Insertion Sort Runtime: 0.00048 centiseconds

    }

    //ShellSort: Pivot.txt
    {
        //Read from Pivot.txt
        vector<int> original = readFile("Pivot.txt");
        int n = original.size();

        //Make a copy of Pivot.txt data 
        vector<int> copy = original;

        //Runtime Analysis of ShellSort
        Clock clock;
        clock.Start();
        shellSort(copy.data(), n);
        double duration = clock.Stop();

        PivSh = duration;
        cout << "O(n^1.5)  -  ShellSort Runtime: " << duration << " centiseconds" << endl; //O(n log n)  -  ShellSort Runtime: 0.00057 centiseconds

    }

    //HeapSort
    {
        //Read from Pivot.txt
        vector<int> original = readFile("Pivot.txt");
        int n = original.size();

        //Make a copy of Pivot.txt data 
        vector<int> copy = original;

        //Runtime Analysis of HeapSort
        Clock clock;
        clock.Start();
        heapSort(copy.data(), n);
        double duration = clock.Stop();
        
        PivH = duration;
        cout << "O(n log n)  -  HeapSort Runtime: " << duration << " centiseconds" << endl; //HeapSort Runtime: 0.00086 centiseconds

    }

    //MergeSort: Pivot.txt
    {
        //Read from Pivot.txt
        vector<int> original = readFile("Pivot.txt");
        int n = original.size();

        //Make a copy of Pivot.txt data 
        vector<int> copy = original;

        //Runtime Analysis of MergeSort 
        Clock clock;
        clock.Start();
        mergeSort(copy.data(), 0, n - 1);
        double duration = clock.Stop();

        PivM = duration;
        cout << "O(n log n)  -  MergeSort Runtime: " << duration << " centiseconds" << endl; // MergeSort Runtime: 0.00216 centiseconds
    }

    //QuickSort: Pivot.txt
    {
        //Read from Pivot.txt
        vector<int> original = readFile("Pivot.txt");
        int n = original.size();

        //Make a copy of Pivot.txt data 
        vector<int> copy = original;

        //Runtime Analysis of QuickSort 
        Clock clock;
        clock.Start();
        quickSort(copy.data(), 0, n - 1);
        double duration = clock.Stop();

        PivQ = duration;
        cout << "O(n log n)  -  QuickSort Runtime: "  << duration << " centiseconds" << endl; // QuickSort Runtime: 0.00058 centiseconds

    }

    //IntroSort: Pivot.txt
    {
        //Read from Pivot.txt
        vector<int> original = readFile("Pivot.txt");
        int n = original.size();

        //Make a copy of Pivot.txt data 
        vector<int> copy = original;

        //Runtime Analysis of IntroSort 
        Clock clock;
        clock.Start();
        introSort(copy.data(), n);
        double duration = clock.Stop();

        PivIntr = duration;
        cout << "O(n log n)  -  introSort Runtime: "  << duration << " centiseconds" << endl; //O(n log n)  -  introSort Runtime: 0.00054 centiseconds



    }

    //----------------------Random.txt----------------------------//
    cout << "------------Sort------Random.txt----------------"<<endl;

     //bubbleSort  : Random.txt (Print sorted array)
    {
        vector<int> original = readFile("Random.txt");
        int n = original.size();

        //Make a copy of Random.txt data
        vector<int> copy = original;

        bubbleSort(copy.data(), n); 

        //Print sorted output
        cout << "Bubble Sort- Random.txt: " <<endl;
        for(int x: copy){
            cout << x << " ";
        }
        cout << endl;

    }

    cout << "---------------Sort--Random.txt---------------"<<endl;
    //bubbleSort : Random.txt
    {
        vector<int> original = readFile("Random.txt");
        int n = original.size();

        //Make a copy of Random.txt data
        vector<int> copy = original;
    
        //Runtime Analysis of bubbleSort
        Clock clock;
        clock.Start();
        bubbleSort(copy.data(), n);
        double duration = clock.Stop();

        RandBubble = duration;
        cout << "O(n^2)      -  Bubble Sort RunTime: " << duration << " centiseconds" <<endl; // O(n^2)      -  Bubble Sort RunTime: 0.00222 centiseconds
    }

    //selectionSort : Random.txt
    {
        //Read from Random.txt
        vector<int> original = readFile("Random.txt");
        int n = original.size();

        //Make a copy of Random.txt data
        vector<int> copy = original;

        //Runtime Analysis of selectionSort
        Clock clock;
        clock.Start();
        selectionSort(copy.data(), n);
        double duration = clock.Stop();

        RandS = duration;
        cout << "O(n^2)      -  Selection Sort Runtime: " << duration << " centiseconds" << endl; // O(n^2)- selectionSort Runtime:: 0.00092 centiseconds

    }

    //InsertionSort: Random.txt
    {
        //Read from Random.txt
        vector<int> original = readFile("Random.txt");
        int n = original.size();

        //Make a copy of Random.txt data
        vector<int> copy = original;


        //Runtime Analysis of insertionSort
        Clock clock;
        clock.Start();
        insertionSort(copy.data(), n);
        double duration = clock.Stop();

        RandI = duration;
        cout << "O(n^2)      -  Insertion Sort Runtime: " << duration << " centiseconds" << endl; // O(n^2) - Insertion Sort Runtime:  0.0007 centiseconds

    }


    //ShellSort: Random.txt
    {
        //Read from Random.txt
        vector<int> original = readFile("Random.txt");
        int n = original.size();

        //Make a copy of Random.txt data 
        vector<int> copy = original;

        //Runtime Analysis of ShellSort 
        Clock clock;
        clock.Start();
        shellSort(copy.data(), n);
        double duration = clock.Stop();

        RandSh = duration;
        cout << "O(n^1.5)  -  ShellSort Runtime: " << duration << " centiseconds" << endl; // O(n log n)  -  ShellSort Runtime: 0.00078 centiseconds

    }

    //HeapSort
    {
        //Read from Random.txt
        vector<int> original = readFile("Random.txt");
        int n = original.size();

        //Make a copy of Random.txt data 
        vector<int> copy = original;

        //Runtime Analysis of HeapSort 
        Clock clock;
        clock.Start();
        heapSort(copy.data(), n);
        double duration = clock.Stop();
        
        RandH = duration;
        cout << "O(n log n)  -  HeapSort Runtime: " << duration << " centiseconds" << endl; //O(n log n)  -  HeapSort Runtime: 0.00084 centiseconds

    }

    
    //MergeSort: Random.txt
    {
        //Read from Random.txt
        vector<int> original = readFile("Random.txt");
        int n = original.size();

        //Make a copy of Random.txt data 
        vector<int> copy = original;

        //Runtime Analysis of MergeSort 
        Clock clock;
        clock.Start();
        mergeSort(copy.data(), 0, n - 1);
        double duration = clock.Stop();

        RandM = duration;
        cout << "O(n log n)  -  MergeSort Runtime: " << duration << " centiseconds" << endl; // MergeSort Runtime: 0.00237 centiseconds
    }

    //QuickSort: Random.txt
    {
        //Read from Random.txt
        vector<int> original = readFile("Random.txt");
        int n = original.size();

        //Make a copy of Random.txt data 
        vector<int> copy = original;

        //Runtime Analysis of QuickSort 
        Clock clock;
        clock.Start();
        quickSort(copy.data(), 0, n - 1);
        double duration = clock.Stop();

        RandQ = duration;
        cout << "O(n log n)  -  QuickSort Runtime: "  << duration << " centiseconds" << endl; // O(n log n)  -  QuickSort Runtime: 0.00054 centiseconds

    }

    //IntroSort: Random.txt
    {
        //Read from Random.txt
        vector<int> original = readFile("Random.txt");
        int n = original.size();

        //Make a copy of Random.txt data 
        vector<int> copy = original;

        //Runtime Analysis of IntroSort 
        Clock clock;
        clock.Start();
        introSort(copy.data(), n);
        double duration = clock.Stop();

        RandIntr = duration;
        cout << "O(n log n)  -  introSort Runtime: "  << duration << " centiseconds" << endl; //O(n log n)  -  introSort Runtime: 0.00115 centiseconds



    }

     //----------------------SemiSorted.txt----------------------------//
    cout << "-----------Sort---SemiSorted.txt-------------"<<endl;

    //BubbleSort: SemiSorted.txt
    {
        //Read from SemiSorted.txt
        vector<int> original = readFile("SemiSorted.txt");
        int n = original.size();

        //Make a copy of SemiSorted.txt data
        vector<int> copy = original;

        bubbleSort(copy.data(), n); 

        //Print sorted output
        cout << "Bubble Sort- SemiSorted.txt: " <<endl;
        for(int x: copy){
            cout << x << " ";
        }
        cout << endl;

    }

    cout << "-----------Sort---SemiSorted.txt-------------"<<endl;
    //bubbleSort : SemiSorted.txt
    {
        vector<int> original = readFile("SemiSorted.txt");
        int n = original.size();

        //Make a copy of SemiSorted.txt data
        vector<int> copy = original;
    
        //Runtime Analysis of bubbleSort
        Clock clock;
        clock.Start();
        bubbleSort(copy.data(), n);
        double duration = clock.Stop();

        SemiBubble = duration;
        cout << "O(n^2)      -  Bubble Sort RunTime: " << duration << " centiseconds" <<endl; // O(n^2)      -  Bubble Sort RunTime: 0.00071 centiseconds
    }

    //selectionSort : SemiSorted.txt
    {
        //Read from SemiSorted.txt
        vector<int> original = readFile("SemiSorted.txt");
        int n = original.size();

        //Make a copy of SemiSorted.txt data
        vector<int> copy = original;

        //Runtime Analysis of selectionSort
        Clock clock;
        clock.Start();
        selectionSort(copy.data(), n);
        double duration = clock.Stop();

        SemiS  = duration;
        cout << "O(n^2)      -  Selection Sort Runtime: " << duration << " centiseconds" << endl; // O(n^2)      -  Selection Sort Runtime: 0.00063 centiseconds
    }

    //InsertionSort: SemiSorted.txt
    {
        //Read from SemiSorted.txt
        vector<int> original = readFile("SemiSorted.txt");
        int n = original.size();

        //Make a copy of SemiSorted.txt data
        vector<int> copy = original;


        //Runtime Analysis of insertionSort
        Clock clock;
        clock.Start();
        insertionSort(copy.data(), n);
        double duration = clock.Stop();

        SemiI = duration;
        cout << "O(n^2)      -  Insertion Sort Runtime: " << duration << " centiseconds" << endl; // O(n^2)      -  Insertion Sort Runtime: 0.00011 centiseconds
    }

    //ShellSort: SemiSorted.txt
    {
        //Read from SemiSorted.txt
        vector<int> original = readFile("SemiSorted.txt");
        int n = original.size();

        //Make a copy of SemiSorted.txt data 
        vector<int> copy = original;

        //Runtime Analysis of ShellSort 
        Clock clock;
        clock.Start();
        shellSort(copy.data(), n);
        double duration = clock.Stop();

        SemiSh = duration;
        cout << "O(n^1.5)  -  ShellSort Runtime: " << duration << " centiseconds" << endl; // O(n log n)  -  ShellSort Runtime: 0.0002 centiseconds

    }

    //HeapSort
    {
        //Read from SemiSorted.txt
        vector<int> original = readFile("SemiSorted.txt");
        int n = original.size();

        //Make a copy of SemiSorted.txt data 
        vector<int> copy = original;

        //Runtime Analysis of HeapSort 
        Clock clock;
        clock.Start();
        heapSort(copy.data(), n);
        double duration = clock.Stop();
        
        SemiH = duration;
        cout << "O(n log n)  -  HeapSort Runtime: " << duration << " centiseconds" << endl; //O(n log n)  -  HeapSort Runtime: 0.00138 centiseconds

    }

    
    //MergeSort: SemiSorted.txt
    {
        //Read from SemiSorted.txt
        vector<int> original = readFile("SemiSorted.txt");
        int n = original.size();

        //Make a copy of SemiSorted.txt data 
        vector<int> copy = original;

        //Runtime Analysis of MergeSort 
        Clock clock;
        clock.Start();
        mergeSort(copy.data(), 0, n - 1);
        double duration = clock.Stop();

        SemiM = duration;
        cout << "O(n log n)  -  MergeSort Runtime: " << duration << " centiseconds" << endl; // O(n log n)  -  MergeSort Runtime: 0.00218 centiseconds
    }

    
    //QuickSort: SemiSorted.txt
    {
        //Read from SemiSorted.txt
        vector<int> original = readFile("SemiSorted.txt");
        int n = original.size();

        //Make a copy of SemiSorted.txt data 
        vector<int> copy = original;

        //Runtime Analysis of QuickSort 
        Clock clock;
        clock.Start();
        quickSort(copy.data(), 0, n - 1);
        double duration = clock.Stop();

        SemiQ = duration;
        cout << "O(n log n)  -  QuickSort Runtime: "  << duration << " centiseconds" << endl; // O(n log n)  -  QuickSort Runtime: 0.00181 centiseconds
    }

    //IntroSort: SemiSorted.txt
    {
        //Read from SemiSorted.txt
        vector<int> original = readFile("SemiSorted.txt");
        int n = original.size();

        //Make a copy of SemiSorted.txt data 
        vector<int> copy = original;

        //Runtime Analysis of IntroSort 
        Clock clock;
        clock.Start();
        introSort(copy.data(), n);
        double duration = clock.Stop();

        SemiIntr = duration;
        cout << "O(n log n)  -  introSort Runtime: "  << duration << " centiseconds" << endl; //O(n log n)  -  introSort Runtime: 0.00115 centiseconds

    }
    cout <<endl;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    {
    cout << fixed << setprecision(6);   // decimals

    cout << left << setw(20) << "Algorithm(Big-O)" << setw(12) << "Gaps" << setw(16) << "Partition" << setw(12) << "Pivot" << setw(12) << "Random" << setw(12) << "SemiSorted" << endl;
    cout << "----------------------------------------------------------------------------------"<<endl;;

    cout <<left<<setw(20) <<"Bubble O(n^2)"     <<setw(12) <<GapsBubble  << setw(16) << PartBubble << setw(12) << PivBubble <<  setw(12) << RandBubble << setw(12) << SemiBubble << endl;
    cout <<left<<setw(20) <<"Select O(n^2)"     <<setw(12) <<GapsS       << setw(16) << PartS      << setw(12) << PivS      <<  setw(12) << RandS      << setw(12) << SemiS      << endl;
    cout <<left<<setw(20) <<"Insert O(n^2)"     <<setw(12) <<GapsI       << setw(16) << PartI      << setw(12) << PivI      <<  setw(12) << RandI      << setw(12) << SemiI      << endl;
    cout <<left<<setw(20) <<"Shell  O(n^1.5)"   <<setw(12) <<GapsSh      << setw(16) << PartSh     << setw(12) << PivSh     <<  setw(12) << RandSh     << setw(12) << SemiSh     << endl;
    cout <<left<<setw(20) <<"Heap   O(n log n)" <<setw(12) <<GapsH       << setw(16) << PartH      << setw(12) << PivH      <<  setw(12) << RandH      << setw(12) << SemiH      << endl;
    cout <<left<<setw(20) <<"Merge  O(n log n)" <<setw(12) <<GapsM       << setw(16) << PartM      << setw(12) << PivM      <<  setw(12) << RandM      << setw(12) << SemiM      << endl;
    cout <<left<<setw(20) <<"Quick  O(n log n)" <<setw(12) <<GapsQ       << setw(16) << PartQ      << setw(12) << PivQ      <<  setw(12) << RandQ      << setw(12) << SemiQ      << endl;
    cout <<left<<setw(20) <<"Intro  O(n log n)" <<setw(12) <<GapsIntr    << setw(16) << PartIntr   << setw(12) << PivIntr   <<  setw(12)  << RandIntr  <<setw(12)  << SemiIntr    <<  endl;

    }
    cout <<endl;

    ////////////////////////////////////////////////Fastest to  Slowest////////////////////////////////////
    //Gaps.txt
    {
        string names[8] = {"Bubble","Selection","Insertion","Shell","Heap","Merge","Quick","Intro"};
        double times[8] = {GapsBubble, GapsS, GapsI, GapsSh, GapsH, GapsM, GapsQ, GapsIntr};

        int n = 8; //Number of Algorithms

        for (int i = 0; i < n - 1; i++) {
            int minIndex = i;
            for (int j = i + 1; j < n; j++) {
                if (times[j] < times[minIndex]) minIndex = j;
            }
            swap(times[i], times[minIndex]);
            swap(names[i], names[minIndex]); 
        }

        //Print
        cout << "\nFastest    ->  Slowest (Gaps.txt)\n";
        cout << "-------------------------------------\n";
      
        for (int i = 0; i < n; i++) {
        cout << setw(12) << names[i] << setw(12) << times[i] << " centiseconds\n";
        }

    }

    //Partition.txt
    {
        string names[8] = {"Bubble","Selection","Insertion","Shell","Heap","Merge","Quick","Intro"};
        double times[8] = {PartBubble, PartS, PartI, PartSh, PartH, PartM, PartQ, PartIntr};

        int n = 8; //Number of Algorithms

        for (int i = 0; i < n - 1; i++) {
            int minIndex = i;
            for (int j = i + 1; j < n; j++) {
                if (times[j] < times[minIndex]) minIndex = j;
            }
            swap(times[i], times[minIndex]);
            swap(names[i], names[minIndex]); 
        }

        //Print
        cout << "\nFastest    ->  Slowest (Partition.txt)\n";
        cout << "-------------------------------------\n";
      
        for (int i = 0; i < n; i++) {
        cout << setw(12) << names[i] << setw(12) << times[i] << " centiseconds\n";
        }

    }

    //Pivot.txt
    {
        string names[8] = {"Bubble","Selection","Insertion","Shell","Heap","Merge","Quick","Intro"};
        double times[8] = {PivBubble, PivS, PivI, PivSh, PivH, PivM, PivQ, PivIntr};

        int n = 8; //Number of Algorithms

        for (int i = 0; i < n - 1; i++) {
            int minIndex = i;
            for (int j = i + 1; j < n; j++) {
                if (times[j] < times[minIndex]) minIndex = j;
            }
            swap(times[i], times[minIndex]);
            swap(names[i], names[minIndex]); 
        }

        //Print
        cout << "\nFastest    ->  Slowest (Pivot.txt)\n";
        cout << "-------------------------------------\n";
      
        for (int i = 0; i < n; i++) {
        cout << setw(12) << names[i] << setw(12) << times[i] << " centiseconds\n";
        }

    }

    //Random.txt
    {
        string names[8] = {"Bubble","Selection","Insertion","Shell","Heap","Merge","Quick","Intro"};
        double times[8] = {RandBubble, RandS, RandI, RandSh, RandH, RandM, RandQ, RandIntr};

        int n = 8; //Number of Algorithms

        for (int i = 0; i < n - 1; i++) {
            int minIndex = i;
            for (int j = i + 1; j < n; j++) {
                if (times[j] < times[minIndex]) minIndex = j;
            }
            swap(times[i], times[minIndex]);
            swap(names[i], names[minIndex]); 
        }

        //Print
        cout << "\nFastest    ->  Slowest (Random.txt)\n";
        cout << "-------------------------------------\n";
      
        for (int i = 0; i < n; i++) {
        cout << setw(12) << names[i] << setw(12) << times[i] << " centiseconds\n";
        }

    }

    //SemiSorted.txt
    {
        string names[8] = {"Bubble","Selection","Insertion","Shell","Heap","Merge","Quick","Intro"};
        double times[8] = {SemiBubble, SemiS, SemiI, SemiSh, SemiH, SemiM, SemiQ, SemiIntr};

        int n = 8; //Number of Algorithms

        for (int i = 0; i < n - 1; i++) {
            int minIndex = i;
            for (int j = i + 1; j < n; j++) {
                if (times[j] < times[minIndex]) minIndex = j;
            }
            swap(times[i], times[minIndex]);
            swap(names[i], names[minIndex]); 
        }

        //Print
        cout << "\nFastest    ->  Slowest (SemiSorted.txt)\n";
        cout << "-------------------------------------\n";
      
        for (int i = 0; i < n; i++) {
        cout << setw(12) << names[i] << setw(12) << times[i] << " centiseconds\n";
        }

    }
        

    return 0;


}

//Analysis and Report ( Comparing Runtime of each sorting algorithm)

// Fastest  -----> Slowest

//--Gaps.txt--
// Quick, Shell, Insertion, Heap, Intro, Selection,  :  Bubble, Merge(Slowest)

//--Partition.txt--
// Shell, Insertion, Quick, Intro, Heap, Selection,  :  Bubble, Merge(Slowest)

//--Pivot.txt
// Quick, Intro,  Shell, Insertion, Heap, Selection, :  Bubble, Merge(Slowest)

//--Random.txt
// Insertion, Shell, Quick, Intro, Heap, Selection,  :   Merge, Bubble(Slowest)

//--SemiSorted.txt
// Insertion, Shell, Bubble, Selection, Heap, Quick, :   Intro, Merge(Slowest)

// Overall Observations: ------------------------
// It seems for all data sets grouped together
// The fastest group are -( Insertion, Shell, Quick)
// The slowest group are - (Merge and Bubble), except for SemiSorted-Bubble is in the middle in terms of speed
// Merge consistently seems to be the slowest even though it is O(n log n)

// Correlation:  ----------------------------------
// Insertion is fastest because it does very few shifts (almost already sorted → close to O(n).
// QuickSort is fast because it sorts in-place with low overhead and usually produces balanced partitions, giving average O(n log n) performance with good cache locality.
// ShellSort is also fast because it reduces large inversions early using gaps, which speeds up insertion in later passes compared to standard InsertionSort.
// Bubble and Selection are slow because they perform many comparisons and swaps regardless of data order, resulting in O(n^2) behavior for all datasets.
// Merge is the slowest because it allocates temporary vectors (L and R) at every merge step, causing extra copying and extra memory allocation overhead



