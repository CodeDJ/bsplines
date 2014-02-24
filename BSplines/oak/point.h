#ifndef POINT_H
#define POINT_H

namespace oak
{

class PointF
{
public:
    PointF()
        : _x(0), _y(0)
    { }

    PointF(float x, float y)
        : _x(x), _y(y)
    { }

    inline float x() const { return _x; }
    inline float y() const  { return _y; }
    inline float& rx() { return _x; }
    inline float& ry() { return _y; }
    inline void setX(float x) { _x = x; }
    inline void setY(float y) { _y = y; }

private:
    float _x;
    float _y;
};

}

#endif // POINT_H
