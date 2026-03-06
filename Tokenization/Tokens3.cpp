#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;


vector<string> tokenize(const string& code) {
    string spaced;
    for (char c : code) {
        if (ispunct((unsigned char)c)) {
            spaced += ' ';
            spaced += c;
            spaced += ' ';
        } 
        else {
            spaced += c;
        }
    }

    vector<string> tokens;
    stringstream ss(spaced);
    string word;
    
    while (ss >> word) {
        tokens.push_back(word);
    }
    return tokens;
}

void printTokens(const string& code) {
    vector<string> tokens = tokenize(code);

    cout << "TOKENS: ";
    for (string t : tokens) {
        cout << " [ " << t << " ] ";
    }
    cout << endl;
}


int main() {
    string code = "I love CIS 22C";

    vector<string> t = tokenize(code);

    for (string s : t)
        cout << s << endl;

    string t6 = "for(int i=0; i<n; i++)";
    printTokens(t6); 
    // TOKENS:  [ for ]  [ ( ]  [ int ]  [ i ]  [ = ]  [ 0 ]  [ ; ]  [ i ]  [ < ]  [ n ]  [ ; ]  [ i ]  [ + ]  [ + ]  [ ) ]
    
   return 0;
}