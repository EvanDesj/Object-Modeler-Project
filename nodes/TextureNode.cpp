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
#include "TextureNode.h"

TextureNode::TextureNode(GLuint texture)
{
    this->type = TEXTURE;
    this->texture = texture;
    this->textureType = CUBE;
    this->applyTexture = false;
};

TextureNode::TextureNode(GLuint texture, NodeType texturetype)
{
    this->type = TEXTURE;
    this->texture = texture;
    this->textureType = texturetype;
    this->applyTexture = false;
};

void TextureNode::pushNode()
{
    if (this->applyTexture)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, this->texture);

        if (this->textureType == TORUS)
        {
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
        }
        glBindTexture(GL_TEXTURE_2D, this->texture);
        glEnable(GL_TEXTURE_2D);
    }
};

void TextureNode::popNode()
{
    if (this->applyTexture)
    {
        if (this->textureType == TORUS)
        {
            glDisable(GL_TEXTURE_GEN_S);
            glDisable(GL_TEXTURE_GEN_T);
        }
        glDisable(GL_TEXTURE_2D);
    }
};