#ifndef OLCPP_WIN32_SYSTEM_H
#define OLCPP_WIN32_SYSTEM_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "olcpp/base.h"

static inline void _Hook_system(const char *cmd)
{
    if (strcmp(cmd, "cls") == 0)
    {
        printf("\e[2J\e[3J\e[1;1H");
    }
    else if (strncmp(cmd, "start ", sizeof("start ") - 1) == 0 && strlen(cmd) > sizeof("start ") - 1)
    {
        olcpp::openurl(std::string(cmd + sizeof("start ") - 1).c_str());
    }
    else
    {
        system(cmd);
    }
}
#define system _Hook_system

#endif