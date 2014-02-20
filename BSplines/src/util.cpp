#include "util.h"

#include <fstream>

namespace Util
{

std::string loadFile(const std::string& fileName)
{
    return loadFile(fileName.c_str());
}

std::string loadFile(const char* fileName)
{
    std::ifstream ifs(fileName);
    if (ifs.is_open())
        return std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    return std::string();
}

}


