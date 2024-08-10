#include <iostream>

#include "olcpp/base.h"
#include "olcpp/console.h"

using namespace std;
using olcpp::console;

void show_style(short id, const char *title)
{
    console.reset();
    printf("%s(id = %d): ", title, id);
    console.style(id);
    printf("这是%s效果！\n", title);
    console.reset();
}

void show_color256()
{
    console.reset();
    for (int i = 0; i < 256; i++)
    {
        console.bgcolor(i).fgcolor(15);
        printf("%03d", i);
        console.reset();
        printf(" ");
        if (i % 16 == 15)
        {
            cout << endl;
        }
    }
    console.reset();
}

int main()
{
    olcpp::title("控制台文字效果示例");
    cout << "文字样式：" << endl;
    show_style(1, "粗体");
    show_style(2, "虚化");
    show_style(3, "斜体");
    show_style(4, "下划线");
    show_style(7, "反转");
    show_style(9, "删除线");
    cout << endl;
    cout << "256色：" << endl;
    show_color256();
    cout << endl;
    return 0;
}