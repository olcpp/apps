#include <iostream>

#include "olcpp/base.h"
#include "olcpp/console.h"

using namespace std;
using olcpp::console;

int main()
{
    olcpp::title("控制台光标控制示例");
    cout << "WASD 控制光标位置，E 输入新位置，Z 显示/隐藏光标" << endl;
    console.raw(true);
    bool cursorVisible = true;
    while (true)
    {
        int x = console.getch();
        switch (x)
        {
        case 'w':
        case 'W':
            console.move(0, -1);
            break;
        case 'a':
        case 'A':
            console.move(-1, 0);
            break;
        case 's':
        case 'S':
            console.move(0, 1);
            break;
        case 'd':
        case 'D':
            console.move(1, 0);
            break;
        case 'e':
        case 'E':
            console.raw(false).clear();
            cout << "输入光标的新位置(x,y): ";
            int x, y;
            cin >> x >> y;
            console.raw(true).cursor(x, y);
            break;
        case 'z':
        case 'Z':
            cursorVisible = !cursorVisible;
            console.cursor(cursorVisible);
            break;
        }
    }
    return 0;
}