#include <iostream>
#include <string>
using namespace std;

class HashTable {
private:
    static const int SIZE = 11;

    int keys[SIZE];
    string values[SIZE];

    enum State { EMPTY, OCCUPIED, DELETED };
    State state[SIZE];

    int h1(int key) const {
        return key % SIZE;
    }

    int h2(int key) const {
        return 7 - (key % 7); // must be non-zero
    }

public:
    HashTable() {
        for (int i = 0; i < SIZE; i++)
            state[i] = EMPTY;
    }

    void insert(int key, const string& value) {
        int i = 0;
        int index;
        int firstDeleted = -1;

        while (i < SIZE) {
            index = (h1(key) + i * h2(key)) % SIZE;

            if (state[index] == EMPTY)
                break;

            if (state[index] == OCCUPIED && keys[index] == key) {
                values[index] = value;
                return;
            }

            if (state[index] == DELETED && firstDeleted == -1)
                firstDeleted = index;

            i++;
        }

        if (firstDeleted != -1)
            index = firstDeleted;

        keys[index] = key;
        values[index] = value;
        state[index] = OCCUPIED;
    }

    string search(int key) const {
        for (int i = 0; i < SIZE; i++) {
            int index = (h1(key) + i * h2(key)) % SIZE;

            if (state[index] == EMPTY)
                return "Not found";

            if (state[index] == OCCUPIED && keys[index] == key)
                return values[index];
        }
        return "Not found";
    }

    bool remove(int key) {
        for (int i = 0; i < SIZE; i++) {
            int index = (h1(key) + i * h2(key)) % SIZE;

            if (state[index] == EMPTY)
                return false;

            if (state[index] == OCCUPIED && keys[index] == key) {
                state[index] = DELETED;
                return true;
            }
        }
        return false;
    }

    void print() const {
        for (int i = 0; i < SIZE; i++) {
            cout << i << ": ";
            if (state[i] == OCCUPIED)
                cout << "(" << keys[i] << ", " << values[i] << ")";
            else if (state[i] == DELETED)
                cout << "<deleted>";
            cout << endl;
        }
    }
};

int main() {
    HashTable ht;

    ht.insert(12, "Apple");
    ht.insert(23, "Banana");
    ht.insert(34, "Cherry");

    ht.print();

    cout << "\nSearch 23: " << ht.search(23) << endl;

    ht.remove(23);

    cout << "\nAfter delete:\n";
    ht.print();
}