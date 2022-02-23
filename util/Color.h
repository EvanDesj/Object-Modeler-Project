#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
    float r, g, b, a;
    Color();
    Color(float r, float g, float b);
    Color(float r, float g, float b, float a);
    float* getRGBA();
    float* getHSVA();
};

#endif