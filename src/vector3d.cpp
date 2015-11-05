#include <math.h>
#include "vector3d.hpp"

const Vector3D Vector3D::Zero  = Vector3D(0, 0, 0);
const Vector3D Vector3D::UnitX = Vector3D(1, 0, 0);
const Vector3D Vector3D::UnitY = Vector3D(0, 1, 0);
const Vector3D Vector3D::UnitZ = Vector3D(0, 0, 1);

Vector3D::Vector3D(double _x, double _y, double _z)
{
    x = _x;
    y = _y;
    z = _z;
}

Vector3D& Vector3D::operator=(const Vector3D& v)
{
    this->x=v.x;
    this->y=v.y;
    this->z=v.z;
    return *this;
}

Vector3D& Vector3D::operator+=(const Vector3D& v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
    return *this;       
}
Vector3D& Vector3D::operator-=(const Vector3D& v)
{
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
    return *this;       
}
Vector3D& Vector3D::operator*=(double k)
{
    this->x *= k;
    this->y *= k;
    this->z *= k;
    return *this;   
}
Vector3D& Vector3D::operator/=(double k)
{
    this->x /= k;
    this->y /= k;
    this->z /= k;
    return *this;   
}

bool Vector3D::operator==(const Vector3D& v)
{
    return this->x == v.x && this->y == v.y && this->z == v.z;
}


Vector3D Vector3D::operator+()
{
    return *this;
}
Vector3D Vector3D::operator-()
{
    return Vector3D(-x, -y, -z);
}

//二項演算子
Vector3D operator+(const Vector3D& u,const Vector3D& v)
{
    return Vector3D(u.x + v.x, u.y + v.y, u.z + v.z);
}
Vector3D operator-(const Vector3D& u,const Vector3D& v)
{
    return Vector3D(u.x - v.x, u.y - v.y, u.z - v.z);
}
double operator*(const Vector3D& u,const Vector3D& v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}
Vector3D operator*(const Vector3D& v, double k)
{
    return Vector3D(v.x*k, v.y*k, v.z*k);
}
Vector3D operator*(double k ,const Vector3D& v)
{
    return Vector3D(v.x*k, v.y*k, v.z*k);
}
Vector3D operator/(const Vector3D& v, double k)
{
    return Vector3D(v.x/k, v.y/k, v.z/k);
}

Vector3D Vector3D::Normalize(const Vector3D& v)
{
    return v / v.Length();
}

Vector3D Vector3D::CrossProduct(const Vector3D& u, const Vector3D& v)
{
    double x = u.y*v.z - u.z*v.y;
    double y = u.z*v.x - u.x*v.z;
    double z = u.x*v.y - u.y*v.x;
    return Vector3D(x, y, z);
}

// Rotate
// qv, rvのインスタンス作成は重い？
Vector3D Vector3D::Rotate(const Vector3D& v, const Vector3D& axis, double r)
{
    // normalize
    Vector3D pv = Normalize(v);
    Vector3D qv = Normalize(axis) * sin(r/2);
    Vector3D rv = -qv;

    double pt = 0;
    double qt = cos(r/2);
    double rt = cos(r/2);

    double wt = rt*pt - rv*pv;
    Vector3D wv = rt*pv + pt*rv + Vector3D::CrossProduct(rv, pv);
    double st = wt*qt - wv*qv;
    Vector3D sv = wt*qv + qt*wv + Vector3D::CrossProduct(wv, qv);

    return sv * v.Length();
}