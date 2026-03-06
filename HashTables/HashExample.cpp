#include <iostream>
#include <unordered_map>
using namespace std;

int main(){

    unordered_map<string, int> table;

    table["Alice"] = 90;
    table["Bob"]   = 85;
    table["Chris"] = 95;

    cout << "Grade is: " << table["Bob"] << endl;; // Grade is: 85

    //Check if key exists
    if(table.count("Alice"))
        cout << "Alice found!" << endl;


    //Loop through hash table
    for(auto &p : table)
        cout << p.first << " -> " << p.second << endl;

// 

//OutPut
// Grade is: 85
// Alice found!
// Chris -> 95
// Bob -> 85
// Alice -> 90

    return 0;

}