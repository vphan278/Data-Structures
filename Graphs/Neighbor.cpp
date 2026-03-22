#include <iostream>
#include <vector>
using namespace std;


/// BUILDING NEIGHBORS
const int SIZE = 3; // small grid for testing

// GridNode class
class GridNode
{
private:
    int row, col;
    bool upWall, downWall, leftWall, rightWall;

public:
    GridNode(int r = 0, int c = 0)
    {
        row = r;
        col = c;

        upWall = downWall = leftWall = rightWall = false;
    }

    int getRow() const { return row; }
    int getCol() const { return col; }

    bool hasUpWall() const { return upWall; }
    bool hasDownWall() const { return downWall; }
    bool hasLeftWall() const { return leftWall; }
    bool hasRightWall() const { return rightWall; }

    void setUpWall(bool val) { upWall = val; }
    void setDownWall(bool val) { downWall = val; }
    void setLeftWall(bool val) { leftWall = val; }
    void setRightWall(bool val) { rightWall = val; }
};

// neighbors function
vector<GridNode*> getNeighbors(GridNode grid[SIZE][SIZE], GridNode& node)
{
    vector<GridNode*> neighbors;

    int r = node.getRow();
    int c = node.getCol();

    //Go up
    if (!node.hasUpWall() && r > 0)
        neighbors.push_back(&grid[r - 1][c]);

    //Go down
    if (!node.hasDownWall() && r < SIZE - 1)
        neighbors.push_back(&grid[r + 1][c]);

    //Go left
    if (!node.hasLeftWall() && c > 0)
        neighbors.push_back(&grid[r][c - 1]);

    //Go right
    if (!node.hasRightWall() && c < SIZE - 1)
        neighbors.push_back(&grid[r][c + 1]);

    return neighbors;
}

int main()
{
    GridNode grid[SIZE][SIZE];

    // initialize grid
    for (int r = 0; r < SIZE; r++)
    {
        for (int c = 0; c < SIZE; c++)
        {
            grid[r][c] = GridNode(r, c);
        }
    }

    // Example: block UP and LEFT for (1,1)
    grid[1][1].setUpWall(true); //made a wall at top
    grid[1][1].setLeftWall(true);  // made a wall at left

    // Get neighbors of (1,1)
    vector<GridNode*> neighbors = getNeighbors(grid, grid[1][1]);

    cout << "Neighbors of (1,1):\n";
    for (GridNode* n : neighbors)
    {
        cout << "(" << n->getRow() << "," << n->getCol() << ")\n";
    }

    return 0;
}