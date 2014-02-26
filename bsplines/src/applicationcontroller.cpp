
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

#define TIMER_MS 10

#define CURVE_STRIPS 4
#define CURVE_STRIP_SEGMENTS 30

#define DEFAULT_CURVE_ALPHA 1.0
#define DEFAULT_CURVE_WIDTH 3

namespace
{
static const bool g_DefaultWindowFullscreen = false;
static const bool g_DefaultVSyncOn = false;
static const bool g_DefaultAnimationOn = true;
static const bool g_DefaultUseTessellation = true;
static const bool g_DefaultHelpVisible = true;
static const bool g_DefaultRandomSplines = false;
static const oak::SplineContinuityConstraint g_DefaultContinuityConstraint = oak::SplineContinuityConstraint::Continous1stDeriv;
static const size_t g_DefaultCurvesCount = 100;

static const std::vector<std::string> g_OptionTemplates = {
    "FPS:",
    "[T]esselation:  ",
    "[F]ull Screen:   ",
    "[V]Sync:           ",
    "[H]elp",
    "[P]ause",
    "[R]andomize",
    "[S]eed",
    "[1]",
    "[2]",
    "[3]",
    "[Q]uit"
};

static const int _fpsWeights[5] = { 1, 2, 3, 4, 5 };
}

void ApplicationController::updateOptionTexts(std::vector<std::string>& texts)
{
    static const std::string yesNo[2] = { "No", "Yes" };
    // tesselation
    texts[1] = g_OptionTemplates[1] + yesNo[_config.useTessellation];
    // fullscreen
    texts[2] = g_OptionTemplates[2] + yesNo[_config.fullscreen];
    // vsync
    texts[3] = g_OptionTemplates[3] + yesNo[_config.vsyncOn];
}

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
              g_DefaultRandomSplines,
              g_DefaultContinuityConstraint,
              g_DefaultCurvesCount,
            })
{
     memset(_fps, 0, 5);
    _referenceTimePoint = std::chrono::time_point_cast<std::chrono::seconds>(_fpsClock.now());

    _window = new oak::Window(WINDOW_X, WINDOW_Y, WINDOW_W, WINDOW_H, _app->args()[0]);
    _window->setVSync(_config.vsyncOn);
    setContinuityConstraint(_config.constraint);

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
            std::chrono::high_resolution_clock::time_point newTimePoint = _fpsClock.now();
            int seconds = std::chrono::duration_cast<std::chrono::seconds>(newTimePoint - _referenceTimePoint).count();
            if (seconds >= 5)
            {
                //memmove(_fps, _fps + seconds - 5, 5+5 - seconds);
                //memset(_fps + 5+5 - seconds, 0, seconds - 5);
                //seconds = 4;
            }

            glClear(GL_COLOR_BUFFER_BIT);
            try
            {
                if (_splinePainter)
                    _splinePainter->paint(window);
                updateOptionTexts(_staticTextPainter->staticText().lines());
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
        _staticTextPainter = new GlslStaticTextPainter(oak::StaticText(
                                                           oak::RectF(10.0f, 10.0f, 130.0f, 150.0f),
                                                           g_OptionTemplates,
                                                           oak::Color(0.0f, 0.0f, 0.0f, 1.0f),
                                                           oak::Color(1.0f, 1.0f, 1.0f, 1.0f),
                                                           0.8f
                                                           )
                                                       );
    }
    return true;
}

void ApplicationController::initAndSetSplines()
{
    std::vector<oak::Spline> splines;
    if (_config.randomSplines)
    {
        srand(time(0));
        splines = oak::Spline::generate(CURVE_STRIPS, _config.curvesCount);
    }
    else
    {
        splines = oak::Spline::defaultSplines(_config.curvesCount);
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
    _config.randomSplines = random;
    restart(false, true);
}

void ApplicationController::setContinuityConstraint(oak::SplineContinuityConstraint constraint)
{
    _config.constraint = constraint;
    oak::Spline::setConstraint(constraint);
}

void ApplicationController::setCurvesCount(size_t curvesCount)
{
    if (_config.curvesCount != curvesCount)
    {
        _config.curvesCount = curvesCount;
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
        setRandomSplines(true);
        break;
    case 's':
        setRandomSplines(false);
        break;
    case '1':
        setContinuityConstraint(oak::SplineContinuityConstraint::None);
        break;
    case '2':
        setContinuityConstraint(oak::SplineContinuityConstraint::Continous1stDeriv);
        break;
    case '3':
        setContinuityConstraint(oak::SplineContinuityConstraint::Continous2ndDeriv);
        break;
    case '/':
        setCurvesCount(_config.curvesCount == 1 ? g_DefaultCurvesCount : 1);
        break;
    default:
        break;
    }
}
