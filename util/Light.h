#ifndef LIGHT_H
#define LIGHT_H

#include "../math/Vector3.h"
#include "../util/Material.h"

class Light
{
public:
    Vector3 position;
    Material material;
    int id;
    bool selected;
    Light(int id, Vector3 position, Material material);
    void enable();
    void disable();
    void render();
};

#endif