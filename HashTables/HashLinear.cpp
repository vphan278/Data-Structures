#include <iostream>
#include <string>
using namespace std;

//Hash Linear with no delete

class HashTable{

    private:
        static const int SIZE = 10;

        int keys[SIZE];
        string values[SIZE];
        bool occupied[SIZE];

        //Hash function
        int hashFunction(int key){
            return key % SIZE;
        }

    public:
        //Constructor
        HashTable(){
            for(int i = 0; i < SIZE; i++){
                occupied[i] = false;
            }
        }

        //Insert using LINEAR PROBING
        void insert(int key, string value){
            int index = hashFunction(key);

            //Move forward until an empty slot is found
            while(occupied[index]){
                //If same key, update value
                if(keys[index] == key){
                    values[index] = value;
                    return;
                }
                index = (index + 1) % SIZE; // Wrap arounf
            }

            //Store New Entry
            keys[index] = key;
            values[index] = value;
            occupied[index] = true;

        }

        //Search
        string search(int key){
            int index = hashFunction(key);
            int start = index;

            while(occupied[index]){
                if(keys[index] == key)
                    return values[index];

                index = (index + 1) % SIZE;

                if(index == start) break; //full loop
            }
            return "Not found";

        }

        //Print table
        void print(){
            for(int i = 0; i < SIZE; i++){
                cout << i << ": ";
                
                if(occupied[i])
                    cout << "(" << keys[i] << values[i] << ")";
                    
                cout <<endl;
            }
        }

};

int main(){

    HashTable ht;

    ht.insert(12, "Apple");  // goes to index 2
    ht.insert(22, "Banana"); //collision -> index 3
    ht.insert(32, "Cherry"); //collision -> index 4

    ht.print();
//Output
// 0: 
// 1: 
// 2: (12Apple)
// 3: (22Banana)
// 4: (32Cherry)
// 5:
// 6:
// 7:
// 8:
// 9:

    cout << "\nSearch 22: " << ht.search(22) << endl; // Search 22: Banana
    cout << "\nSearch 12: " << ht.search(12) << endl; // Search 12: Apple


}
