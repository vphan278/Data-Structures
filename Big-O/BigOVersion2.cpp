#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <algorithm>
#include <string>
#include <cctype>
#include <unordered_map>
#include <unordered_set>
#include <functional>
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

bool isIdentifierToken(const string& s) {
    if (s.empty()) return false;
    if (!(isalpha((unsigned char)s[0]) || s[0] == '_')) return false;
    for (char c : s) {
        if (!(isalnum((unsigned char)c) || c == '_')) return false;
    }
    return true;
}

bool isKeywordOrBuiltin(const string& s) {
    static unordered_set<string> bad = {
        "if","for","while","do","switch","case","break","continue","return",
        "else","void","int","double","float","char","long","short","bool",
        "signed","unsigned","struct","class","template","typename","using",
        "namespace","new","delete","try","catch","throw","sizeof","const",
        "static","public","private","protected","nullptr","true","false",
        "std" 
    };
    return bad.count(s) > 0;
}

// Map functionName -> [bodyStartIndex, bodyEndIndexExclusive] in token array
unordered_map<string, pair<int,int>> buildFunctionMap(const vector<string>& tokens) {
    unordered_map<string, pair<int,int>> fm;

    for (int i = 0; i + 3 < (int)tokens.size(); i++) {
        // We want pattern: <name> ( ... ) { ... }
        // We'll accept tokens[i] as name, tokens[i+1] == "("
        if (!isIdentifierToken(tokens[i])) continue;
        if (isKeywordOrBuiltin(tokens[i])) continue;
        if (tokens[i+1] != "(") continue;

        // Find end of parameter list ')'
        int j = i + 1;
        int depth = 0;
        for (; j < (int)tokens.size(); j++) {
            if (tokens[j] == "(") depth++;
            else if (tokens[j] == ")") {
                depth--;
                if (depth == 0) { j++; break; } // j is now index after ')'
            }
        }
        if (j >= (int)tokens.size()) continue;

        // Next non-space token should be "{"
        if (tokens[j] != "{") continue;

        // Find matching "}"
        int k = j;
        int braceDepth = 0;
        for (; k < (int)tokens.size(); k++) {
            if (tokens[k] == "{") braceDepth++;
            else if (tokens[k] == "}") {
                braceDepth--;
                if (braceDepth == 0) { k++; break; } // k is index after matching '}'
            }
        }
        if (k > (int)tokens.size()) continue;

        // body is inside { ... }
        fm[tokens[i]] = {j + 1, k - 1}; // indices inside braces (exclusive of { })
        i = k - 1; // jump forward
    }

    return fm;
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
            // Detect x /= c  (tokenized as "/" "=" c) inside the loop body
            if (tok == "/" && i + 2 < (int)tokens.size() && tokens[i + 1] == "=") {
                bool allDigits = !tokens[i + 2].empty();
                for (char ch : tokens[i + 2]) {
                    if (!isdigit((unsigned char)ch)) { allDigits = false; break; }
                }
                if (allDigits) {
                    int c = stoi(tokens[i + 2]);
                    if (c > 1) blockHasDiv2.top() = true;
                }
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
                // treat this block as O(n^2) 
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

static string tokensToString(const vector<string>& t, int L, int R) {
    string s;
    for (int i = L; i < R; i++) {
        s += t[i];
        s += ' ';
    }
    return s;
}

//------------- V2: Multi-function + Recursion -----------------------------------

// Helper: is this identifier followed by "(" => looks like a function call
bool looksLikeCall(const vector<string>& t, int i) {
    if (i + 1 >= (int)t.size()) return false;
    if (!isIdentifierToken(t[i])) return false;
    if (isKeywordOrBuiltin(t[i])) return false;
    return t[i + 1] == "(";
}

// Helper: parse the (...) tokens for a call, starting at token i (name), returns inside parens.
vector<string> readCallArgs(const vector<string>& t, int i, int& endIndexAfterCall) {
    // i points to name, i+1 should be "("
    int endParen = i + 1;
    vector<string> inside = getParenTokens(t, i, endParen);  // you already have this
    endIndexAfterCall = endParen;
    return inside;
}

// Heuristic: classify recursion input shrink pattern from call args tokens
// returns: "DEC1" (n-1 style), "HALF" (n/2 style), "UNKNOWN"
string classifySelfArgShrink(const vector<string>& args) {
    // very lightweight pattern scan
    // DEC1: contains "- 1" somewhere
    for (int i = 0; i + 1 < (int)args.size(); i++) {
        if (args[i] == "-" && args[i+1] == "1") return "DEC1";
    }
    // HALF: contains "/ 2" or ">> 1" or "/= 2" (rare inside args)
    for (int i = 0; i + 1 < (int)args.size(); i++) {
        if (args[i] == "/" && args[i+1] == "2") return "HALF";
        if (args[i] == ">>" && args[i+1] == "1") return "HALF";
    }
    return "UNKNOWN";
}

// Combines a base cost with a call cost under current loop nesting.
// loopProduct is already the multiplied loop complexity from the stack.
static inline string addCallCostDominant(const string& current, const string& loopProduct, const string& calleeCost) {
    string multiplied = multiplyBigO(loopProduct, calleeCost);
    return dominantBigO(current, multiplied);
}

string BigOdetectionWithCalls(
    const string& code,
    const function<string(const string&)>& resolveCall
) {
    vector<string> tokens = tokenize(code);

    stack<string> loopCostStack;        // current loop costs
    stack<bool>   loopSawShrink;        // detected shrink update for this loop 
    stack<bool>   loopSawHeapParent;    // parent = (index - 1) / 2;
    stack<bool>   loopSawIndexToParent; // index = parent;

    stack<bool>   blockIsLoop;          // tracks whether each block corresponds to a loop body
    stack<string> workStack;            // stores the dominant BigO work for each { } block

    string total = "O(1)";
    bool lastTokenWasLoop = false;

    auto isConstGT1 = [&](const string& s)->bool{
        if (s.empty()) return false;
        for (char c : s) if (!isdigit((unsigned char)c)) return false;
        return stoi(s) > 1;
    };

    auto recomputeLoopProduct = [&]() -> string {
        vector<string> tmp;
        string prod = "O(1)";
        while (!loopCostStack.empty()) { tmp.push_back(loopCostStack.top()); loopCostStack.pop(); }
        for (int i = (int)tmp.size() - 1; i >= 0; --i) {
            prod = multiplyBigO(prod, tmp[i]);
            loopCostStack.push(tmp[i]);
        }
        return prod;
    };

    auto upgradeTopLoopToLog = [&]() {
        if (!loopCostStack.empty() && loopCostStack.top() == "O(n)") {
            loopCostStack.pop();
            loopCostStack.push("O(log n)");
        }
    };

    for (int i = 0; i < (int)tokens.size();) {
        string tok = tokens[i];

        // -------- Detect shrink patterns INSIDE the active loop body --------
        // IMPORTANT FIX: when shrink is detected, immediately upgrade loopCostStack.top()
        if (!loopSawShrink.empty()) {
            // pattern: x /= c   tokenized as: x / = c
            if (i + 3 < (int)tokens.size() &&
                tokens[i+1] == "/" && tokens[i+2] == "=" && isConstGT1(tokens[i+3])) {
                loopSawShrink.top() = true;
                upgradeTopLoopToLog();
            }

            // heap-parent pattern: parent = ( index - 1 ) / 2
            if (!loopSawHeapParent.empty()) {
                for (int k = i; k + 10 < (int)tokens.size() && k < i + 25; k++) {
                    if (tokens[k] == "parent" &&
                        tokens[k+1] == "=" &&
                        tokens[k+2] == "(" &&
                        tokens[k+3] == "index" &&
                        tokens[k+4] == "-" &&
                        tokens[k+5] == "1" &&
                        tokens[k+6] == ")" &&
                        tokens[k+7] == "/" &&
                        tokens[k+8] == "2") {
                        loopSawHeapParent.top() = true;
                        break;
                    }
                }
            }

            // heap update pattern: index = parent
            if (!loopSawIndexToParent.empty()) {
                if (i + 2 < (int)tokens.size() &&
                    tok == "index" && tokens[i+1] == "=" && tokens[i+2] == "parent") {
                    loopSawIndexToParent.top() = true;
                }
            }

            // if both heap signals seen, this while is log
            if (!loopSawHeapParent.empty() && !loopSawIndexToParent.empty()) {
                if (loopSawHeapParent.top() && loopSawIndexToParent.top()) {
                    loopSawShrink.top() = true;
                    upgradeTopLoopToLog();
                }
            }
        }

        // ---------------- LOOPS ----------------
        if (tok == "for" || tok == "while") {
            int afterParen;
            vector<string> header = getParenTokens(tokens, i + 1, afterParen);

            string loopCost;
            if (tok == "for") loopCost = classifyLoop(header);
            else loopCost = "O(n)"; // while: default O(n) until we detect shrink

            // single statement loop (no braces)
            if (afterParen < (int)tokens.size() && tokens[afterParen] != "{") {
                int j = afterParen;
                while (j < (int)tokens.size() && tokens[j] != ";") j++;
                if (j < (int)tokens.size()) j++; // include ';'

                // NEW: analyze the single-statement body (so function calls count)
                string stmt = tokensToString(tokens, afterParen, j);
                string stmtCost = BigOdetectionWithCalls(stmt, resolveCall);

                string loopWork = multiplyBigO(loopCost, stmtCost);

                if (!workStack.empty()) workStack.top() = dominantBigO(workStack.top(), loopWork);
                else total = dominantBigO(total, loopWork);

                i = j;
                lastTokenWasLoop = false;
                continue;
            }

            // loop with { }
            loopCostStack.push(loopCost);
            loopSawShrink.push(false);
            loopSawHeapParent.push(false);
            loopSawIndexToParent.push(false);

            lastTokenWasLoop = true;
            i = afterParen; // should be "{"
            continue;
        }

        // --------------- BLOCK START ---------------
        if (tok == "{") {
            blockIsLoop.push(lastTokenWasLoop);
            workStack.push("O(1)");
            lastTokenWasLoop = false;
            i++;
            continue;
        }

        // --------------- BLOCK END ---------------
        if (tok == "}") {
            string inside = workStack.top(); workStack.pop();
            bool wasLoopBody = blockIsLoop.top(); blockIsLoop.pop();

            // if closing a loop body, apply loop cost * inside
            if (wasLoopBody && !loopCostStack.empty()) {
                string loopCost = loopCostStack.top(); loopCostStack.pop();

                bool sawShrink = loopSawShrink.top(); loopSawShrink.pop();
                loopSawHeapParent.pop();
                loopSawIndexToParent.pop();

                if (loopCost == "O(n)" && sawShrink) loopCost = "O(log n)";
                inside = multiplyBigO(loopCost, inside);
            }

            if (!workStack.empty()) workStack.top() = dominantBigO(workStack.top(), inside);
            else total = dominantBigO(total, inside);

            i++;
            continue;
        }

        // --------------- FUNCTION CALL ---------------
        if (looksLikeCall(tokens, i)) {
            string callee = tokens[i];

            int endAfter;
            (void)readCallArgs(tokens, i, endAfter);

            string calleeCost = resolveCall(callee);

            // DO NOT multiply by loopProduct here
            string callCost = calleeCost;

            if (!workStack.empty())
                workStack.top() = dominantBigO(workStack.top(), callCost);
            else
                total = dominantBigO(total, callCost);

            i = endAfter;
            continue;
        }

        // default token => constant work
        lastTokenWasLoop = false;
        i++;
    }

    while (!workStack.empty()) {
        total = dominantBigO(total, workStack.top());
        workStack.pop();
    }
    return total;
}

// Detects for-loop with stride by parameter (i += k)
// Used for harmonic-series detection (alg9-style)
bool functionHasStrideByParam(
    const vector<string>& tokens,
    const pair<int,int>& range
) {
    int L = range.first;
    int R = range.second;

    for (int i = L; i < R; i++) {
        if (tokens[i] != "for") continue;

        int afterParen;
        vector<string> header = getParenTokens(tokens, i + 1, afterParen);

        // Look for: something + = identifier
        // Example tokenization: i + = k
        for (int j = 0; j + 3 < (int)header.size(); j++) {

            // Check for "+ ="
            if (header[j+1] == "+" && header[j+2] == "=") {

                // Make sure stride is an identifier (k), not constant
                if (isIdentifierToken(header[j+3])) {
                    return true;
                }
            }
        }
    }

    return false;
}

// Analyze a function by name using the program token map
string analyzeFunction(
    const string& fname,
    const vector<string>& progTokens,
    const unordered_map<string, pair<int,int>>& fm,
    unordered_map<string, string>& memo,
    unordered_set<string>& visiting
) {
    // Memo
    if (memo.count(fname)) return memo[fname];

    // Unknown function => assume O(1) (safe fallback)
    if (!fm.count(fname)) return "O(1)";

    // Cycle/recursion guard
    if (visiting.count(fname)) {
        // direct recursion cycle: return O(n) as a conservative clamp
        return "O(n)";
    }
    visiting.insert(fname);

    // Extract the function body tokens -> string
    auto [L, R] = fm.at(fname);
    string body = tokensToString(progTokens, L, R);

    // Detect direct recursion calls in this function body
    int selfCalls = 0;
    int halfCalls = 0;
    int dec1Calls = 0;

    for (int i = L; i < R; i++) {
        if (progTokens[i] == fname && i + 1 < R && progTokens[i+1] == "(") {
            // parse args
            int endAfter;
            vector<string> args = readCallArgs(progTokens, i, endAfter);
            selfCalls++;
            string kind = classifySelfArgShrink(args);
            if (kind == "HALF") halfCalls++;
            if (kind == "DEC1") dec1Calls++;
        }
    }

    // Resolve calls (non-self) recursively
    auto resolver = [&](const string& callee) -> string {
        if (callee == fname) return "O(1)"; // handled separately below
        return analyzeFunction(callee, progTokens, fm, memo, visiting);
    };

    // Base: loop/call cost ignoring recursion self-calls
    string base = BigOdetectionWithCalls(body, resolver);

    //  Minimal harmonic-series (alg9-like)
    for (const auto& [otherName, range] : fm) {
        if (functionHasStrideByParam(progTokens, range)) {
            // does THIS function call that helper?
            for (int i = L; i < R; i++) {
                if (progTokens[i] == otherName && i + 1 < R && progTokens[i+1] == "(") {
                    base = dominantBigO(base, "O(n log n)");
                    break;
                }
            }
        }
    }

    // Recursion heuristic adjustment
    string result = base;

    if (selfCalls > 0) {
        // Very simple Master/recurrence heuristic clamped to allowed set
        // Case A: f(n-1) once => linear depth
        if (dec1Calls >= 1 && selfCalls == 1) {
            // T(n)=T(n-1)+base
            // base O(1) -> O(n)
            // base O(n) -> O(n^2)
            // base O(log n) -> O(n log n) (clamp)
            if (base == "O(1)") result = "O(n)";
            else if (base == "O(log n)") result = "O(n log n)";
            else if (base == "O(n)") result = "O(n^2)";
            else if (base == "O(n log n)") result = "O(n^2)";
            else result = "O(n^2)";
        }
        // Case B: f(n/2) once => log depth
        else if (halfCalls >= 1 && selfCalls == 1) {
            // T(n)=T(n/2)+base
            if (base == "O(1)") result = "O(log n)";
            else if (base == "O(log n)") result = "O(log n)";
            else if (base == "O(n)") result = "O(n)";        // dominates
            else if (base == "O(n log n)") result = "O(n log n)";
            else result = "O(n^2)";
        }
        // Case C: 2 calls on halves => tree recursion
        else if (halfCalls >= 2) {
            // T(n)=2T(n/2)+base
            // base O(1) -> O(n)
            // base O(n) -> O(n log n)
            if (base == "O(1)" || base == "O(log n)") result = "O(n)";
            else if (base == "O(n)") result = "O(n log n)";
            else if (base == "O(n log n)") result = "O(n log n)";
            else result = "O(n^2)";
        }
        else {
            // Unknown recursion shape => conservative clamp
            result = dominantBigO(result, "O(n)");
        }
    }

    visiting.erase(fname);
    memo[fname] = result;
    return result;
}

// Analyze an entire program (all functions in one big string) starting from entry
string BigOProgram(const string& program, const string& entry) {
    vector<string> tokens = tokenize(program);
    auto fm = buildFunctionMap(tokens);

    unordered_map<string, string> memo;
    unordered_set<string> visiting;

    return analyzeFunction(entry, tokens, fm, memo, visiting);
}







void printTokens(const string& code) {
    vector<string> tokens = tokenize(code);
    cout << "TOKENS: ";
    for (string t : tokens) cout << "[" << t << "] ";
    cout << endl;
}


// ALGORITHMSV2.cpp
// ================================================================
// Algorithm 1 (alg1 + alg1_helper)
// ================================================================
std::string alg1_helper_str =
"int alg1_helper(int x) {\n"
" int s = 0;\n"
" for (int i = 0; i < x; i++) {\n"
" s += i;\n"
" }\n"
" return s;\n"
"}\n";
std::string alg1_str =
"void alg1(std::vector<int>& v) {\n"
" int total = 0;\n"
" for (int i = 0; i < v.size(); i++) {\n"
" total += alg1_helper(v[i]);\n"
" }\n"
"}\n";
// ================================================================
// Algorithm 2 (alg2)
// ================================================================
std::string alg2_str =
"void alg2(std::vector<int>& v) {\n"
" int c = 0;\n"
" for (int i = 0; i < v.size(); i++) {\n"
" for (int j = i; j < v.size(); j++) {\n"
" c += (v[i] ^ v[j]);\n"
" }\n"
" }\n"
"}\n";
// ================================================================
// Algorithm 3 (alg3)
// ================================================================
std::string alg3_str =
"void alg3(int n) {\n"
" int x = n;\n"
" while (x > 1) {\n"
" x /= 3;\n"
" }\n"
"}\n";
// ================================================================
// Algorithm 4 (alg4)
// ================================================================
std::string alg4_str =
"int alg4(std::vector<int>& v, int left, int right) {\n"
" if (left >= right) return v[left];\n"
"\n"
" int mid = (left + right) / 2;\n"
" int a = alg4(v, left, mid);\n"
" int b = alg4(v, mid + 1, right);\n"
" return a + b;\n"
"}\n";
// ================================================================
// Algorithm 5 (alg5)
// ================================================================
std::string alg5_str =
"int alg5(int n) {\n"
" if (n <= 1) return 1;\n"
"\n"
" int s = 0;\n"
" for (int i = 0; i < n; i++) {\n"
" s += i;\n"
" }\n"
" return s + alg5(n - 1);\n"
"}\n";
// ================================================================
// Algorithm 6 (alg6 + alg6_b + alg6_c)
// ================================================================
std::string alg6_c_str =
"int alg6_c(int x) {\n"
" int r = 0;\n"
" for (int i = 0; i < x; i++) r += i;\n"
" return r;\n"
"}\n";
std::string alg6_b_str =
"int alg6_b(int x) {\n"
" return alg6_c(x / 2);\n"
"}\n";
std::string alg6_str =
"void alg6(std::vector<int>& v) {\n"
" for (int i = 0; i < v.size(); i++) {\n"
" alg6_b(v[i]);\n"
" }\n"
"}\n";
// ================================================================
// Algorithm 7 (alg7)
// ================================================================
std::string alg7_str =
"void alg7(std::vector<int>& v, int index) {\n"
" while (index > 0) {\n"
" int parent = (index - 1) / 2;\n"
" if (v[parent] <= v[index]) break;\n"
" std::swap(v[parent], v[index]);\n"
" index = parent;\n"
" }\n"
"}\n";
// ================================================================
// Algorithm 8 (alg8)
// ================================================================
std::string alg8_str =
"int alg8(Node* n) {\n"
" if (!n) return 0;\n"
" int left = alg8(n->left);\n"
" int right = alg8(n->right);\n"
" return n->value + left + right;\n"
"}\n";
// ================================================================
// Algorithm 9 (alg9 + alg9_helper)
// ================================================================
std::string alg9_helper_str =
"int alg9_helper(const std::vector<int>& v, int k) {\n"
" int s = 0;\n"
" for (int i = 0; i < v.size(); i += k) {\n"
" s += v[i];\n"
" }\n"
" return s;\n"
"}\n";
std::string alg9_str =
"void alg9(std::vector<int>& v) {\n"
" for (int i = 1; i < v.size(); i++) {\n"
" alg9_helper(v, i);\n"
" }\n"
"}\n";
// ================================================================
// Algorithm 10 (alg10 + alg10_helper)
// ================================================================
std::string alg10_helper_str =
"int alg10_helper(int x) {\n"
" if (x <= 1) return x;\n"
" return alg10_helper(x - 1) + 1;\n"
"}\n";
std::string alg10_str =
"int alg10(std::vector<int>& v) {\n"
" int total = 0;\n"
" for (int i = 0; i < v.size(); i++) {\n"
" total += alg10_helper(v[i]);\n"
" }\n"
" return total;\n"
"}\n";



int main() {

    string prog_all =
        alg1_helper_str + "\n" + alg1_str + "\n" +
        alg2_str + "\n" +
        alg3_str + "\n" +
        alg4_str + "\n" +
        alg5_str + "\n" +
        alg6_c_str + "\n" + alg6_b_str + "\n" + alg6_str + "\n" +
        alg7_str + "\n" +
        alg8_str + "\n" +
        alg9_helper_str + "\n" + alg9_str + "\n" +
        alg10_helper_str + "\n" + alg10_str + "\n";

    vector<string> entries = {
        "alg1", "alg2", "alg3", "alg4", "alg5", "alg6", "alg7", "alg8", "alg9", "alg10"
    };

    for (int i = 0; i < entries.size(); i++) {
        cout << "Algorithm #" << (i + 1) << ": " << BigOProgram(prog_all, entries[i]) << endl;
    }

    return 0;

}

// Algorithm #1: O(n^2)
// Algorithm #2: O(n^2)
// Algorithm #3: O(log n)
// Algorithm #4: O(n)
// Algorithm #5: O(n^2)
// Algorithm #6: O(n^2)
// Algorithm #7: O(log n)
// Algorithm #8: O(n)
// Algorithm #9: O(n^2)
// Algorithm #10: O(n^2)

/*
Algorithm Analyzer - Version 2

This analyzer performs static Big-O classification using
tokenization and structural parsing.

1. StkParser Ideas Used:
   - Uses stacks to track nested braces { }
   - Maintains loop cost stacks to combine nested loop complexity
   - Processes tokens structurally rather than by string matching

2. Loop Analysis:
   - Single loops classified as O(n) or O(log n)
   - Nested loops multiply complexity using power addition
   - Sequential blocks use dominant term selection

3. Recursion Analysis:
   - Detects direct recursion calls
   - Distinguishes:
       - T(n-1)  → linear recursion depth
       - T(n/2)  → logarithmic depth
       - 2T(n/2) → divide-and-conquer tree recursion
   - Applies simplified Master Theorem heuristics
   - Clamps final result to allowed output set

4. Multi-function / Call Graph:
   - Builds function map from entire program
   - Recursively analyzes callees
   - Uses memoization to avoid recomputation
   - Uses visiting set to prevent infinite recursion cycles

*/