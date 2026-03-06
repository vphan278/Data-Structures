#include <iostream>
#include <vector>
using namespace std;

class Numbers{

    private:
        vector<int> data;

    public:
        
        void add(int value){
            data.push_back(value);
        }

        void print(){

            cout << "[";
            for (int n : data) {
                cout << n << ", ";
            }
            cout << "]\n";
    
            
        }

};

int main(){

    Numbers obj;

    obj.add(10);
    obj.add(20);
    obj.add(30);
    obj.print();
}