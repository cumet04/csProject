#ifndef _INC_CAMERA
#define _INC_CAMERA

#include "main.hpp"
#include "vector3d.hpp"

#define CAMERA_DISPLAY_NUM 2

class Planet;

class Camera
{
private:
    double m_fovy;
    double m_zNear;
    double m_zFar;

public:
    const Planet* TargetPlanet;
    Vector3D Direction;
    double Distance;

    Camera(const Planet* target, const Vector3D& direction, double distance);

    void setPerspective();
    void Timer(double time_y);
    void Rotate(const Vector3D& axis, double r);
    void setLookat();
};

#endif