#include <iostream>
using namespace std;


class HeapNode{

    private:
        int data;
        HeapNode *left;
        HeapNode *right;
        HeapNode *parent;


    public:
        HeapNode(int value): data(value), left(nullptr), right(nullptr), parent(nullptr){}

        int getData(){
            return data;
        }

        //Getters
        HeapNode *getLeft() const{
            return left;
        }
        HeapNode *getRight() const{
            return right;
        }
        HeapNode *getParent() const{
            return parent;
        }

        //Setters
        void setLeft(HeapNode *node){
            left = node;
        }
        void setRight(HeapNode *node){
            right = node;
        }
        void setParent(HeapNode *node){
            parent = node;
        }

};

//    Manually buuild thiss
//        50
//      /   \  
//    30      40
//   /  \   
// 10   20

int main(){

    //Create nodes
    HeapNode *n50 = new HeapNode(50);
    HeapNode *n30 = new HeapNode(30);
    HeapNode *n40 = new HeapNode(40);
    HeapNode *n10 = new HeapNode(10);
    HeapNode *n20 = new HeapNode(20);

    //Wire pointers

    //50's children
    n50->setLeft(n30);
    n50->setRight(n40);
    n30->setParent(n50);
    n40->setParent(n50);

    //30's children
    n30->setLeft(n10);
    n30->setRight(n20);
    n10->setParent(n30);
    n20->setParent(n30);

    //root pointer
    HeapNode *root = n50;

    //checks
    cout << "root: " << root->getData() << "\n";       //root: 50
    cout << "root left: " << root->getLeft()->getData() << "\n";  //root left: 30
    cout << "30 parent: " << n30->getParent()->getData() << "\n"; //30 Parent: 50
    cout << "30 left: " << n30->getLeft()->getData()<< "\n";  //30 left: 10
    cout << "30 right: " << n30->getRight()->getData()<< "\n";  //30 right: 20

    //cleanup
    delete n10;
    delete n20;
    delete n30;
    delete n40;
    delete n50;

    return 0;
 
}
