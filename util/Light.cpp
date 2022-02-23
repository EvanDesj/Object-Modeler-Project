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

#include "Light.h"

Light::Light(int id, Vector3 position, Material material)
{
    this->id = id;
    this->position = position;
    this->material = material;
    this->selected = false;
}

void Light::enable()
{
    if (this->id == 0)
    {
        glEnable(GL_LIGHT0);
    }
    else if (this->id == 1)
    {
        glEnable(GL_LIGHT1);
    }
}

void Light::disable()
{
    if (this->id == 0)
    {
        glDisable(GL_LIGHT0);
    }
    else if (this->id == 1)
    {
        glDisable(GL_LIGHT1);
    }
}

void Light::render()
{
    float position[4] = {this->position.x, this->position.y, this->position.z, 1.0f};
    if (this->id == 0)
    {
        glLightfv(GL_LIGHT0, GL_POSITION, position);
        glLightfv(GL_LIGHT0, GL_AMBIENT,  this->material.amb.getRGBA());
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  this->material.diff.getRGBA());
        glLightfv(GL_LIGHT0, GL_SPECULAR, this->material.spec.getRGBA());
    }
    else if (this->id == 1)
    {
        glLightfv(GL_LIGHT1, GL_POSITION, position);
        glLightfv(GL_LIGHT1, GL_AMBIENT, this->material.amb.getRGBA());
        glLightfv(GL_LIGHT1, GL_DIFFUSE, this->material.diff.getRGBA());
        glLightfv(GL_LIGHT1, GL_SPECULAR, this->material.spec.getRGBA());
    }
    // Render Light Source Sphere (for showroom light effect)
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(this->position.x, this->position.y, this->position.z);
    glutSolidSphere(1.0f, 50, 50);
    if (this->selected) {
        glPushMatrix();
            glDisable(GL_LIGHTING);
            glColor3f(0.0f, 1.0f, 0.0f); // Toggle to green
            glutWireSphere(1.2f, 18, 18); // Draw wireframe around light
        glPopMatrix();
    }
    glEnable(GL_LIGHTING);
    glPopMatrix();
    if (this->id == 0)
    {
        glLightfv(GL_LIGHT0, GL_POSITION, position);
    }
    else if (this->id == 1)
    {
        glLightfv(GL_LIGHT1, GL_POSITION, position);
    }
}