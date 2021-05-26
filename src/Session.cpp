//
// Created by spl211 on 07/11/2020.
//

#include "../include/Session.h"
#include <vector>
#include <list>
#include <fstream>
#include "../include/json.hpp"
#include "../include/Agent.h"

using namespace std;
using json = nlohmann::json ;

Session::Session(const std::string &path):g(vector<vector<int>>()),treeType(),agents(vector<Agent*>()), infectedQueue(list<int>()),toDequeue(0),currCycle(0) { //constructor
    ifstream read(path);
    json jsonFile = json ::parse(read);

    vector<vector<int>> matrix;
    for(auto& element: jsonFile["graph"]){
        matrix.push_back(element);
    }
    g = Graph(matrix);

    for(auto& element:jsonFile["agents"]){
        if (element[0] == "C"){
            ContactTracer* CT = new ContactTracer;
            agents.push_back(CT);
        } else{
            int vertex = element[1];
            Virus* V = new Virus(vertex);
            agents.push_back(V);
            g.infectNode(vertex);
        }
    }

    if (jsonFile["tree"] == "C"){
        treeType = Cycle;
    } else if (jsonFile["tree"] == "M"){
        treeType = MaxRank;
    } else{
        treeType = Root;
    }
}


// rule of 5

// destructor
Session::~Session() {
    clear();
}

void Session::clear() {
    int size = agents.size();
    for (int i = 0; i < size; ++i) {
        if (agents[i]) delete agents[i];
    }
    agents.clear();
}

// copy constructor
Session::Session(const Session &other):g(other.g) ,treeType(other.treeType), agents(vector<Agent*>()), infectedQueue(other.infectedQueue),toDequeue(other.toDequeue),currCycle(other.currCycle)  {
    int size = other.agents.size();
    for (int i = 0; i < size; ++i) {
        Agent* currAgent = other.agents[i];
        addAgent(*currAgent);
    }
}

// move constructor
Session::Session(Session &&other):g(other.g) ,treeType(other.treeType), agents(other.agents), infectedQueue(other.infectedQueue),toDequeue(other.toDequeue),currCycle(other.currCycle)  {
        other.agents.clear();
}

//assignment operator
const Session & Session::operator=(const Session &other) {
    if (this != &other){
        g = other.g;
        infectedQueue = other.infectedQueue;
        toDequeue = other.toDequeue;
        clear();
        int size = other.agents.size();
        for (int i = 0; i < size; ++i) {
            this->addAgent(*other.agents[i]);
        }

    }
    return *this;
}

// move assignment operator
const Session & Session::operator=(Session &&other) {
    if (this != &other){
        clear();
        g = other.g;
        infectedQueue = other.infectedQueue;
        toDequeue = other.toDequeue;
        agents = other.agents;
        other.agents.clear();

    }
    return *this;
}

void Session::addAgent(const Agent &agent) {
    Agent* newAgent = agent.clone();
    agents.push_back(newAgent);
}

void Session::AddAgent(Agent* agent) {
    agents.push_back(agent);
}

int Session::dequeueInfected() {
    int inefectedNode = infectedQueue.front();
    infectedQueue.pop_front();
    return inefectedNode;
}
void Session::enqueueInfected(int vertex) {
    infectedQueue.push_back(vertex);
}

void Session::simulate() {
    while (!(g.finishConditions())){
        int agentsSize = agents.size();
        for(int i = 0; i < agentsSize; i++){
            agents[i]->act(*this);
        }
        currCycle++;
    }
    output();
}

Graph Session::getGraph() {return g;}

TreeType Session::getTreeType() const {
    return treeType;
}

int Session::getCurrCycle() const {
    return currCycle;
}

list<int> Session::getInfectedQueue() {
    return infectedQueue;
}

void Session::output() {
    json j2;
    j2["graph"] = g.getEdges();
    vector<int> infected;
    for (int j = 0; j < g.getNumOfVertices(); ++j) {
        if (g.isInfected(j)){
            infected.push_back(j);
        }
    }
    j2["infected"] = infected;
    ofstream output("./output.json");
    output << j2;
}

void Session::setGraph(const Graph &graph) {
    g = graph;
}

