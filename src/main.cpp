#include <stdio.h>
#include <iostream>
#include <math.h>
#include <list>
#include "main.hpp"
#include "drawing.hpp"
#include "world.hpp"
#include "objects.hpp"
#include "camera.hpp"
#include "gui.hpp"

/*----------------------------------------------------------------------------*/
// ********ToDoMemo********
// back-ground texture
// the ring of Saturn/Uranus
/*----------------------------------------------------------------------------*/

namespace Main
{
    double Time_y;
    double Timestep_y;
    bool IsReverseTime;
    double TimeSpeed;
    double TimeLength;

    /*----------------------------------------------------------------------------*/
    // System-Params
    /*----------------------------------------------------------------------------*/
    int WindowWidth         = 1280;
    int WindowHeight        = 800;
    int RefreshRate         = 30;
    bool IsAbsolutePosition = true;
    bool IsDrawTail         = true;
    bool IsTailAlpha        = true;
    int TailPointSkip       = 2;
    bool IsCapture          = false;
    const char* CaptureDir = "/tmp/Download/frame/frame_%d.bmp";
    const char* WorldFileList[] = { "data/World1",
                                    "data/World2",
                                    "data/World3"};
    // convert real-diameter to draw-diameter
    double ScalePlanetSize(double d){ return pow(d/1000, 0.4); }
    // convert real-radius to draw-radius
    double ScaleRadius(double r2)
    {
        // adjust param : base_r, a
        static double base_r = 100;
        static double a = 0.6; // a <= 1
        if(r2 == 0) r2 = 1; // pow(a, b) don't return valid value if a=0 and b < 0
        return base_r * pow(r2, a-1);
    }

    World *m_world = NULL;

    void TimerFunc();
    void Draw3D();
    void DrawGrid(double lengthX, double lengthY, double lengthZ, 
                  double spaceX,  double spaceY,  double spaceZ);

    string Initialize()
    {
        if(m_world != NULL) delete m_world;

        // load world
        static int world_count = 0;
        string result(WorldFileList[world_count]);
        m_world = new World(WorldFileList[world_count]);

        world_count++;
        int world_size = sizeof(WorldFileList) / sizeof(char*);
        if(world_count == world_size) world_count = 0;

        Time_y = 0;
        Timestep_y = 1;

        TimeLength = m_world->getTimeLength();
        Drawing::CurrentCam = new Camera(m_world->OriginPlanet, Vector3D(-2, 3, -2), 2000);

        return result;
    }

    void Draw3D()
    {
        m_world->Draw(Time_y);

        // DrawGrid(500, 500, 500, 100, 100, 100);
    }

    void TimerFunc()
    {
        if( !IsReverseTime ) Time_y += TimeSpeed * Timestep_y / RefreshRate;
        else                 Time_y -= TimeSpeed * Timestep_y / RefreshRate;
        if(Time_y < 0) Time_y = 0;
        if(Time_y > TimeLength) Time_y = TimeLength;

        Drawing::CurrentCam->Timer(Time_y);
    }

    void DrawGrid(double lengthX, double lengthY, double lengthZ, 
                  double spaceX,  double spaceY,  double spaceZ)
    {
        glDisable(GL_LIGHTING);
        glBegin(GL_LINES);
        // x-y
        glColor3d(0.2, 0.1, 0.1);
        for(double x = -lengthX; x <= lengthX; x += spaceX )
        {
            glVertex3d(x, -lengthY, 0);
            glVertex3d(x, lengthY, 0);
        }
        for(double y = -lengthY; y <= lengthY; y += spaceY )
        {
            glVertex3d(-lengthX, y, 0);
            glVertex3d(lengthX, y, 0);
        }
        // x-z
        glColor3d(0.1, 0.2, 0.1);
        for(double x = -lengthX; x <= lengthX; x += spaceX )
        {
            glVertex3d(x, 0, -lengthZ);
            glVertex3d(x, 0, lengthZ);
        }
        for(double z = -lengthZ; z <= lengthZ; z += spaceZ )
        {
            glVertex3d(-lengthX, 0, z);
            glVertex3d(lengthX, 0, z);
        }
        // y-z
        glColor3d(0.1, 0.1, 0.2);
        for(double y = -lengthY; y <= lengthY; y += spaceY )
        {
            glVertex3d(0, y, -lengthZ);
            glVertex3d(0, y, lengthZ);
        }
        for(double z = -lengthZ; z <= lengthZ; z += spaceZ )
        {
            glVertex3d(0, -lengthY, z);
            glVertex3d(0, lengthY, z);
        }
        glEnd();
        glEnable(GL_LIGHTING);
    }

    // glutMainLoop関数は終わることがないので、main関数の最後にリソース解放処理を置くことができない。
    // なのでグローバルなクラス変数のデストラクタを用いてリソースの解放を行う。
    class Destructor
    {
    public:
        ~Destructor()
        {
            delete Drawing::CurrentCam;
            delete m_world;
        }
    } Dest;
}

/*----------------------------------------------------------------------------*/
// entry point
/*----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    Drawing::CreateDisplay(&argc, argv, NULL, 0, 0, Main::WindowWidth, Main::WindowHeight);

    Drawing::setTimerFunc(Main::TimerFunc);
    Drawing::setDraw3DFunc(Main::Draw3D);
    Drawing::setDraw2DFunc(GUI::Draw2D);
    Drawing::setKeyEventFunc(GUI::KeyEvent);
    Drawing::setMouseEventFunc(GUI::MouseEvent);

    Main::Initialize();

    glutMainLoop();

#ifdef __linux__
    // This code is to avoid the bug of Ubuntu13.10 + NVidia-driver : Bug #1248642.
    // it does not run.
    int i = pthread_getconcurrency();
#endif
    return 0;
}