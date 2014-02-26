
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

    bool logTo(const std::string& file);
    bool logTo(std::ostream& stream);
    void setLogLevel(LogLevel level);

    std::ostream& log(LogLevel level, const char* func, const char* file, int line);

    std::string getOpenGLErrorString(unsigned int error);
}

#define LOG_TRACE() oak::log(oak::LogTrace,    __FUNCTION__, __FILE__, __LINE__)
#define LOG_INFO()  oak::log(oak::LogInfo,     __FUNCTION__, __FILE__, __LINE__)
#define LOG_WARN()  oak::log(oak::LogWarning,  __FUNCTION__, __FILE__, __LINE__)
#define LOG_ERR()   oak::log(oak::LogError,    __FUNCTION__, __FILE__, __LINE__)
#define LOG_CRIT()  oak::log(oak::LogCritical, __FUNCTION__, __FILE__, __LINE__)
#define LOG_FATAL(what) \
    do { \
        oak::log(oak::LogCritical, __FUNCTION__, __FILE__, __LINE__) << what << std::endl; \
        throw -1; \
    } while(0)

#define LOG_OPENGL_ERROR(error) LOG_FATAL(oak::getOpenGLErrorString(error))
#define LOG_OPENGL_ERROR_OBJECT(error, objectName) LOG_FATAL(std::string("[")+objectName+"] "+oak::getOpenGLErrorString(error))

#define CHECK_OPENGL_ERROR() \
    do { \
        GLenum error = glGetError(); \
        if (error != GL_NO_ERROR) \
            LOG_OPENGL_ERROR(error); \
    } while(0)

#define CHECK_OPENGL_ERROR_OBJECT(objectName) \
    do { \
        GLenum error = glGetError(); \
        if (error != GL_NO_ERROR) \
            LOG_OPENGL_ERROR_OBJECT(error, objectName); \
    } while(0)

#endif // OAK_LOG_H
