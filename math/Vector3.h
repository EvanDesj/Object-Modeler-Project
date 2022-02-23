#ifndef VECTOR_3_H
#define VECTOR_3_H

class Vector3 {
public:
    float x, y, z;
    Vector3();
    Vector3(float x, float y, float z);

    // Overload Operators
    Vector3 operator+(Vector3& v);
    Vector3 operator+=(Vector3& v);
    Vector3 operator-(Vector3 &v);
    Vector3 operator-=(Vector3 &v);
    Vector3 operator*(float scalar);
    Vector3 operator*=(float scalar);
    float operator*(Vector3 &v);
    float operator*=(Vector3 &v);
    Vector3 operator/(float scalar);
    Vector3 operator/=(float scalar);
    Vector3 operator/(Vector3 &v);
    Vector3 operator/=(Vector3 &v);

    // Vector operations
    float magnitude();
    Vector3 normalize();
    Vector3 getNormal(Vector3 v1, Vector3 v2);
    Vector3 cross(Vector3 &v);
    float distance(Vector3 &v);
};

#endif
