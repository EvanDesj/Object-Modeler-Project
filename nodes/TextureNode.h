#ifndef TEXTURE_NODE_H
#define TEXTURE_NODE_H
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif

#include "Node.h"
#include "../math/Vector3.h"
#include "../util/Color.h"

class TextureNode : public Node
{
public:
    GLuint texture;
    NodeType textureType;
    bool applyTexture;
    TextureNode(GLuint texture);
    TextureNode(GLuint texture, NodeType ttype);
    virtual void pushNode();
    virtual void popNode();
};

#endif