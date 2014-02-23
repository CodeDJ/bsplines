#ifndef BEZIER_H
#define BEZIER_H

#include "geometricobject.h"
#include "point.h"

#include <vector>

namespace geometry
{

class Bezier : public GeometricObject
{
public:
    Bezier(const std::vector<Point>& controlPoints);
    virtual ~Bezier();

    const std::vector<Point>& controlPoints() const;

private:
    std::vector<Point> _controlPoints;
};

}
#endif // BEZIER_H
