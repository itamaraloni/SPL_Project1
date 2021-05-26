#include <vector>
#include <list>

#include "../include/Tree.h"


using namespace std;



//Tree
Tree::Tree(int rootLabel) : node(rootLabel), children(vector<Tree *>()) {}


void Tree::addChild(const Tree &child) {
    Tree *newChild = child.clone();
    children.push_back(newChild);

}

void Tree::AddChild(Tree* child) {
    children.push_back(child);
}

Tree *Tree::createTree(const Session &session, int rootLabel) {
    TreeType type = session.getTreeType();
    Tree *root;
    switch (type) {
        case Cycle:
            root = new CycleTree(rootLabel, session.getCurrCycle());
            break;
        case MaxRank:
            root = new MaxRankTree(rootLabel);
            break;
        case Root:
            root = new RootTree(rootLabel);

    }
    return root;
}

// Rule of 5
Tree::~Tree() {
    clear();
}

// copy const
Tree::Tree(const Tree &other):node(other.node), children(vector<Tree*>()) {
    for (auto currTree : other.children) {
        addChild(*currTree);
    }
}

// move const
Tree::Tree(Tree &&other):node(other.node), children(other.children) {
    other.children.clear();
}

// assi operator
const Tree &Tree::operator=(const Tree &other) {
    if (this != &other) {
        clear();
        node = other.node;
        int size = other.children.size();
        for (int i = 0; i < size; ++i) {
            this->addChild(*other.children[i]);
        }
    }
    return *this;
}

// move assi operator
const Tree& Tree::operator=(Tree &&other){
    if (this != &other) {
        clear();
        node = other.node;
        children = other.children;
        other.children.clear();
    }
    return *this;
}

void Tree::clear() {
    int size = children.size();
    for (int i = 0; i < size; ++i) {
        if (children[i]) delete children[i];
    }
    children.clear();
}

int Tree::numOfChildren() {
    return children.size();
}

int Tree::getRoot() {
    return node;
}

int Tree::nodeDepth(int node, int depth) {
    while (node != getRoot()) {
        for (auto tmpTree : children) {
            depth++;
            tmpTree->nodeDepth(tmpTree->getRoot(), depth);
        }
    }
    return depth;
}

int Tree::findNode(int currCycle) {
    if (currCycle == 0 || children.empty()) {
        return getRoot();
    }
    int newCurrCycle = currCycle - 1;
    return children[0]->findNode(newCurrCycle);
};


void Tree::BFS(int infectedVertex, Session &session) {
    int numOfVertices = session.getGraph().getNumOfVertices();
    vector<bool> visited(numOfVertices);
    std::list<Tree *> queue;
    // Mark the current node as visited and enqueue it
    visited[infectedVertex] = true;
    queue.push_back(this);
    Graph g = session.getGraph();
    while (!queue.empty()) {
        // Dequeue a vertex from queue and print it
        Tree *currTree = queue.front();
        for (int child: g.neighborsOf(currTree->getRoot())) {
            if (!visited[child]) {
                visited[child] = true;
                Tree *childTree = Tree::createTree(session, child);
                queue.push_back(childTree);
                currTree->AddChild(childTree);
            }
        }
        queue.pop_front();
    }
}

vector<Tree *> Tree::getChildren() {
    return children;
}

CycleTree::CycleTree(int rootLabel, int currCycle) : Tree(rootLabel), currCycle(currCycle) {

}

int CycleTree::traceTree() {
    return findNode(currCycle);
};

Tree *CycleTree::clone() const {
    return new CycleTree(*this);
}


MaxRankTree::MaxRankTree(int rootLabel) : Tree(rootLabel) {}

int MaxRankTree::traceTree() {
    // case 1- checking by maximum children
    std::list<Tree *> ans = checkByMaxChildren();
    if (ans.size() == 1) {
        return ans.front()->getRoot();
    }
    // case 2 by depth
    checkByDepth(ans);
    if (ans.size() == 1) {
        return ans.front()->getRoot();
    } else // case 3 by left most child
        return ans.front()->getRoot();
};

std::list<Tree *> MaxRankTree::checkByMaxChildren() {
    int maxChildren = numOfChildren();
    std::list<Tree *> vertices;
    std::list<Tree *> maxChild; // will be returned with all the nodes with max children
    vertices.push_back(this);
    while (!vertices.empty()) {
        Tree *tmpTree = vertices.front();
        vertices.pop_front();
        if (tmpTree == nullptr) {
            continue;
        }
        int childNum = tmpTree->numOfChildren();
        if (childNum > maxChildren) { // new max children node
            maxChild.clear();
            maxChild.push_back(tmpTree);
            maxChildren = childNum;
        }
        if (childNum == maxChildren) {// case for equal numOfChildren
            maxChild.push_back(tmpTree);
        }

        for (int i = 0; i < tmpTree->numOfChildren(); i++) { // insert all children of current node to review them next
            vertices.push_back(tmpTree->getChildren()[i]);
        }
    }

    return maxChild;
}

void MaxRankTree::checkByDepth(std::list<Tree *> ans) {
    Tree *firstTree = *ans.begin();
    int depth = firstTree->nodeDepth(firstTree->getRoot(), 0);
    for (list<Tree *>::iterator it = ans.begin(); it != ans.end(); it++) {
        Tree *tmpTree = *it;
        int tmpDepth = tmpTree->nodeDepth(tmpTree->getRoot(), 0);
        if (tmpDepth > depth) { // found new minDepth node
            ans.pop_front();
            depth = tmpDepth;
        }
    }
}

Tree * MaxRankTree::clone() const {
    return new MaxRankTree(*this);
}

RootTree::RootTree(int rootLabel) : Tree(rootLabel) {}

int RootTree::traceTree() {
    return node;

}

Tree *RootTree::clone() const {
    return new RootTree(*this);
}




