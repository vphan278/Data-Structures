#include <iostream>
using namespace std;

// Computes n! (factorial value)
// Time complexity: O(n)

//4! = 4 x 3 x 2 x 1 = 24
int factorial(int n) {
    int result = 1;

    for (int i = 1; i <= n; i++) {
        result *= i;
    }

    return result;
}

int main() {
    int n;
    cout << "Enter a number: ";
    cin >> n;

    cout << "Factorial of " << n << " is " << factorial(n) << endl;

    return 0;
}


