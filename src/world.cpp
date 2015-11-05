#include <stdio.h>
#include <math.h>
#include <string.h>
#include <string>
#include <map>
#include <list>
#include "world.hpp"
#include "objects.hpp"
#include "camera.hpp"
#include "vector3d.hpp"

World::World(const char* propertyname)
{
    m_TimeLength = 0;
    m_ReadProperty(propertyname);
}
World::~World()
{
    while(!m_PlanetList.empty())
    {
        PlanetList::iterator it = m_PlanetList.begin();
        delete (*it).second;
        m_PlanetList.erase(it);
    }
}

std::list<string> World::getPlanetNames() const
{
    std::list<string> name_list;

    PlanetList::const_iterator it;
    for(it = m_PlanetList.begin(); it != m_PlanetList.end(); ++it)
    {
        name_list.push_back(it->first);
    }

    return name_list;
}

const Planet* World::getPlanet(string name) const
{
    return m_PlanetList.at(name);
}

void World::Draw(double time_y) const
{
    PlanetList::const_iterator it;
    for(it = m_PlanetList.begin(); it != m_PlanetList.end(); ++it)
    {
        string  name = (*it).first;
        Planet* planet = (*it).second;

        planet->Draw(time_y);
    }

    return;
}

bool World::m_ReadProperty(const char* propertyname)
{
    FILE *file = fopen(propertyname, "r");

    if(file == NULL)
    {
        std::cerr << "World::ReadPlanets : file open error (" << propertyname << ")" << std::endl;
        return false;
    }

    // TODO: char-fileread to string-fileread

    char line[0xFF];
    char c_directory[0xFF];

    fgets(line, 0xFF, file); // skip header
    fscanf(file, "%s\n", c_directory);

    // read planet-properties from each line
    while(fgets(line, 0xFF, file) != NULL)
    {
        char c_name[0xFF], pfile[0xFF], tfile[0xFF];
        float d, length, r, g, b;
        //                    name     diameter lifespan r  g  b        points   texture
        const char* format = "%s%*[\t ]%f%*[\t ]%f%*[\t ]%f,%f,%f%*[\t ]%s%*[\t ]%s";
        if( sscanf(line, format, c_name, &d, &length, &r, &g, &b, pfile, tfile) < 8) break;

        // char* to string
        string name(c_name);
        string point_file = string(c_directory) + string(pfile);
        string texture_file(tfile);

        bool emission = false;
        if(name == "Sun") emission = true;

        Planet *planet = new Planet(*this, d, r, g, b, length, point_file, texture_file, emission);
        if(planet->getTimeLength() > m_TimeLength) m_TimeLength = planet->getTimeLength();
        m_PlanetList[name] = planet;
    }

    // set origin point
    OriginPlanet = m_PlanetList["Sun"];

    return true;
}