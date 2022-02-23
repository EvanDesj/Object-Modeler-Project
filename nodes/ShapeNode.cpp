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
#include "ShapeNode.h"

ShapeNode::ShapeNode(NodeType type)
{
    Shape s;
    this->type = type;
    this->selected = false;
    this->isDrawable = true;
    this->sphere = NULL;
};

void ShapeNode::pushNode()
{
    switch (this->type)
    {
    case CUBE:
        glColor3f(1.0f, 0.0f, 1.0f); // Set Default Color
        if (this->selected)
        {
            glPushMatrix();
                glDisable(GL_LIGHTING);
                glColor3f(0.0f, 1.0f, 0.0f); // Toggle to green
                glutWireCube(2.0f); // Draw wireframe around cube
            glPopMatrix();
        }
        glEnable(GL_LIGHTING);
        s.glutSolidCube2(1.0f);
        break;
    case SPHERE:
        this->sphere = gluNewQuadric();
        gluQuadricDrawStyle(this->sphere, GLU_FILL);
        gluQuadricTexture(this->sphere, GL_TRUE);
        gluQuadricNormals(this->sphere, GLU_SMOOTH);
        glColor3f(1.0f, 0.0f, 1.0f); // Set Default Color
        if (this->selected)
        {
            glPushMatrix();
            glDisable(GL_LIGHTING);
            glColor3f(0.0f, 1.0f, 0.0f); // Toggle to green
            glutWireCube(2.0f);          // Draw wireframe around cube
            glPopMatrix();
        }
        glEnable(GL_LIGHTING);
        gluSphere(this->sphere, 1.0f, 20, 20);
        break;
    case TEAPOT:
        glColor3f(1.0f, 0.0f, 1.0f); // Set Default Color
        if (this->selected)
        {
            glPushMatrix();
            glDisable(GL_LIGHTING);
            glColor3f(0.0f, 1.0f, 0.0f);
            glutWireCube(2.0f);
            glPopMatrix();
        }
        glEnable(GL_LIGHTING);
        glutSolidTeapot(1.0f);
        break;
    case TORUS:
        glColor3f(1.0f, 0.0f, 1.0f); // Set Default Color
        if (this->selected)
        {
            glPushMatrix();
            glDisable(GL_LIGHTING);
            glColor3f(0.0f, 1.0f, 0.0f);
            glutWireCube(2.0f);
            glPopMatrix();
        }
        glEnable(GL_LIGHTING);
        glutSolidTorus(0.3f, 0.6f, 100, 100);
        break;
    case TETRAHEDRON:
        glColor3f(1.0f, 0.0f, 1.0f); // Set Default Color
        if (this->selected)
        {
            glPushMatrix();
            glDisable(GL_LIGHTING);
            glColor3f(0.0f, 1.0f, 0.0f);
            glutWireCube(2.0f);
            glPopMatrix();
        }
        glEnable(GL_LIGHTING);
        s.glutSolidTetrahedron2();
        break;
    }
};

void ShapeNode::popNode()
{
};