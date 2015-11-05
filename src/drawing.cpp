#include <stdio.h>
#include <iostream>
#include "main.hpp"
#include "camera.hpp"
#include "drawing.hpp"

using namespace std;

namespace Drawing
{
    /*----------------------------------------------------------------------------*/
    // parameters
    /*----------------------------------------------------------------------------*/
    Camera* CurrentCam;
    void (*m_timer_func)();
    void (*m_draw3d_func)();
    void (*m_draw2d_func)();
    void (*m_KeyEventFunc)(bool KeyState[0x100], bool SpKeyState[0x100]);
    void (*m_MouseEventFunc)(int mx, int my, bool Button[3]);

    /*----------------------------------------------------------------------------*/
    // input status setting
    /*----------------------------------------------------------------------------*/

    // keyboard event
    bool m_KeyState[0x100];     // 0x100 = max value of unsigned char
    bool m_SpkeyState[0x100];   // TODO: check whether SpKeyCode is less than 0x100

    void event_keydown(unsigned char key, int x, int y){ m_KeyState[key] = true; }
    void event_keyup(unsigned char key, int x, int y){	m_KeyState[key] = false; }
    void event_spkeydown(int key, int x, int y){ m_SpkeyState[key & 0xFF] = true; }
    void event_spkeyup(int key, int x, int y){ m_SpkeyState[key & 0xFF] = false; }

    // mouse event
    int m_MouseX;
    int m_MouseY;
    bool m_MouseState[3];

    void event_mousemove(int x, int y){ m_MouseX = x; m_MouseY = y; }
    void event_mousebutton(int button, int state, int x, int y){ m_MouseState[button] = (state == GLUT_DOWN); }

    /*----------------------------------------------------------------------------*/
    // GLUT callback event
    /*----------------------------------------------------------------------------*/

    void event_display(void)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw 3D
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glNewList(1, GL_COMPILE);
        (*m_draw3d_func)();
        glEndList();
        CurrentCam->setLookat();
        glCallList(1);

        // draw 2D
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
            glLoadIdentity();
            glOrtho(0, Main::WindowWidth, 0, Main::WindowHeight, 0, 1);

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
                glLoadIdentity();
                (*m_draw2d_func)();
            glPopMatrix();
            glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glutSwapBuffers();
    }

    void event_resize(int w, int h)
    {
        Main::WindowWidth = w;
        Main::WindowHeight = h;
        glViewport(0, 0, Main::WindowWidth, Main::WindowHeight);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        CurrentCam->setPerspective();
    }

    void OutputCurrentFrame();
    void event_timer(int value)
    {
        if(value != 1) return;

        glutTimerFunc(1000/Main::RefreshRate, event_timer, 1);

        // run event
        (*m_KeyEventFunc)(m_KeyState, m_SpkeyState);
        (*m_MouseEventFunc)(m_MouseX, Main::WindowHeight - m_MouseY, m_MouseState);
        (*m_timer_func)();

        // draw
        glutPostRedisplay();

        // frame output
        if(Main::IsCapture) OutputCurrentFrame();
    }

    void event_idle(void)
    {
    	if(m_KeyState['q']) exit(0);
    }

    /*----------------------------------------------------------------------------*/
    // parameter setter/getter
    /*----------------------------------------------------------------------------*/
    void setTimerFunc(void (*func)()){ m_timer_func = func; }
    void setDraw3DFunc(void (*func)()){ m_draw3d_func = func; }
    void setDraw2DFunc(void (*func)()){ m_draw2d_func = func; }
    void setKeyEventFunc(void (*func)(bool*, bool*)){ m_KeyEventFunc = func; }
    void setMouseEventFunc(void (*func)(int, int, bool*)){ m_MouseEventFunc = func; }

    /*----------------------------------------------------------------------------*/
    // constructor
    /*----------------------------------------------------------------------------*/
    void CreateDisplay(int *argcp, char **argv, char *title, int px, int py, int width, int height)
    {
        // initialize
        glutInit(argcp, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
        glutInitWindowPosition(px, py);
        glutInitWindowSize(width, height);
        glutCreateWindow(title);
        
        // set event handler
        glutDisplayFunc(event_display);
        glutReshapeFunc(event_resize);
        glutIdleFunc(event_idle);
        glutTimerFunc(1000/Main::RefreshRate, event_timer, 1);

        glutMouseFunc(event_mousebutton);
        glutMotionFunc(event_mousemove);
        glutPassiveMotionFunc(event_mousemove);
        glutKeyboardFunc(event_keydown);
        glutKeyboardUpFunc(event_keyup);
        glutSpecialFunc(event_spkeydown);
        glutSpecialUpFunc(event_spkeyup);

        // set attribute
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_LIGHTING);

        glClearColor(0.0, 0.0, 0.0, 1.0);
    }

    /*----------------------------------------------------------------------------*/
    // frame output
    /*----------------------------------------------------------------------------*/
    struct tagBITMAPFILEHEADER {
        unsigned short bfType;
        unsigned long  bfSize;
        unsigned short bfReserved1;
        unsigned short bfReserved2;
        unsigned long  bfOffBits;
    } bmp_fileheader;

    struct tagBITMAPCOREHEADER{
        unsigned long  bcSize;
        short          bcWidth;
        short          bcHeight;
        unsigned short bcPlanes;
        unsigned short bcBitCount;
    } bmp_coreheader;

    void OutputCurrentFrame()
    {
        static short width = Main::WindowWidth;
        static short height = Main::WindowHeight;
        static int count = 0;

        unsigned char buf[width*height*3];

        char filename[0xFF];
        sprintf(filename, Main::CaptureDir, count);
        FILE *fp = fopen(filename, "wb");

        // write header
        bmp_fileheader.bfType      = 0x4D42;
        bmp_fileheader.bfSize      = 26 + sizeof(buf);
        bmp_fileheader.bfReserved1 = 0;
        bmp_fileheader.bfReserved2 = 0;
        bmp_fileheader.bfOffBits   = 26;
        fwrite(&bmp_fileheader.bfType,      2, 1, fp);
        fwrite(&bmp_fileheader.bfSize,      4, 1, fp);
        fwrite(&bmp_fileheader.bfReserved1, 2, 1, fp);
        fwrite(&bmp_fileheader.bfReserved2, 2, 1, fp);
        fwrite(&bmp_fileheader.bfOffBits,   4, 1, fp);

        bmp_coreheader.bcSize     = 12;
        bmp_coreheader.bcWidth    = width;
        bmp_coreheader.bcHeight   = height;
        bmp_coreheader.bcPlanes   = 1;
        bmp_coreheader.bcBitCount = 24;
        fwrite(&bmp_coreheader.bcSize,     4, 1, fp);
        fwrite(&bmp_coreheader.bcWidth,    2, 1, fp);
        fwrite(&bmp_coreheader.bcHeight,   2, 1, fp);
        fwrite(&bmp_coreheader.bcPlanes,   2, 1, fp);
        fwrite(&bmp_coreheader.bcBitCount, 2, 1, fp);

        // write body
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buf);
        fwrite(buf, sizeof(unsigned char), sizeof(buf), fp);

        fclose(fp);

        count++;
    }
}