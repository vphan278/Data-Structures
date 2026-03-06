#include <iostream>
#include <vector>
#include <queue>
using namespace std;

vector<vector<int>> graph;
vector<bool> visited;

void BFS(int start)
{
    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty())
    {
        int v = q.front();
        q.pop();

        cout << v << " ";

        for (int neighbor : graph[v])
        {
            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}

int main()
{
    graph = {
        {1, 3},  // 0 -> A
        {0, 2},  // 1 -> B
        {1, 3},  // 2 -> C
        {0, 2}   // 3 -> D
    };

    visited = vector<bool>(4, false);

    cout << "BFS starting from vertex 0 (A): ";
    BFS(0);

    return 0;
}