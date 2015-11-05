#ifndef _INC_VECTOR3D
#define _INC_VECTOR3D

#include <math.h>

class Vector3D
{
private:

public:
    double x, y, z;

    Vector3D(double _x = 0, double _y = 0, double _z = 0);
    Vector3D& operator=(const Vector3D& v);
    Vector3D& operator+=(const Vector3D& v);
    Vector3D& operator-=(const Vector3D& v);
    Vector3D& operator*=(double k);
    Vector3D& operator/=(double k);
    bool operator==(const Vector3D& v);
    Vector3D operator+();
    Vector3D operator-();
    static Vector3D CrossProduct(const Vector3D& u, const Vector3D& v);
    inline double Length() const
    {
        return sqrt(x*x + y*y + z*z);
    }
    static Vector3D Normalize(const Vector3D& v);

    static Vector3D Rotate(const Vector3D& v, const Vector3D& axis, double r);
    void Rotate(const Vector3D& axis, double r){ *this = Vector3D::Rotate(*this, axis, r); }

    static const Vector3D Zero;
    static const Vector3D UnitX;
    static const Vector3D UnitY;
    static const Vector3D UnitZ;
};


Vector3D operator+(const Vector3D& u,const Vector3D& v);
Vector3D operator-(const Vector3D& u,const Vector3D& v);

double operator*(const Vector3D& u,const Vector3D& v);

Vector3D operator*(const Vector3D& v, double k);
Vector3D operator*(double k ,const Vector3D& v);
Vector3D operator/(const Vector3D& v, double k);

#endif