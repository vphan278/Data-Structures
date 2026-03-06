#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <algorithm>
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
        } else {
            spaced += c;
        }
    }

    vector<string> tokens;
    stringstream ss(spaced);
    string word;
    while (ss >> word) tokens.push_back(word);
    return tokens;
}


// Big-O helpers (dominant + multiply)  
int rankBigO(const string& b) {
    if (b == "O(1)") return 0;
    if (b == "O(log n)") return 1;
    if (b == "O(n)") return 2;
    if (b == "O(n log n)") return 3;
    if (b == "O(n^2)") return 4;

    return 2; // default
}

string dominantBigO(const string& a, const string& b) {
    return (rankBigO(a) >= rankBigO(b)) ? a : b;
}

//////////////////////////////////////////////////////////////////////////////////////
// Convert allowed Big-O strings -> exponents (nPow, logPow)
// Allowed set for assignment: O(1), O(log n), O(n), O(n log n), O(n^2)
void toPowers(const string& bigO, int& nPow, int& logPow) {
    nPow = 0; logPow = 0;

    if (bigO == "O(1)") {
        return;
    }
    else if (bigO == "O(log n)") {
        logPow = 1;
    }
    else if (bigO == "O(n)") {
        nPow = 1;
    }
    else if (bigO == "O(n log n)") {
        nPow = 1;
        logPow = 1;
    }
    else if (bigO == "O(n^2)") {
        nPow = 2;
    }
}

// Convert exponents back into allowed Big-O strings (clamped)
string fromPowersClamped(int nPow, int logPow) {
    if (nPow >= 2)
        return "O(n^2)";
    if (nPow == 1 && logPow == 1)
        return "O(n log n)";
    if (nPow == 1)
        return "O(n)";
    if (logPow == 1)
        return "O(log n)";
    return "O(1)";
}
////////////////////////////////////////////////////////////////////////////////////

// Multiply nested complexities correctly, then clamp to allowed set.
string multiplyBigO(const string& outer, const string& inner) {
    int n1, l1, n2, l2;
    toPowers(outer, n1, l1);
    toPowers(inner, n2, l2);

    int nPow = n1 + n2;
    int logPow = l1 + l2;

    return fromPowersClamped(nPow, logPow);
}

// Extract tokens inside matching parentheses after for/while
vector<string> getParenTokens(const vector<string>& tokens, int startIndex, int& endIndex) {
    vector<string> inside;
    int i = startIndex;

    // find '('
    while (i < tokens.size() && tokens[i] != "(") i++;
    i++; // skip '('

    // collect until ')'
    while (i < tokens.size() && tokens[i] != ")") {
        inside.push_back(tokens[i]);
        i++;
    }

    endIndex = i + 1; // move past ')'
    return inside;
}

// Classify ONE loop from header tokens (token rules)
// - shrinking loop: /2, /=2, >>=1 -> O(log n)
// - otherwise -> O(n)
string classifyLoop(const vector<string>& tokens) {
    
    for (int i = 0; i < tokens.size(); i++) {

        // detect i = i / 2 or n = n / 2
        if (tokens[i] == "/" && i + 1 < tokens.size() && tokens[i + 1] == "2")
            return "O(log n)";

        // detect i /= 2
        if (tokens[i] == "/" && i + 2 < tokens.size() &&
            tokens[i + 1] == "=" && tokens[i + 2] == "2")
            return "O(log n)";

        // detect i >>= 1 (bit shift divide by 2)
        if (tokens[i] == ">" && i + 3 < tokens.size() &&
            tokens[i + 1] == ">" && tokens[i + 2] == "=" && tokens[i + 3] == "1")
            return "O(log n)";
    }

    // default loop is linear
    return "O(n)";
}

// Big-O Analyzer 
// FIXED: handles loops without braces by treating next statement as loop body
string BigOdetection(const string& code) {
    vector<string> tokens = tokenize(code);

    stack<string> loopStack;     // loop costs for brace-block loops
    stack<bool>   blockIsLoop;   // for each "{", whether it belongs to a loop
    stack<string> workStack;     // work terms and "{" markers

    bool lastTokenWasLoop = false;

    for (int i = 0; i < (int)tokens.size(); ) {
        string token = tokens[i];

        // 1) for/while loops
        if (token == "for" || token == "while") {
            int endIndex = i;
            vector<string> header = getParenTokens(tokens, i + 1, endIndex);
            string loopCost = classifyLoop(header);

            // If next token is NOT "{", treat next statement as loop body (one statement)
            if (endIndex < (int)tokens.size() && tokens[endIndex] != "{") {
                int j = endIndex;
                while (j < (int)tokens.size() && tokens[j] != ";") j++;
                if (j < (int)tokens.size() && tokens[j] == ";") j++; // consume ';'

                // one statement inside loop => O(1) per iteration
                workStack.push(multiplyBigO(loopCost, "O(1)"));

                i = j;
                lastTokenWasLoop = false;
                continue;
            }

            // Normal brace-body loop
            loopStack.push(loopCost);
            lastTokenWasLoop = true;
            i = endIndex;
            continue;
        }

        // 1b) do-while: treat as a loop start (we assume O(n) here)
        if (token == "do") {
            loopStack.push("O(n)");
            lastTokenWasLoop = true;
            i++;
            continue;
        }

        // 2) Block start
        if (token == "{") {
            workStack.push("{");
            blockIsLoop.push(lastTokenWasLoop);
            lastTokenWasLoop = false;
            i++;
            continue;
        }

        // 3) Statement end
        if (token == ";") {
            workStack.push("O(1)");
            i++;
            continue;
        }

        // 4) Block end
        if (token == "}") {
            string inside = "O(1)";
            while (!workStack.empty() && workStack.top() != "{") {
                inside = dominantBigO(inside, workStack.top());
                workStack.pop();
            }
            if (!workStack.empty() && workStack.top() == "{") workStack.pop();

            bool isLoopBody = false;
            if (!blockIsLoop.empty()) {
                isLoopBody = blockIsLoop.top();
                blockIsLoop.pop();
            }

            if (isLoopBody && !loopStack.empty()) {
                string loopCost = loopStack.top();
                loopStack.pop();
                workStack.push(multiplyBigO(loopCost, inside));
            } else {
                workStack.push(inside);
            }

            i++;
            continue;
        }

        lastTokenWasLoop = false;
        i++;
    }

    // Final reduce (top-level sequential dominant)
    string total = "O(1)";
    while (!workStack.empty()) {
        if (workStack.top() != "{") total = dominantBigO(total, workStack.top());
        workStack.pop();
    }
    while (!loopStack.empty()) {
        total = dominantBigO(total, loopStack.top());
        loopStack.pop();
    }
    return total;
}


// Token Debug Printer
void printTokens(const string& code) {
    vector<string> tokens = tokenize(code);
    cout << "TOKENS: ";
    for (string t : tokens) cout << "[" << t << "] ";
    cout << endl;
}

// ALGORITHM STRINGS
string locateItem =
"void locateItem(int array[], int n, int target) {\n"
"    for (int i = 0; i < n; ++i)\n"
"        if (array[i] == target)\n"
"            return;\n"
"}";

string focusedScan =
"void focusedScan(int array[], int n, int key) {\n"
"    int min = 0, max = n;\n"
"    while (min < max) {\n"
"        int mid = (min + max) / 2;\n"
"        if (array[mid] == key) return;\n"
"        (array[mid] < key) ? min = mid + 1 : max = mid;\n"
"    }\n"
"}";

string stabilizeStream =
"void stabilizeStream(int array[], int n) {\n"
"    bool swapped = true;\n"
"    int pass = 0;\n"
"    while (swapped) {\n"
"        swapped = false;\n"
"        for (int j = 0; j < n - pass - 1; ++j) {\n"
"            if (array[j] > array[j + 1]) {\n"
"                swap(array[j], array[j + 1]);\n"
"                swapped = true;\n"
"            }\n"
"        }\n"
"        ++pass;\n"
"    }\n"
"}";

string slotAligner =
"void slotAligner(int array[], int n) {\n"
"    for (int i = 1; i < n; ++i) {\n"
"        int j = i;\n"
"        while (j > 0 && array[j] < array[j - 1]) {\n"
"            swap(array[j], array[j - 1]);\n"
"            --j;\n"
"        }\n"
"    }\n"
"}";

string minimumPlacer =
"void minimumPlacer(int array[], int n) {\n"
"    for (int i = 0; i < n - 1; ++i) {\n"
"        int minPos = i;\n"
"        for (int j = n - 1; j > i; --j)\n"
"            if (array[j] < array[minPos])\n"
"                minPos = j;\n"
"        if (minPos != i)\n"
"            swap(array[i], array[minPos]);\n"
"    }\n"
"}";

string spacingPass =
"void spacingPass(int array[], int n) {\n"
"    for (int space = n / 2; space > 0; space = int(space / 2.2 + 0.5)) {\n"
"        for (int i = space; i < n; ++i) {\n"
"            int val = array[i], j = i;\n"
"            for (; j >= space && array[j - space] > val; j -= space)\n"
"                array[j] = array[j - space];\n"
"            array[j] = val;\n"
"        }\n"
"    }\n"
"}";

string ladderWalk =
"void ladderWalk(int array[], int n) {\n"
"    int i = 1;\n"
"    do {\n"
"        if (array[i] < array[i - 1]) {\n"
"            swap(array[i], array[i - 1]);\n"
"            if (--i == 0) i = 1;\n"
"            continue;\n"
"        }\n"
"        ++i;\n"
"    } while (i < n);\n"
"}";

string rippleScan =
"void rippleScan(int array[], int n) {\n"
"    bool changes = true;\n"
"    int min = 0, max = n - 1;\n"
"    while (changes) {\n"
"        changes = false;\n"
"        for (int i = min; i < max; ++i) {\n"
"            if (array[i] > array[i + 1]) {\n"
"                swap(array[i], array[i + 1]);\n"
"                changes = true;\n"
"            }\n"
"        }\n"
"        if (!changes) break;\n"
"        changes = false;\n"
"        --max;\n"
"        for (int i = max; i > min; --i) {\n"
"            if (array[i] < array[i - 1]) {\n"
"                swap(array[i], array[i - 1]);\n"
"                changes = true;\n"
"            }\n"
"        }\n"
"        ++min;\n"
"    }\n"
"}";

string shrinkingArea =
"void shrinkingArea(int array[], int n) {\n"
"    int space = n;\n"
"    bool sorted = false;\n"
"    const float shrink = 1.3f;\n"
"    while (!sorted) {\n"
"        space = int(space / shrink);\n"
"        if (space <= 1) {\n"
"            space = 1;\n"
"            sorted = true;\n"
"        }\n"
"        for (int i = 0; i + space < n; ++i) {\n"
"            if (array[i] > array[i + space]) {\n"
"                swap(array[i], array[i + space]);\n"
"                sorted = false;\n"
"            }\n"
"        }\n"
"    }\n"
"}";

string parityWave =
"void parityWave(int array[], int n) {\n"
"    bool done = false;\n"
"    for (int phase = 0; !done; ++phase) {\n"
"        done = true;\n"
"        for (int i = phase % 2; i + 1 < n; i += 2) {\n"
"            if (array[i] > array[i + 1]) {\n"
"                swap(array[i], array[i + 1]);\n"
"                done = false;\n"
"            }\n"
"        }\n"
"    }\n"
"}";

string balance =
"void balance(int array[], int n) {\n"
"    if (n==0) return;\n"
"    int max_val = max_element(array, array+n);\n"
"    vector<int> group(max_val + 1, 0);\n"
"    for (int num : array) {\n"
"        ++group[num];\n"
"    }\n"
"    int index = 0;\n"
"    for (int i = 0; i < group.size(); ++i) {\n"
"        while (group[i]-- > 0) {\n"
"            array[index++] = i;\n"
"        }\n"
"    }\n"
"}";


// ------------------------------------------------------------

int main() {

    cout << "--------Algorithms Test---------------------" << endl;

    cout << "locateItem: "      << BigOdetection(locateItem)      << endl; // locateItem: O(n)
    cout << "focusedScan: "     << BigOdetection(focusedScan)     << endl; // focusedScan: O(n log n)
    cout << "stabilizeStream: " << BigOdetection(stabilizeStream) << endl; // stabilizeStream: O(n^2)
    cout << "slotAligner: "     << BigOdetection(slotAligner)     << endl; // slotAligner: O(n^2)
    cout << "minimumPlacer: "   << BigOdetection(minimumPlacer)   << endl; // minimumPlacer: O(n^2)
    cout << "spacingPass: "     << BigOdetection(spacingPass)     << endl; // spacingPass: O(n^2)
    cout << "ladderWalk: "      << BigOdetection(ladderWalk)      << endl; // ladderWalk: O(n)
    cout << "rippleScan: "      << BigOdetection(rippleScan)      << endl; // rippleScan: O(n^2)
    cout << "shrinkingArea: "   << BigOdetection(shrinkingArea)   << endl; // shrinkingArea: O(n^2)
    cout << "parityWave: "      << BigOdetection(parityWave)      << endl; // parityWave: O(n^2)
    cout << "balance: "         << BigOdetection(balance)         << endl; // balance: O(n^2)

    cout << BigOdetection("while (n > 1) { n = n / 2; }") << endl;  // O(n)
    cout << BigOdetection("for (int i=0;i<n;i++) { x++; }") << endl;  // O(n)
    cout << BigOdetection("for (i=0;i<n;i++) { for (j=0;j<n;j++) { x++; } }") << endl;  // O(n^2)

    cout << "\n--------TOKEN TEST--------\n";
    printTokens("for(int i=0; i<n; i++){ n = n/2; }");

    return 0;
}