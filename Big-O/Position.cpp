#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "hello world";

    size_t pos1 = s.find("world");   // should find
    size_t pos2 = s.find("cat");     // should NOT find

    cout << "world position: " << pos1 << endl;

    if (pos2 == string::npos)
        cout << "cat NOT found (npos)" << endl;
    else
        cout << "cat position: " << pos2 << endl;

    return 0;
}