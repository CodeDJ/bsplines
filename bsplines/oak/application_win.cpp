
#include "application.h"
#include "log.h"

#include <string>

#include <Windows.h>

namespace
{
//    static NSString* rsrcDir()
//    {
//        return [[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent: @"Contents/Resources"];
//    }

//    static NSString* rsrcPath(const char* path)
//    {
//        return [rsrcDir() stringByAppendingPathComponent: [NSString stringWithUTF8String: path]];
//    }

    static std::string exePath()
    {
        std::vector<char> executablePath(MAX_PATH);

        // Try to get the executable path with a buffer of MAX_PATH characters.
        DWORD result = ::GetModuleFileNameA(nullptr, &executablePath[0], static_cast<DWORD>(executablePath.size()));

        // As long the function returns the buffer size, it is indicating that the buffer
        // was too small. Keep enlarging the buffer by a factor of 2 until it fits.
        while (result == executablePath.size())
        {
            executablePath.resize(executablePath.size() * 2);
            result = ::GetModuleFileNameA(nullptr, &executablePath[0], static_cast<DWORD>(executablePath.size()));
        }

        // If the function returned 0, something went wrong
        if (result == 0)
        {
            return std::string();
        }

        // We've got the path, construct a standard string from it
        return std::string(executablePath.begin(), executablePath.begin() + result);
    }

}

namespace oak
{

std::string Application::resourceDir()
{
    std::string result;
    //LOG_INFO() << result << std::endl;
    return result;
}

std::string Application::resourcePath(const std::string& path)
{
    std::string result(path);
    //result = exePath();
    //LOG_INFO() << result << std::endl;
    return result;
}

}
