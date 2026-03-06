\#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

//Vinh Phan
//CIS-22C: C++ Data Structures and Algorithms
//CRN 33571
       

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

class SkipList{

    private:

        class Level{

            public:
                GridNode *head;
                Level *down;

                Level(GridNode *h = nullptr, Level *d = nullptr): head(h), down(d){}

        };
        Level *topLevel; //For now this will be level 0 only
        double probability; //Used Later for random height
        int maxHeight;  // used later for tower limit
        GridNode* lastInsertedBottom = nullptr;


    
    public:
        SkipList(double p = 0.5, int maxH = 16) : topLevel(new Level()), probability(p), maxHeight(maxH) {}

        void insert(int key, int value){

            int h = randomHeight();  //Step 1
            ensureHeight(h);         //Step 2

            // For NOW, we still only insert into Level 0 (bottom)
            // We'll add tower insertion next step.
            Level* bottom = topLevel;
            while (bottom->down) bottom = bottom->down;

            bool insertedNew = false;

            //Milestone 1: insert base node into Level 0
            GridNode* baseInserted = insertIntoLevel(bottom, key, value, insertedNew);
            lastInsertedBottom = baseInserted;
            if (!insertedNew) return;


            //Milestone 2: build vertical tower
            GridNode* lower = baseInserted;

            for (int i = 1; i < h; i++) {
                GridNode* upper = new GridNode(key, value);

                upper->down = lower;
                lower->up = upper;

                lower = upper;
            }
    
            //Milestone 3: insert each tower node 
            GridNode* lowerInserted = baseInserted;

            for (int level = 1; level < h; level++) {
                GridNode* upperNode = lowerInserted->up;
                Level* targetLevel = getLevelFromBottom(level);

                GridNode* pred = nullptr;
                if (targetLevel->head != nullptr) {
                    pred = findPredAbove(lowerInserted, key);
                }

                upperNode->left = nullptr;
                upperNode->right = nullptr;

                insertAfter(targetLevel->head, pred, upperNode);

                lowerInserted = upperNode;
            }

           
            
        }

        GridNode *search(int targetKey) const{
            Level *L = topLevel;

            //Find the first non-empty Level hed from the top
            while(L && L->head == nullptr){
                L = L->down;
            }
            if(!L) return nullptr;

            GridNode *curr = L->head;

            while(L && curr){

                //If we are sitting on the key, done
                if(curr->key == targetKey) return curr;

                //Move right while we can and it doesn't pass targetKey
                while(curr->right && curr->right->key <= targetKey){
                    curr = curr->right;
                    if(curr->key == targetKey) return curr;
                }

                //If at Level 0, and next is > targetKey(or null), it's not present
                if(L->down == nullptr){
                    return nullptr;
                }

                //Move down one level(one using "down")
                L = L->down;

                //If node has a down pointer, follow it; otherwise drop to the level head
                if(curr->down){
                    curr = curr->down;
                }
                else{
                    curr = L->head;
                    //In case this level is empty, keep dropping
                    while(L && curr == nullptr){
                        L = L->down;
                        if(!L) return nullptr;
                        curr = L->head;
                    }
                }
            }
            return nullptr;
        }

        void printLevels() const {
            int level = levelCount() - 1;   // highest level number

            for (Level* L = topLevel; L != nullptr; L = L->down) {
                cout << "Level " << level-- << ": ";
                printOneLevel(L);
            }
        }
        
        void printLastTower() const {
            if (lastInsertedBottom) printTower(lastInsertedBottom);
        }
    
        



    private:
        
        GridNode *insertIntoLevel(Level *level,int key, int value, bool& insertedNew){
            GridNode*& head = level->head; // Head of this level

            //Case 1: empty level
            if(!head){
                head = new GridNode(key, value);
                insertedNew = true;
                return head;
            }

            //Case 2: insert before head
            if(key < head->key){
                GridNode *n = new GridNode(key, value);
                n->right = head;
                head->left = n;
                head = n;
                insertedNew = true;
                return n;
            }

            //Walk until we find position:curr.key <= key, and curr.right.key > key
            GridNode *curr = head;
            while (curr->right && curr->right->key < key) curr = curr->right;

            // NEW: check current node
            if (curr->key == key) {
                curr->value = value;
                insertedNew = false;
                return curr;
            }

            // If curr itself matches
            if(curr->right && curr->right->key == key){
                curr->right->value = value;
                insertedNew = false;
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

            insertedNew = true;
            return n;

        } 
   


        //////////////////////////////////////////////////////////////////
        int randomHeight() const {
            int height = 1;
            while ((rand() / double(RAND_MAX)) < probability && height < maxHeight)
                height++;
            return height;
        }

        /////////////////////////////////////////////////////////
        int levelCount() const {
            int count = 0;
            for (Level* L = topLevel; L != nullptr; L = L->down) {
                count++;
            }
            return count;
        }

        ////////////////////////////////////////////////////////
        Level* ensureHeight(int height) {
            // how many levels do we have now?
            int currH = levelCount();

            // if we already have enough levels, nothing to do
            while (currH < height) {
                // create a new empty level ABOVE the current top
                Level* newTop = new Level(nullptr, topLevel);
                topLevel = newTop;
                currH++;
            }
            return topLevel;
        }

        
        /////Insert each tower node horizontally//////////////////////////
        GridNode* findPredAbove(GridNode* lowerInserted, int key) {
            // Start from the LEFT of the inserted node (important fix)
            GridNode* p = lowerInserted->left;

            // Move left until we find a node that has an UP pointer
            while (p && !p->up) p = p->left;

            // If none found, insertion point is before head
            if (!p) return nullptr;

            // Go up one level
            p = p->up;

            // Move right to find correct predecessor for this key
            while (p->right && p->right->key < key) {
                p = p->right;
            }

            return p;  // predecessor on the upper level
        }
        
        ///////Inser each tower node horizontally////////////////////////////
        GridNode* insertAfter(GridNode*& head, GridNode* pred, GridNode* node) {
            // If pred is null, insert at head
            GridNode* next = (pred ? pred->right : head);

            node->left = pred;
            node->right = next;

            if (pred) pred->right = node;
            else head = node;              // <-- critical

            if (next) next->left = node;

            return node;
        }
        //////////////////////////////////////////////////////////////////
        Level *getLevelFromBottom(int offsetFromBottom){
            //offsetFromBottom = 0 => bottom Level
            //offsetFromBottom = 1 => one above bottom, etc

            int total = levelCount();
            int targetIndexFromTop = (total - 1) - offsetFromBottom;

            Level *L = topLevel;
            for(int i = 0; i < targetIndexFromTop; i++){
                L = L->down;
            }
            return L;


        }



        void printOneLevel(Level *level) const{

            GridNode *c = level->head;
            while(c){
                cout << c->key << ":" << c->value;
                if(c->right) cout << " <-> ";
                c = c->right;
            }
            cout << "\n";
        }
        //////////////////////////////////////////////////////////
        void printTower(GridNode* bottom) const {
            cout << "Tower (bottom -> top): ";

            int count = 0;
            for (GridNode* curr = bottom; curr; curr = curr->up) {
                cout << curr->key;
                count++;
                if (curr->up) cout << " ^ ";
            }

            cout << "   (nodes=" << count << ")\n";
        }
        
};

int main(){

    //////Random Height////////////////////////////////////////////////////////////////////////////////////
    srand(time(nullptr));


    /////Level 0///////////////////////////////////////////////////////////////////////////////////////////

    SkipList skip(0.95, 6);

    skip.insert(10, 100);
    skip.insert(5, 50);
    skip.insert(30, 300);
    skip.insert(20, 200);
    skip.insert(1, 10);
    skip.insert(1, 20);
    skip.insert(40, 400);
    skip.insert(3, 30);

    skip.printLevels(); // Level 0: 1:20 <-> 2:20 <-> 3:30 <-> 5:50 <-> 10:100 <-> 20:200 <-> 30:300 <-> 40:400
    cout << endl;
    /////////////////////////////////////////////////////////////////////////////////////////
    skip.printLastTower();
    
    //////Milestone 4////////////////////////////////////////////////////////////////////////////
    cout << "\nSearch Test:\n";  //Search Test

    GridNode *hit = skip.search(20);
    cout << (hit ? "Found 20\n": "Not found 20\n");  // Found 20

    hit = skip.search(30);
    cout << (hit ? "Found 30\n" : "Not found 30\n"); // Found 30

    hit = skip.search(80);
    cout << (hit ? "Found 80\n": "Not found 80\n");  // Not found 80

    return 0;


}



// Your skiplist implementation is one of the more complete and structurally thoughtful submissions. 
// You correctly implemented the GridNode and Level classes, and your insertion logic at the base 
// level maintains sorted order with proper left/right rewiring. You also built vertical towers cleanly, 
// linking nodes with up/down pointers and ensuring that duplicate keys update the entire tower rather 
// than creating new nodes. Your search method follows the intended right‑and‑down traversal pattern, 
// and your printLevels output is clear and easy to interpret. Overall, the code demonstrates a solid 
// understanding of the basic mechanics of skiplist behavior and is organized in a way that makes 
// the structure straightforward to follow. Where your submission stands out is that you did attempt 
// the required pointer‑guided insertion strategy for upper levels. Your  method correctly walks left 
// from the lower node until it finds a node with an up pointer, then uses that position to guide 
// insertion on the upper level. This is the core milestone of the midterm, and you implemented it 
// successfully. The only limitation is that your bottom‑level insertion still relies on a fresh horizontal 
// scan rather than using the tower structure to guide the process, but this is a minor issue compared to 
// the overall correctness of your pointer‑rewiring logic. The structure is robust, the tower insertion is 
// well‑reasoned, and the code reflects a strong grasp of the assignment’s goals.

