#include <iostream>
#include <vector>

using namespace std;

// Adjacency list (global for simplicity)
vector<vector<int>> graph;

// Visited array
vector<bool> visited;

// Depth-First Search function
void DFS(int v)
{
    visited[v] = true;          // Mark vertex as visited
    cout << v << " ";           // Visit (print) vertex

    for (int neighbor : graph[v])
    {
        if (!visited[neighbor])
        {
            DFS(neighbor);      // Go deeper
        }
    }
}

int main()
{
    // Number of vertices: A, B, C, D
    int V = 4;

    // Initialize graph and visited array
    graph.resize(V);
    visited.resize(V, false);

    /*
        Vertex mapping:
        0 -> A
        1 -> B
        2 -> C
        3 -> D

        Graph shape:
        A ─ B
        │   │
        D ─ C
    */

    // Build adjacency list
    graph[0] = {1, 3}; // A: B, D
    graph[1] = {0, 2}; // B: A, C
    graph[2] = {1, 3}; // C: B, D
    graph[3] = {0, 2}; // D: A, C

    cout << "DFS starting from vertex 0 (A): ";
    DFS(1);

    cout << endl;
    return 0;
}