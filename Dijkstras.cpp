#include <iostream>
#include <vector>
#include <cassert>
#include <limits>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

int MAX_INT = numeric_limits<int>::max();

// graph class implementation
class Graph
{
public:

	Graph(int edgeNum = 0, int verticeNum = 5, double density = 0.2);

	int getVerticeNum() const { return verticeNum; }

	int getEdgeNum() const { return edgeNum; }

	// tests whether there is an edge from node x to node y
	bool adjacent(int x, int y) const;

	// lists all nodes y such that there is an edge from
	// x to y
	vector<int> neighbors(int x) const;

	// return all vertices in the graph
	vector<int> getVertices() const;

	bool add_edge(int x, int y, int value);

	bool delete_edge(int x, int y);

	int get_edge_value(int x, int y) const;

	void set_edge_value(int x, int y, int value);

private:
	int edgeNum;
	int verticeNum;

	vector<vector<int> > adMatrix;
};

Graph::Graph(int edgeNum, int verticeNum, double density)
{
	this->edgeNum = edgeNum;

	if (verticeNum <= 0)
	{
		this->verticeNum = 0;
		return;
	}
	else
		this-> verticeNum = verticeNum;

	adMatrix = vector<vector<int> >(verticeNum, vector<int>(verticeNum));

	srand(time(0));

	const int RANGE = 10;
	for (int i = 0; i < verticeNum-1; i++)
	{
		for (int j = 1; j < verticeNum; j++)
		{
			double prob = (rand() % 100) / 100.0;
			if (prob < density && i!= j) {
				int value = rand() % RANGE + 1;
				if (adMatrix[i][j] == 0 && adMatrix[j][i] == 0) {
					add_edge(i, j, value);
					add_edge(j, i, value);
				}
			}
		}
	}
}

bool Graph::adjacent(int x, int y) const
{
	assert(x>=0 && x<verticeNum && y>=0 && y<verticeNum);

	return adMatrix[x][y] > 0;
}

vector<int> Graph::neighbors(int x) const
{
	assert(x>=0 && x<verticeNum);

	vector<int> list;
	for (int i=0; i<verticeNum; i++)
	{
		if (adMatrix[x][i] > 0)
		{
			list.push_back(i);
		}
	}

	return list;
}

vector<int> Graph::getVertices() const
{
	vector<int> vertices;

	for (int i=0; i<verticeNum; i++)
	{
		vertices.push_back(i);
	}

	return vertices;
}

bool Graph::add_edge(int x, int y, int value)
{
	assert(x>=0 && x<verticeNum && y>=0 && y<verticeNum);

	if (adMatrix[x][y] > 0)
		return false;

	adMatrix[x][y] = value;
	adMatrix[y][x] = value;

	return true;
}

bool Graph::delete_edge(int x, int y)
{
	assert(x>=0 && x<verticeNum && y>=0 && y<verticeNum);

	if (adMatrix[x][y] > 0)
	{
		adMatrix[x][y] = 0;
		adMatrix[y][x] = 0;
		return true;
	}

	return false;
}

int Graph::get_edge_value(int x, int y) const
{
	assert(x>=0 && x<verticeNum && y>=0 && y<verticeNum);

	return adMatrix[x][y];
}

void Graph::set_edge_value(int x, int y, int value)
{
	assert(x>=0 && x<verticeNum && y>=0 && y<verticeNum);

	adMatrix[x][y] = value;
	adMatrix[y][x] = value;
}

struct QueueNode
{
	int symbol;
	int priority;

	QueueNode(int symbol=-1, int priority=INT_MAX):
		symbol(symbol), priority(priority)
	{
	}
};

void swap(QueueNode *x, QueueNode *y)
{
	QueueNode temp = *x;
    *x = *y;
    *y = temp;
}


class MinHeap
{
public:
    // Constructor
    MinHeap(vector<QueueNode> &symbols, int verticeNum = 5);

    void MinHeapify(int );

    int parent(int i) { return (i-1)/2; }

    // to get index of left child of node at index i
    int left(int i) { return (2*i + 1); }

    // to get index of right child of node at index i
    int right(int i) { return (2*i + 2); }

    // to extract the root which is the minimum element
    void minPrioirty();

    QueueNode& top();

    bool contains(int symbol);

    void chgNodeValue(int symbol, int value);

    int size(){return minHeap.size();}

    int getPriority (int symbol);

private:
    vector<QueueNode> minHeap;
    int capacity;
};

MinHeap::MinHeap(vector<QueueNode> &symbols, int verticeNum)
{
	capacity = verticeNum;
	for (int i = 0; i < symbols.size(); i++){
		minHeap.push_back(QueueNode(i, MAX_INT));
	}
}

int MinHeap::getPriority(int symbol){
	for (int i = 0; i < capacity; i++){
		if (minHeap[i].symbol == symbol){
			return minHeap[i].priority;
		}
	}
	return MAX_INT;

}


bool MinHeap::contains(int symbol){
	vector<int> temp;

	for (int i = 0; i < minHeap.size(); i++){
		temp.push_back(minHeap[i].symbol);
	}

	if (symbol >= 0){
		for (int i = 0; i < temp.size(); i++){
			if (temp[i] == symbol){
				return true;
			}
		}

	}

	return false;
}

void MinHeap::chgNodeValue(int symbol, int value)
{
	for (int i = 0; i < capacity; i++){
		if (minHeap[i].symbol == symbol){
			minHeap[i].priority = value;
		}
	}

	MinHeapify(0);

}

QueueNode& MinHeap::top()
{
	return minHeap[0];
}


void MinHeap::minPrioirty()
{

	swap(&minHeap.front(), &minHeap.back());
	minHeap.pop_back();
	MinHeapify(0);

}

void MinHeap::MinHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < minHeap.size() && minHeap[l].priority < minHeap[i].priority)
        smallest = l;
    if (r < minHeap.size() && minHeap[r].priority < minHeap[smallest].priority)
        smallest = r;
    if (smallest != i)
    {
        swap(&minHeap[i], &minHeap[smallest]);
        MinHeapify(smallest);
    }
}



class ShortestPath
{
public:
	// find the shortest path from u to w
	vector<int> path(const Graph &graph, int u, int w);

	// return the path cost associated with the shortest path
	int pathSize(const Graph &graph, int u, int w);

private:
	int dijkstrAlgo(const Graph &graph, int u, int w);
	vector<int> traces;
};

vector<int> ShortestPath::path(const Graph &graph, int u, int w)
{
	vector<int> route;

	if (dijkstrAlgo(graph, u, w) == MAX_INT){
		return route;
	}

	int cur = w;
	do
	{
		route.push_back(cur);
		cur = traces[cur];

	} while (cur != -1);

	reverse(route.begin(), route.end());

	return route;
}

int ShortestPath::pathSize(const Graph &graph, int u, int w){
	return dijkstrAlgo(graph, u, w);
}


int ShortestPath::dijkstrAlgo(const Graph &graph, int u, int w)
{
	int size = graph.getVerticeNum();
	assert(u>=0 && u<size && w>=0 && w<size);

	// initiate traces;
	traces.clear();
	traces.resize(size, -1);

	vector<QueueNode> heap(size);

	MinHeap pQueue(heap, size);
	pQueue.chgNodeValue(u, 0);

	while (pQueue.size() > 0)
	{
		QueueNode top = pQueue.top();
		pQueue.minPrioirty(); // removes the minimum node value node

		if (top.symbol == w){
			return top.priority;
		}

		vector<int> neighbors = graph.neighbors(top.symbol);

		QueueNode node;
		for (int i = 0; i < neighbors.size(); i++)
		{
			node.symbol = neighbors[i];
			node.priority = pQueue.getPriority(node.symbol);

			if (pQueue.contains(node.symbol))
			{
				int edge = graph.get_edge_value(top.symbol, node.symbol);
				if (edge + top.priority < node.priority)
				{
					traces[node.symbol] = top.symbol;
					node.priority = edge + top.priority;
					pQueue.chgNodeValue(node.symbol, node.priority);
				}
			}
		}
	}

	return MAX_INT;
}

int main()
{

	int verticeNum = 50;

	double density[2] = {0.2, 0.4};
	ShortestPath sp;

	for (int j = 0; j < 2; j++){

		int sum = 0;
		int count = 0;
		Graph graph(0, verticeNum, density[j]);

		for (int n = 1; n < verticeNum; n++){

			int pathSize = sp.pathSize(graph, 0, n);

			if (pathSize != MAX_INT && pathSize >= 0){
				sum += pathSize;
				count += 1;
			}
		}
		cout << "For the graph with density = " << density[j];
		cout << ", the average path length = " << (static_cast<double>(sum) / (count-1)) << endl;
	}

	return 0;
}
