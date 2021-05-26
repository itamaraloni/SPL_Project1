//
// Created by spl211 on 03/11/2020.
//

#include "../include/Graph.h"

using namespace std;

Graph::Graph(std::vector<std::vector<int>> matrix):edges(matrix),numOfVertices(matrix.size()), whoIsInfected() {
    int vertex = 0;
    while (vertex < numOfVertices){ // initialize all vertices to be healthy
        whoIsInfected.push_back(0);
        vertex++;
    }
}

void Graph::infectNode(int nodeInd) { // node occupies the virus
    whoIsInfected[nodeInd] = 1;
}

void Graph::makeNodeSick(int nodeInd){ // node fully infects the virus
    whoIsInfected[nodeInd] = 2;
}

bool Graph::isInfected(int nodeInd) {
    return whoIsInfected[nodeInd] != 0;
}

bool Graph::isHealthy(int nodeInd) {return whoIsInfected[nodeInd] == 0;}

void Graph::disconnect(int vertex) {
    for (int i = 0; i < numOfVertices ; ++i) {
        edges[vertex][i] = 0;
        edges[i][vertex] = 0;
    }
}

bool Graph::isSick(int nodeInd) {
    return whoIsInfected[nodeInd] == 2;
}
int Graph::getNumOfVertices() {return numOfVertices;}

int Graph::chooseNodeToSpread(int nodeInd) {
    for (int i = 0; i < numOfVertices; ++i) {
        if (edges[nodeInd][i] == 1){
            if (isHealthy(i)){
                infectNode(i);
                return i;
            }

        }
    }
    return -1;
}

bool Graph::finishConditions() {
    for (int vertex = 0; vertex < numOfVertices; ++vertex) {
        if (isInfected(vertex)){
            if (virusHasMoreToSpread(vertex) || !(isSick(vertex))){
                return false;
            }
        }
    }
    return true;
}

bool Graph::virusHasMoreToSpread(int vertex) {
    for (int i = 0; i < numOfVertices; ++i) {
        if (edges[vertex][i] == 1){
            if (isHealthy(i)){
                return true;
            }
        }
    }
    return false;
}

vector<int> Graph::neighborsOf(int node) {
    vector<int> result;
    for (int i = 0; i < numOfVertices; ++i) {
        if (edges[node][i] == 1) {
            result.push_back(i);
        }
    }
    return result;
}

vector<vector<int>> Graph::getEdges() {
    return edges;
}





