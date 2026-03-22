#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
using namespace std;

const int SIZE = 10;

//GridNode class
class GridNode{
    
    private:
        int row, col;
        bool upWall, downWall, leftWall, rightWall;
        bool visited;
        char mark;


    public:
        //Constructor
        GridNode(int r = 0, int c = 0){
            row = r;
            col = c;

            upWall = downWall = leftWall = rightWall = false;
            visited = false;
            mark = ' ';
        }

        //Getters
        int getRow() const{ 
            return row;
        }
        int getCol() const{
             return col;
        }
        bool hasUpWall()const{
            return upWall;
        }
        bool hasDownWall() const{ 
            return downWall;
        }
        bool hasLeftWall() const{ 
            return leftWall; 
        }
        bool hasRightWall() const{
            return rightWall; 
        }
        bool isVisited() const{ 
            return visited; 
        }
        char getMark() const{ 
            return mark; 
        }

        //Setters
        void setUpWall(bool val){
            upWall = val;
        }
        void setDownWall(bool val){ 
            downWall = val; 
        }
        void setLeftWall(bool val){ 
            leftWall = val; 
        }
        void setRightWall(bool val){ 
            rightWall = val; 
        }

        void setVisited(bool val){ 
            visited = val; 
        }
        void setMark(char m){ 
            mark = m; 
        }


};


//Building Neighbors
vector<GridNode*> getNeighbors(GridNode grid[SIZE][SIZE], GridNode& node){
    
    vector<GridNode*> neighbors;

    int r = node.getRow();
    int c = node.getCol();

    //Go Up
    if(!node.hasUpWall() && r > 0){
        neighbors.push_back(&grid[r -1][c]);
    }

    //Go Down
    if(!node.hasDownWall() && r < SIZE -1){
        neighbors.push_back(&grid[r + 1][c]);
    }

    //Go Left
    if(!node.hasLeftWall() && c > 0){
        neighbors.push_back(&grid[r][c - 1]);
    }

    //Go Right
    if(!node.hasRightWall() && c < SIZE -1){
        neighbors.push_back(&grid[r][c + 1]);
    }

    return neighbors;


};

void buildGridFromMaze(int maze[SIZE][SIZE], GridNode grid[SIZE][SIZE]) {
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            int value = maze[r][c];

            grid[r][c] = GridNode(r, c);

            grid[r][c].setUpWall((value & 1) != 0);
            grid[r][c].setDownWall((value & 2) != 0);
            grid[r][c].setLeftWall((value & 4) != 0);
            grid[r][c].setRightWall((value & 8) != 0);

            grid[r][c].setVisited(false);
            grid[r][c].setMark(' ');
        }
    }
};

bool loadMazeFromCSV(const string& filename, int maze[SIZE][SIZE], const string& mazeName ) {
    ifstream file(filename);

    if (!file) {
        cout << "Error: could not open file " << filename << endl;
        return false;
    }

    string line;
    bool foundMaze = false;
    int row = 0;

    while (getline(file, line)) {

        // remove '\r' (Windows issue)
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // look for Maze#1
        if (!foundMaze) {
            if (line == mazeName) {
                foundMaze = true;
            }
            continue;
        }

        // skip empty lines
        if (line.empty()) continue;

        stringstream ss(line);
        string cell;
        int col = 0;

        while (getline(ss, cell, ',') && col < SIZE) {
            maze[row][col] = stoi(cell);
            col++;
        }

        row++;

        if (row == SIZE) break;
    }

    file.close();

    if (!foundMaze) {
        cout << "Error: " << mazeName << " not found\n";
        return false;
    }

    return true;
};

void printWallInfo(GridNode& node) {
    cout << "Node (" << node.getRow() << "," << node.getCol() << ")\n";
    cout << "Up wall: " << node.hasUpWall() << endl;
    cout << "Down wall: " << node.hasDownWall() << endl;
    cout << "Left wall: " << node.hasLeftWall() << endl;
    cout << "Right wall: " << node.hasRightWall() << endl;
    cout << "Visited: " << node.isVisited() << endl;
    cout << "Mark: [" << node.getMark() << "]" << endl;
};

bool DFS(GridNode grid[SIZE][SIZE]) {
    stack<GridNode*> s;

    // Start at (0,0)
    GridNode* start = &grid[0][0];
    start->setVisited(true);
    start->setMark('*');
    s.push(start);

    while (!s.empty()) {
        GridNode* current = s.top();

        // Check if we reached exit (9,9)
        if (current->getRow() == SIZE - 1 && current->getCol() == SIZE - 1) {
            return true;
        }

        vector<GridNode*> neighbors = getNeighbors(grid, *current);

        GridNode* nextUnvisited = nullptr;

        // Find first unvisited neighbor
        for (GridNode* neighbor : neighbors) {
            if (!neighbor->isVisited()) {
                nextUnvisited = neighbor;
                break;
            }
        }

        // If found, move forward
        if (nextUnvisited != nullptr) {
            nextUnvisited->setVisited(true);
            nextUnvisited->setMark('*');
            s.push(nextUnvisited);
        }
        // If not, backtrack
        else {
            current->setMark('.');
            s.pop();
        }
    }

    return false;
};

void resetGrid(GridNode grid[SIZE][SIZE]) {
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            grid[r][c].setVisited(false);
            grid[r][c].setMark(' ');
        }
    }
};

void printMaze(GridNode grid[SIZE][SIZE]) {

    for (int r = 0; r < SIZE; r++) {

        // Print top walls for row r
        for (int c = 0; c < SIZE; c++) {
            cout << "+";
            if (grid[r][c].hasUpWall()) {
                cout << "---";
            } else {
                cout << "   ";
            }
        }
        cout << "+\n";

        // Print left/right walls and cell marks
        for (int c = 0; c < SIZE; c++) {
            if (grid[r][c].hasLeftWall()) {
                cout << "|";
            } else {
                cout << " ";
            }

            cout << " " << grid[r][c].getMark() << " ";
        }

        // Print the right wall of the last cell in the row
        if (grid[r][SIZE - 1].hasRightWall()) {
            cout << "|";
        } else {
            cout << " ";
        }

        cout << "\n";
    }

    // Print bottom walls for the last row
    for (int c = 0; c < SIZE; c++) {
        cout << "+";
        if (grid[SIZE - 1][c].hasDownWall()) {
            cout << "---";
        } else {
            cout << "   ";
        }
    }
    cout << "+\n";
};


int main(){

    

    //Part 1
    int maze[SIZE][SIZE];
        GridNode grid[SIZE][SIZE];

        for (int i = 1; i <= 5; i++) {

            string mazeName = "Maze#" + to_string(i);

            cout << "\n---------------------------\n";
            cout << "Testing " << mazeName << endl;
            cout << "-----------------------------\n";

            if (!loadMazeFromCSV("MazeFive.csv", maze, mazeName)) {
                cout << "Failed to load " << mazeName << endl;
                continue;
            }

            buildGridFromMaze(maze, grid);

            // Test one cell (top-left)
            printWallInfo(grid[0][0]);
        }
        cout << "--------------------------------------------------------" << endl;

    
    //Part2
    for (int i = 1; i <= 5; i++) {

        string mazeName = "Maze#" + to_string(i);

        cout << "\n" << mazeName << endl;

        if (!loadMazeFromCSV("MazeFive.csv", maze, mazeName)) {
            cout << "Failed to load " << mazeName << endl;
            continue;
        }

        buildGridFromMaze(maze, grid);

        cout << "Neighbors of (0,0):\n";

        vector<GridNode*> neighbors = getNeighbors(grid, grid[0][0]);

        for (GridNode* n : neighbors) {
            cout << "(" << n->getRow() << "," << n->getCol() << ")\n";
        }
    }

    //Part 3, 4, and 5
    {
       for (int i = 1; i <= 5; i++) {

            int maze[SIZE][SIZE];
            GridNode grid[SIZE][SIZE];

            string mazeName = "Maze#" + to_string(i);

            cout << "\n-----------------------------------------\n";
            cout << "Maze: " << mazeName << endl;
            

            if (!loadMazeFromCSV("MazeFive.csv", maze, mazeName)) {
                cout << "Failed to load " << mazeName << endl;
                continue;
            }

            buildGridFromMaze(maze, grid);

            bool reachedExit = DFS(grid);

            // Print maze (Part 4)
            printMaze(grid);

            // Print result (Part 5)
            if (reachedExit) {
                cout << "Exit reached!\n";
            } else {
                cout << "No path to exit.\n";
            }
        }
    }
        

    return 0;

}