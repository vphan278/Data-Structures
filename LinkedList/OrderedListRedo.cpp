#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

// Vinh Phan
// CIS-22C: C++ Data Structures and Algorithms
// CRN 33571

class Node {
public:
    int value;               // Stored value
    Node* prev = nullptr;    // Points to a smaller value subtree/chain
    Node* next = nullptr;    // Points to a larger or equal value subtree/chain

    Node(int val) : value(val) {}
};

// OrderedList class that manages Node objects
class OrderedList {
private:
    Node* center = nullptr;  // Starting point for all traversals

    void clearRec(Node* node) {
        if (!node) return;
        clearRec(node->prev);
        clearRec(node->next);
        delete node;
    }

public:
    OrderedList() = default;

    ~OrderedList() {
        clear();
    }

    void clear() {
        clearRec(center);
        center = nullptr;
    }

    void setCenter(int val) {
        clear();
        center = new Node(val);
    }

    Node* getCenter() const {
        return center;
    }

    // ONE centered insertion algorithm (used for BOTH parts)
    void insert(int val) {
        if (!center) {
            center = new Node(val);
            return;
        }

        Node* curr = center;

        while (true) {
            if (val < curr->value) {
                if (curr->prev == nullptr) {
                    curr->prev = new Node(val);
                    return;
                }
                curr = curr->prev;
            } else {
                if (curr->next == nullptr) {
                    curr->next = new Node(val);
                    return;
                }
                curr = curr->next;
            }
        }
    }
};

// Provided visualization (unchanged)
void printLayer(Node* node, string prefix = "", bool isLast = true, string label = "Root") {
    if (!node) return;
    cout << prefix << (isLast ? "+-- " : "|-- ");
    cout << (label == "Center" ? "Center: " : label == "Prev" ? "<-- " : "--> ");
    cout << node->value << endl;

    prefix += isLast ? "    " : "|   ";
    bool hasPrev = node->prev != nullptr;
    bool hasNext = node->next != nullptr;
    if (hasPrev) printLayer(node->prev, prefix, !hasNext, "Prev");
    if (hasNext) printLayer(node->next, prefix, true, "Next");
}

int main() {
    srand((unsigned)time(nullptr));

    // Generate random vector of 10 ints
    vector<int> values;
    for (int i = 0; i < 10; i++) values.push_back(rand() % 100);

    cout << "Random Numbers: ";
    for (int v : values) cout << v << " ";
    cout << "\n\n";



    // Part 1: Unsorted centered insertion////////////////////////
    OrderedList part1;
    part1.setCenter(values[5]);
    for (int i = 0; i < 10; i++) {
        if (i == 5) continue;
        part1.insert(values[i]);
    }

    cout << "Part 1 - Unsorted List:\n";
    printLayer(part1.getCenter(), "", true, "Center");

    // Part 2: Sort vector and rebuild with sorted insertion
    vector<int> sortedValues = values;
    sort(sortedValues.begin(), sortedValues.end());

    cout << "\nSorted Numbers: ";
    for (int v : sortedValues) cout << v << " ";
    cout << "\n\n";

    OrderedList part2;
    part2.setCenter(sortedValues[5]);
    for (int i = 0; i < 10; i++) {
        if (i == 5) continue;
        part2.insert(sortedValues[i]);
    }

    cout << "Part 2 - Sorted List:\n";
    printLayer(part2.getCenter(), "", true, "Center");

    // 2–3 sentence comparison (print it so it's visible)
    cout << "\nComparison:\n";
    cout << "Using unsorted input, values arrive in mixed order so insertions often travel different directions and create a more irregular shape.\n";
    cout << "Using sorted input, insertions follow a more predictable path and the structure tends to become more chain-like on one side.\n";

    return 0;
}

// Random Numbers: 13 98 80 11 55 78 94 60 62 58 

// Part 1 - Unsorted List:
// +-- Center: 78
//     |-- <-- 13
//     |   |-- <-- 11
//     |   +-- --> 55
//     |       +-- --> 60
//     |           |-- <-- 58
//     |           +-- --> 62
//     +-- --> 98
//         +-- <-- 80
//             +-- --> 94

//                      78
//                    /    \ 
//                  13      98
//                 /  \     /
//               11   55   80
//                     \     \ 
//                     60     94
//                    /  \ 
//                  58    62




// Sorted Numbers: 11 13 55 58 60 62 78 80 94 98

// Part 2 - Sorted List:
// +-- Center: 62
//     |-- <-- 11
//     |   +-- --> 13
// +-- Center: 62
//     |-- <-- 11
// +-- Center: 62
//     |-- <-- 11
//     |   +-- --> 13
// +-- Center: 62
//     |-- <-- 11
//     |   +-- --> 13
//     |       +-- --> 55
// +-- Center: 62
//     |-- <-- 11
// +-- Center: 62
//     |-- <-- 11
// +-- Center: 62
//     |-- <-- 11
//     |   +-- --> 13
// +-- Center: 62
//     |-- <-- 11
//     |   +-- --> 13
//     |       +-- --> 55
// +-- Center: 62
//     |-- <-- 11
// +-- Center: 62
//     |-- <-- 11
//     |   +-- --> 13
//     |       +-- --> 55
//     |           +-- --> 58
//     |               +-- --> 60
//     +-- --> 78
//         +-- --> 80
//             +-- --> 94
//                 +-- --> 98


//                  62
//                /    \ 
//              11      78
//                \       \ 
//                13       80
//                  \        \ 
//                  55        94
//                    \         \ 
//                    58         98
//                      \ 
//                       60

