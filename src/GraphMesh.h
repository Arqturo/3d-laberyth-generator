#include <queue>
#include <random>
#include "Edge.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <limits> 
#include <unordered_set>


using namespace std;

class GraphMesh
{
private:
    priority_queue<Edge> edgeL;
    int edge_id;
    int numvertexs;
    int anchor;
    int height;
    vector<unordered_map<string, int>> list;
    vector<int> character;

public:
    GraphMesh(int anchor, int length)
    {
        this->edge_id = 0;
        this->numvertexs = anchor * length;
        this->anchor = anchor;
        this->height = length;
        this->list.resize(this->numvertexs);
        this->character.resize(this->numvertexs);
    }


std::pair<int, int> getCoordinatesFromIndex(int index, int numColumns) {
    int row = index / numColumns;
    int col = index % numColumns;
    return std::make_pair(row, col);
}



int getRandomSideNodeIndex() {
    int side = rand() % 4 + 1;;
    int index;

            switch (side) {
            case 1: // Top side
                index = rand() % this->anchor +1;
                break;
            case 2: // Right side
                index = (rand() % (this->height - 2) + 2) * this->anchor;
                break;
            case 3: // Bottom side
                index = (rand() % this->anchor + 1) + (this->height - 1) * this->anchor;
                break;
            case 4: // Left side
                index = (rand() % (this->height - 2) + 2) * this->anchor + 1;
                break;
            default:
                break;
        }


    while (vertex_grade(index) != 1) {
        int side = rand() % 4 + 1;;
        switch (side) {
            case 1: // Top side
                index = rand() % this->anchor + 1;
                break;
            case 2: // Right side
                index = (rand() % (this->height - 2) + 2) * this->anchor;
                break;
            case 3: // Bottom side
                index = (rand() % this->anchor + 1) + (this->height - 1) * this->anchor;
                break;
            case 4: // Left side
                index = (rand() % (this->height - 2) + 2) * this->anchor + 1;
                break;
            default:
                break;
        }
    }

    return index;
}





    void fullrandom()
    {
        srand(time(NULL));
        int ramdon = 0;
        int i = 1;
        int e = 1;
        while (i <= this->height)
        {
            while (e <= this->anchor)
            {
                if (i > 1)
                {
                    ramdon = rand() % 100 + 1;
                    this->aristand_UP(i, e, ramdon, true);
                }
                if (e < this->anchor)
                {
                    ramdon = rand() % 100 + 1;
                    this->aristand_RIGHT(i, e, ramdon, true);
                }
                e = e + 1;
            }
            i = i + 1;
            e = 1;
        }
    }

    void delVertex(int id)
    {
        this->list.erase(this->list.begin() + id);
    }

    void aristand_UP(int row, int column, int weight, bool conditional)
    {
        if ((column >= 1 && column <= this->anchor) || conditional == false)
        {
            if (((row > 1) && (row <= this->height)) || conditional == false)
            {
                int rowo = row;
                row = row * this->anchor;
                row = row - this->anchor;
                int vertex = row + column;
                this->edge_id = this->edge_id + 1;
                Edge newA(vertex, vertex - this->anchor, weight, this->edge_id, rowo, column, "up");
                this->list[vertex - 1].insert({"up", this->edge_id});
                this->edgeL.push(newA);
                this->character[vertex - 1] = this->character[vertex - 1] + 8;
                this->edge_id = this->edge_id + 1;
                Edge newB(vertex - this->anchor, vertex, weight, this->edge_id, rowo - 1, column, "down");
                this->list[(vertex - this->anchor) - 1].insert({"down", this->edge_id});
                this->edgeL.push(newB);
                this->character[(vertex - this->anchor) - 1] = this->character[(vertex - this->anchor) - 1] + 4;
            }
            else
            {
                cout << "up error " << row << " "
                     << " " << column << endl;
            }
        }
    }

    void aristand_DOWN(int row, int column, int weight, bool conditional)
    {
        if ((row > 0 && row <= this->height) || conditional == false)
        {
            if (((column < this->anchor) && (column >= 1)) || conditional == false)
            {
                int rowo = row;
                row = row * this->anchor;
                row = row - this->anchor;
                int vertex = row + column;
                this->edge_id = this->edge_id + 1;
                Edge newA(vertex, vertex + this->anchor, weight, this->edge_id, rowo, column, "down");
                this->list[vertex - 1].insert({"down", this->edge_id});
                this->edgeL.push(newA);
                this->character[vertex - 1] = this->character[vertex - 1] + 4;
                this->edge_id = this->edge_id + 1;
                Edge newB(vertex + this->anchor, vertex, weight, this->edge_id, rowo + 1, column, "up");
                this->list[(vertex + this->anchor) - 1].insert({"up", this->edge_id});
                this->edgeL.push(newB);
                this->character[vertex + this->anchor - 1] = this->character[vertex + this->anchor - 1] + 8;
            }
            else
            {
                cout << "down error" << endl;
            }
        }
    }

    void aristand_RIGHT(int row, int column, int weight, bool conditional)
    {
        if ((row > 0 && row <= this->height) || conditional == false)
        {
            if (((column < this->anchor) && (column >= 1)) || conditional == false)
            {
                int rowo = row;
                row = row * this->anchor;
                row = row - this->anchor;
                int vertex = row + column;
                this->edge_id = this->edge_id + 1;
                Edge newA(vertex, vertex + 1, weight, this->edge_id, rowo, column, "right");
                this->list[vertex - 1].insert({"right", this->edge_id});
                this->edgeL.push(newA);
                this->character[vertex - 1] = this->character[vertex - 1] + 1;
                this->edge_id = this->edge_id + 1;
                Edge newB(vertex + 1, vertex, weight, this->edge_id, rowo, column + 1, "left");
                this->edgeL.push(newB);
                this->list[(vertex)].insert({"left", this->edge_id});
                this->character[vertex] = this->character[vertex] + 2;
            }
            else
            {
                cout << "right error" << endl;
            }
        }
    }

    void aristand_LEFT(int row, int column, int weight, bool conditional)
    {
        if ((row > 0 && row <= this->height) || conditional == false)
        {
            if (((column < this->anchor) && (column >= 1)) || conditional == false)
            {
                int rowo = row;
                row = row * this->anchor;
                row = row - this->anchor;
                int vertex = row + column;
                this->edge_id = this->edge_id + 1;
                Edge newA(vertex, vertex - 1, weight, this->edge_id, rowo, column, "left");
                this->list[vertex - 1].insert({"left", this->edge_id});
                this->character[vertex - 1] = this->character[vertex - 1] + 2;
                this->edgeL.push(newA);
                this->edge_id = this->edge_id + 1;
                Edge newB(vertex - 1, vertex, weight, this->edge_id, rowo, column - 1, "right");
                this->list[(vertex - 2)].insert({"right", this->edge_id});
                this->edgeL.push(newB);
                this->character[vertex - 2] = this->character[vertex - 2] + 1;
            }
            else
            {
                cout << "right error" << endl;
            }
        }
    }

    int vertex_grade(int id)
    {
        return this->list[id - 1].size();
    }

    int vertexQ()
    {
        return this->numvertexs;
    }

    unordered_map<string, int> debug(int id)
    {
        return this->list[id - 1];
    }

    void show()
    {
        int d = 0;
        for (int x = 0; x < this->height; x++)
        {
            cout << endl;
            for (int e = 0; e < this->anchor; e++)
            {
                cout << this->decode(this->character[d]);
                d = d + 1;
            }
        }
    }

    int **diagram()
    {
        int **outputArray = new int *[this->height];
        for (int i = 0; i < this->height; ++i)
        {
            outputArray[i] = new int[this->anchor];
        }

        int d = 0;
        for (int x = 0; x < this->height; x++)
        {
            for (int e = 0; e < this->anchor; e++)
            {
                // Assuming this->character[d] holds integers
                outputArray[x][e] = this->character[d];
                d = d + 1;
            }
        }
        return outputArray;
    }

    // Modified decode function to return strings instead of characters
std::string decode(int number)
{
    switch (number)
    {
        case 1:
            return "00"; // '╶';
        case 2:
            return "01"; // '╴';
        case 3:
            return "02"; // '─';
        case 4:
            return "03"; // '╷';
        case 5:
            return "04"; // '┌';
        case 6:
            return "05"; // '┐';
        case 7:
            return "06"; // '┬';
        case 8:
            return "07"; // '╵';
        case 9:
            return "08"; // '└';
        case 10:
            return "09"; // '┘';
        case 11:
            return "10"; // '┴';
        case 12:
            return "11"; // '│';
        case 13:
            return "12"; // '├';
        case 14:
            return "13"; // '┤';
        case 15:
            return "14"; // '┼';
        default:
            return "15"; // ' ';
    }
}


    bool find(vector<int> &Arr, int A, int B)
    {
        if (Arr[A] == Arr[B])
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void unionR(vector<int> &Arr, int N, int A, int B)
    {
        int TEMP = Arr[A];
        for (int i = 0; i < N; i++)
        {
            if (Arr[i] == TEMP)
            {
                Arr[i] = Arr[B];
            }
        }
    }

    void addEdge(Edge edge)
    {
        if (edge.direction == "up")
        {
            this->aristand_UP(edge.row, edge.column, edge.weight, false);
        }
        if (edge.direction == "down")
        {
            this->aristand_DOWN(edge.row, edge.column, edge.weight, false);
        }
        if (edge.direction == "right")
        {
            this->aristand_RIGHT(edge.row, edge.column, edge.weight, false);
        }
        if (edge.direction == "left")
        {
            this->aristand_LEFT(edge.row, edge.column, edge.weight, false);
        }
    }

    void addEdgeDouble(Edge edge)
    {
        if (edge.direction == "up")
        {
            this->aristand_UP(edge.row, edge.column, edge.weight, false);
        }
        if (edge.direction == "down")
        {
            this->aristand_DOWN(edge.row, edge.column, edge.weight, false);
        }
        if (edge.direction == "right")
        {
            this->aristand_RIGHT(edge.row, edge.column, edge.weight, false);
        }
        if (edge.direction == "left")
        {
            this->aristand_LEFT(edge.row, edge.column, edge.weight, false);
        }
    }

    void addEdgeF(Edge edge)
    {
        int e = 0;
        int i = 0;
        int d = 0;
        for (i = 0; i < this->height + 1; i++)
        {
            for (e = 0; e < this->anchor + 1; e++)
            {
                d = d + 1;
                if (d == edge.vertex_A)
                {
                    break;
                }
            }
            if (d == edge.vertex_A)
            {
                break;
            }
        }
        if (edge.direction == "up")
        {
            aristand_UP(i, e, edge.weight, false);
        }
        else if (edge.direction == "down")
        {
            aristand_DOWN(i, e, edge.weight, false);
        }
        else if (edge.direction == "right")
        {
            aristand_RIGHT(i, e, edge.weight, false);
        }
        else if (edge.direction == "left")
        {
            aristand_LEFT(i, e, edge.weight, false);
        }
        else
        {
            std::cout << "error" << std::endl;
        }
    }

    GraphMesh kruskal()
    {
        GraphMesh minimun(this->anchor, this->height);
        vector<int> Arr(this->numvertexs);
        for (int i = 0; i < this->numvertexs; i++)
        {
            Arr[i] = i;
        }
        while (!this->edgeL.empty())
        {
            Edge a = this->edgeL.top();
            edgeL.pop();
            if (this->find(Arr, a.myVertex_A() - 1, a.myVertex_B() - 1) == false)
            {
                unionR(Arr, this->numvertexs, a.myVertex_A() - 1, a.myVertex_B() - 1);
                minimun.addEdgeDouble(a);
            }
        }
        return minimun;
    }

    vector<int> D_relax(int v, vector<int> &explored, vector<int> &Dijkstra_nodelist, unordered_map<int, Edge> &edgelist)
    {
        explored.push_back(v);
        unordered_map<string, int> a = this->list[v - 1];
        vector<int> temp;
        for (auto x : a)
        {
            Edge y = edgelist[x.second];
            int next;
            if (!(y.vertex_A - 1 == v - 1))
            {
                next = y.vertex_A;
            }
            else
            {
                next = y.vertex_B;
            }
            bool doo = true;
            for (auto x : explored)
            {
                if (x == next)
                {
                    doo = false;
                }
            }
            if (doo == true)
            {
                Dijkstra_nodelist.push_back(next);
            }
        }
        return Dijkstra_nodelist;
    }

    void Dijkstra(int origin, int end)
    {
        vector<int> Dijkstra_nodelist;
        Dijkstra_nodelist.push_back(origin);
        int calls = 0;
        vector<int> explored;
        unordered_map<int, Edge> edgelist;
        int iterator = 0;
        while (!this->edgeL.empty())
        {
            Edge a = this->edgeL.top();
            this->edgeL.pop();
            edgelist.insert({a.id, a});
        }
        while (Dijkstra_nodelist.size() > 0)
        {
            int v = Dijkstra_nodelist[0];
            Dijkstra_nodelist.erase(Dijkstra_nodelist.begin());
            if (end == v)
            {
                break;
            }
            Dijkstra_nodelist = this->D_relax(v, explored, Dijkstra_nodelist, edgelist);
            calls = calls + 1;
        }
        cout << "Dijkstra: " << calls << endl;
    }

    

    void short_find()
    {
        int first = -1;
        int last;
        int node = 1;
        cout << endl;
        for (auto x : this->list)
        {
            if (x.size() == 1)
            {
                if (first == -1)
                {
                    first = node;
                }
                last = node;
            }
            node = node + 1;
        }
        this->Dijkstra(first, last);
    }


int farthestVertex(int startVertex) {
    vector<int> distances(this->numvertexs, std::numeric_limits<int>::min());
    
  
    this->Dijkstra(startVertex, -1); 

    for (int i = 0; i < this->numvertexs; ++i) {
        distances[i] = this->character[i];
    }
    
    int farthestVertex = startVertex;
    int maxDistance = std::numeric_limits<int>::min(); // Initialize maxDistance to minimum value
    for (int i = 0; i < this->numvertexs; ++i) {
        if (distances[i] > maxDistance && distances[i] != std::numeric_limits<int>::max()) {
            maxDistance = distances[i];
            farthestVertex = i + 1; // Vertex indices are 1-based
        }
    }
    
    return farthestVertex;
}


};
