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

#include "SceneGraph.h"
#include "../nodes/ShapeNode.h"
#include "../nodes/TransformationNode.h"

#include <vector>
#include <cmath>

SceneGraph::SceneGraph()
{
    this->rootNode = new Node();
    this->currentNode = this->rootNode;
};

void SceneGraph::toRoot()
{
    this->currentNode = this->rootNode;
};

void SceneGraph::toParent()
{
    this->currentNode = this->currentNode->parent;
};

void SceneGraph::toChild(int i)
{
    if (0 <= i && i < this->currentNode->children->size())
    {
        this->currentNode = this->currentNode->children->at(i);
    }
};

void SceneGraph::toChildID(int id)
{
    for (Node *child : *this->currentNode->children)
    {
        if (child->id == id)
        {
            this->currentNode = child;
            return;
        }
    }
};

int SceneGraph::getCurrentNodeID()
{
    return this->currentNode->id;
};

void SceneGraph::insertNode(Node *node)
{
    this->currentNode->children->push_back(node);
};

void SceneGraph::deleteCurrentNode()
{
    std::vector<Node *> *currentChildren = this->currentNode->children;
    toParent();
    for (Node *child : *currentChildren)
    {
        insertNode(child);
    }
};

void SceneGraph::deleteNode(int id)
{
    toRoot();
    for (int i = 0; i < this->currentNode->children->size(); i++)
    {
        if (this->currentNode->children->at(i)->id == id)
        {
            this->currentNode->children->erase(this->currentNode->children->begin() + i);
            return;
        }
    }
};

void SceneGraph::render()
{
    this->rootNode->render();
};