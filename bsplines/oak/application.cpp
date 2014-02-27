
#include "application.h"

#include "config.h"
#include "log.h"

#ifdef Q_OS_MAC
#include <GLUT/glut.h>
#endif
#ifdef Q_OS_WIN
//#include <GL/glew.h>
#include <GL/freeglut.h>
#endif

#include <stdlib.h>

namespace
{

#ifdef Q_OS_MAC

#   define glutMainLoop glutMainLoopMac

    static bool terminateMainLoop = false;
    void glutMainLoopMac()
    {
        terminateMainLoop = false;
        while (!terminateMainLoop)
        {
            glutCheckLoop();
        }
    }

    void glutLeaveMainLoop()
    {
        terminateMainLoop = true;
    }

#endif // Q_OS_MAC

#ifdef Q_OS_WIN
    static void glutWarningHandler(const char* fmt, va_list ap)
    {
        char buffer[200];
        vsnprintf(buffer, sizeof(buffer)/sizeof(char) - 1, fmt, ap);
        LOG_WARN() << buffer;
    }

    static void glutErrorHandler(const char* fmt, va_list ap)
    {
        char buffer[200];
        vsnprintf(buffer, sizeof(buffer)/sizeof(char) - 1, fmt, ap);
        LOG_CRIT() << buffer;
    }

#endif // Q_OS_WIN

}

oak::Application::Application(int* argc, char** argv)
    : _openGLMajor(-1),
      _openGLMinor(-1)
{
    initOpenGL(argc, argv);
    for (int i = 0; i < *argc; i++)
    {
        _args.push_back(argv[i]);
    }
}

oak::Application::~Application()
{
}

const std::vector<std::string>& oak::Application::args() const
{
    return _args;
}

std::string oak::Application::glRenderer() const
{
    return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

std::string oak::Application::glVersion() const
{
    return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}

int oak::Application::exec()
{
    _exitCode = 0;

    try
    {
        glutMainLoop();
    }
    catch(int exitCode)
    {
        _exitCode = exitCode;
    }

    return _exitCode;
}

void oak::Application::exit(int code)
{
    _exitCode = code;
    glutLeaveMainLoop();
}

void oak::Application::initOpenGL(int* argc, char** argv)
{
#ifdef Q_OS_WIN
    glutInitWarningFunc(glutWarningHandler);
    glutInitErrorFunc(glutErrorHandler);
#endif

    glutInit(argc, argv);

#ifdef OPENGL_2
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
#else

#ifdef Q_OS_MAC
    glutInitDisplayMode (/*GLUT_3_2_CORE_PROFILE | */GLUT_RGBA | GLUT_DOUBLE);
    _openGLMajor = 2;
    _openGLMinor = 1;
#endif

#ifdef Q_OS_WIN
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
    glutInitContextVersion(3, 2);
#endif

#endif
}
