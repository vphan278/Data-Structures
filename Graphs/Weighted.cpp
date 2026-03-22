#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Graph
{
private:
    // vertex → list of (neighbor, weight)
    map<int, vector<pair<int, int>>> g;

public:
    void addVertex(int v)
    {
        if (g.find(v) == g.end())
        {
            g[v] = vector<pair<int, int>>();
        }
    }

    void addEdge(int from, int to, int weight)
    {
        addVertex(from);
        addVertex(to);

        g[from].push_back({to, weight});  // one direction
    }

    void print()
    {
        for (auto& p : g)
        {
            cout << p.first << ": ";

            for (auto& edge : p.second)
            {
                cout << "(" << edge.first << ", w=" << edge.second << ") ";
            }

            cout << endl;
        }
    }
};

int main()
{
    Graph g;

    g.addEdge(1, 2, 5);  // 1 → 2 (cost 5)
    g.addEdge(1, 3, 2);  // 1 → 3 (cost 2)
    g.addEdge(2, 4, 7);  // 2 → 4 (cost 7)

    g.print();

    return 0;
}