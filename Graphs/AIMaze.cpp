#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

const int SIZE = 10;

struct GridNode {
    int row;
    int col;

    bool upWall;
    bool downWall;
    bool leftWall;
    bool rightWall;

    bool visited;
    char mark;   // ' ' = unvisited, '*' = forward, '.' = backtracked
};

class GraphMaze {
private:
    int matrix[SIZE][SIZE];
    GridNode grid[SIZE][SIZE];

public:
    GraphMaze() {
        // initialize matrix to 0
        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                matrix[r][c] = 0;
            }
        }
    }

    bool loadMazeFromCSV(const string& filename, const string& mazeLabel = "Maze#5") {
        ifstream fin(filename);
        if (!fin) {
            cerr << "Error: Could not open file " << filename << endl;
            return false;
        }

        string line;
        bool foundMaze = false;
        int row = 0;

        while (getline(fin, line)) {
            if (line.empty()) continue;

            // Look for requested maze section
            if (line == mazeLabel) {
                foundMaze = true;
                row = 0;
                continue;
            }

            // If we already found the maze, read next 10 rows
            if (foundMaze && row < SIZE) {
                stringstream ss(line);
                string value;
                int col = 0;

                while (getline(ss, value, ',') && col < SIZE) {
                    matrix[row][col] = stoi(value);
                    col++;
                }

                if (col != SIZE) {
                    cerr << "Error: Maze row does not contain 10 values.\n";
                    return false;
                }

                row++;
                if (row == SIZE) break;
            }
        }

        fin.close();

        if (!foundMaze) {
            cerr << "Error: Could not find " << mazeLabel << " in file.\n";
            return false;
        }

        if (row != SIZE) {
            cerr << "Error: Maze section incomplete.\n";
            return false;
        }

        return true;
    }

    void interpretMatrixIntoGridNodes() {
        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                int value = matrix[r][c];

                grid[r][c].row = r;
                grid[r][c].col = c;

                grid[r][c].upWall    = (value & 1) != 0;
                grid[r][c].downWall  = (value & 2) != 0;
                grid[r][c].leftWall  = (value & 4) != 0;
                grid[r][c].rightWall = (value & 8) != 0;

                grid[r][c].visited = false;
                grid[r][c].mark = ' ';
            }
        }
    }

    vector<GridNode*> getNeighbors(GridNode& node) {
        vector<GridNode*> neighbors;
        int r = node.row;
        int c = node.col;

        // Up
        if (!node.upWall && r > 0) {
            neighbors.push_back(&grid[r - 1][c]);
        }

        // Right
        if (!node.rightWall && c < SIZE - 1) {
            neighbors.push_back(&grid[r][c + 1]);
        }

        // Down
        if (!node.downWall && r < SIZE - 1) {
            neighbors.push_back(&grid[r + 1][c]);
        }

        // Left
        if (!node.leftWall && c > 0) {
            neighbors.push_back(&grid[r][c - 1]);
        }

        return neighbors;
    }

    void printIntegerMatrix() {
        cout << "Integer Matrix:\n";
        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                cout << matrix[r][c];
                if (c < SIZE - 1) cout << "\t";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    void printWallInfo() {
        cout << "Decoded Wall Information:\n";
        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                GridNode& n = grid[r][c];
                cout << "(" << r << "," << c << ") "
                     << "U:" << n.upWall
                     << " D:" << n.downWall
                     << " L:" << n.leftWall
                     << " R:" << n.rightWall << "\n";
            }
        }
        cout << "\n";
    }

    void printNeighbors() {
        cout << "Neighbor Relationships:\n";
        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                cout << "(" << r << "," << c << ") -> ";
                vector<GridNode*> neighbors = getNeighbors(grid[r][c]);

                if (neighbors.empty()) {
                    cout << "none";
                } else {
                    for (size_t i = 0; i < neighbors.size(); i++) {
                        cout << "(" << neighbors[i]->row << "," << neighbors[i]->col << ")";
                        if (i + 1 < neighbors.size()) cout << ", ";
                    }
                }
                cout << "\n";
            }
        }
        cout << "\n";
    }

    bool solveDFSWithExplicitStack() {
        stack<GridNode*> st;

        GridNode* start = &grid[0][0];
        GridNode* exitNode = &grid[SIZE - 1][SIZE - 1];

        start->visited = true;
        start->mark = '*';
        st.push(start);

        while (!st.empty()) {
            GridNode* current = st.top();

            if (current == exitNode) {
                return true;
            }

            vector<GridNode*> neighbors = getNeighbors(*current);
            GridNode* nextNode = nullptr;

            // choose first unvisited neighbor in DFS order
            for (GridNode* neighbor : neighbors) {
                if (!neighbor->visited) {
                    nextNode = neighbor;
                    break;
                }
            }

            if (nextNode != nullptr) {
                nextNode->visited = true;
                nextNode->mark = '*';
                st.push(nextNode);
            } else {
                // dead end, backtrack
                if (!(current->row == SIZE - 1 && current->col == SIZE - 1)) {
                    current->mark = '.';
                }
                st.pop();
            }
        }

        return false;
    }

    void printAsciiMaze() {
        for (int r = 0; r < SIZE; r++) {
            // Print top walls for row r
            for (int c = 0; c < SIZE; c++) {
                cout << "+";
                if (grid[r][c].upWall) cout << "---";
                else                   cout << "   ";
            }
            cout << "+\n";

            // Print left wall + cell mark + right boundary style by next cell
            for (int c = 0; c < SIZE; c++) {
                if (grid[r][c].leftWall) cout << "|";
                else                     cout << " ";

                cout << " " << grid[r][c].mark << " ";
            }

            // right boundary of last cell
            if (grid[r][SIZE - 1].rightWall) cout << "|";
            else                             cout << " ";
            cout << "\n";
        }

        // Print bottom walls of last row
        for (int c = 0; c < SIZE; c++) {
            cout << "+";
            if (grid[SIZE - 1][c].downWall) cout << "---";
            else                            cout << "   ";
        }
        cout << "+\n";
    }
};

int main() {
    GraphMaze maze;

    // Change Maze#5 to Maze#1, Maze#2, etc. if needed
    if (!maze.loadMazeFromCSV("MazeFive.csv", "Maze#5")) {
        return 1;
    }

    maze.interpretMatrixIntoGridNodes();

    cout << "==============================\n";
    cout << "PART 1: INTEGER MATRIX\n";
    cout << "==============================\n";
    maze.printIntegerMatrix();

    cout << "==============================\n";
    cout << "PART 1: DECODED GRID NODES\n";
    cout << "==============================\n";
    maze.printWallInfo();

    cout << "==============================\n";
    cout << "PART 2: NEIGHBOR RELATIONSHIPS\n";
    cout << "==============================\n";
    maze.printNeighbors();

    cout << "==============================\n";
    cout << "INITIAL ASCII MAZE\n";
    cout << "==============================\n";
    maze.printAsciiMaze();
    cout << "\n";

    cout << "==============================\n";
    cout << "PART 3: DFS WITH EXPLICIT STACK\n";
    cout << "==============================\n";
    bool reachedExit = maze.solveDFSWithExplicitStack();

    cout << "==============================\n";
    cout << "FINAL ASCII MAZE WITH TRAVERSAL\n";
    cout << "==============================\n";
    maze.printAsciiMaze();
    cout << "\n";

    if (reachedExit) {
        cout << "Exit reached at (9,9).\n";
    } else {
        cout << "Exit was NOT reached.\n";
    }

    return 0;
}