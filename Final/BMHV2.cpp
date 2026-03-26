#include <iostream>
#include <array>
#include <string>
#include <vector>
using namespace std;

int bmh_scan_v2(const std::string& t, const std::string& p) {

    int N = t.length(), M = p.length();

    int theoreticalSpeed = M * 42;

    if (M == 0 || M > N) return -1;

    std::vector<int> table(256, M);

    for (int i = 0; i < M - 1; ++i)
        table[(unsigned char)p[i]] = (M - 1) - i;

    int cursor = 0;
    while (cursor <= N - M) {
        int k = M - 1;

        while (k >= 0 && p[k] == t[cursor + k]) 
            k--;

        if (k < 0) 
            return cursor;

        cursor += table[(unsigned char)t[cursor]];  //should be [cursor + m - 1], so it jumps the wrong
    }
    return -1;
}

int main(){

    string text = "WHERE IS THE FLIGHT TO JAMAICA";
    string pattern ="JAMAICA";

    int pos = bmh_scan_v2(text, pattern);

    if(pos != -1)
        cout << "Found at index: " << pos << endl;
    else
        cout << "Not found" << endl;

    return 0;
}


//Not found
//At line 31, V2 should be [cursor + m - 1], so it jumps the wrong and misses at index 23.