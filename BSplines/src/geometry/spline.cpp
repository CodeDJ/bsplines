#include "spline.h"

namespace geometry
{

Spline::Spline(const std::vector<Point>& controlPoints)
    : GeometricObject(),
      _controlPoints(controlPoints)
{
}

Spline::~Spline()
{
}

const std::vector<Point>& Spline::controlPoints() const
{
    return _controlPoints;
}

}
