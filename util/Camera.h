#ifndef CAMERA_H
#define CAMERA_H

#include "../math/Vector3.h"

class Camera
{
public:
    Vector3 position,
        up,
        lookAt;
    Camera(Vector3 position, Vector3 up, Vector3 look);
    void render();
};

#endif