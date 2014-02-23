#ifndef COLOR_H
#define COLOR_H


class Color
{
public:
    Color()
        : _r(0.0), _g(0.0), _b(0.0), _a(1.0)
    { }

    Color(float r, float g, float b, float a)
        : _r(r), _g(g), _b(b), _a(a)
    { }

public:
    float _r;
    float _g;
    float _b;
    float _a;
};

#endif // COLOR_H
