#ifndef RECTF_H
#define RECTF_H

namespace oak
{
class RectF
{
public:
    RectF()
        : _x(0), _y(0), _width(0), _height(0)
    { }

    RectF(float x, float y, float width, float height)
        : _x(x), _y(y), _width(width), _height(height)
    { }

    float x() const { return _x; }
    float y() const { return _y; }
    float width() const { return _width; }
    float height() const { return _height; }

private:
    float _x;
    float _y;
    float _width;
    float _height;
};
}

#endif // RECTF_H
