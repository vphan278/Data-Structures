#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Graph
{
private:
    map<int, vector<int>> g;  // vertex → neighbors

public:
    void addEdge(int a, int b)
    {
        g[a].push_back(b);
        g[b].push_back(a);  // undirected
    }

    void print()
    {
        for (auto& p : g)
        {
            cout << p.first << ": ";

            for (int n : p.second)
                cout << n << " ";

            cout << endl;
        }
    }
};

int main()
{
    Graph g;

    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);

    g.print();

    return 0;
}