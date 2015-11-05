#ifndef _INC_GUI
#define _INC_GUI

#include "main.hpp"

namespace GUI
{
    // event function
    extern void Draw2D();
    extern void KeyEvent(bool KeyState[0x100], bool SpKeyState[0x100]);
    extern void MouseEvent(int mx, int my, bool Button[3]);

    // private function
    void DrawText(const char* str, float x, float y, const double* color);
    void DrawRect(int x, int y, int width, int height, const double* color);

    void Seekbar_Draw();
    void Seekbar_Mouse(int mx, int my, bool button[3]);
}

#endif