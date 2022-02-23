#ifndef NODE_H
#define NODE_H

#include "NodeType.h"
#include "../util/Shape.h"
#include <vector>

extern int getID();

class Node
{
public:
    Node();
    Node* parent;
    std::vector<Node*> *children;
    int id, currentChild;
    bool isDrawable;
    NodeType type;

    void render();
    virtual void pushNode();
    virtual void popNode();
};

#endif