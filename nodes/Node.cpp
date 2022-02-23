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
#include <vector>
#include <cmath>
#include "Node.h"

Node::Node()
{
    this->id = getID();
    this->parent = 0;
    this->children = new std::vector<Node*>();
    this->currentChild = 0;
    this->isDrawable = false;
    this->type = ROOT;
}

void Node::render()
{
    pushNode();
    for (int i = 0; i < this->children->size(); i++)
        this->children->at(i)->render();
    popNode();
}

void Node::pushNode()
{
}

void Node::popNode()
{
}