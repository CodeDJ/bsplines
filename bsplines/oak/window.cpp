
#include "window.h"

#include "clock.h"
#include "config.h"
#include "log.h"

#ifdef Q_OS_MAC
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/freeglut.h>
#endif

#include <map>

#define DEFAULT_WINDOW_X        300
#define DEFAULT_WINDOW_Y        300
#define DEFAULT_WINDOW_WIDTH    640
#define DEFAULT_WINDOW_HEIGHT   480

namespace
{
    static std::map<int, oak::Window*> _windowMap;
}

oak::Window::Window(const std::string& name) :
    Window(DEFAULT_WINDOW_X, DEFAULT_WINDOW_Y, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, name)
{
}

oak::Window::Window(int x, int y, int w, int h, const std::string& name)
    : _lastWidth(-1),
      _lastHeight(-1),
      _lastX(-1),
      _lastY(-1)
{

    glutInitWindowSize(w, h);
    glutInitWindowPosition(x, y);
    _glutWindow = glutCreateWindow(name.c_str());

    _windowMap[_glutWindow] = this;

#ifdef Q_OS_WIN
    glewExperimental = GL_TRUE;
    glewInit();
    glGetError();
    CHECK_OPENGL_ERROR();
#endif

    glClearColor (0.0f, 0.0f, 0.0f, 0.0f);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    glutDisplayFunc(paintCb);
    glutKeyboardFunc(keyCb);
    glutReshapeFunc(resizeCb);

    // ?
    resizeCb(w, h);
}

oak::Window::~Window()
{
    _windowMap.erase(_windowMap.find(_glutWindow));
    glutDestroyWindow(_glutWindow);
    _glutWindow = 0;
}

void oak::Window::makeCurrent() const
{
    if (glutGetWindow() != _glutWindow)
        glutSetWindow(_glutWindow);
}

void oak::Window::showFullScreen(bool fullscreen)
{
    makeCurrent();
    if (fullscreen)
    {
        getPos(&_lastX, &_lastY);
        getSize(&_lastWidth, &_lastHeight);
        glutFullScreen();
    }
    else
    {
        setSize(_lastWidth, _lastHeight);
        setPos(_lastX, _lastY);
    }
}

void oak::Window::setVSync(bool enable)
{
#ifdef Q_OS_WIN
    if (enable)
        wglSwapIntervalEXT(1);
    else
        wglSwapIntervalEXT(0);
#endif
#ifdef Q_OS_MAC
    GLint vsyncOn = enable ? 1 : 0;
    CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &vsyncOn);
#endif
}

void oak::Window::repaint()
{
    glutPostWindowRedisplay(_glutWindow);
}

int oak::Window::x() const
{
    makeCurrent();
    return glutGet(GLUT_WINDOW_X);
}

int oak::Window::y() const
{
    makeCurrent();
    return glutGet(GLUT_WINDOW_Y);
}

int oak::Window::width() const
{
    makeCurrent();
    return glutGet(GLUT_WINDOW_WIDTH);
}

int oak::Window::height() const
{
    makeCurrent();
    return glutGet(GLUT_WINDOW_HEIGHT);
}

void oak::Window::getPos(int* x, int* y) const
{
    makeCurrent();
    *x = glutGet(GLUT_WINDOW_X);
    *y = glutGet(GLUT_WINDOW_Y);
}

void oak::Window::getSize(int* w, int* h) const
{
    makeCurrent();
    *w = glutGet(GLUT_WINDOW_WIDTH);
    *h = glutGet(GLUT_WINDOW_HEIGHT);
}

void oak::Window::setSize(int w, int h)
{
    makeCurrent();
    glutReshapeWindow(w, h);
}

void oak::Window::setPos(int x, int y)
{
    makeCurrent();;
    glutPositionWindow(x, y);
}

void oak::Window::getGeometry(int* x, int* y, int* w, int* h) const
{
    makeCurrent();
    *x = glutGet(GLUT_WINDOW_X);
    *y = glutGet(GLUT_WINDOW_Y);
    *w = glutGet(GLUT_WINDOW_WIDTH);
    *h = glutGet(GLUT_WINDOW_HEIGHT);
}

void oak::Window::paintEvent()
{
}

void oak::Window::onPaint(const PaintFunc& paintFunc)
{
    _paintFunc = paintFunc;
}

void oak::Window::paintCb()
{
    oak::Clock clck;

    Window* window = _windowMap[glutGetWindow()];

    glClear (GL_COLOR_BUFFER_BIT);

    window->paintEvent();
    if (window->_paintFunc)
        window->_paintFunc(window);

    glutSwapBuffers();

    LOG_TRACE() << clck << std::endl;
}

void oak::Window::keyEvent(unsigned char /*key*/, int /*x*/, int /*y*/)
{
}

void oak::Window::onKey(const KeyFunc &keyFunc)
{
    _keyFunc = keyFunc;
}

void oak::Window::keyCb(unsigned char key, int x, int y)
{
    Window* window = _windowMap[glutGetWindow()];
    window->keyEvent(key, x, y);
    if (window->_keyFunc)
        window->_keyFunc(window, key, x, y);
}

void oak::Window::resizeEvent(int /*width*/, int /*height*/)
{
}

void oak::Window::onResize(const ResizeFunc &resizeFunc)
{
    _resizeFunc = resizeFunc;
}

void oak::Window::resizeCb(int width, int height)
{
    Window* window = _windowMap[glutGetWindow()];

    glViewport(0, 0, width, height);

    window->resizeEvent(width, height);
    if (window->_resizeFunc)
        window->_resizeFunc(window, width, height);
}
