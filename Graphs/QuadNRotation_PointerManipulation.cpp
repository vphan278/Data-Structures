#include <iostream>
using namespace std;

//Vinh Phan
//CIS-22C: C++ Data Structures and Algorithms
//CRN 33571


// Starting layout
//       [A]
//        ↑
//  [D] ← X → [B]
//        ↓
//       [C]

//Pointer Manipulation
// temp  = north
// north = west
// west  = south
// south = east
// east  = temp

class Rotor {

    private: 
        char north;
        char east;
        char south;
        char west;
        int degrees;

   
    public:
        //Constructor- initial layout
        Rotor() : north('A'), east('B'), south('C'), west('D'), degrees(0) {}

    
    char rotateClockwise() {
        //initial layout
        char temp = north;
        north = west;
        west  = south;
        south = east;
        east  = temp;

        degrees = (degrees + 90) % 360;
        return north;
    }
    
    void printRotor() {
        for (int i = 0; i < 4; i++) {
            cout << "After 90 degree rotation, north =  " << rotateClockwise() << " :: "; //After rotation each 90 degree rotation, north: D
        }
        cout << endl;
    }

};


 //First clockwise rotation

    //       [D]
    //        ↑
    //  [C] ← X → [A]
    //        ↓
    //       [B]

int main(){

    //initial layout
    Rotor r;
    r.printRotor();

    return 0;

    
}

    


