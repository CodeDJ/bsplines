#ifndef COLOR_H
#define COLOR_H


namespace oak
{
class Color
{
public:
    Color()
        : _r(0.0), _g(0.0), _b(0.0), _a(1.0)
    { }

    Color(float r, float g, float b, float a)
        : _r(r), _g(g), _b(b), _a(a)
    { }

    static const Color& white() { static Color c(1.0, 1.0, 1.0, 1.0); return c; }
    static const Color& black() { static Color c(0.0, 0.0, 0.0, 1.0); return c; }
    static const Color& transparent() { static Color c(0.0, 0.0, 0.0, 0.0); return c; }

public:
    float _r;
    float _g;
    float _b;
    float _a;
};
}

#endif // COLOR_H
