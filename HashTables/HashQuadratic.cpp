#include <iostream>
#include <string>
using namespace std;

class HashTable {
    private:
        static const int SIZE = 11; // prime size helps probing

        int keys[SIZE];
        string values[SIZE];

        enum State { EMPTY, OCCUPIED, DELETED };
        State state[SIZE];

        int hashFunction(int key) const {
            return key % SIZE;
        }

    public:
        HashTable() {
            for (int i = 0; i < SIZE; i++)
                state[i] = EMPTY;
        }

        void insert(int key, const string& value) {
            int index = hashFunction(key);
            int i = 1;
            int firstDeleted = -1;

            while (state[index] != EMPTY) {
                if (state[index] == OCCUPIED && keys[index] == key) {
                    values[index] = value;
                    return;
                }

                if (state[index] == DELETED && firstDeleted == -1)
                    firstDeleted = index;

                index = (hashFunction(key) + i * i) % SIZE;
                i++;

                if (i == SIZE) break;
            }

            if (firstDeleted != -1)
                index = firstDeleted;

            keys[index] = key;
            values[index] = value;
            state[index] = OCCUPIED;
        }

        string search(int key) const {
            int i = 0;
            int index;

            while (i < SIZE) {
                index = (hashFunction(key) + i * i) % SIZE;

                if (state[index] == EMPTY)
                    return "Not found";

                if (state[index] == OCCUPIED && keys[index] == key)
                    return values[index];

                i++;
            }

            return "Not found";
        }

        bool remove(int key) {
            int i = 0;
            int index;

            while (i < SIZE) {
                index = (hashFunction(key) + i * i) % SIZE;

                if (state[index] == EMPTY)
                    return false;

                if (state[index] == OCCUPIED && keys[index] == key) {
                    state[index] = DELETED;
                    return true;
                }

                i++;
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