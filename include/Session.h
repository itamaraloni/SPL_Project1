#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <string>
#include "Graph.h"
#include <list>

class Agent;


enum TreeType{
    Cycle,
    MaxRank,
    Root
};

class Session{
public:
    Session(const std::string& path); //constructor

    //rule of 5:
    virtual ~Session(); // destructor
    void clear();
    Session(const Session &other); // copy constructor
    Session(Session&& other);// move constructor
    const Session& operator=(const Session& other);// assignment operator
    const Session& operator=(Session&& other); // move assignment operator

    void simulate();
    void addAgent(const Agent& agent); // makes a copy of the agents and adds it to agents
    void AddAgent(Agent* agent); // adds the agent to agents
    void setGraph(const Graph& graph); // update g according to graph
    void enqueueInfected(int); // enqueue the infected node to the queue
    int dequeueInfected(); // dequeue the next infected node in the queue and use it to build tree
    Graph getGraph();
    TreeType getTreeType() const;
    int getCurrCycle() const;
    list<int> getInfectedQueue();
    void output(); // generates the output to a json file

private:
    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;
    std::list<int> infectedQueue;
    int toDequeue;
    int currCycle;

};

#endif