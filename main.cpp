#include <iostream>

#include <algorithm>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

class Edge {
  public:
    int node1;
    int node2;
    int weight;

    Edge(int node1, int node2, int weight) {
      this->node1 = node1;
      this->node2 = node2;
      this->weight = weight;
    }
};

class Tuple {
  public:
    int x;
    int y;

    Tuple(int x, int y) {
      this->x = x;
      this->y = y;
    }
};

class AdjListHead
{
  public:
    int headNode;
    vector<Tuple> adjListBody;
    AdjListHead(int, vector<Tuple>);
    void addWeightedNode(int, int);
};

AdjListHead::AdjListHead(int headNode, vector<Tuple> adjListBody) {
  this->headNode = headNode;
  this->adjListBody = adjListBody;
}

void AdjListHead::addWeightedNode(int node2, int weight) {
      this->adjListBody.push_back(Tuple(node2, weight));
}

class Graph {
  private:
    int nodes;
    int edges;

  public:
    vector<Edge> edgeList;
    vector<AdjListHead> adjacencyList;
    void setEdge(int, int, int);
    void graphMaker (string);
    int getNodes() { return this->nodes; }
    int getEdges() { return this->edges; }
};

void Graph::setEdge(int node1, int node2, int weight) {
  Edge edge(node1, node2, weight);
  edgeList.push_back(edge);
}

void Graph::graphMaker (string fileName) {
  ifstream infile(fileName);
  vector<int> tracker;

  int nodes, edges, node1, node2, weight, key;

  infile >> nodes >> edges;
  this->nodes = nodes;
  this->edges = edges;

  //get edge list from file
  for(int i = 0; i < edges; i++) {
  infile >> node1 >> node2 >> weight;
  this->setEdge(node1, node2, weight);
  }

  //build list of nodes in graph from node1
  vector<Edge>::iterator itr;
  for(itr = this->edgeList.begin(); itr != this->edgeList.end(); itr++) {
    if( find(tracker.begin(), tracker.end(), (itr->node1)) != tracker.end()) {
      continue;
    }
    else {
      vector<Tuple> adjListBody;
      tracker.push_back(itr->node1);
      this->adjacencyList.push_back(AdjListHead(itr->node1, adjListBody));
    }
  }

  //build adjacency list
    for(itr=(this->edgeList.begin()); itr != this->edgeList.end(); itr++) {
      key = itr->node1;

      vector<AdjListHead>::iterator rowChecker = find_if(
          this->adjacencyList.begin(), this->adjacencyList.end(),
              [key] (const AdjListHead& row) { return row.headNode == key; });

      if (rowChecker != (this->adjacencyList.end())) {
          auto index = distance(this->adjacencyList.begin(), rowChecker);
              this->adjacencyList.at(index).addWeightedNode(itr->node2, itr->weight);
      }
    }

  }

int main() {
    Graph graph;
    graph.graphMaker("edges.txt");

    vector<AdjListHead>::iterator itr;
    vector<Tuple>::iterator it;

    ofstream output;
    output.open("output.txt");

    for(itr = graph.adjacencyList.begin(); itr != graph.adjacencyList.end(); itr++) {
      output << "u: " << itr->headNode << " ";
      for(it = itr->adjListBody.begin(); it != itr->adjListBody.end(); it++ ) {
        output << "v: " << it->x << " " << it->y << " ";
      }
      output << endl;
    }
    output.close();
    return 0;

}
