#ifndef _INC_ObjectBase
#define _INC_ObjectBase

#include "main.hpp"
#include "vector3d.hpp"
#include <limits.h>

class PointReader;
class World;

class Planet
{
private:
    const World& m_Owner;

    char   m_Name[0xFF];
    double m_Diameter;
    double m_Color[3];
    double m_TailLength;
    double m_Interval;
    double m_TimeLength;
    bool   m_IsEmission;
    PointReader *m_PointReader;

    unsigned int m_TextureID;

    void m_LoadTexture(const char* filename, int width, int height);
    void m_DrawPlanet();
    void m_DrawTail(double time_y);
    Vector3D m_getRealPosition(double time_y) const;

public:
    Planet( const World& owner, double d, double r, double g, double b,
            double length, string point_file, string texture_file, bool emission);
    ~Planet();

    // properties getter
    const char* getName() const { return m_Name; }
    const double getTimeLength() const { return m_TimeLength; }
    Vector3D getPosition(double time_y) const;

    void Draw(double time_y);
};

#endif
