#ifndef OLCPP_WIN32_WINDOWS_H
#define OLCPP_WIN32_WINDOWS_H

#include <cstdio>

#include "olcpp/base.h"

typedef FILE *HANDLE;
#define STD_INPUT_HANDLE stdin
#define STD_OUTPUT_HANDLE stdout
#define STD_ERROR_HANDLE stderr
typedef struct
{
    short X, Y;
} COORD;
typedef struct
{
    int dwSize;
    bool bVisible;
} CONSOLE_CURSOR_INFO;

/** 实现在某些情况不等效 */
static inline void Sleep(unsigned long ms)
{
    olcpp::sleep(ms);
}
/** 实现在某些情况不等效 */
static inline HANDLE GetStdHandle(FILE *stdhandle)
{
    return stdhandle;
}
/** 实现不完全 */
static inline void SetConsoleTitle(const char *title)
{
    olcpp::title(title);
}
/** Impls... */
namespace SetConsoleTextAttribute_Impl
{
static inline void color_trans(int &a)
{
    switch (a)
    {
    case 1:
        a = 4;
        break;
    case 3:
        a = 6;
        break;
    case 4:
        a = 1;
        break;
    case 6:
        a = 3;
        break;
    case 8 + 1:
        a = 8 + 4;
        break;
    case 8 + 3:
        a = 8 + 6;
        break;
    case 8 + 4:
        a = 8 + 1;
        break;
    case 8 + 6:
        a = 8 + 3;
        break;
    }
}
} // namespace SetConsoleTextAttribute_Impl
/** 实现不完全 */
static inline void SetConsoleTextAttribute(HANDLE stdhandle, int attr)
{
    int back = attr / 16, forg = attr % 16;
    SetConsoleTextAttribute_Impl::color_trans(back);
    SetConsoleTextAttribute_Impl::color_trans(forg);
    if (back < 8)
        back += 40;
    else
        back += 100 - 8;
    if (forg < 8)
        forg += 30;
    else
        forg += 90 - 8;
    fprintf(stdhandle, "\e[%d;%dm", back, forg);
}
/** Impls... */
extern "C"
{
    extern int _Impl_GetKeyState(int keyCode);
}
/** 实现在部分情况下不等效 */
static inline int GetAsyncKeyState(int keyCode)
{
    return _Impl_GetKeyState(keyCode) ? 0x8000 : 0;
}
/** 实现不完全 */
static inline void SetConsoleCursorPosition(HANDLE stdhandle, COORD coord)
{
    fprintf(stdhandle, "\e[%d;%dH", coord.Y + 1, coord.X + 1);
}
/** 未实现 */
static inline void GetConsoleCursorInfo(HANDLE stdhandle, CONSOLE_CURSOR_INFO *cinfo)
{
    cinfo->bVisible = 0;
    cinfo->dwSize = 0;
}
/** 实现不完全 */
static inline void SetConsoleCursorInfo(HANDLE stdhandle, const CONSOLE_CURSOR_INFO *cinfo)
{
    fprintf(stdhandle, "\e[?25%c", cinfo->bVisible ? 'h' : 'l');
}

#endif
