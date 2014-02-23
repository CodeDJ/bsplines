#ifndef SPLINE_H
#define SPLINE_H

#include "geometricobject.h"
#include "point.h"

#include <vector>

namespace geometry
{

class Spline : public GeometricObject
{
public:
    Spline(const std::vector<Point>& controlPoints);
    virtual ~Spline();

    const std::vector<Point>& controlPoints() const;

private:
    std::vector<Point> _controlPoints;
};

}
#endif // SPLINE_H
