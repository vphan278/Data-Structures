#include <iostream>
#include <sstream> 
using namespace std;

void tokenize(string text){

    string token;
    stringstream ss(text);

    while(ss >> token){
        cout << token << endl;
    }
}

int main(){

    tokenize(" I love CIS 22C yes  boat buy soon ");

    return 0;
}

//I
//love
//CIS
//22C