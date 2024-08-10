// Basic APIs

#ifndef OLCPP_BASE_H
#define OLCPP_BASE_H

#include <iostream>
#include <string>

namespace olcpp
{

void sleep(unsigned int millionSeconds = 0);
const char *title();
void title(const char *title);
void openurl(const char *url);
void logs(const char *str);
void logf(const char *fmt, ...);

#if __cplusplus >= 201103L
class FileSystem final
#else
class FileSystem
#endif
{
    typedef const FileSystem &self;
    typedef const char *cstring;

  public:
    enum FSType
    {
        IDBFS
    };
    self mount(cstring mountpoint, FSType type = IDBFS) const;
    self unmount(cstring mountpoint) const;
    self mkdir(cstring path, int mode = 0777) const;
    self symlink(cstring oldpath, cstring newpath) const;
    self rename(cstring oldpath, cstring newpath) const;
    self rmdir(cstring path) const;
    self unlink(cstring path) const;
    std::string readFile(cstring path, cstring flags = "r") const;
    std::string writeFile(cstring path, cstring data, cstring flags = "w") const;
    std::string cwd() const;
    self chdir(cstring path) const;
    std::vector<std::string> readdir(cstring path) const;
};
extern const FileSystem fs;

} // namespace olcpp

#endif