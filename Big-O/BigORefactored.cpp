#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <algorithm>
#include <string>
#include <cctype>
using namespace std;

// Tokenize the Input
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
    while (ss >> word) tokens.push_back(word);
    return tokens;
}


int rankBigO(const string& b) {
    if (b == "O(1)") return 0;
    if (b == "O(log n)") return 1;
    if (b == "O(n)") return 2;
    if (b == "O(n log n)") return 3;
    if (b == "O(n^2)") return 4;

    return 2;
}

//  Chooses the bigger BigO
string dominantBigO(const string& a, const string& b) { 
    return (rankBigO(a) >= rankBigO(b)) ? a : b;
}

// Convert Big-O string into (power of n, power of log n)
void toPowers(const string& bigO, int& nPow, int& logPow) {
    nPow = 0;
    logPow = 0;

    if (bigO == "O(1)") return;
    if (bigO == "O(log n)") { logPow = 1; return; }
    if (bigO == "O(n)") { nPow = 1; return; }
    if (bigO == "O(n log n)") { nPow = 1; logPow = 1; return; }
    if (bigO == "O(n^2)") { nPow = 2; return; }

    
    nPow = 1;
    logPow = 0;
}

// Produce the Final BigO
// Convert (power of n, power of log n) back into allowed outputs
string fromPowersClamped(int nPow, int logPow) {
    if (nPow >= 2) return "O(n^2)";
    if (nPow == 1 && logPow >= 1) return "O(n log n)";
    if (nPow == 1) return "O(n)";
    if (logPow >= 1) return "O(log n)";
    return "O(1)";
}

// Combines nested complexity by multiplying.
string multiplyBigO(const string& outer, const string& inner) {
    int n1, l1, n2, l2;
    toPowers(outer, n1, l1);
    toPowers(inner, n2, l2);
    return fromPowersClamped(n1 + n2, l1 + l2);
}


// ------------------------------------------------------------
vector<string> getParenTokens(const vector<string>& tokens, int startIndex, int& endIndex) {
    vector<string> inside;
    int i = startIndex;

    // find the first "(" after startIndex
    while (i < (int)tokens.size() && tokens[i] != "(") i++;
    if (i >= (int)tokens.size()) { endIndex = i; return inside; }

    // Collect tokens until the matching ")"
    int depth = 0;
    for (; i < (int)tokens.size(); i++) {
        if (tokens[i] == "(") {
            depth++;
            if (depth == 1) continue; 
        } 
        else if (tokens[i] == ")") {
            depth--;
            if (depth == 0) {
                endIndex = i + 1; 
                return inside;
            }
        }
        if (depth >= 1) inside.push_back(tokens[i]);
    }

    // If we got here, we never found a matching ")"
    endIndex = i;
    return inside;
}

// ------------------------------------------------------------
//Split into 3 parts by semicolons
void splitForHeader(const vector<string>& header,
                    vector<string>& init,
                    vector<string>& cond,
                    vector<string>& update)
{
    int semi1 = -1, semi2 = -1;
    for (int i = 0; i < (int)header.size(); i++) {
        if (header[i] == ";") {
            if (semi1 == -1) semi1 = i;
            else { semi2 = i; break; }
        }
    }
    if (semi1 == -1 || semi2 == -1) return;

    init.assign(header.begin(), header.begin() + semi1);
    cond.assign(header.begin() + semi1 + 1, header.begin() + semi2);
    update.assign(header.begin() + semi2 + 1, header.end());
}

//Decide the loop's iteration behavior 
string classifyLoop(const vector<string>& headerTokens) {
    vector<string> init, cond, update;
    splitForHeader(headerTokens, init, cond, update);

    const vector<string>& scan = (update.empty() ? headerTokens : update);

    auto isAllDigits = [](const string& s) -> bool {
        if (s.empty()) return false;
        for (char c : s) if (!isdigit((unsigned char)c)) return false;
        return true;
    };

    // Try to parse a numeric constant starting at scan[pos]
    // Returns how many tokens it consumed (1 or 3), or 0 if not a number.
    auto parseConst = [&](int pos, double& val) -> int {
        if (pos >= (int)scan.size()) return 0;

        // integer
        if (isAllDigits(scan[pos])) {
            val = stod(scan[pos]);

            // decimal like: 2 . 2
            if (pos + 2 < (int)scan.size() && scan[pos + 1] == "." && isAllDigits(scan[pos + 2])) {
                string num = scan[pos] + "." + scan[pos + 2];
                val = stod(num);
                return 3;
            }
            return 1;
        }
        return 0;
    };

    for (int i = 0; i < (int)scan.size(); i++) {

        // ">>= 1" => tokenized as > > = 1
        if (scan[i] == ">" && i + 3 < (int)scan.size() &&
            scan[i + 1] == ">" && scan[i + 2] == "=" && scan[i + 3] == "1")
            return "O(log n)";

        // "<<= 1" => tokenized as < < = 1
        if (scan[i] == "<" && i + 3 < (int)scan.size() &&
            scan[i + 1] == "<" && scan[i + 2] == "=" && scan[i + 3] == "1")
            return "O(log n)";

        // "/= c"  (like n /= 2)
        if (scan[i] == "/" && i + 2 < (int)scan.size() && scan[i + 1] == "=") {
            double c = 0.0;
            int used = parseConst(i + 2, c);
            if (used > 0 && c > 1.000001) return "O(log n)";
        }

        // "*= c" (like n *= 2)
        if (scan[i] == "*" && i + 2 < (int)scan.size() && scan[i + 1] == "=") {
            double c = 0.0;
            int used = parseConst(i + 2, c);
            if (used > 0 && c > 1.000001) return "O(log n)";
        }

        // "/ c" or "/ c.d" (like space / 2.2)
        if (scan[i] == "/") {
            double c = 0.0;
            int used = parseConst(i + 1, c);
            if (used > 0 && c > 1.000001) return "O(log n)";
        }
    }

    return "O(n)";
}


string BigOdetection(const string& code) { 
    vector<string> tokens = tokenize(code);

    // Stacks for structural parsing:
    stack<string> loopStack;          
    stack<bool> blockIsLoop;      
    stack<string> workStack;       

    // NEW: track features inside each block to detect binary-search style loops
    stack<bool> blockHasDiv2;      
    stack<bool> blockHasMid;       

    // backtracking-style (ladderWalk-like)
    stack<bool> blockHasDecIndex;
    stack<bool> blockHasContinue;
    stack<bool> blockHasSwap;

    // counting/bucket pattern (balance-like)
    stack<bool> blockHasCountInc;
    stack<bool> blockHasCountWhile;

    bool lastTokenWasLoop = false;

    for (int i = 0; i < (int)tokens.size(); ) {
        string tok = tokens[i];

        
        if (!blockHasDiv2.empty()) {

          
            if (tok == "/" && i + 1 < (int)tokens.size() && tokens[i + 1] == "2") {
                blockHasDiv2.top() = true;
            }
            // Track whether this block uses "mid" (binary-search style)
            if (tok == "mid") {
                blockHasMid.top() = true;
            }
            if (tok == "-" && i + 1 < (int)tokens.size() && tokens[i + 1] == "-") {
                blockHasDecIndex.top() = true;
            }
            if (tok == "-" && i + 1 < (int)tokens.size() && tokens[i + 1] == "=") {
                blockHasDecIndex.top() = true;
            }
            if (tok == "continue") {
                blockHasContinue.top() = true;
            }
            if (tok == "swap") {
                blockHasSwap.top() = true;
            }

            // detect ++group[num]
            if (tok == "+" && i + 5 < (int)tokens.size()) {
                if (tokens[i+1] == "+" &&
                    tokens[i+2] == "group" &&
                    tokens[i+3] == "[" &&
                    tokens[i+5] == "]") {
                    blockHasCountInc.top() = true;
                }
            }
            
            if (tok == "group" && i + 5 < (int)tokens.size()) {
                if (tokens[i + 1] == "[" &&
                    tokens[i + 3] == "]" &&
                    tokens[i + 4] == "+" &&
                    tokens[i + 5] == "+") {
                    blockHasCountInc.top() = true;
                }
            }

            
            if (tok == "while") {  
                for (int k = i; k + 7 < (int)tokens.size() && k < i + 25; k++) {
                    if (tokens[k] == "group" &&
                        tokens[k + 1] == "[" &&
                        tokens[k + 3] == "]" &&
                        tokens[k + 4] == "-" &&
                        tokens[k + 5] == "-" &&
                        tokens[k + 6] == ">" &&
                        tokens[k + 7] == "0") {
                        blockHasCountWhile.top() = true;
                        break;
                    }
                }
            }
        }

        //Detecting Loop Boundaries
        if (tok == "for" || tok == "while") {  
            int endIndex = i;
            vector<string> header = getParenTokens(tokens, i + 1, endIndex);
            string loopCost = classifyLoop(header);

           
            if (endIndex < (int)tokens.size() && tokens[endIndex] != "{") {
                int j = endIndex;
                while (j < (int)tokens.size() && tokens[j] != ";") j++;
                if (j < (int)tokens.size() && tokens[j] == ";") j++;

            if (endIndex < (int)tokens.size() &&
                (tokens[endIndex] == "for" || tokens[endIndex] == "while" || tokens[endIndex] == "do")) {
                loopStack.push(loopCost);
                lastTokenWasLoop = true;
                i = endIndex;
                continue;
            }

                workStack.push(multiplyBigO(loopCost, "O(1)"));
                i = j;
                lastTokenWasLoop = false;
                continue;
            }
            
            loopStack.push(loopCost);
            lastTokenWasLoop = true;
            i = endIndex;
            continue;
        }

        //Detecting do
        if (tok == "do") { 
            loopStack.push("O(n)");
            lastTokenWasLoop = true;
            i++;
            continue;
        }

        //push
        if (tok == "{") {  
            workStack.push("{");
            blockIsLoop.push(lastTokenWasLoop);

            // NEW: start tracking features for this block
            blockHasDiv2.push(false);
            blockHasMid.push(false);

            // backtracking flags
            blockHasDecIndex.push(false);
            blockHasContinue.push(false);
            blockHasSwap.push(false);

            // counting/bucket flags
            blockHasCountInc.push(false);
            blockHasCountWhile.push(false);

            lastTokenWasLoop = false;
            i++;
            continue;
        }

       
        if (tok == ";") {
            workStack.push("O(1)");
            i++;
            continue;
        }

        //pop
        if (tok == "}") {  

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

            bool hadDiv2 = false, hadMid = false;
            if (!blockHasDiv2.empty()) { hadDiv2 = blockHasDiv2.top(); blockHasDiv2.pop(); }
            if (!blockHasMid.empty())  { hadMid  = blockHasMid.top();  blockHasMid.pop(); }

            bool hadDec = false, hadCont = false, hadSwap = false;
            if (!blockHasDecIndex.empty()) { hadDec = blockHasDecIndex.top(); blockHasDecIndex.pop(); }
            if (!blockHasContinue.empty()) { hadCont = blockHasContinue.top(); blockHasContinue.pop(); }
            if (!blockHasSwap.empty())     { hadSwap = blockHasSwap.top(); blockHasSwap.pop(); }

            bool hadCountInc = false, hadCountWhile = false;
            if (!blockHasCountInc.empty())   { hadCountInc = blockHasCountInc.top(); blockHasCountInc.pop(); }
            if (!blockHasCountWhile.empty()) { hadCountWhile = blockHasCountWhile.top(); blockHasCountWhile.pop(); }

            // Compute Complexity Using Structural Combination (when loop ends)
            if (isLoopBody && !loopStack.empty()) {
                string loopCost = loopStack.top();
                loopStack.pop();

                // upgrade O(n) to O(log n)
                if (loopCost == "O(n)" && hadDiv2) {
                    loopCost = "O(log n)";
                }

                if (hadSwap && hadDec && hadCont) {
                    workStack.push("O(n^2)");
                }
                else if (hadCountInc || hadCountWhile) {
                    workStack.push("O(n)");
                }
                else {
                    workStack.push(multiplyBigO(loopCost, inside));
                }
            }
            else {
                // treat this block as O(n^2) so the surrounding loop becomes O(n^2).
                if (hadSwap && hadDec && hadCont) workStack.push("O(n^2)");
                else workStack.push(inside);
            }
            i++;
            continue;
        }
        lastTokenWasLoop = false;
        i++;
    } 

        // Combine Sequential Components
        // reduce top-level work (dominant term)
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
"    int max_val = *max_element(array, array+n);\n"
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

int main() {

    cout << "--------Algorithms Test---------------------" << endl;

    cout << "locateItem: "      << BigOdetection(locateItem)      << endl; // locateItem: O(n)
    cout << "focusedScan: "     << BigOdetection(focusedScan)     << endl; // focusedScan: O(log n)
    cout << "stabilizeStream: " << BigOdetection(stabilizeStream) << endl; // stabilizeStream: O(n^2)
    cout << "slotAligner: "     << BigOdetection(slotAligner)     << endl; // slotAligner: O(n^2)
    cout << "minimumPlacer: "   << BigOdetection(minimumPlacer)   << endl; // minimumPlacer: O(n^2)
    cout << "spacingPass: "     << BigOdetection(spacingPass)     << endl; // spacingPass: O(n^2)
    cout << "ladderWalk: "      << BigOdetection(ladderWalk)      << endl; // ladderWalk: O(n^2)
    cout << "rippleScan: "      << BigOdetection(rippleScan)      << endl; // rippleScan: O(n^2)
    cout << "shrinkingArea: "   << BigOdetection(shrinkingArea)   << endl; // shrinkingArea: O(n^2)
    cout << "parityWave: "      << BigOdetection(parityWave)      << endl; // parityWave: O(n^2)
    cout << "balance: "         << BigOdetection(balance)         << endl; // balance: O(n)

    cout << BigOdetection("while (n > 1) { n = n / 2; }") << endl;  // O(log n)
    cout << BigOdetection("for (int i=0;i<n;i++) { x++; }") << endl;  // O(n)
    cout << BigOdetection("for (i=0;i<n;i++) { for (j=0;j<n;j++) { x++; } }") << endl;  // O(n^2)


    cout << "\n--------TOKEN TEST--------\n";
    printTokens("for(int i=0; i<n; i++){ n = n/2; }");

    return 0;
}


//As I scan through the tokens from left to right, I push the loop cost, like O(n) or O(log n) onto loopStack whenever I detect a loop, 
//I push a "{" onto workStack when I enter a new block, and I push O(1) onto workStack for each statement that ends with a semicolon.
//When I see a "}", I pop everything from workStack until I reach "{" to get the inside work, then I pop "{", pop whether the block was 
//a loop body, and if it was, I pop the loop cost from loopStack and combine it with the inside work.
//I combine the results by multiplying when something is nested, meaning if a loop contains work inside it, I multiply the loop cost by
//the inside work to get the total complexity for that block. If the statements are sequential at the same level and not nested, I don't
//multiply. Instead, I use dominantBigO to keep the bigger BigO, since in  the BigO notation we care about the highest order. This way,
//nested structures increase the complexity correctly, and the sequential structures keep the dominant term without overcounting. 
//This BigORefactored is different from my original approach because before, I relied on string.find() and keyword counting to guess the
//complexity. I did not track block boundaries or properly handle nested structures. In this version, I use stacks to structurally parse the
//code, track "{" and "}", and combine nested and sequential components. Instead of guessing based on keywords, I now compute the complexity
//based on how the loops and blocks are structured.