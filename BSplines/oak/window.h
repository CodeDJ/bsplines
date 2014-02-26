
#ifndef OAK_WINDOW_H
#define OAK_WINDOW_H

#include <string>
#include <functional>

namespace oak
{

class Window
{
public:
    typedef std::function<void(Window*)> PaintFunc;
    typedef std::function<void(Window*, unsigned char, int, int)> KeyFunc;
    typedef std::function<void(Window*, int, int)> ResizeFunc;

public:
    explicit Window(const std::string& name = std::string());
    Window(int x, int y, int w, int h, const std::string& name = std::string());
    virtual ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

public:
    void showFullScreen(bool fullscreen);

    void setVSync(bool enable);

    void repaint();

public:
    int x() const;
    int y() const;
    int width() const;
    int height() const;
    void getPos(int* x, int* y) const;
    void getSize(int* w, int* h) const;
    void getGeometry(int* x, int* y, int* w, int* h) const;
    void setSize(int w, int h);
    void setPos(int x, int y);

public:
    void onPaint(const PaintFunc& paintFunc);
    void onKey(const KeyFunc& keyFunc);
    void onResize(const ResizeFunc& resizeFunc);

protected:
    virtual void paintEvent();
    virtual void keyEvent(unsigned char key, int x, int y);
    virtual void resizeEvent(int width, int height);

private:
    void makeCurrent() const;

private:
    static void paintCb();
    static void keyCb(unsigned char key, int x, int y);
    static void resizeCb(int width, int height);

private:
    int _glutWindow;

    PaintFunc _paintFunc;
    KeyFunc _keyFunc;
    ResizeFunc _resizeFunc;
    int _lastWidth;
    int _lastHeight;
    int _lastX;
    int _lastY;
};

}

#endif // OAK_WINDOW_H
