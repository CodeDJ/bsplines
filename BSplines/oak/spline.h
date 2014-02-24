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

    size_t controlPointsCount() const;
    const std::vector<PointF>& controlPoints() const;
    Color color() const;
    float width() const;
    size_t segmentCount() const;

public:
    static size_t segmentsForControlPoints(size_t controlPointsCount);
    static size_t controlPointsForSegments(size_t segmentCount);
    static size_t neededControlPoints(size_t availableControlPoints);

public:
    static void ensureContinous1stDeriv(std::vector<PointF>& controlPoints);
    static void ensureContinous2ndDeriv(std::vector<PointF>& controlPoints);

public:
    static std::vector<oak::Spline> generate(size_t segments, size_t count);
    static std::vector<oak::Spline> defaultSplines(size_t count);
    static void animate(std::vector<oak::Spline>& splines);

private:
    std::vector<PointF> _controlPoints;
    Color _color;
    float _width;
};

}
#endif // SPLINE_H
