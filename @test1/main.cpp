#include <filesystem>
#include <fstream>
#include <iostream>

#include <sys/stat.h>
#include <unistd.h>

#include "olcpp/base.h"

using namespace std;

string readfile(string path)
{
    ifstream t(path);
    return string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
}

int main()
{
    olcpp::initIDBFS();
    // ofstream of("/data/@test1.txt");
    // of << 1;
    // of.close();
    mkdir("/data/xacraft", S_IRWXU);
    chdir("/data/xacraft");
    for (const auto &file : filesystem::recursive_directory_iterator("."))
    {
        cout << file.path().generic_string() << endl;
        ifstream fin(file.path().filename());
        string a;
        getline(fin, a);
        cout << "line1: " << a << endl << endl;
        getline(fin, a);
        cout << "line2: " << a << endl << endl;
    }
    return 0;
}