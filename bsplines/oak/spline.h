#ifndef SPLINE_H
#define SPLINE_H

#include "geometricobject.h"
#include "pointf.h"
#include "color.h"

#include <vector>

namespace oak
{

enum class SplineContinuityConstraint {
    None, Continous1stDeriv, Continous2ndDeriv
};

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

    static void setConstraint(oak::SplineContinuityConstraint constraint);

public:
    static size_t segmentsForControlPoints(size_t controlPointsCount);
    static size_t controlPointsForSegments(size_t segmentCount);
    static size_t neededControlPoints(size_t availableControlPoints);

public:
    static std::vector<PointF>& ensureContinous1stDeriv(std::vector<PointF>& controlPoints);
    static std::vector<PointF>& ensureContinous2ndDeriv(std::vector<PointF>& controlPoints);

public:
    static std::vector<oak::Spline> generate(size_t segments, size_t count);
    static std::vector<oak::Spline> defaultSplines(size_t count);
    static void animate(std::vector<oak::Spline>& splines);

private:
    std::vector<PointF> _controlPoints;
    Color _color;
    float _width;
    static std::vector<PointF>& (*_constraint)(std::vector<PointF>&);

};

}
#endif // SPLINE_H
