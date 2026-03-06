#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>

using namespace std;

// ===== ANSI COLOR CODES =====
const string reset = "\033[0m";
const string red = "\033[91m";
const string green = "\033[92m";
const string yellow = "\033[93m";
const string blue = "\033[94m";
const string purple = "\033[38;5;141m";
const string cyan = "\033[96m";
const string white = "\033[97m";
const string darkgray = "\033[38;5;250m";
const string orange = "\033[38;5;208m";

// ===== Node Definition =====
class Node {
public:
    int key;
    vector<Node*> right;   // horizontal links
    vector<Node*> down;    // vertical links 

    Node(int key, int levels)
        : key(key),
        right(levels + 1, nullptr),
        down(levels + 1, nullptr) {
    }
};

// ===== Skip List =====
class SkipList {
public:
    int maxLevel;
    float pointerLevels;
    int level;
    Node* header;

    SkipList(int M, float P)
        : maxLevel(M), pointerLevels(P), level(0)
    {
        header = new Node(-1, maxLevel);
    }

    int generateLevel() {
        int lvl = 0;
        while (((float)rand() / RAND_MAX) < pointerLevels && lvl < maxLevel)
            lvl++;
        return lvl;
    }

    void insert(int key) {
        cout << purple << "\n<<< Inserting key >>> "
            << yellow << key << reset << "\n";

        vector<Node*> update(maxLevel + 1, nullptr);
        Node* node = header;

        for (int i = level; i >= 0; i--) {
            while (node->right[i] && node->right[i]->key < key) {
                cout << cyan << "Level " << i
                    << ": visiting " << yellow << node->right[i]->key << reset << "\n";
                node = node->right[i];
            }
            update[i] = node;
        }

        node = node->right[0];

        if (!node || node->key != key) {
            int rlevel = generateLevel();

            if (rlevel > level) {
                for (int i = level + 1; i <= rlevel; i++)
                    update[i] = header;
                level = rlevel;
            }

            Node* newNode = new Node(key, rlevel);

            for (int i = 0; i <= rlevel; i++) {
                newNode->right[i] = update[i]->right[i];
                update[i]->right[i] = newNode;

                if (i > 0)
                    newNode->down[i] = newNode->down[i - 1];
            }

            cout << green << "Inserted key " << yellow << key
                << green << " at level " << yellow << rlevel << reset << "\n";
        }
    }

    void search(int key) {
        cout << purple << "\n>>> Search <<< "
            << yellow << key << reset << "\n";

        Node* node = header;

        for (int i = level; i >= 0; i--) {
            while (node->right[i] && node->right[i]->key < key) {
                cout << cyan << "Searching level " << i
                    << ": " << yellow << node->right[i]->key << reset << "\n";
                node = node->right[i];
            }
        }

        node = node->right[0];
        if (node && node->key == key)
            cout << red << "Found key: " << yellow << key << reset << "\n";
        else
            cout << red << "Key not found" << reset << "\n";
    }

    void output() {
        cout << orange << ">>> Skip List <<<" << reset << "\n";

        vector<int> columns;
        Node* node = header->right[0];
        while (node) {
            columns.push_back(node->key);
            node = node->right[0];
        }

        for (int i = level; i >= 0; i--) {
            cout << green << "Level " << i << ": " << reset;

            Node* cursor = header->right[i];
            size_t colIndex = 0;

            for (int key : columns) {
                if (cursor && cursor->key == key) {
                    cout << yellow << setw(4) << key << reset;
                    cursor = cursor->right[i];
                }
                else {
                    cout << darkgray << setw(4) << "-" << reset;
                }
            }
            cout << "\n";
        }
    }
};

int main() {
    srand((unsigned)time(nullptr));

    array<int, 10> ilist;
    for (int i = 0; i < 10; i++)
        ilist[i] = rand() % 100 + 1;

    int nlevels = log(ilist.size()) / log(2);
    SkipList skiplist(nlevels, 0.5);

    for (int i = 0; i < 10; i++) {
        skiplist.insert(ilist[i]);
        skiplist.output();
    }

    cout << "\n";
    skiplist.output();
    skiplist.search(59);
}