#ifndef NODE_TYPE_H
#define NODE_TYPE_H

#include <string.h>
#include <string>

enum NodeType
{
    ROOT = 0,
    GROUP = 1,
    // Transformation Node Types
    SCALE = 2,
    TRANSLATE = 3,
    ROTATE = 4,
    // Shape Node Types
    CUBE = 5,
    SPHERE = 6,
    TEAPOT = 7,
    TORUS = 8,
    TETRAHEDRON = 9,
    // Appearance Node Types
    MATERIAL = 10,
    TEXTURE = 11
};

std::string getShapeString(int shapeType);

NodeType getNodeType(std::string type);

#endif