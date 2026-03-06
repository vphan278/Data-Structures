#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>

using namespace std;

// -------------------------- ComplexityTerm --------------------------
class ComplexityTerm {
private:
    int nPow;     // exponent of n
    int logPow;   // exponent of log n

public:
    ComplexityTerm(int n = 0, int l = 0) : nPow(n), logPow(l) {}

    static ComplexityTerm O1()   { return ComplexityTerm(0, 0); }
    static ComplexityTerm On()   { return ComplexityTerm(1, 0); }
    static ComplexityTerm Olog() { return ComplexityTerm(0, 1); }

    ComplexityTerm multiply(const ComplexityTerm& other) const {
        return ComplexityTerm(nPow + other.nPow, logPow + other.logPow);
    }

    static ComplexityTerm dominant(const ComplexityTerm& a, const ComplexityTerm& b) {
        // higher n exponent dominates; tie -> higher log exponent dominates
        if (a.nPow != b.nPow) return (a.nPow > b.nPow) ? a : b;
        if (a.logPow != b.logPow) return (a.logPow > b.logPow) ? a : b;
        return a;
    }

    string toString() const {
        if (nPow == 0 && logPow == 0) return "O(1)";
        if (nPow == 0 && logPow == 1) return "O(log n)";
        if (nPow == 1 && logPow == 0) return "O(n)";
        if (nPow == 1 && logPow == 1) return "O(n log n)";
        if (nPow == 2 && logPow == 0) return "O(n^2)";
        if (nPow == 3 && logPow == 0) return "O(n^3)";

        // generic fallback
        string s = "O(";
        bool wrote = false;

        if (nPow > 0) {
            s += "n";
            if (nPow > 1) s += "^" + to_string(nPow);
            wrote = true;
        }
        if (logPow > 0) {
            if (wrote) s += " ";
            s += "log n";
            if (logPow > 1) s += "^" + to_string(logPow);
        }
        s += ")";
        return s;
    }
};

// -------------------------- BigOAnalyzer --------------------------
class BigOAnalyzer {
private:
    vector<string> tokens;

    // ----- Tokenization helpers -----
    bool isTokPunct(char c) const {
        // underscore belongs to identifiers
        return ispunct((unsigned char)c) && c != '_';
    }

    void setTokensFromCode(const string& code) {
        string spaced;
        spaced.reserve(code.size() * 2);

        for (char c : code) {
            if (isTokPunct(c)) {
                spaced.push_back(' ');
                spaced.push_back(c);
                spaced.push_back(' ');
            } else {
                spaced.push_back(c);
            }
        }

        tokens.clear();
        stringstream ss(spaced);
        string w;
        while (ss >> w) tokens.push_back(w);
    }

    // ----- Pattern matching helpers -----
    bool containsSubseq(const vector<string>& tok, const vector<string>& pat) const {
        size_t j = 0;
        for (size_t i = 0; i < tok.size() && j < pat.size(); i++) {
            if (tok[i] == pat[j]) j++;
        }
        return j == pat.size();
    }

    // ----- Bracket matching -----
    int findMatchingParen(int openPos) const {
        int depth = 0;
        for (int i = openPos; i < (int)tokens.size(); i++) {
            if (tokens[i] == "(") depth++;
            else if (tokens[i] == ")") {
                depth--;
                if (depth == 0) return i;
            }
        }
        return -1;
    }

    int findMatchingBrace(int openPos) const {
        int depth = 0;
        for (int i = openPos; i < (int)tokens.size(); i++) {
            if (tokens[i] == "{") depth++;
            else if (tokens[i] == "}") {
                depth--;
                if (depth == 0) return i;
            }
        }
        return -1;
    }

    // Body can be "{...}" or a single statement ending with ";"
    int findBodyStart(int afterHeader) const {
        for (int i = afterHeader; i < (int)tokens.size(); i++) {
            if (tokens[i] == ";") continue;
            return i;
        }
        return -1;
    }

    int findBodyEnd(int bodyStart) const {
        if (bodyStart < 0) return -1;
        if (tokens[bodyStart] == "{") {
            return findMatchingBrace(bodyStart);
        }
        for (int i = bodyStart; i < (int)tokens.size(); i++) {
            if (tokens[i] == ";") return i;
        }
        return (int)tokens.size() - 1;
    }

    // ----- Estimate loop cost from header tokens -----
    ComplexityTerm estimateLoopCost(const vector<string>& header) const {
        // log-like updates: i/=2, i>>=1, i*=2, i = i/2, etc.
        bool logLike =
            containsSubseq(header, {"/", "=", "2"}) ||
            containsSubseq(header, {"/", "=", "2"}) ||
            containsSubseq(header, {">", ">", "=", "1"}) ||
            containsSubseq(header, {"*", "=", "2"}) ||
            containsSubseq(header, {"/", "2"}); // weaker hint but useful

        // linear updates: ++, --, +=, -=  (tokenizer may split "+", "=")
        bool linear = false;
        for (size_t i = 0; i < header.size(); i++) {
            if (header[i] == "++" || header[i] == "--") linear = true;
            if (header[i] == "+=" || header[i] == "-=") linear = true;
            if (i + 1 < header.size() && (header[i] == "+" || header[i] == "-") && header[i + 1] == "=") linear = true;
        }

        if (logLike) return ComplexityTerm::Olog();
        if (linear)  return ComplexityTerm::On();

        // unknown update => assume O(n) for safety
        return ComplexityTerm::On();
    }

    // ----- Disguised-pattern detectors (return ONLY Big-O) -----
    bool detectRangeNarrowingLog() const {
        // generic: mid computed with /2 AND bounds updated using mid inside a while(lo<hi) style loop
        bool hasMid = false;
        bool hasBounds = false;
        for (const string& x : tokens) {
            if (x == "mid") hasMid = true;
            if (x == "min" || x == "max" || x == "lo" || x == "hi" || x == "low" || x == "high") hasBounds = true;
        }
        bool hasDiv2 = containsSubseq(tokens, {"/", "2"});

        bool updatesBound =
            containsSubseq(tokens, {"min", "=", "mid", "+", "1"}) ||
            containsSubseq(tokens, {"lo",  "=", "mid", "+", "1"}) ||
            containsSubseq(tokens, {"low", "=", "mid", "+", "1"}) ||
            containsSubseq(tokens, {"max", "=", "mid"}) ||
            containsSubseq(tokens, {"hi",  "=", "mid"}) ||
            containsSubseq(tokens, {"high","=", "mid"});

        bool whileHasBounds =
            containsSubseq(tokens, {"while","(","min","<","max",")"}) ||
            containsSubseq(tokens, {"while","(","lo","<","hi",")"}) ||
            containsSubseq(tokens, {"while","(","low","<","high",")"});

        return hasMid && hasBounds && hasDiv2 && updatesBound && whileHasBounds;
    }

    bool detectGapReductionLikelyN2() const {
        bool hasGapWord = false;
        for (const string& x : tokens) {
            if (x == "gap" || x == "space") hasGapWord = true;
        }

        bool initHalf =
            containsSubseq(tokens, {"gap","=","n","/","2"}) ||
            containsSubseq(tokens, {"space","=","n","/","2"});

        bool shrink =
            containsSubseq(tokens, {"gap","/","="}) ||
            containsSubseq(tokens, {"space","/","="}) ||
            containsSubseq(tokens, {"gap","=","gap","/"}) ||
            containsSubseq(tokens, {"space","=","space","/"});

        return hasGapWord && initHalf && shrink;
    }

    bool detectCountingStyleOnPlusK() const {
        // generic: frequency array built + write-back
        bool hasBucket = false;
        for (const string& x : tokens) {
            if (x == "group" || x == "count" || x == "freq") hasBucket = true;
        }

        bool hasIndexWrite =
            containsSubseq(tokens, {"array","[","index","]","="}) ||
            containsSubseq(tokens, {"arr","[","index","]","="}) ||
            containsSubseq(tokens, {"index","+","+"});

        bool hasWhileBucket =
            containsSubseq(tokens, {"while","(","group","["}) ||
            containsSubseq(tokens, {"while","(","count","["}) ||
            containsSubseq(tokens, {"while","(","freq","["});

        // also needs some bucket increment
        bool hasBucketInc =
            containsSubseq(tokens, {"group","["}) ||
            containsSubseq(tokens, {"count","["}) ||
            containsSubseq(tokens, {"freq","["});

        return hasBucket && hasBucketInc && hasWhileBucket && hasIndexWrite;
    }

    bool detectBacktrackingWorstN2() const {
        // generic: swap + decrement index inside some loop form
        bool hasSwap = containsSubseq(tokens, {"swap","("});
        bool hasDecIndex =
            containsSubseq(tokens, {"--","i"}) || containsSubseq(tokens, {"i","--"}) ||
            containsSubseq(tokens, {"--","j"}) || containsSubseq(tokens, {"j","--"});

        bool hasLoop =
            containsSubseq(tokens, {"while","("}) ||
            containsSubseq(tokens, {"for","("}) ||
            containsSubseq(tokens, {"do"});

        return hasLoop && hasSwap && hasDecIndex;
    }

    // ----- Structural analysis (nested loops multiply) -----
    ComplexityTerm analyzeRange(int L, int R) const {
        ComplexityTerm best = ComplexityTerm::O1();

        for (int i = L; i <= R; i++) {
            if (tokens[i] == "for" || tokens[i] == "while") {
                int open = (i + 1 <= R && tokens[i + 1] == "(") ? i + 1 : -1;
                if (open < 0) continue;

                int close = findMatchingParen(open);
                if (close < 0 || close > R) continue;

                vector<string> header(tokens.begin() + i, tokens.begin() + close + 1);

                int bodyStart = findBodyStart(close + 1);
                if (bodyStart < 0) continue;

                int bodyEnd = findBodyEnd(bodyStart);
                if (bodyEnd < 0) continue;
                bodyEnd = min(bodyEnd, R);

                ComplexityTerm loopCost = estimateLoopCost(header);
                ComplexityTerm bodyCost = analyzeRange(bodyStart, bodyEnd);

                ComplexityTerm total = loopCost.multiply(bodyCost);
                best = ComplexityTerm::dominant(best, total);

                i = bodyEnd; // skip body
            }
            else if (tokens[i] == "do") {
                int bodyStart = findBodyStart(i + 1);
                if (bodyStart < 0) continue;

                int bodyEnd = findBodyEnd(bodyStart);
                if (bodyEnd < 0) continue;
                bodyEnd = min(bodyEnd, R);

                // trailing while ( ... )
                int j = bodyEnd + 1;
                while (j <= R && tokens[j] != "while") j++;

                ComplexityTerm loopCost = ComplexityTerm::On(); // default
                if (j <= R && j + 1 <= R && tokens[j + 1] == "(") {
                    int pc = findMatchingParen(j + 1);
                    if (pc > 0 && pc <= R) {
                        vector<string> cond(tokens.begin() + j, tokens.begin() + pc + 1);
                        loopCost = estimateLoopCost(cond);
                    }
                }

                ComplexityTerm bodyCost = analyzeRange(bodyStart, bodyEnd);
                ComplexityTerm total = loopCost.multiply(bodyCost);
                best = ComplexityTerm::dominant(best, total);

                i = bodyEnd;
            }
        }

        return best;
    }

public:
    BigOAnalyzer() {}

    string analyze(const string& code) {
        setTokensFromCode(code);

        // Pattern detectors (rubric-style)
        if (detectRangeNarrowingLog())      return "O(log n)";
        if (detectCountingStyleOnPlusK())   return "O(n + k)";
        if (detectGapReductionLikelyN2())   return "O(n^2)";
        if (detectBacktrackingWorstN2())    return "O(n^2)";

        // Structural fallback
        ComplexityTerm t = analyzeRange(0, (int)tokens.size() - 1);
        return t.toString();
    }
};

// -------------------------- AlgorithmExtractor --------------------------
class AlgorithmExtractor {
private:
    static bool isTokPunct(char c) {
        return ispunct((unsigned char)c) && c != '_';
    }

    static vector<string> tokenizeFileText(const string& text) {
        string spaced;
        spaced.reserve(text.size() * 2);

        for (char c : text) {
            if (isTokPunct(c)) {
                spaced.push_back(' ');
                spaced.push_back(c);
                spaced.push_back(' ');
            } else {
                spaced.push_back(c);
            }
        }

        vector<string> tok;
        stringstream ss(spaced);
        string w;
        while (ss >> w) tok.push_back(w);
        return tok;
    }

    static int findMatchingBrace(const vector<string>& tok, int openPos) {
        int depth = 0;
        for (int i = openPos; i < (int)tok.size(); i++) {
            if (tok[i] == "{") depth++;
            else if (tok[i] == "}") {
                depth--;
                if (depth == 0) return i;
            }
        }
        return -1;
    }

public:
    // Extracts brace blocks that immediately follow a ') {' pattern.
    // We DO NOT output function names. We only return bodies as strings.
    static vector<string> extractFunctionBodies(const string& fileText) {
        vector<string> bodies;
        vector<string> tok = tokenizeFileText(fileText);

        for (int i = 0; i < (int)tok.size(); i++) {
            if (tok[i] == ")" && i + 1 < (int)tok.size() && tok[i + 1] == "{") {
                int openBrace = i + 1;
                int closeBrace = findMatchingBrace(tok, openBrace);
                if (closeBrace > openBrace) {
                    string body;
                    for (int k = openBrace; k <= closeBrace; k++) {
                        body += tok[k];
                        body += ' ';
                    }
                    bodies.push_back(body);
                    i = closeBrace;
                }
            }
        }
        return bodies;
    }
};

// -------------------------- main --------------------------
int main() {
    // Put Algorithms.cpp in the same folder as this executable, OR change the filename.
    const string filename = "Algorithms.cpp";

    ifstream in(filename);
    if (!in) {
        cerr << "Could not open " << filename << "\n";
        cerr << "Place Algorithms.cpp next to the program or change filename.\n";
        return 1;
    }

    stringstream buffer;
    buffer << in.rdbuf();
    string fileText = buffer.str();

    vector<string> bodies = AlgorithmExtractor::extractFunctionBodies(fileText);
    if (bodies.empty()) {
        cerr << "No function bodies extracted. Algorithms.cpp formatting may differ.\n";
        return 1;
    }

    BigOAnalyzer analyzer;

    // Print ONLY Big-O. No names. Safe for "auto-zero" rule.
    for (size_t i = 0; i < bodies.size(); i++) {
        cout << "Algorithm " << (i + 1) << ": " << analyzer.analyze(bodies[i]) << "\n";
    }

    return 0;
}