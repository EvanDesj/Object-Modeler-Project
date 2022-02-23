// Include Dependencies
#include "Vector3.h"
#include <cmath>

Vector3::Vector3()
{
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
};

Vector3::Vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
};

// Overload Operators
Vector3 Vector3::operator+(Vector3 &v)
{
    Vector3 result;
    result.x = this->x + v.x;
    result.y = this->y + v.y;
    result.z = this->z + v.z;
    return result;
};

Vector3 Vector3::operator+=(Vector3 &v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
    return *this;
};

Vector3 Vector3::operator-(Vector3 &v)
{
    Vector3 result;
    result.x = this->x - v.x;
    result.y = this->y - v.y;
    result.z = this->z - v.z;
    return result;
};

Vector3 Vector3::operator-=(Vector3 &v)
{
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
    return *this;
};

Vector3 Vector3::operator*(float scalar)
{
    Vector3 result;
    result.x = this->x * scalar;
    result.y = this->y * scalar;
    result.z = this->z * scalar;
    return result;
};

Vector3 Vector3::operator*=(float scalar)
{
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
    return *this;
};

float Vector3::operator*(Vector3 &v)
{
    return (this->x * v.x) + (this->y * v.y) + (this->z * v.z);
};

float Vector3::operator*=(Vector3 &v)
{
    return (this->x * v.x) + (this->y * v.y) + (this->z * v.z);
};

Vector3 Vector3::operator/(float scalar)
{
    Vector3 result;
    result.x = this->x / scalar;
    result.y = this->y / scalar;
    result.z = this->z / scalar;
    return result;
};

Vector3 Vector3::operator/=(float scalar)
{
    this->x /= scalar;
    this->y /= scalar;
    this->z /= scalar;
    return *this;
};

Vector3 Vector3::operator/(Vector3 &v)
{
    Vector3 result;
    result.x = this->x / v.x;
    result.y = this->y / v.y;
    result.z = this->z / v.z;
    return result;
};

Vector3 Vector3::operator/=(Vector3 &v)
{
    this->x /= v.x;
    this->y /= v.y;
    this->z /= v.z;
    return *this;
};

// Vector operations
float Vector3::magnitude()
{
    return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
};

Vector3 Vector3::normalize()
{
    Vector3 result;
    float mag = this->magnitude();
    result.x = this->x / mag;
    result.y = this->y / mag;
    result.z = this->z / mag;
    return result;
};

Vector3 Vector3::cross(Vector3 &v)
{
    Vector3 result;
    result.x = (this->y * v.z) - (this->z * v.y);
    result.y = (this->z * v.x) - (this->x * v.z);
    result.z = (this->x * v.y) - (this->y * v.x);
    return result;
};

float Vector3::distance(Vector3 &v)
{
    return sqrt(pow(this->x - v.x, 2) + pow(this->y - v.y, 2) + pow(this->z - v.z, 2));
};