#include <iostream>
#include <string>
using namespace std;

// Hash Table with LINEAR PROBING + DELETE (TOMBSTONES)
class HashTable {
private:
    static const int SIZE = 10;

    int keys[SIZE];
    string values[SIZE];

    // 3 states are required for correct deletion in linear probing
    enum State { EMPTY, OCCUPIED, DELETED };
    State state[SIZE];

    int hashFunction(int key) const{
        return key % SIZE;
    }

public:
    HashTable() {
        for (int i = 0; i < SIZE; i++) {
            state[i] = EMPTY;
        }
    }

    // Insert using linear probing (reuses DELETED slots)
    void insert(int key, const string& value) {
        int index = hashFunction(key);
        int start = index;
        int firstDeleted = -1;

        while (state[index] != EMPTY) {
            // If key exists, update
            if (state[index] == OCCUPIED && keys[index] == key) {
                values[index] = value;
                return;
            }

            // Remember first tombstone so we can reuse it
            if (state[index] == DELETED && firstDeleted == -1) {
                firstDeleted = index;
            }

            index = (index + 1) % SIZE;

            // Prevent infinite loop if table is full
            if (index == start) break;
        }

        // If we found a tombstone earlier, insert there
        if (firstDeleted != -1) index = firstDeleted;

        // If table is truly full (no EMPTY and no DELETED), we can't insert
        if (state[index] == OCCUPIED) {
            cout << "Insert failed: table is full.\n";
            return;
        }

        keys[index] = key;
        values[index] = value;
        state[index] = OCCUPIED;
    }

    // Search (continues past DELETED; stops only at EMPTY)
    string search(int key) const {
        int index = hashFunction(key);
        int start = index;

        while (state[index] != EMPTY) {
            if (state[index] == OCCUPIED && keys[index] == key) {
                return values[index];
            }

            index = (index + 1) % SIZE;
            if (index == start) break; // full loop
        }

        return "Not found";
    }

    // Remove (marks slot as DELETED, not EMPTY)
    bool remove(int key) {
        int index = hashFunction(key);
        int start = index;

        while (state[index] != EMPTY) {
            if (state[index] == OCCUPIED && keys[index] == key) {
                state[index] = DELETED;   // tombstone
                values[index] = "";       // optional: clear for readability
                return true;
            }

            index = (index + 1) % SIZE;
            if (index == start) break; // full loop
        }

        return false;
    }

    // Print table (shows OCCUPIED, DELETED, EMPTY)
    void print() const {
        for (int i = 0; i < SIZE; i++) {
            cout << i << ": ";

            if (state[i] == OCCUPIED) {
                cout << "(" << keys[i] << ", " << values[i] << ")";
            } else if (state[i] == DELETED) {
                cout << "<deleted>";
            } // else EMPTY prints nothing

            cout << endl;
        }
    }
};

int main() {
    HashTable ht;

    ht.insert(12, "Apple");    // 12 % 10 = 2
    ht.insert(22, "Banana");   // 22 % 10 = 2 -> probes to 3
    ht.insert(32, "Cherry");   // 32 % 10 = 2 -> probes to 4

    cout << "Initial table:\n";
    ht.print();

    cout << "\nSearch 22: " << ht.search(22) << endl;
    cout << "Search 12: " << ht.search(12) << endl;

    cout << "\nRemoving 22...\n";
    cout << (ht.remove(22) ? "Removed.\n" : "Key not found.\n");

    cout << "\nAfter deletion:\n";
    ht.print();

    // This is the key test: Cherry must still be found even after deleting Banana
    cout << "\nSearch 32: " << ht.search(32) << endl;

   
}