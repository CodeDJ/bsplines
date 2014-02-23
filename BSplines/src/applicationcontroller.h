
#ifndef APPLICATION_CONTROLLER_H
#define APPLICATION_CONTROLLER_H

namespace oak
{
class Application;
class Window;
class Timer;
}

class GlslSplinePainter;

class ApplicationController
{
public:
    explicit ApplicationController(oak::Application* application);
    ~ApplicationController();

    ApplicationController(const ApplicationController&) = delete;
    ApplicationController& operator=(const ApplicationController&) = delete;

private:
    void onPaint(oak::Window* window);

private:
    oak::Application* _app;
    oak::Window* _window;
    oak::Timer* _timer;
    GlslSplinePainter* _splinePainter;
};

#endif // APPLICATION_CONTROLLER_H
