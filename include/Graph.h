#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
using namespace std;

class Graph{
public:
    Graph(std::vector<std::vector<int>> matrix); // constructor
    void infectNode(int nodeInd); // make the input node infected (1)
    void makeNodeSick(int nodeInt); // makes the node sick (2)
    bool isInfected(int nodeInd); // check if input node is either infected or sick
    void disconnect(int vertex); // deletes the edges of the input node
    bool isSick(int nodeInd); //check if input node is either infected or sick
    bool isHealthy(int nodeInd); // check if the node is healthy (0)
    int getNumOfVertices();
    vector<vector<int>> getEdges();
    int chooseNodeToSpread(int nodeInd); // choose the first healthy node neighbor if such exists
    bool finishConditions(); // all components are either fully sick or fully healthy
    bool virusHasMoreToSpread(int vertex); // check if the input node has a healthy neighbor
    std::vector <int> neighborsOf(int node);

private:
    std::vector<std::vector<int>> edges;
    int numOfVertices;
    std::vector<int> whoIsInfected;
};

#endif