#include <cstdarg>
#include <cstdio>

#include "emscripten.h"

#ifndef OLCPP_APP_TYPE_CONSOLE
#define OLCPP_APP_TYPE_CONSOLE
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
    extern void _Impl_SetRawMode(int);
    extern unsigned int _Impl_GetConsoleSize();
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

const FileSystem &FileSystem::mount(const char *mountpoint, FSType type) const
{
    switch (type)
    {
    case IDBFS:
        _Impl_InitIDBFS(mountpoint);
        break;
    }
    return *this;
}
const FileSystem fs;

} // namespace olcpp

// console.h
#include "olcpp/console.h"
namespace olcpp
{
Console::Color::Color(short r, short g, short b) : r(r), g(g), b(b)
{
}
Console::Position::Position(short x, short y) : x(x), y(y)
{
}
const Console &Console::raw(bool on) const
{
    _Impl_SetRawMode(on);
    return *this;
}
const Console &Console::clear() const
{
    printf("\e[2J\e[3J\e[1;1H");
    fflush(stdout);
    return *this;
}
const Console &Console::style(short id, bool use) const
{
    if (use)
    {
        printf("\e[%dm", id);
    }
    else
    {
        if (id == 1)
        {
            id++;
        }
        printf("\e[%dm", id + 20);
    }
    fflush(stdout);
    return *this;
}
const Console &Console::reset() const
{
    printf("\e[0m");
    fflush(stdout);
    return *this;
}
const Console &Console::bgcolor() const
{
    printf("\e[49m");
    fflush(stdout);
    return *this;
}
const Console &Console::bgcolor(short id) const
{
    printf("\e[48;5;%dm", id);
    fflush(stdout);
    return *this;
}
const Console &Console::bgcolor(const Color &color) const
{
    printf("\e[48;2;%d;%d;%dm", color.r, color.g, color.b);
    fflush(stdout);
    return *this;
}
const Console &Console::bgcolor(short r, short g, short b) const
{
    return bgcolor(Color(r, g, b));
}
const Console &Console::fgcolor() const
{
    printf("\e[39m");
    fflush(stdout);
    return *this;
}
const Console &Console::fgcolor(short id) const
{
    printf("\e[38;5;%dm", id);
    fflush(stdout);
    return *this;
}
const Console &Console::fgcolor(const Color &color) const
{
    printf("\e[38;2;%d;%d;%dm", color.r, color.g, color.b);
    fflush(stdout);
    return *this;
}
const Console &Console::fgcolor(short r, short g, short b) const
{
    return fgcolor(Color(r, g, b));
}
const Console &Console::cursor(const Position &pos) const
{
    printf("\e[%d;%dH", pos.y, pos.x);
    fflush(stdout);
    return *this;
}
const Console &Console::cursor(short x, short y) const
{
    return cursor(Position(x, y));
}
const Console &Console::move(const Position &pos) const
{
    if (pos.y != 0)
    {
        printf("\e[%d%c", abs(pos.y), pos.y < 0 ? 'A' : 'B');
    }
    if (pos.x != 0)
    {
        printf("\e[%d%c", abs(pos.x), pos.x > 0 ? 'C' : 'D');
    }
    fflush(stdout);
    return *this;
}
const Console &Console::move(short x, short y) const
{
    return move(Position(x, y));
}
const Console &Console::cursor(bool visible) const
{
    printf("\e[?25%c", visible ? 'h' : 'l');
    fflush(stdout);
    return *this;
}
int Console::getch() const
{
    return _Impl_GetCharI();
}
Console::Position Console::size() const
{
    unsigned int ret = _Impl_GetConsoleSize();
    return Position((ret >> 16) & 0xFFFF, ret & 0xFFFF);
}
std::ostream &operator<<(std::ostream &os, const Console &)
{
    return os;
}
const Console console;
} // namespace olcpp