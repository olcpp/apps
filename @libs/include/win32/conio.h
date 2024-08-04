#ifndef OLCPP_WIN32_CONIO_H
#define OLCPP_WIN32_CONIO_H

#include "olcpp/console.h"

static inline int getch()
{
    return olcpp::console.getch();
}
static inline int _getch()
{
    return olcpp::console.getch();
}

#endif