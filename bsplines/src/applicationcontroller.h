
#ifndef APPLICATION_CONTROLLER_H
#define APPLICATION_CONTROLLER_H

#include <chrono>
#include <vector>

namespace oak
{
class Application;
class Window;
class Timer;
enum class SplineContinuityConstraint;
}

class GlslSplinePainter;
class GlslStaticTextPainter;
enum class GlslSplinePainterVersion;
class OpenglVertexArray;

class ApplicationController
{
public:
    explicit ApplicationController(oak::Application* application);
    ~ApplicationController();

    ApplicationController(const ApplicationController&) = delete;
    ApplicationController& operator=(const ApplicationController&) = delete;

private:
    void keyPressed(oak::Window*, unsigned char, int, int);
    void initAndSetSplines();
    bool initShaders();
    void restart(bool reinitShaders, bool reinitSplines);
    GlslSplinePainterVersion getSplinePainterVersion();

    void setFullscreen(bool on);
    void setVSyncOn(bool on);
    void setAnimatingOn(bool on);
    void setUseTessellation(bool use);
    void setHelpVisible(bool visible);
    void setRandomSplines(bool random);
    void setContinuityConstraint(oak::SplineContinuityConstraint constraint);
    void setCurvesCount(size_t curvesCount);

    void updateOptionTexts(std::vector<std::string>& texts, float fps);

private:
    oak::Application* _app;
    oak::Window* _window;
    oak::Timer* _timer;
    GlslSplinePainter* _splinePainter;
    GlslStaticTextPainter* _staticTextPainter;
    OpenglVertexArray* _vertexArray;
    int _fps[5];
    std::chrono::high_resolution_clock _fpsClock;
    std::chrono::high_resolution_clock::time_point _referenceTimePoint;

    struct Config
    {
        bool fullscreen;
        bool vsyncOn;
        bool animatingOn;
        bool useTessellation;
        bool helpVisible;
        bool randomSplines;
        oak::SplineContinuityConstraint constraint;
        size_t curvesCount;
    };

    Config _config;

    struct OpenGLSupport
    {
        OpenGLSupport(int major = 0, int minor = 0)
            : _major(major),
              _minor(minor)
        {
        }
        bool hasVBO() const { return _major >= 3; }

        int _major;
        int _minor;
    };

    OpenGLSupport _openGL;
};

#endif // APPLICATION_CONTROLLER_H
