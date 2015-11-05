#include <stdio.h>
#include <iostream>
#include "drawing.hpp"
#include "camera.hpp"
#include "gui.hpp"
#include "vector3d.hpp"

namespace GUI
{
    // color-preset
    const double color_white[]     = {1.0, 1.0, 1.0};
    const double color_lightgray[] = {0.7, 0.7, 0.7};
    const double color_gray[]      = {0.5, 0.5, 0.5};
    const double color_darkgray[]  = {0.2, 0.2, 0.2};

    /*----------------------------------------------------------------------------*/
    // Seekbar
    /*----------------------------------------------------------------------------*/


    bool IsSeek = false;
    void Seekbar_Draw()
    {
        const int bar_width = Main::WindowWidth - 40;
        double time_rate = Main::Time_y / Main::TimeLength;

        DrawRect(15 + time_rate*bar_width, 10, 10, 30, color_lightgray);
        DrawRect(20, 20, bar_width, 10, color_darkgray);
    }
    void Seekbar_Mouse(int mx, int my, bool button[3])
    {
        const int bar_width = Main::WindowWidth - 40;

        if(button[0] == false)
        {
            IsSeek = false;
            return;
        }

        if(IsSeek)
        {
            double time_rate = (double)(mx - 20) / bar_width;
            if(time_rate < 0) time_rate = 0;
            if(time_rate > 1) time_rate = 1;
            Main::Time_y = time_rate * Main::TimeLength;
        }else
        {
            if(mx < 20 || mx > bar_width + 20 || my < 10 || my > 40) return;
            else IsSeek = true;
        }
    }

    /*----------------------------------------------------------------------------*/
    // KeyEvent
    /*----------------------------------------------------------------------------*/
    void Event_ChangeWorld(bool Key);

    bool IsRotate = false;
    void KeyEvent(bool KeyState[0x100], bool SpKeyState[0x100])
    {
        using namespace Main;

        // time skip
        IsReverseTime = false;
        TimeSpeed = 1;
        if(KeyState['p']) TimeSpeed = 0;
        if(KeyState['1']) TimeSpeed = 50;
        if(KeyState['2']) TimeSpeed = 100;
        if(KeyState['3']) TimeSpeed = 500;
        if(KeyState['4']) { TimeSpeed =  50; IsReverseTime = true; }
        if(KeyState['5']) { TimeSpeed =  100; IsReverseTime = true; }
        if(KeyState['6']) { TimeSpeed =  500; IsReverseTime = true; }

        if(KeyState['r']) IsRotate = true;
        if(KeyState['r'] == false) IsRotate = false;

        // change world
        Event_ChangeWorld(KeyState['n']);
    }
    
    string log_changeworld;
    void Event_ChangeWorld(bool Key)
    {
        static const int interval_s = 2;
        static int count = 0;

        if(count > 0)
        {
            --count;
            return;
        }else
        {
            log_changeworld = "";
        }

        if(Key)
        {
            log_changeworld = "Load World : ";
            log_changeworld += Main::Initialize();
            count = interval_s * Main::RefreshRate;
        }
    }

    /*----------------------------------------------------------------------------*/
    // MouseEvent
    /*----------------------------------------------------------------------------*/
    void MouseEvent(int mx, int my, bool button[3])
    {
        Seekbar_Mouse(mx, my, button);
        if(IsSeek) return;

        Camera& camera = *Drawing::CurrentCam;

        // rotate Camera
        static int beforeX = 0;
        static int beforeY = 0;
        static Vector3D beforeDir;
        if( IsRotate == false )
        {
            beforeX = mx;
            beforeY = my;
            beforeDir = camera.Direction;
        }else
        {
            double rx = 0.01*(mx - beforeX);
            double ry = 0.01*(my - beforeY);

            Vector3D axis = Vector3D::CrossProduct(beforeDir, Vector3D::UnitZ);
            camera.Direction = Vector3D::Rotate(beforeDir, axis, -ry);
            camera.Direction.Rotate(Vector3D::UnitZ, rx);
        }

        // zoom-in/out
        if(button[0]) camera.Distance *= 0.97;
        if(button[2]) camera.Distance *= 1.03;
    }
    
    /*----------------------------------------------------------------------------*/
    // Draw
    /*----------------------------------------------------------------------------*/
    void Draw2D()
    {
        glDisable(GL_LIGHTING);

        glPushMatrix();
            // static int i = 0;
            // glTranslatef(++i, 0, 0);
            char buf[0xFF];
            setlocale(LC_NUMERIC,"ja_JP.utf8");
            sprintf(buf, "current time : %'.2f[year] / %'.2f[year]", Main::Time_y, Main::TimeLength);
            DrawText(buf, 10, 50, color_white);
        glPopMatrix();

        if(log_changeworld.length() != 0)
        {
            DrawText(log_changeworld.c_str(), 10, Main::WindowHeight - 20, color_white);
        }

        Seekbar_Draw();

        glEnable(GL_LIGHTING);
    }


    void DrawRect(int x, int y, int width, int height, const double* color)
    {
        glColor3dv(color);
        glBegin(GL_QUADS);
            glVertex2d(x        , y         );
            glVertex2d(x + width, y         );
            glVertex2d(x + width, y + height);
            glVertex2d(x        , y + height);
        glEnd();
    }

    void DrawText(const char* str, float x, float y, const double* color)
    {
        glColor3dv(color);
        glRasterPos2f(x, y);
        for(const char *pos = str; *pos != 0; pos++)
        {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *pos);
        }
    }
}
