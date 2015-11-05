#include "objects.hpp"
#include "camera.hpp"
#include "vector3d.hpp"

Camera::Camera(const Planet* target, const Vector3D& direction, double distance)
    : TargetPlanet(target)
{
    Direction = direction / direction.Length();
    Distance = distance;

    m_fovy = 30;
    m_zNear = 1;
    m_zFar = Distance + 50000;
}

void Camera::Rotate(const Vector3D& axis, double r)
{
    Direction.Rotate(axis, r);
}

void Camera::Timer(double time_y)
{
    glNewList(CAMERA_DISPLAY_NUM, GL_COMPILE);

    Vector3D lookat = TargetPlanet->getPosition(time_y);
    Vector3D pos = lookat - Direction*Distance;
    gluLookAt(pos.x, pos.y, pos.z, lookat.x, lookat.y, lookat.z, 0, 0, 1);

    glEndList();
}

void Camera::setPerspective()
{
    double aspect_rate = (double)Main::WindowWidth / (double)Main::WindowHeight;
    gluPerspective(m_fovy, aspect_rate, m_zNear, m_zFar);
}

void Camera::setLookat()
{
    glCallList(CAMERA_DISPLAY_NUM);
}