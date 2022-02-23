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

#include "Camera.h"

Camera::Camera(Vector3 position, Vector3 up, Vector3 lookAt)
{
    this->position = position;
    this->up = up;
    this->lookAt = lookAt;
}

void Camera::render()
{
    gluLookAt(
        this->position.x, this->position.y, this->position.y,
        this->lookAt.x, this->lookAt.y, this->lookAt.z,
        this->up.x, this->up.y, this->up.y);
}