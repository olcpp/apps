// Web APIs in Console Apps

#ifndef OLCPP_CONSOLE_H
#define OLCPP_CONSOLE_H

#include <cstddef>
#include <iostream>

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
    /** 24位真彩色 */
    struct Color
    {
        short r, g, b;
        Color(short r, short g, short b);
    };
    /** x 表示列，y 表示行 */
    struct Position
    {
        short x, y;
        Position(short x, short y);
    };
    /** 切换原始模式（true 开启，false 关闭），在此模式下，需要自行处理回显、退格等行为 */
    const Console &raw(bool on) const;
    /** 清空屏幕 */
    const Console &clear() const;
    /** 重置所有文字样式（包括颜色） */
    const Console &reset() const;
    /** 设置文字样式 */
    const Console &style(short id, bool use = true) const;
    /** 重置背景色 */
    const Console &bgcolor() const;
    /** 设置背景色（256色） */
    const Console &bgcolor(short id) const;
    /** 设置背景色（24位真彩色） */
    const Console &bgcolor(const Color &color) const;
    /** 设置背景色（24位真彩色） */
    const Console &bgcolor(short r, short g, short b) const;
    /** 重置前景色 */
    const Console &fgcolor() const;
    /** 设置前景色（256色） */
    const Console &fgcolor(short id) const;
    /** 设置背景色（24位真彩色） */
    const Console &fgcolor(const Color &color) const;
    /** 设置背景色（24位真彩色） */
    const Console &fgcolor(short r, short g, short b) const;
    /** 设置光标位置（x 表示列，y 表示行） */
    const Console &cursor(const Position &position) const;
    /** 设置光标位置（x 表示列，y 表示行） */
    const Console &cursor(short x, short y) const;
    /** 移动光标（x > 0 向下，x < 0 向上；y > 0 向右，y < 0 向左；为 0 则不变） */
    const Console &move(const Position &position) const;
    /** 移动光标（x > 0 向下，x < 0 向上；y > 0 向右，y < 0 向左；为 0 则不变） */
    const Console &move(short x, short y) const;
    /** 设置光标可见性（true 表示可见；false 表示隐藏 */
    const Console &cursor(bool visible) const;
    /** 读取一个字符（不等待回车） */
    int getch() const;
    /** 读取控制台大小 */
    Position size() const;
};
std::ostream &operator<<(std::ostream &os, const Console &);

extern const Console console;

} // namespace olcpp

#endif