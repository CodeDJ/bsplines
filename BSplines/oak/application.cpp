
#include "application.h"

#include "config.h"

#ifdef Q_OS_MAC
#include <GLUT/glut.h>
#endif
#ifdef Q_OS_WIN
#include <GL/freeglut.h>
#endif

#include <stdlib.h>

#ifdef Q_OS_MAC
#define glutMainLoop glutMainLoopMac

namespace
{
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
}
#endif // Q_OS_MAC

oak::Application::Application(int* argc, char** argv)
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
    glutMainLoop();
    return _exitCode;
}

void oak::Application::exit(int code)
{
    _exitCode = code;
    glutLeaveMainLoop();
}

void oak::Application::initOpenGL(int* argc, char** argv)
{
    glutInit(argc, argv);
#ifdef OPENGL_2
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
#else
#ifdef Q_OS_MAC
    glutInitDisplayMode (GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE);
#endif
#ifdef Q_OS_WIN
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
    glutInitContextVersion(3, 2);
#endif
#endif
#ifdef Q_OS_WIN
    glewExperimental = GL_TRUE;
    glewInit();
#endif
}
