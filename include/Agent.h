#ifndef AGENT_H_
#define AGENT_H_

#include <vector>
#include "Session.h"

class Agent{
public:
    Agent(); // constructor
    virtual ~Agent(); // destructor
    virtual Agent* clone() const =0;
    virtual void act(Session& session)=0; // abstract func
};

class ContactTracer: public Agent{
public:
    ContactTracer(); // constructor
    virtual Agent* clone() const; // makes a deep copy of the contact tracer
    virtual void act(Session& session); // iteration as described in the assignment (dequeue, treeTrace, update graph(disconnect vertices))
};


class Virus: public Agent{
public:
    Virus(int nodeInd); // constructor
    virtual Agent* clone() const; // makes a deep copy of the virus
    virtual void act(Session& session); // infect node it occupies, spread itself to new node (create now virus object)
private:
    const int nodeInd;
};

#endif