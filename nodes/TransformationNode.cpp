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
#include "TransformationNode.h"

TransformationNode::TransformationNode(NodeType type, Vector3 point)
{
    this->type = type;
    this->transformations = point;
};

void TransformationNode::pushNode()
{
    switch (this->type)
    {
    case TRANSLATE:
        glTranslatef(
            transformations.x,
            transformations.y,
            transformations.z);
        break;
    case ROTATE:
        glRotatef(transformations.x, 1.0f, 0.0f, 0.0f);
        glRotatef(transformations.y, 0.0f, 1.0f, 0.0f);
        glRotatef(transformations.z, 0.0f, 0.0f, 1.0f);
        break;
    case SCALE:
        glScalef(
            transformations.x,
            transformations.y,
            transformations.z);
        break;
    }
};

void TransformationNode::popNode()
{
};