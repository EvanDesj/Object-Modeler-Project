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

#include "Material.h"

#include "Color.h"
#include "../math/Vector3.h"

Material::Material()
{
    this->amb = Color(0.2f, 0.2f, 0.2f);
    this->diff = Color(0.8f, 0.8f, 0.8f);
    this->spec = Color(1.0f, 1.0f, 1.0f);
    this->shininess = 32.0f;
};

Material::Material(Color amb, Color diff, Color spec, float shininess)
{
    this->amb = amb;
    this->diff = diff;
    this->spec = spec;
    this->shininess = shininess;
};

void Material::setMaterial(Materials m)
{
    switch (m)
    {
    case CHROME:
        this->amb = Color(0.25f, 0.25f, 0.25f);
        this->diff = Color(0.4f, 0.4f, 0.4f);
        this->spec = Color(0.774597f, 0.774597f, 0.774597f);
        this->shininess = 76.8f;
        break;
    case RUBBER:
        this->amb = Color(0.05f, 0.05f, 0.05f);
        this->diff = Color(0.5f, 0.5f, 0.5f);
        this->spec = Color(0.7f, 0.7f, 0.7f);
        this->shininess = 10.0f;
        break;
    case GOLD:
        this->amb = Color(0.24725f, 0.1995f, 0.0745f);
        this->diff = Color(0.75164f, 0.60648f, 0.22648f);
        this->spec = Color(0.628281f, 0.555802f, 0.366065f);
        this->shininess = 51.2f;
        break;
    case TIN:
        this->amb = Color(0.105882f, 0.058824f, 0.113725f);
        this->diff = Color(0.427451f, 0.470588f, 0.541176f);
        this->spec = Color(0.3333f, 0.3333f, 0.521569f);
        this->shininess = 9.84615f;
        break;
    case PERL:
        this->amb = Color(0.25f, 0.20725f, 0.20725f);
        this->diff = Color(1.0f, 0.829f, 0.829f);
        this->spec = Color(0.296648f, 0.296648f, 0.296648f);
        this->shininess = 11.264f;
        break;
    case DEFAULT:
    default:
        this->amb = Color(0.2f, 0.2f, 0.2f);
        this->diff = Color(0.8f, 0.8f, 0.8f);
        this->spec = Color(1.0f, 1.0f, 1.0f);
        this->shininess = 32.0f;
        break;
    }
};

void Material::render()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, this->amb.getRGBA());
    glMaterialfv(GL_FRONT, GL_DIFFUSE, this->diff.getRGBA());
    glMaterialfv(GL_FRONT, GL_SPECULAR, this->spec.getRGBA());
    glMaterialf(GL_FRONT, GL_SHININESS, this->shininess);
};