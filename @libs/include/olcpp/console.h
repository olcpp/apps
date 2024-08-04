// Web APIs in Console Apps

#ifndef OLCPP_CONSOLE_H
#define OLCPP_CONSOLE_H

#ifndef OLCPP_APP_TYPE_CONSOLE
#error ONLY console apps are allowed to include console.h!
#endif

namespace olcpp
{

#if __cplusplus >= 201103L
class Console final
#else
class Console
#endif
{
  public:
    struct Color
    {
        short r, g, b;
    };
    void clear() const;
    void bgcolor(const Color &color) const;
    void fgcolor(const Color &color) const;
    int getch() const;
};

extern const Console console;

} // namespace olcpp

#endif