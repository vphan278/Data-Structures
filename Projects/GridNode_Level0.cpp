#include <iostream>
using namespace std;

class GridNode{

    public:
        int key;
        int value;

        GridNode *left;
        GridNode *right;
        GridNode *up;
        GridNode *down;

        GridNode(int k, int v) : key(k), value(v), left(nullptr), right(nullptr), up(nullptr), down(nullptr){}

};

class Level{

    private:
        GridNode *head; //first node in this level

    public:
        Level(GridNode *h = nullptr) : head(h){}

        GridNode *getHead() const {
            return head;
        }

        // Insert into this One level horizontally in sorted order(by key).
        // - If key exists: update value, return existing node.
        // - Else: insert new node, return new node
        GridNode *insertSorted(int key, int value){
            //Case 1: empty level
            if(!head){
                head = new GridNode(key, value);
                return head;
            }

            //Case 2: insert before head
            if(key < head->key){
                GridNode *n = new GridNode(key, value);
                n->right = head;
                head->left = n;
                head = n;
                return n;
            }

            //Walk until we find position:curr.key <= key, and curr.right.key > key
            GridNode *curr = head;
            while(curr->right && curr->right->key < key){
                curr = curr->right;
            }

            // Now, either
            // curr->key == key (update)
            // curr->right->key == key (update
            // or insert between curr and curr->right

            // If curr itself matches
            if(curr->right && curr->right->key == key){
                curr->right->value = value;
                return curr->right;
            }

            //Insert AFTER curr
            GridNode *n = new GridNode(key, value);

            GridNode *next = curr->right; // could be a nullptr(insert at end
            
            //Wire n between curr and next
            n->left = curr;
            n->right = next;

            curr->right = n;
            if(next) next->left = n;

            return n;

        }

        void print() const{

            cout << "Level: ";
            GridNode *c = head;

            while(c){
                cout << c->key << ":" << c->value;
                if(c->right) cout << " <-> ";
                c = c->right;
            }
            cout << "\n";

        }

        GridNode* search(int key) const {
            GridNode* curr = head;
            while (curr && curr->key < key) {
                curr = curr->right;
            }
            if (curr && curr->key == key) return curr;
            return nullptr;
        }

        bool deleteKey(int key) {
            GridNode* node = search(key);
            if (!node) return false;

            GridNode* L = node->left;
            GridNode* R = node->right;

            if (L) L->right = R;
            else  head = R;          // deleted head

            if (R) R->left = L;

            delete node;
            return true;
        }

};

int main(){

    Level level;

    //Insert keys out of order'
    level.insertSorted(10,100);
    level.insertSorted(5,50);
    level.insertSorted(30,300);
    level.insertSorted(20,200);
    level.insertSorted(1,10);

    level.print(); // Level: 1:10 <-> 5:50 <-> 10:100 <-> 20:200 <-> 30:300

    level.deleteKey(20);
    level.print(); // Level: 1:10 <-> 5:50 <-> 10:100 <-> 30:300

}