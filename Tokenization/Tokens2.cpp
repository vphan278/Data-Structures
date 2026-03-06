#include <iostream>
#include <sstream>
using namespace std;

int main() {
    string line = "GeeksForGeeks is a must try";

    stringstream ss(line);
    string word;

    while (ss >> word) {   // automatically splits by spaces
        cout << word << endl;
    }

    return 0;
}

//GeeksForGeeks
//is 
//a 
//must
//try