#include <iostream>
#include <vector>
#include <list>
using namespace std;

///CHAINING

class HashTable{

    private:
        static const int SIZE = 10;

        //Each bucket is a list of(key, value)
        vector<list<pair<int, string>>> table;

        int hashFunction(int key){
            return key % SIZE;
        }

    public:
        HashTable(){
            table.resize(SIZE);
        }

        //insert key-value
        void insert(int key, string value){
            int index = hashFunction(key);

            //Check if key already exists -> update
            for(auto &p : table[index]){
                if(p.first == key){
                    p.second = value;
                    return;
                }
            }

            //Otherwise add new pair
            table[index].push_back({key, value});

        }

        //Remove a key
        bool remove(int key){
            int index = hashFunction(key);

            for(auto it = table[index].begin(); it !=table[index].end(); it++){
                if(it->first == key){
                    table[index].erase(it); //delete from list
                    return true;
                }
            }
            return false; // key not found


        }

        //Search
        string search(int key){
            int index = hashFunction(key);

            for(auto &p : table[index]){
                if(p.first == key)
                    return p.second;
            }
            return "Not found";
        }

        //Display table
        void print(){
            for(int i = 0; i < SIZE; i++){
                cout << i << ": ";

                for(auto &p : table[i])
                cout << "(" << p.first << "," << p.second << ") ";

                cout <<endl;
            }
        }

};

int main(){

    HashTable ht;

    ht.insert(12, "Apple"); // key % 10, remainder 2, therefor index 2
    ht.insert(22, "Banana");// key % 10. remainder 2, also index 2 
    ht.insert(32, "Cherry");// key % 10, remainder 2, is also index 2

    ht.print(); 

// 0: 
// 1:
// 2: (12,Apple) (22,Banana) (32,Cherry)
// 3:
// 4:
// 5:
// 6:
// 7:
// 8:
// 9:

    ht.remove(22);
    ht.print();

// Output
// 1:
// 2: (12,Apple) (32,Cherry)
// 3:
// 4:
// 5:
// 6:
// 7:
// 8:
// 9:

    cout <<"\nSearch 22: " << ht.search(22) << endl; // Search 22: Not found

}