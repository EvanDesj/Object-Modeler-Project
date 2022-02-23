#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include "../nodes/Node.h"

class SceneGraph
{
public:
    Node *rootNode;
    Node *currentNode;

    SceneGraph();

    void toRoot();
    void toParent();
    void toChild(int i);
    void toChildID(int id);
    int getCurrentNodeID();
    void insertNode(Node *node);
    void deleteCurrentNode();
    void deleteNode(int id);
    void render();
};

#endif