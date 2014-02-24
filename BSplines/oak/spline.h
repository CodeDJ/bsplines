#ifndef SPLINE_H
#define SPLINE_H

#include "geometricobject.h"
#include "point.h"
#include "color.h"

#include <vector>

namespace oak
{

class Spline : public GeometricObject
{
public:
    Spline(const std::vector<PointF>& controlPoints, Color color, float width = 1.0);
    virtual ~Spline();

    static std::vector<oak::Spline> generate(unsigned int segments, unsigned int count);
    static std::vector<oak::Spline> defaultSplines(unsigned int count);
    static void animate(std::vector<oak::Spline>& splines);

    unsigned int controlPointsCount() const;
    const std::vector<PointF>& controlPoints() const;
    Color color() const;
    float width() const;
    unsigned int segments() const;

private:
    std::vector<PointF> _controlPoints;
    Color _color;
    float _width;
};

}
#endif // SPLINE_H
