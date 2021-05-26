#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include <list>
#include "Graph.h"
#include "Session.h"

class Session;

class Tree{
public:
    Tree(int rootLabel); // constructor
    virtual Tree* clone() const =0;
    void addChild(const Tree& child); // makes a copy of the child and adds it to children
    void AddChild(Tree* child); // adds child to children
    static Tree* createTree(const Session& session, int rootLabel); // creates a tree with the suitable treeType
    virtual int traceTree()=0;
    void BFS (int infectedVertex, Session &session); // creates a BFS tree with the infectedVertex as it's root
    int numOfChildren();
    int getRoot();
    vector<Tree*> getChildren();
    int nodeDepth(int node, int depth); // returns the depth of node in the BFS tree
    int findNode(int currCycle); // returns the left most child to support CycleTree traceTree

// Rule of 5
    virtual ~Tree(); // dest
    Tree (const Tree &other); //copy cons
    Tree (Tree&& other); // move cons
    const Tree& operator=(const Tree& other);// assi ope
    const Tree& operator=(Tree&& other);// move  assi ope
    void clear();

protected:
    int node;
    std::vector<Tree*> children;
};

class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle); // constructor
    virtual int traceTree(); // returns the node to disconnect
    virtual Tree* clone() const; // makes a deep copy of a CycleTree

private:
    int currCycle;
};

class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel); // constructor
    virtual int traceTree(); // returns the node to disconnect
    virtual Tree* clone() const; // makes a deep copy of a MaxRankTree
    std::list<Tree*> checkByMaxChildren(); // checks which node in the BFS tree has the most children
    void checkByDepth(std::list<Tree*> ans); // checks which node is the least deep in the BFS tree


//    int maxChildrenInd();
};

class RootTree: public Tree{
public:
    RootTree(int rootLabel); // constructor
    virtual int traceTree(); // returns the node to disconnect
    virtual Tree* clone() const; // makes a deep copy of a RootTree
};

#endif
