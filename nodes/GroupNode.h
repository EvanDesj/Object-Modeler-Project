#ifndef GROUP_NODE_H
#define GROUP_NODE_H

#include "Node.h"

class GroupNode : public Node
{
public:
    GroupNode();
    virtual void pushNode();
    virtual void popNode();
};

#endif