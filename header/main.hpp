#ifndef _INC_MAIN
#define _INC_MAIN

// include GLUT-header
#ifdef __linux__
    #include <pthread.h>
    #include <GL/glut.h>
#endif
#ifdef __APPLE__
    #include <GLUT/glut.h>
#endif

#include <iostream>
#include <string>
using std::string;

#define test(x) std::cout << x << std::endl

namespace Main
{
    extern double Time_y;       // current time [year]
    extern double TimeLength;
    extern double Timestep_y;   // time step [year/sec]
    extern double TimeSpeed;
    extern bool IsReverseTime;

    extern int WindowWidth;
    extern int WindowHeight;
    extern int RefreshRate;
    extern bool IsAbsolutePosition;
    extern bool IsDrawTail;
    extern int TailPointSkip;
    extern bool IsTailAlpha;
    extern double ScalePlanetSize(double d);
    extern double ScaleRadius(double r);
    extern bool IsCapture;
    extern const char* CaptureDir;

    extern string Initialize();
}

#endif