#include <iostream>
#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

//Variant 1
int bmh_findV1(const std::string& text, const std::string& pat) {


    int n = text.size(), m = pat.size();
    if (m == 0 || m > n) return -1;

    std::vector<int> shift(256, m);

    for (int i = 0; i < m - 1; ++i)
        shift[(unsigned char)pat[i]] = m - 1 - i;

    int pos = 0;
    while (pos <= n - m) {
        int j = m - 1;

        int lastCheck = j;

        while (j >= 0 && pat[j] == text[pos + j]) j--;

        if (j < 0) return pos;

        pos += shift[(unsigned char)text[pos + m - 1]];
    }
    return -1;
}

//Variant 2
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

        while (k >= 0 && p[k] == t[cursor + k]) k--;

        if (k < 0) return cursor;

        cursor += table[(unsigned char)t[cursor]];
    }
    return -1;
}

//Variant 3
int horspool_match_v3(const std::string& hay, const std::string& needle) {
    int n = hay.size(), m = needle.size();

    if (m == 0 || m > n) return -1;

    std::vector<int> shift(256, m);

    for (int i = 0; i < m; ++i) // Should be i < m -1, so it is in an infinite loop
        shift[(unsigned char)needle[i]] = m - 1 - i;

    int i = 0;
    while (i <= n - m) {
        int j = m - 1;

        while (j >= 0 && needle[j] == hay[i + j]) j--;

        if (j < 0) return i;

        i += shift[(unsigned char)hay[i + m - 1]];
    }
    return -1;
}

string readFile(const string& filename) {
    ifstream fin(filename, ios::binary);  // ✅ IMPORTANT

    string content((istreambuf_iterator<char>(fin)),
                    istreambuf_iterator<char>());

    return content;
};


string decodePrefix(const string& cipher, int startR1, int startR2, int K) {
    string decoded;

    int r1 = startR1;
    int r2 = startR2;
    int initialR1 = startR1;
    int count = 0;

    for (int i = 0; i < cipher.size() && count < K; i++) {
        char ch = cipher[i];

        if (ch == '\r' || ch == '\n') {
            decoded += ch;

            r1 = (r1 + 1) % 95;
            if (r1 == initialR1) {
                r2 = (r2 + 1) % 95;
            }

            continue;
        }

        int x = ch - 32;

        x = (x - r2 + 95) % 95;
        x = (x - r1 + 95) % 95;

        decoded += char(x + 32);

        count++;

        // step rotors
        r1 = (r1 + 1) % 95;
        if (r1 == initialR1) {
            r2 = (r2 + 1) % 95;
        }
    }

    return decoded;
};

string decodeFull(const string& cipher, int startR1, int startR2) {
    string decoded;

    int r1 = startR1;
    int r2 = startR2;
    int initialR1 = startR1;

    for (char ch : cipher) {

        if (ch == '\r' || ch == '\n') {
            decoded += ch;

            // step rotors
            r1 = (r1 + 1) % 95;
            if (r1 == initialR1) {
                r2 = (r2 + 1) % 95;
            }

            continue;
        }

        int x = ch - 32;

        x = (x - r2 + 95) % 95;
        x = (x - r1 + 95) % 95;

        decoded += char(x + 32);

        // step rotors
        r1 = (r1 + 1) % 95;
        if (r1 == initialR1) {
            r2 = (r2 + 1) % 95;
        }
    }

    return decoded;
};

int getPrefixEndIndex(const string& cipher, int K) {
    int printableCount = 0;

    for (int i = 0; i < cipher.size(); i++) {
        char ch = cipher[i];

        if (ch != '\r' && ch != '\n') {
            printableCount++;
        }

        if (printableCount == K) {
            return i + 1;   // end position after prefix
        }
    }

    return cipher.size();
}

int main(){

    //Version 1
    string text = "WHERE IS THE FLIGHT TO JAMAICA";
    string pattern ="JAMAICA";

    int pos = bmh_findV1(text, pattern);

    if(pos != -1)
        cout << "V1 Found at index: " << pos << endl;
    else
        cout << "Not found" << endl;

    //Version 2
    int pos2 = bmh_scan_v2(text, pattern);
    if(pos2 != -1)
        cout << "V2 Found at index: " << pos2 << endl;
    else
        cout << "V2 Not found\n";

    
    //Version 3
    bool V3 = false;   

    if(V3){
        int pos3 = horspool_match_v3(text, pattern);
        if(pos3 != -1)
            cout << "V3 Found at index: " << pos3 << endl;
        else
            cout << "V3 Not found\n";
    } 
    else {
        cout << "\nV3 skipped (to avoid infinite loop)\n";
    }


    //Run Ciphertext string cipher = readFile("Cipher.txt");
    string cipher = readFile("Cipher.txt");
    vector<string> anchors = {
        "THE", "AND", "TO", "OF", "IN", "THAT", "IS", "IT", "FOR", "YOU"
    };

    //Test: Print first 100 characters
    cout << cipher.substr(0, 100) << endl; // e,>4)'B31*F32820<;N?7Q~CG?8FdYRDBPD_UJHcRU\V]KTZ`ma_dWr`^a\wZh^ebls ughxm&hvm* tr."y(w&(5.'.(~;'-?-1

    //Decode Prefix
    int K = 500;

    for (int r1 = 0; r1 < 95; r1++) {
        for (int r2 = 0; r2 < 95; r2++) {

            string prefix = decodePrefix(cipher, r1, r2, K);

            int scoreV1 = 0;
            int scoreV2 = 0;

            for (const string& word : anchors) {
                if (bmh_findV1(prefix, word) != -1) {
                    scoreV1++;
                }
                if (bmh_scan_v2(prefix, word) != -1) {
                    scoreV2++;
                }
            }

            if (scoreV1 >= 6) {
                cout << "\nRotor found:\n";
                cout << "r1 = " << r1 << ", r2 = " << r2 << endl;

                cout << "V1 score: " << scoreV1 << endl;
                cout << "V2 score: " << scoreV2 << endl;
                cout << "V3: not run (infinite loop)" << endl;

                //Prefix
                cout << "\nDecoded prefix:\n";
                cout << prefix << endl;

                //Entire ciphertext
                string fullDecoded = decodeFull(cipher, r1, r2);

                cout << "\nFull Decoded Ciphertext:\n";
                cout << fullDecoded << endl;

                //Remaining ciphertext after prefix
                int prefixEnd = getPrefixEndIndex(cipher, K);

                string remainingCipher = cipher.substr(prefixEnd);

                cout << "\nRemaining ciphertext after prefix:\n";
                cout << remainingCipher << endl;



                return 0;
            }
        }
    }

    cout << "\nNo match found.\n";

    //Decoded prefix: )N_THE_OLD_KINGDOM_OF_,ORIANk_WHERE_THE_MOUNTAINS_ROSE_LIKE_ANCIENT_TEETH_AND_THE_RIVERS_WOUND_IN_LONG_SILVER_THREADSk_THERE_LIVED_A_YOUNG_ARCHIVIST_NAMED_%LIRAm_3HE_WORKED_IN_THE_'RAND_3CRIPTORIUMk_A_PLACE_WHERE_EVERY_SCRAP_OF_KNOWLEDGEllMAPSk_SPELLSk_HISTORIESk_EVEN_HALFlFORGOTTEN_JOKESllWAS_STORED_IN_ENDLESS_SHELVES_THAT_SPIRALED_UPWARD_LIKE_A_3425#452%_CARVED_FROM_MOONLIGHTm_%LIRA_LOVED_THE_QUIET_OF_THE_STACKSk_THE_SMELL_OF_PARCHMENTk_AND_THE_WAY_THE_DUST_MOTES_DRIFTED_LIKE_TINY_STARSm
                    
        

    return 0;
}




//V1 Found at index: 23. It is the correct version of BMH(Boyer-Moore-Horspool)
//V2 Not found
//At line 31, V2 should be [cursor + m - 1], so it jumps the wrong and misses at index 23.
// At line 71 for the Version 3(horspool_match_v3), the for loop is i < m where it should be i < m -1. This is why
//it is in an infinite loop and will not give the output.