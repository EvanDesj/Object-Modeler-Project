#ifndef SHAPE_NODE_H
#define SHAPE_NODE_H

#include "Node.h"
#include "../util/Shape.h"
#include <iostream>
#include <fstream>

class ShapeNode : public Node
{
public:
    Shape s;
    bool selected;
    GLUquadricObj *sphere;
    ShapeNode(NodeType type);
    virtual void pushNode();
    virtual void popNode();
};

#endif