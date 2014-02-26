#ifndef POINTF_H
#define POINTF_H

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

public:
    inline float x() const { return _x; }
    inline float y() const  { return _y; }
    inline float& rx() { return _x; }
    inline float& ry() { return _y; }
    inline void setX(float x) { _x = x; }
    inline void setY(float y) { _y = y; }

public:
    float size() const; //*< Euclidian distance to origin

public:
    inline PointF operator- () const { return PointF(-_x, -_y); }
    inline PointF operator+ (const PointF& other) const { return PointF(_x + other._x, _y + other._y); }
    inline PointF operator- (const PointF& other) const { return PointF(_x - other._x, _y - other._y); }
    inline PointF operator* (float factor) const { return PointF(_x * factor, _y * factor); }
    inline PointF operator/ (float factor) const { return PointF(_x / factor, _y / factor); }

private:
    float _x;
    float _y;
};

}

#endif // POINTF_H
