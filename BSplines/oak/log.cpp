
#include "log.h"

#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>

#ifdef Q_OS_MAC
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/freeglut.h>
#endif


namespace
{
static std::ostream* logStream = &std::cout;
static std::unique_ptr<std::ofstream> fileStream = nullptr;
static std::ofstream nullStream;
static oak::LogLevel currentLevel = oak::LogTrace;

static const char* logLevelString[] =
    {
        "TRC",
        "INF",
        "WRN",
        "ERR",
        "CRI"
    };
}

namespace oak
{

bool logTo(const std::string& file)
{
    std::unique_ptr<std::ofstream> newFileStream(new std::ofstream());
    newFileStream->open(file, std::ios_base::out | std::ios_base::trunc);
    if (newFileStream->fail())
    {
        std::cout << "Failed to open log file: " << file;
        return false;
    }
    fileStream.swap(newFileStream);
    logStream = fileStream.get();
    return true;
}

bool logTo(std::ostream& stream)
{
    if (logStream != &stream)
    {
        fileStream.reset();
    }
    logStream = &stream;
    return true;
}

void setLogLevel(LogLevel level)
{
    currentLevel = level;
}

std::ostream& log(LogLevel level, const char* func, const char* file, int line)
{
    if (level < currentLevel)
    {
        return nullStream;
    }

    // timestamp
    auto now = std::chrono::system_clock::now();
    auto now_tt = std::chrono::system_clock::to_time_t(now);
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now - std::chrono::system_clock::from_time_t(now_tt));
    *logStream << std::put_time(std::localtime(&now_tt), "%F %T.");

    // milliseconds
    auto oldFill = logStream->fill();
    auto oldWidth = logStream->width();
    logStream->fill('0');
    logStream->width(3);
    *logStream << millis.count();
    logStream->fill(oldFill);
    logStream->width(oldWidth);

    *logStream << " " << logLevelString[level] << ": " << func << "@" << file << ":" << line << " ";

    return *logStream;
}

std::string getOpenGLErrorString(unsigned int error)
{
    return reinterpret_cast<const char*>(gluErrorString(error));
}


}
