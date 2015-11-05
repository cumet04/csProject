#ifndef _INC_WORLD
#define _INC_WORLD

#include <map>
#include <list>
#include "main.hpp"
#include "vector3d.hpp"

class Planet;
typedef std::map<string, Planet*> PlanetList;

class World
{
private:
    double m_TimeLength;
    PlanetList m_PlanetList;

    bool m_ReadProperty(const char* propertyname);

public:
    const Planet* OriginPlanet;

    World(const char* propertyname);
    ~World();

    const double getTimeLength() const { return m_TimeLength; }
    std::list<string> getPlanetNames() const;
    const Planet* getPlanet(string name) const;
    void Draw(double time_y) const;
};

#endif