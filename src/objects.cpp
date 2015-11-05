#include <math.h>
#include <string.h>
#include <limits.h>
#include "world.hpp"
#include "objects.hpp"
#include "pointreader.hpp"
#include "vector3d.hpp"


Planet::Planet( const World& owner, double d, double r, double g, double b,
                double length, string point_file, string texture_file, bool emission) : m_Owner(owner)
{
    // set parameters
    m_Diameter = d;
    m_TailLength = length;
    m_Color[0] = r;
    m_Color[1] = g;
    m_Color[2] = b;
    m_IsEmission = emission;
    unsigned int count;
    m_PointReader = new PointReader(point_file.c_str(), &m_Interval, &count);
    m_TimeLength = m_Interval * count;
    if(m_TailLength > m_TimeLength) m_TailLength = m_TimeLength;

    // load texture
    int width, height;
    char buf[0xFF];
    sscanf(texture_file.c_str(), "%*[^_]_%dx%d.raw", &width, &height);

    texture_file = "data/texture/" + texture_file;

    m_LoadTexture(texture_file.c_str(), width, height);
}

void Planet::m_LoadTexture(const char* filename, int width, int height)
{
    GLubyte image_data[width][height][4];
    FILE *fp;

    if ((fp = fopen(filename, "rb")) != NULL)
    {
        fread(image_data, sizeof(image_data), 1, fp);
        fclose(fp);
    }
    else
    {
        std::cerr << "Planet::Planet : fileopen failed(" << filename << ")." << std::endl;
        m_TextureID = UINT_MAX;
        return;
    }

    // set texture object
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1 , &m_TextureID);
    glBindTexture(GL_TEXTURE_2D , m_TextureID);
    gluBuild2DMipmaps(  GL_TEXTURE_2D, GL_RGBA, width, height,
                        GL_RGBA, GL_UNSIGNED_BYTE, image_data);
}

Planet::~Planet()
{
    glDeleteTextures(1 , &m_TextureID);
    delete m_PointReader;
}

Vector3D Planet::m_getRealPosition(double time_y) const
{
    // get real-position
    int index;
    if(m_Interval == 0) index = 0;
    else                index = (int)(time_y / m_Interval);
    return (*m_PointReader)[index];
}

Vector3D Planet::getPosition(double time_y) const
{
    // calculate draw-position
    Vector3D pos = this->m_getRealPosition(time_y);
    Vector3D origin = m_Owner.OriginPlanet->m_getRealPosition(time_y);
    pos -= origin;
    double scale_r = Main::ScaleRadius(pos.Length());
    pos *= scale_r;

    if(Main::IsAbsolutePosition)
    {
        double base_r = Main::ScaleRadius(0);
        pos += origin * base_r;
    }

    return pos;
}

/*----------------------------------------------------------------------------*/
// Draw Planet and tail
/*----------------------------------------------------------------------------*/
void Planet::Draw(double time_y)
{
    // draw sphere
    Vector3D planet_pos = getPosition(time_y);

    glPushMatrix();
        glTranslated(planet_pos.x, planet_pos.y, planet_pos.z);
        if(m_IsEmission)
        {
            static GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
            glMaterialfv(GL_FRONT, GL_EMISSION, white);

            GLfloat light_pos[] = { planet_pos.x, planet_pos.y, planet_pos.z, 1.0 };
            glEnable(GL_LIGHT1);
            glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
        }else
        {
            static float black[] = {0, 0, 0, 0};
            glMaterialfv(GL_FRONT, GL_EMISSION, black);
        }
        m_DrawPlanet();
    glPopMatrix();

    // draw tail
    if(Main::IsDrawTail) m_DrawTail(time_y);
}

void Planet::m_DrawPlanet()
{
    static GLUquadricObj* sphere = NULL;
    // initialize sphere-obj
    if(sphere == NULL)
    {
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_FILL);
        gluQuadricNormals(sphere, GLU_SMOOTH);
        gluQuadricTexture(sphere, GL_TRUE);
    }

    // draw sphere
    double draw_diameter = Main::ScalePlanetSize(m_Diameter);
    if(m_TextureID == UINT_MAX)
    {
        // if texture is null
        glEnable(GL_COLOR_MATERIAL);
        glColor3d(m_Color[0], m_Color[1], m_Color[2]);
        gluSphere(sphere, draw_diameter, 32, 32);
        glRotated(180, 1, 0, 0);
        glDisable(GL_COLOR_MATERIAL);
    }else
    {
        // if texture is set
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D , m_TextureID);
        gluSphere(sphere, draw_diameter, 32, 32);
        glRotated(180, 1, 0, 0);
        glDisable(GL_TEXTURE_2D);
    }
}

void Planet::m_DrawTail(double time_y)
{
    glDisable(GL_LIGHTING);
    glBegin(GL_LINE_STRIP);
    for(double t = time_y - m_TailLength;
        t < time_y;
        t += m_Interval * Main::TailPointSkip)
    {
        // determine color(alpha)
        double alpha;
        if(Main::IsTailAlpha) alpha = 1 - (time_y - t)/m_TailLength;
        else                  alpha = 1;
        glColor3d(alpha*m_Color[0], alpha*m_Color[1], alpha*m_Color[2]);

        // determine position
        Vector3D tail_pos = getPosition(t);
        if(tail_pos == Vector3D::Zero) continue;
        glVertex3f(tail_pos.x, tail_pos.y, tail_pos.z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}
