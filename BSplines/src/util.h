#ifndef UTIL_H
#define UTIL_H

#include <string>

namespace Util
{
    std::string loadFile(const std::string& fileName);
    std::string loadFile(const char* fileName);
}

#endif // UTIL_H
