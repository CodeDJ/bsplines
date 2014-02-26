
#ifndef APPLICATION_CONTROLLER_H
#define APPLICATION_CONTROLLER_H

namespace oak
{
class Application;
class Window;
class Timer;
}

class GlslSplinePainter;
class GlslStaticTextPainter;
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

    void setFullscreen(bool on);
    void setVSyncOn(bool on);
    void setAnimatingOn(bool on);
    void setUseTessellation(bool use);
    void setHelpVisible(bool visible);
    void setRandomSplines(bool random);

private:
    oak::Application* _app;
    oak::Window* _window;
    oak::Timer* _timer;
    GlslSplinePainter* _splinePainter;
    GlslStaticTextPainter* _staticTextPainter;
    OpenglVertexArray* _vertexArray;

    struct Config
    {
        bool fullscreen;
        bool vsyncOn;
        bool animatingOn;
        bool useTessellation;
        bool helpVisible;
        bool randomSplines;
    };

    Config _config;
};

#endif // APPLICATION_CONTROLLER_H
