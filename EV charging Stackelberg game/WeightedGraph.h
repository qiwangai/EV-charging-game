#ifndef H_weightedGraph
#define H_weightedGraph

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cfloat>

using namespace std;

class WeightedGraphType {
protected:
	int gSize;      //number of vertices
	list<int> *graph; // Store adjacency list
	double **weights; // Store weights of edges
public:
	WeightedGraphType(int size = 0);
	~WeightedGraphType();

	list<int> getAdjancencyList(int index) {
		return graph[index];
	}

	double getWeight(int i, int j) {
		return weights[i][j];
	}

	vector<double> shortestPath(int vertex);
	stack<int> shortestPath(int v1, int v2);
	void printShortestDistance(int vertex);
	void printAdjacencyList();
	void printAdjacencyMatrix();
};

WeightedGraphType::WeightedGraphType(int size) {
	gSize = 0;
	ifstream infile;
	char fileName[50] = "Weights.txt";

//	cout << "Enter graph adjacency matrix file name: ";
//	cin >> fileName;
//	cout << endl;

	infile.open(fileName);

	if (!infile) {
		cout << "Cannot open input file." << endl;
		return;
	}

	gSize = size;

	graph = new list<int> [gSize];

	weights = new double*[gSize];

	for (int i = 0; i < gSize; i++)
		weights[i] = new double[gSize];

	for (int i = 0; i < gSize; i++) {
		for (int j = 0; j < gSize; j++) {
			double value;
			infile >> value;
			if (value == 0)
				weights[i][j] = DBL_MAX; //system constant - maximum value of double
			else {
				weights[i][j] = value;
				graph[i].push_back(j);
			}
		}
	}
	infile.close();
}

WeightedGraphType::~WeightedGraphType() {
	for (int i = 0; i < gSize; i++)
		delete[] weights[i];

	delete[] weights;

	for (int index = 0; index < gSize; index++)
		graph[index].clear();

	delete[] graph;
}

void WeightedGraphType::printAdjacencyList() {
	for (int index = 0; index < gSize; index++) {
		cout << index << " ";
		for (int e : graph[index])
			cout << e << " ";
		cout << endl;
	}

	cout << endl;
} //end printGraph

vector<double> WeightedGraphType::shortestPath(int vertex) {
	vector<double> smallestWeight;

	for (int j = 0; j < gSize; j++)
		smallestWeight.push_back(weights[vertex][j]);

	bool *weightFound;
	weightFound = new bool[gSize];

	for (int j = 0; j < gSize; j++)
		weightFound[j] = false;

	weightFound[vertex] = true;
	smallestWeight[vertex] = 0.0;

	for (int i = 0; i < gSize - 1; i++) {
		double minWeight = DBL_MAX;
		int v;

		for (int j = 0; j < gSize; j++)
			if (!weightFound[j])
				if (smallestWeight[j] < minWeight) {
					v = j;
					minWeight = smallestWeight[v];
				}

		weightFound[v] = true;

		for (int j = 0; j < gSize; j++)
			if (!weightFound[j])
				if (minWeight + weights[v][j] < smallestWeight[j])
					smallestWeight[j] = minWeight + weights[v][j];
	}
	return smallestWeight;
} //end shortestPath

stack<int> WeightedGraphType::shortestPath(int v1, int v2) {
	vector<double> smallestWeight;
	int parents[gSize];

	for (int j = 0; j < gSize; j++) {
		smallestWeight.push_back(weights[v1][j]);
		parents[j] = v1;
	}

	bool *weightFound;
	weightFound = new bool[gSize];

	for (int j = 0; j < gSize; j++)
		weightFound[j] = false;

	weightFound[v1] = true;
	smallestWeight[v1] = 0.0;

	for (int i = 0; i < gSize - 1; i++) {
		double minWeight = DBL_MAX;
		int v;

		for (int j = 0; j < gSize; j++)
			if (!weightFound[j])
				if (smallestWeight[j] < minWeight) {
					v = j;
					minWeight = smallestWeight[v];
				}

		weightFound[v] = true;

		for (int j = 0; j < gSize; j++)
			if (!weightFound[j])
				if (minWeight + weights[v][j] < smallestWeight[j]) {
					smallestWeight[j] = minWeight + weights[v][j];
					parents[j] = v;
				}
	}

	stack<int> path;
	int end = v2;
	int k = 0;
	while (end != v1 && k < gSize) {
		path.push(end);
		end = parents[end];
		k++;
	}
	path.push(v1);
	return path;
}


//vector<int> WeightedGraphType::ShortestPath(int v1, int v2) {
//	vector<double> shortestPathv2 = shortestPath(v2);
//
//	vector<int> path;
//	queue<int> queue;
//	path.push_back(v1);
//	if (v1 == v2)
//		return path;
//
//	queue.push(v1);
//	double travelDistance = 0;
//
//	list<int>::iterator graphIt;
//	while (!queue.empty()) {
//		int u = queue.front();
//		queue.pop();
//		for (graphIt = graph[u].begin(); graphIt != graph[u].end(); ++graphIt) {
//			int adjacent = *graphIt;
//			if (adjacent == v2) {
//				path.push_back(v2);
//				return path;
//			}
//
//			if (weights[u][adjacent]>0 && weights[u][adjacent] < DBL_MAX && travelDistance + weights[u][adjacent] + shortestPathv2[adjacent]
//					== shortestPathv2[v1]) {
//				travelDistance += weights[u][adjacent];
//				queue.push(adjacent);
//				path.push_back(adjacent);
//				break;
//			}
//		}
//	}
//
//	return path;
//} //end shortestPath

void WeightedGraphType::printShortestDistance(int vertex) {
	cout << "Source Vertex: " << vertex << endl;
	cout << "Shortest distance from source to each vertex." << endl;
	cout << "Vertex  Shortest_Distance" << endl;
	vector<double> smallestWeight = shortestPath(vertex);

	for (int j = 0; j < gSize; j++)
		cout << setw(4) << j << setw(12) << smallestWeight[j] << endl;
	cout << endl;
} //end printShortestDistance

#endif
