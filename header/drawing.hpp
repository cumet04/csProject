#ifndef _INC_DISPLAY
#define _INC_DISPLAY

class Camera;

namespace Drawing
{
	// parameter setter/getter
	extern int RefreshRate;
    extern Camera* CurrentCam;
	extern void setTimerFunc( void (*func)() );
    extern void setDraw3DFunc( void (*func)() );
    extern void setDraw2DFunc( void (*func)() );
    extern void setKeyEventFunc( void (*func)(bool*, bool*) );
    extern void setMouseEventFunc( void (*func)(int, int, bool*) );
	// extern void setCurrentCamera(Camera *cam);

	// constructor
	extern void CreateDisplay(int *argcp, char **argv, char *title, int px, int py, int width, int height);

}

#endif