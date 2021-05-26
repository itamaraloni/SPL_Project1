//
// Created by spl211 on 07/11/2020.
//

#include "../include/Agent.h"
#include "../include/Tree.h"

//Agent class:

Agent::Agent(){} // constructor

Agent::~Agent() {} // destructor


//Contact tracer class:

ContactTracer::ContactTracer() {} // constructor

void ContactTracer::act(Session &session) {
    if (!session.getInfectedQueue().empty()){ // if the infected queue is empty the ContactTracer has nothing to do
        int infectedVertex = session.dequeueInfected();
        Tree* tree = Tree::createTree(session,infectedVertex);
        tree->BFS(infectedVertex,session);
        if (tree->numOfChildren() == 0){ // if the vertex is already disconnected there is nothing for the CT to do
            delete tree;
            return;
        }
        int vertexToDisconnect = tree->traceTree();
        Graph graph = session.getGraph();
        graph.disconnect(vertexToDisconnect);
        session.setGraph(graph);
        delete tree;
    }
}

Agent * ContactTracer::clone() const {
    return new ContactTracer(*this);
}

//Virus class:

Virus::Virus(int nodeInd):nodeInd(nodeInd) {} // constructor

Agent * Virus::clone() const {
    return new Virus(*this);
}

void Virus::act(Session &session) {
    Graph graph = session.getGraph();
    if (!graph.isSick(nodeInd)){ // make node sick in the case it is not sick
        graph.makeNodeSick(nodeInd);
        session.enqueueInfected(nodeInd);
    }
    int nodeToSpread = graph.chooseNodeToSpread(nodeInd);
    if (nodeToSpread != -1){ //means the virus has spread to a new node
        Virus* v = new Virus(nodeToSpread);
        session.AddAgent(v);
    }
    session.setGraph(graph);
}





