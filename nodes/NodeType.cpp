// Include Dependencies
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif
#include "NodeType.h"
#include "../util/Shape.h"
#include <map>

std::string getShapeString(int shapeType)
{
    return (std::string[12]){
        "root",
        "group",
        "scale",
        "translate",
        "rotate",
        "cube",
        "sphere",
        "teapot",
        "torus",
        "tetrahedron",
        "material",
        "texture"}[shapeType];
}

NodeType getNodeType(std::string type)
{
    return std::map<std::string, NodeType>{
        {"root", ROOT},
        {"group", GROUP},
        {"scale", SCALE},
        {"translate", TRANSLATE},
        {"rotate", ROTATE},
        {"cube", CUBE},
        {"sphere", SPHERE},
        {"teapot", TEAPOT},
        {"torus", TORUS},
        {"tetrahedron", TETRAHEDRON},
        {"material", MATERIAL},
        {"texture", TEXTURE}}[type];
}