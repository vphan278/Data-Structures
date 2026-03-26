#include <iostream>
#include <array>
#include <string>
#include <vector>
using namespace std;

int horspool_match_v3(const std::string& hay, const std::string& needle) {

    int n = hay.size(), m = needle.size();

    if (m == 0 || m > n) return -1;

    std::vector<int> shift(256, m);

    for (int i = 0; i < m; ++i)  // Should be i < m -1, so it is in an infinite loop
        shift[(unsigned char)needle[i]] = m - 1 - i;

    int i = 0;
    while (i <= n - m) {
        int j = m - 1;

        while (j >= 0 && needle[j] == hay[i + j]) 
            j--;

        if (j < 0) 
            return i;

        i += shift[(unsigned char)hay[i + m - 1]];
    }
    return -1;
};

int main(){

    string text = "WHERE IS THE FLIGHT TO JAMAICA";
    string pattern ="JAMAICA";

    int pos = horspool_match_v3(text, pattern);

    if(pos != -1)
        cout << "Found at index: " << pos << endl;
    else
        cout << "Not found" << endl;

    return 0;
}