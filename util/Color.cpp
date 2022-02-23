// Include Dependencies
#include "Color.h"
#include <algorithm>
#include <cmath>

Color::Color()
{
    this->r = 0.0f;
    this->g = 0.0f;
    this->b = 0.0f;
    this->a = 1.0f;
};

Color::Color(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 1.0f;
};

Color::Color(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
};

float* Color::getRGBA()
{
    float* rgba = new float[4];
    rgba[0] = this->r;
    rgba[1] = this->g;
    rgba[2] = this->b;
    rgba[3] = this->a;
    return rgba;
};

float *Color::getHSVA()
{
    float *hsva;
    float max = std::max({this->r, this->g, this->b}),
          min = std::min({this->r, this->g, this->b}),
          delta = max - min,
          h = 0.0f,
          s = 0.0f,
          v = max;
    if (max != 0.0f)
    {
        s = delta / max;
    }
    if (max == this->r)
    {
        h = (this->g - this->b) / delta;
        if (this->g < this->b)
            h += 6.0f;
    }
    else if (max == this->g)
    {
        h = (this->b - this->r) / delta + 2.0f;
    }
    else if (max == this->b)
    {
        h = (this->r - this->g) / delta + 4.0f;
    }
    h /= 6.0f;
    hsva[0] = h;
    hsva[1] = s;
    hsva[2] = v;
    hsva[3] = this->a;
    return hsva;
};
