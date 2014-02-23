#ifndef POINT_H
#define POINT_H

#include "geometricobject.h"

namespace geometry
{

#pragma pack(1)
struct PlainPoint
{
    float x;
    float y;
};

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
