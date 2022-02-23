#ifndef MATERIAL_H
#define MATERIAL_H

#include "Color.h"
#include "../math/Vector3.h"

enum Materials
{
    CHROME,
    GOLD,
    TIN,
    PERL,
    RUBBER,
    DEFAULT
};

class Material
{
public:
    Color amb,
        diff,
        spec;
    float shininess;

    Material();
    Material(Color amb, Color diff, Color spec, float shininess);

    void setMaterial(Materials m);
    void render();
};

#endif