#ifndef POINT_H
#define POINT_H

#include "geometricobject.h"

namespace geometry
{

class Point : public GeometricObject
{
public:
    Point();
    Point(float x, float y);

public:
    float _x;
    float _y;
};

}

#endif // POINT_H
