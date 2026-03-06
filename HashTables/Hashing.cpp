#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <iomanip>
#include <vector>
#include <random>
#include <cstdlib>  
#include <ctime>     
using namespace std;


class Customer {
    private:
        string lastName;
        string firstName;
        string id;

    public:
        Customer() : lastName(""), firstName(""), id("") {}
        Customer(const string& ln, const string& fn, const string& i) : lastName(ln), firstName(fn), id(i) {}

        //Getters
        const string& getLastName() const { 
            return lastName; 
        }
        const string& getFirstName() const { 
            return firstName; 
        }
        const string& getId() const { 
            return id; 
        }

        string toString() const {
            return lastName + "," + firstName + "," + id;
        }
};

class DoubleLinkedList{

    private: 
        class Node{
            private:
                Customer data;
                Node* prev;
                Node* next;
    

            public:
                Node(const Customer& c) : data(c), prev(nullptr), next(nullptr) {}

                //Getters
                const Customer& getData() const { 
                    return data; 
                }
                Node* getPrev() const {
                    return prev; 
                }
                Node* getNext() const { 
                    return next; 
                }

                void setPrev(Node* p) { 
                    prev = p; 
                }
                void setNext(Node* n) { 
                    next = n; 
                }
        };

        Node* head;
        Node* tail;    

    public:
     
        DoubleLinkedList() : head(nullptr), tail(nullptr) {}

        DoubleLinkedList(const DoubleLinkedList&) = delete;
        DoubleLinkedList& operator=(const DoubleLinkedList&) = delete;

       //Clear
        ~DoubleLinkedList() { 
            clear(); 
        }

        bool isEmpty() const { 
            return head == nullptr; 
        }
        
        //Insert customer at end of list
        void pushBack(const Customer& c) {
            Node* n = new Node(c);
            if (isEmpty()) {
                head = tail = n;
            } 
            else {
                n->setPrev(tail);
                tail->setNext(n);
                tail = n;
            }
        }
        
        //Count number of nodes in this bucket
        int size() const {
            int cnt = 0;
            Node* curr = head;
            while (curr != nullptr) {
                cnt++;
                curr = curr->getNext();
            }
            return cnt;
        }
    
        //Clear
        void clear() {
            Node* curr = head;
            while (curr != nullptr) {
                Node* nxt = curr->getNext();
                delete curr;
                curr = nxt;
            }
            head = tail = nullptr;
        }

        //Print
        void print() const {
            Node* cur = head;
            while (cur != nullptr) {
                cout << cur->getData().toString();
                cur = cur->getNext();
                if (cur) cout << "  <->  ";
            }
        }
};

//////////////////////////////////////////////////////////////////////////////////

class HashTable {
    public:
        static const int TABLE_SIZE = 400;

        //Hash Methods
        enum class HashMode { DIVISION, MULTIPLICATION, MID_SQUARE, FOLDING, UNIVERSAL };

        //Statistics for efficiency comparison
        class Stats {
            public:
                int filled = 0;
                int empty = 0;
                int collisions = 0;
                int maxChain = 0;
        };

    private:
        DoubleLinkedList buckets[TABLE_SIZE];
        HashMode mode;

        //Universal hashing parameters
        unsigned long long p;
        unsigned long long a;
        unsigned long long b;

       
        static string digitsOnly(const string& s) {
            string out;
            for (char ch : s) {
                if (isdigit((unsigned char)ch)) out.push_back(ch);
            }
            return out;
        }

        //Convert digit string to number
        static unsigned long long toULL(const string& digits) {
            unsigned long long key = 0;
            for (char d : digits) key = key * 10ULL + (unsigned long long)(d - '0');
            return key;
        }

       
        int hashDivision(unsigned long long key) const {
            return (int)(key % TABLE_SIZE);
        }

        int hashMultiplication(unsigned long long key) const {
           
            double A = (sqrt(5.0) - 1.0) / 2.0;
            double fractional = (key * A) - floor(key * A);
            return (int)floor(TABLE_SIZE * fractional);
        }

        static unsigned long long extractMiddleBits(unsigned long long squared) {
            return (squared >> 16) & 0xFFFFULL;
        }

        int hashMidSquare(unsigned long long key) const {
            unsigned long long squared = key * key;
            unsigned long long middle = extractMiddleBits(squared);
            return (int)(middle % TABLE_SIZE);
        }

        int hashFolding(const string& keyDigits) const {
           
            const int chunkSize = 4;
            unsigned long long total = 0;

            for (int i = 0; i < (int)keyDigits.size(); i += chunkSize) {
                string chunk = keyDigits.substr(i, chunkSize);
                total += stoull(chunk); 
            }
            return (int)(total % TABLE_SIZE);
        }

        void initUniversalParams() {
           
            p = 2147483647ULL; 
            a = 1ULL + (unsigned long long)(rand()) % (p - 1ULL);
            b = (unsigned long long)(rand()) % p;
        }

        int hashUniversal(unsigned long long key) const {
            
            return (int)(((a * key + b) % p) % TABLE_SIZE);
        }

        int indexFor(const Customer& c) const {
            string d = digitsOnly(c.getId()); 
            unsigned long long key = toULL(d);

            switch (mode) {
                case HashMode::DIVISION:       return hashDivision(key);
                case HashMode::MULTIPLICATION: return hashMultiplication(key);
                case HashMode::MID_SQUARE:     return hashMidSquare(key);
                case HashMode::FOLDING:        return hashFolding(d);
                case HashMode::UNIVERSAL:      return hashUniversal(key);
            }
            return hashDivision(key);
        }

    public:
        HashTable() : mode(HashMode::DIVISION), p(0), a(0), b(0) {
            srand((unsigned)time(nullptr)); 
            initUniversalParams();                    
        }

        void setMode(HashMode m) { mode = m; }

        void clear() {
            for (int i = 0; i < TABLE_SIZE; i++) buckets[i].clear();
        }

        void insert(const Customer& c) {
            int idx = indexFor(c);
            buckets[idx].pushBack(c);
        }

        void printAllBuckets() const {
            for (int i = 0; i < TABLE_SIZE; i++) {
                cout << "Index[" << setw(3) << i << "]: ";
                buckets[i].print();
                cout << endl;
            }
        }

        Stats computeStats() const {
            Stats s;
            for (int i = 0; i < TABLE_SIZE; i++) {
                int sz = buckets[i].size(); 
                if (sz == 0) s.empty++;
                else s.filled++;

                if (sz > 0) s.collisions += (sz - 1);
                if (sz > s.maxChain) s.maxChain = sz;
            }
            return s;
        }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
static vector<Customer> readCustomersCSV(const string& filename) {
    ifstream fin(filename);
    if (!fin) throw runtime_error("Could not open file: " + filename);

    vector<Customer> customers;
    string line;

    while (getline(fin, line)) {
        if (line.empty()) continue;

        string ln, fn, id;
        stringstream ss(line);

        if (!getline(ss, ln, ',')) continue;
        if (!getline(ss, fn, ',')) continue;
        if (!getline(ss, id)) continue;

        customers.emplace_back(ln, fn, id);
    }
    return customers;
}

static string modeName(HashTable::HashMode m) {
    switch (m) {
        case HashTable::HashMode::DIVISION:       return "Division";
        case HashTable::HashMode::MULTIPLICATION: return "Multiplication";
        case HashTable::HashMode::MID_SQUARE:     return "Mid-Square";
        case HashTable::HashMode::FOLDING:        return "Folding";
        case HashTable::HashMode::UNIVERSAL:      return "Universal";
    }
    return "Unknown";
}

static bool better(const HashTable::Stats& a, const HashTable::Stats& b) {
    if (a.filled != b.filled) return a.filled > b.filled;
    if (a.maxChain != b.maxChain) return a.maxChain < b.maxChain;
    return a.collisions < b.collisions;
}

/////////////////////////////////////////////////////////////////////////////
int main() {
    try {
        vector<Customer> customers = readCustomersCSV("Customer.csv");
        cout << "Loaded customers: " << customers.size() << endl;

        HashTable table;

        vector<HashTable::HashMode> modes = {
            HashTable::HashMode::DIVISION,
            HashTable::HashMode::MULTIPLICATION,
            HashTable::HashMode::MID_SQUARE,
            HashTable::HashMode::FOLDING,
            HashTable::HashMode::UNIVERSAL
        };

        bool bestInit = false;
        HashTable::HashMode bestMode = modes[0];
        HashTable::Stats bestStats;

        for (auto m : modes) {
            table.clear();
            table.setMode(m);

            for (const auto& c : customers) table.insert(c);

            cout << "\n------------" << modeName(m) << "-------------\n";
            table.printAllBuckets();

            auto st = table.computeStats();
            cout << "  Filled: " << st.filled
                 << "  Empty: " << st.empty
                 << "  Collisions: " << st.collisions
                 << "  MaxChain: " << st.maxChain
                 << "  LoadFactor: " << (double)customers.size() / HashTable::TABLE_SIZE
                 << "\n";

            if (!bestInit || better(st, bestStats)) {
                bestInit = true;
                bestMode = m;
                bestStats = st;
            }
        }

        cout << "\nMOST EFFICIENT: " << modeName(bestMode) << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}