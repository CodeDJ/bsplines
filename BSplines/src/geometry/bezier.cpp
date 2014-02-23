#include "bezier.h"

namespace geometry
{

Bezier::Bezier(const std::vector<Point>& controlPoints)
    : GeometricObject(),
      _controlPoints(controlPoints)
{
}

Bezier::~Bezier()
{
}

const std::vector<Point>& Bezier::controlPoints() const
{
    return _controlPoints;
}

}


