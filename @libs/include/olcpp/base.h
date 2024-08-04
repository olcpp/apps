// Basic APIs

#ifndef OLCPP_BASE_H
#define OLCPP_BASE_H

#include <iostream>

namespace olcpp
{

void sleep(unsigned int millionSeconds);
const char *title();
void title(const char *newTitle);
void openurl(const char *url);
void logs(const char *str);
void logf(const char *fmt, ...);
void initIDBFS(const char *mountpoint = "/data");

} // namespace olcpp

#endif