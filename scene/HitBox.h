#ifndef HITBOX_H
#define HITBOX_H

#include "../math/Vector3.h"

class HitBox
{
public:
    Vector3 position;
    float magnitude;
    HitBox();
    HitBox(Vector3 pos, float m);
};

#endif