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
#include "MaterialNode.h"

MaterialNode::MaterialNode(Material m)
{
    this->type = MATERIAL;
    this->material = m;
};

void MaterialNode::pushNode()
{
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, this->material.amb.getRGBA());
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, this->material.diff.getRGBA());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, this->material.spec.getRGBA());
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, this->material.shininess);
    glPopMatrix();
};

void MaterialNode::popNode()
{
};