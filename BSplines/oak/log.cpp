
#include "log.h"

#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>

namespace
{
static std::ostream* logStream = &std::cout;
static std::ofstream* fileStream = nullptr;

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

bool log_to(const std::string& file)
{
    if (fileStream)
    {
        delete fileStream;
    }
    fileStream = new std::ofstream();
    fileStream->open(file, std::ios_base::out | std::ios_base::trunc);
    if (fileStream->fail())
    {
        std::cout << "Failed to open log file: " << file;
        return false;
    }
    return true;
}

bool log_to(std::ostream& stream)
{
    logStream = &stream;
    if (logStream != &stream)
    {
        delete fileStream;
        fileStream = nullptr;
    }
    return true;
}

std::ostream& log(LogLevel level, const char* func, const char* file, int line)
{
    auto now = std::chrono::system_clock::now();
    auto now_tt = std::chrono::system_clock::to_time_t(now);
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now - std::chrono::system_clock::from_time_t(now_tt));
    *logStream << std::put_time(std::localtime(&now_tt), "%F %T.");
    auto oldFill = logStream->fill();
    auto oldWidth = logStream->width();
    logStream->fill('0');
    logStream->width(3);
    *logStream << millis.count();
    logStream->fill(oldFill);
    logStream->width(oldWidth);
    *logStream << " " << logLevelString[level] << ": " << func << "@" << file << ":" << line;
    return *logStream;
}

}
