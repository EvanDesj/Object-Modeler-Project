#ifndef MATERIAL_NODE_H
#define MATERIAL_NODE_H

#include "Node.h"
#include "../math/Vector3.h"
#include "../util/Color.h"
#include "../util/Material.h"

class MaterialNode : public Node
{
public:
    Material material;
    MaterialNode(Material m);
    virtual void pushNode();
    virtual void popNode();
};

#endif