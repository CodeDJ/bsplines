
#include "application.h"
#include "log.h"

#include <Foundation/Foundation.h>

namespace
{
    static NSString* rsrcDir()
    {
        return [[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent: @"Contents/Resources"];
    }

    static NSString* rsrcPath(const char* path)
    {
        return [rsrcDir() stringByAppendingPathComponent: [NSString stringWithUTF8String: path]];
    }
}

namespace oak
{

std::string Application::resourceDir()
{
    std::string result([rsrcDir() UTF8String]);
    //LOG_INFO() << result << std::endl;
    return result;
}

std::string Application::resourcePath(const std::string& path)
{
    std::string result([rsrcPath(path.c_str()) UTF8String]);
    //LOG_INFO() << result << std::endl;
    return result;
}

}
