
#include "applicationcontroller.h"

#include "oak/application.h"
#include "oak/window.h"
#include "oak/timer.h"
#include "oak/clock.h"
#include "oak/config.h"
#include "oak/spline.h"
#include "oak/log.h"

#include "openglvertexarray.h"
#include "glslpainter.h"
#include "global.h"

#ifdef Q_OS_MAC
#include <OpenGL/gl3.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif

#include <iostream>

#define WITH_TESS true

#define WINDOW_X 300
#define WINDOW_Y 300
#define WINDOW_W 640
#define WINDOW_H 480

#define WINDOW_FULL_SCREEN true
#define DISABLE_VSYNC true

#define ANIMATE true
#define TIMER_MS 10

#define MAX_CURVES 100
#define RANDOM_POINTS false

#define CURVE_STRIPS 4
#define CURVE_STRIP_SEGMENTS 30

// THIS IS ALSO HARDCODED IN TCS
#define CURVE_CONTROL_POINTS (CURVE_STRIPS*3 + 1)

#define DEFAULT_CURVE_ALPHA 1.0
#define DEFAULT_CURVE_WIDTH 3

ApplicationController::ApplicationController(oak::Application* application) :
    _app(application),
    _window(nullptr)
{

    _window = new oak::Window(WINDOW_X, WINDOW_Y, WINDOW_W, WINDOW_H, _app->args()[0]);

    LOG_INFO() << _app->glRenderer() << std::endl; // e.g. Intel HD Graphics 3000 OpenGL Engine
    LOG_INFO() << _app->glVersion() << std::endl;  // e.g. 3.2 INTEL-8.0.61

    /* GL settings */
    _vertexArray = new OpenglVertexArray;
    _vertexArray->bind();

    glClearColor (0.0f, 0.0f, 0.0f, 0.0f);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    if (WINDOW_FULL_SCREEN)
    {
        _window->showFullScreen();
    }
    if (DISABLE_VSYNC)
    {
        _window->setVSync(false);
    }

    srand(time(0));
    std::vector<oak::Spline> splines = RANDOM_POINTS ? oak::Spline::generate(CURVE_STRIPS, MAX_CURVES) :
                                                            oak::Spline::defaultSplines(MAX_CURVES);
    _splinePainter = new GlslSplinePainter(splines, WITH_TESS);
    _splinePainter->prepare();

    _window->onKey(
        [this] (oak::Window*, unsigned char, int, int)
        {
            _app->exit(0);
        });

    _window->onPaint(
        [this] (oak::Window* window)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            _splinePainter->paint(window);
        });

    if (ANIMATE)
    {
        _timer = new oak::Timer(TIMER_MS, true,
            [this] (oak::Timer*)
            {
                oak::Spline::animate(_splinePainter->objects());
                _window->repaint();
            });
        _timer->start();
    }
}

ApplicationController::~ApplicationController()
{
    delete _splinePainter;
    delete _vertexArray;

    delete _timer;
    delete _window;
}
