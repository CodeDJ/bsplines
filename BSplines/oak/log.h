
#ifndef OAK_LOG_H
#define OAK_LOG_H

#include <ostream>

namespace oak
{
    enum LogLevel
    {
        LogTrace,
        LogInfo,
        LogWarning,
        LogError,
        LogCritical
    };

    bool log_to(const std::string& file);
    bool log_to(std::ostream& stream);
    std::ostream& log(LogLevel level, const char* func, const char* file, int line);
}

#define LOG_TRACE() oak::log(oak::LogTrace,    __FUNCTION__, __FILE__, __LINE__)
#define LOG_INFO()  oak::log(oak::LogInfo,     __FUNCTION__, __FILE__, __LINE__)
#define LOG_WARN()  oak::log(oak::LogWarning,  __FUNCTION__, __FILE__, __LINE__)
#define LOG_ERR()   oak::log(oak::LogError,    __FUNCTION__, __FILE__, __LINE__)
#define LOG_CRIT()  oak::log(oak::LogCritical, __FUNCTION__, __FILE__, __LINE__)

#endif // OAK_LOG_H
