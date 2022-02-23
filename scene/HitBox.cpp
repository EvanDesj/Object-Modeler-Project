// Include Dependencies
#include "HitBox.h"
#include "../math/Vector3.h"
#include <cmath>

HitBox::HitBox()
{
    this->position = Vector3();
    this->magnitude = 1;
};

HitBox::HitBox(Vector3 pos, float m)
{
    this->position = pos;
    this->magnitude = m;
};