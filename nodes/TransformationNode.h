#ifndef TRANSFORMATION_NODE_H
#define TRANSFORMATION_NODE_H

#include "Node.h"
#include "../math/Vector3.h"

class TransformationNode : public Node
{
public:
    Vector3 transformations;
    TransformationNode(NodeType type, Vector3 point);
    virtual void pushNode();
    virtual void popNode();
};

#endif