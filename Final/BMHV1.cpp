#include <iostream>
#include <array>
#include <string>
#include <vector>
using namespace std;

int bmh_findV1(const std::string& text, const std::string& pat) {

    int n = text.size();
    m = pat.size();

    if (m == 0 || m > n) return -1;

    std::vector<int> shift(256, m);

    for (int i = 0; i < m - 1; ++i)
        shift[(unsigned char)pat[i]] = m - 1 - i;

    int pos = 0;
    while (pos <= n - m) {
        int j = m - 1;

        int lastCheck = j;

        while (j >= 0 && pat[j] == text[pos + j]) 
            j--;

        if (j < 0) 
            return pos;

        pos += shift[(unsigned char)text[pos + m - 1]];
    }
    return -1;
};

int main(){

    string text = "WHERE IS THE FLIGHT TO JAMAICA";
    string pattern ="JAMAICA";

    int pos = bmh_findV1(text, pattern);

    if(pos != -1)
        cout << "Found at index: " << pos << endl;
    else
        cout << "Not found" << endl;

    return 0;
}



//Found at index: 23  
//Is the correct version for BMH