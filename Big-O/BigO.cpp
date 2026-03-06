#include <iostream>
using namespace std;

// O(1)
void example_O1() {
    int x = 41;
    cout << "x: " << x << endl;
}

// O(log n)
void example_Ologn(int n) {
    int steps = 0;
    while (n > 1) {
        n = n / 2;
        steps++;
    }
    cout << "Steps: " << steps << endl;
}

// O(a + b)  (linear)
void example_Oa_plus_b(int a, int b) {
    for (int i = 0; i < a; i++) {
        cout << i << " ";
    }
    cout << endl;

    for (int j = 0; j < b; j++) {
        cout << j << " ";
    }
    cout << endl;
}

// O(n log n)
void example_Onlogn(int n) {
    for (int i = 0; i < n; i++) {
        int j = n;
        while (j > 1) {
            j = j / 2;
        }
    }
    cout << "done O(n log n) demo" << endl;
}

// O(n^2)
void example_On2(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // constant work
        }
    }
    cout << "done O(n^2) demo" << endl;
}

int main() {
    example_O1();
    example_Ologn(16);
    example_Oa_plus_b(5, 5);
    example_Onlogn(10);
    example_On2(10);
    return 0;
}