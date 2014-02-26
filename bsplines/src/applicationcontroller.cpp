
#include "applicationcontroller.h"

#include "oak/application.h"
#include "oak/window.h"
#include "oak/timer.h"
#include "oak/clock.h"
#include "oak/config.h"
#include "oak/spline.h"
#include "oak/log.h"
#include "oak/rectf.h"

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

#define WINDOW_X 300
#define WINDOW_Y 300
#define WINDOW_W 640
#define WINDOW_H 480

static const bool g_DefaultWindowFullscreen = false;
static const bool g_DefaultVSyncOn = true;
static const bool g_DefaultAnimationOn = true;
static const bool g_DefaultUseTessellation = true;
static const bool g_DefaultHelpVisible = true;
static const bool g_DefaultRandomSplines = false;

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
    _window(nullptr),
    _timer(nullptr),
    _splinePainter(nullptr),
    _staticTextPainter(nullptr),
    _config({ g_DefaultWindowFullscreen,
              g_DefaultVSyncOn,
              g_DefaultAnimationOn,
              g_DefaultUseTessellation,
              g_DefaultHelpVisible,
              g_DefaultRandomSplines})
{

    _window = new oak::Window(WINDOW_X, WINDOW_Y, WINDOW_W, WINDOW_H, _app->args()[0]);

    LOG_INFO() << _app->glRenderer() << std::endl; // e.g. Intel HD Graphics 3000 OpenGL Engine
    LOG_INFO() << _app->glVersion() << std::endl;  // e.g. 3.2 INTEL-8.0.61

    /* GL settings */
    _vertexArray = new OpenglVertexArray;
    _vertexArray->bind();

    _window->onKey(
        [this] (oak::Window* window, unsigned char key, int x, int y)
        {
            keyPressed(window, key, x, y);
        });

    _window->onPaint(
        [this] (oak::Window* window)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            try
            {
                if (_splinePainter)
                    _splinePainter->paint(window);
                if (_config.helpVisible && _staticTextPainter)
                    _staticTextPainter->paint(window);
            }
            catch(int errorCode)
            {
                _app->exit(errorCode);
            }
        });

    _timer = new oak::Timer(TIMER_MS, true,
        [this] (oak::Timer*)
        {
            if (_config.animatingOn)
            {
                oak::Spline::animate(_splinePainter->objects());
                _window->repaint();
            }
        });

    restart(true, true);
}

ApplicationController::~ApplicationController()
{
    delete _splinePainter;
    delete _staticTextPainter;
    delete _vertexArray;

    delete _timer;
    delete _window;
}

bool ApplicationController::initShaders()
{
    if (!_splinePainter)
    {
        _splinePainter = new GlslSplinePainter(_config.useTessellation);
    }
    _splinePainter->setUseTessellation(_config.useTessellation);

    if (!_staticTextPainter)
    {
        std::vector<std::string> options = { "Option 1", "Option 2", "Option 3", "Option 4" };
        _staticTextPainter = new GlslStaticTextPainter(oak::StaticText(oak::RectF(0.0f, 0.0f, 100.0f, 100.0f), options), oak::Color(1.0f, 0.0f, 0.0f, 1.0f));
    }
    return true;
}

void ApplicationController::initAndSetSplines()
{
    std::vector<oak::Spline> splines;
    if (_config.randomSplines)
    {
        srand(time(0));
        splines = oak::Spline::generate(CURVE_STRIPS, MAX_CURVES);
    }
    else
    {
        splines = oak::Spline::defaultSplines(MAX_CURVES);
    }

    if (_splinePainter)
    {
        _splinePainter->setSplines(splines);
    }
}

void ApplicationController::restart(bool reinitShaders, bool reinitSplines)
{
    _timer->stop();
    if (reinitShaders)
    {
        initShaders();
    }
    if (reinitSplines)
    {
        initAndSetSplines();
    }
    if (_splinePainter)
        _splinePainter->prepare();
    if (_staticTextPainter)
        _staticTextPainter->prepare();
    _timer->start();
}

void ApplicationController::setFullscreen(bool on)
{
    if (_config.fullscreen != on)
    {
        _config.fullscreen = on;
        _window->showFullScreen(on);
    }
}

void ApplicationController::setVSyncOn(bool on)
{
    if (_config.vsyncOn != on)
    {
        _config.vsyncOn = on;
        _window->setVSync(on);
    }
}

void ApplicationController::setAnimatingOn(bool on)
{
    _config.animatingOn = on;
}

void ApplicationController::setUseTessellation(bool use)
{
    if (_config.useTessellation != use)
    {
        _config.useTessellation = use;
        restart(true, false);
    }
}

void ApplicationController::setHelpVisible(bool visible)
{
    if (_config.helpVisible != visible)
    {
        _config.helpVisible = visible;
        if (!_config.animatingOn)
        {
            _window->repaint();
        }
    }
}

void ApplicationController::setRandomSplines(bool random)
{
    if (_config.randomSplines != random)
    {
        _config.randomSplines = random;
        restart(false, true);
    }
}


void ApplicationController::keyPressed(oak::Window* window, unsigned char key, int x, int y)
{
    UNUSED(window);
    UNUSED(x);
    UNUSED(y);

    switch(key)
    {
    case 'q':
        _app->exit(0);
        break;
    case 'f':
        setFullscreen(!_config.fullscreen);
        break;
    case 'v':
        setVSyncOn(!_config.vsyncOn);
        break;
    case 'p':
        setAnimatingOn(!_config.animatingOn);
        break;
    case 'h':
        setHelpVisible(!_config.helpVisible);
        break;
    case 't':
        setUseTessellation(!_config.useTessellation);
        break;
    case 'r':
        setRandomSplines(!_config.randomSplines);
        break;

    default:
        break;
    }
}
