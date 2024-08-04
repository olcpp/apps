#include <cstdarg>
#include <cstdio>
#include <iostream>

#include "emscripten.h"

#ifndef GRAPHICS_APP
#define GRAPHICS_APP
#endif
#ifndef CONSOLE_APP
#define CONSOLE_APP
#endif

// impls
extern "C"
{
    // base.js
    extern int _Impl_GetKeyState(int);
    extern void _Impl_InitIDBFS(const char *);
    extern void _Impl_OpenURL(const char *);
    // console.js
    extern int _Impl_GetCharI(void);
}

// base.h
#include "olcpp/base.h"
namespace olcpp
{
void sleep(unsigned int ms)
{
    emscripten_sleep(ms);
}
const char *title()
{
    return emscripten_get_window_title();
}
void title(const char *nt)
{
    emscripten_set_window_title(nt);
}
void openurl(const char *url)
{
    _Impl_OpenURL(url);
}
void logs(const char *str)
{
    fprintf(stderr, "%s", str);
}
void logf(const char *fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    vfprintf(stderr, fmt, vl);
    va_end(vl);
}
void initIDBFS(const char *mp)
{
    _Impl_InitIDBFS(mp);
}
} // namespace olcpp

// console.h
#include "olcpp/console.h"
namespace olcpp
{
void Console::clear() const
{
    printf("\e[2J\e[3J\e[1;1H");
    fflush(stdout);
}
void Console::bgcolor(const Color &color) const
{
    printf("\e[48;2;%d;%d;%dm", color.r, color.g, color.b);
    fflush(stdout);
}
void Console::fgcolor(const Color &color) const
{
    printf("\e[38;2;%d;%d;%dm", color.r, color.g, color.b);
    fflush(stdout);
}
int Console::getch() const
{
    return _Impl_GetCharI();
}
const Console console;
} // namespace olcpp