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
#include "GroupNode.h"

GroupNode::GroupNode()
{
    this->type = GROUP;
}

void GroupNode::pushNode()
{
    glPushMatrix();
}

void GroupNode::popNode()
{
    glPopMatrix();
}