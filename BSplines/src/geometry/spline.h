#ifndef SPLINE_H
#define SPLINE_H

#include "geometricobject.h"
#include "point.h"
#include "../color.h"

#include <vector>

namespace geometry
{

class Spline : public GeometricObject
{
public:
    Spline(const std::vector<Point>& controlPoints, Color color, float width = 1.0);
    Spline(const PlainPoint* const controlPoints, unsigned int controlPointsCount, const Color& color, float width = 1.0);
    Spline(const Spline& spline);
    Spline& operator=(const Spline& spline);
    virtual ~Spline();

    static std::vector<geometry::Spline> generate(unsigned int segments, unsigned int count);
    static std::vector<geometry::Spline> defaultSplines(unsigned int count);
    static void animate(std::vector<geometry::Spline>& splines);

    unsigned int controlPointsCount() const;
    geometry::PlainPoint* controlPoints() const;
    Color color() const;
    float width() const;
    unsigned int segments() const;

private:
    PlainPoint* pointsToPlain(const std::vector<Point>& points);

private:
    geometry::PlainPoint* _controlPoints;
    unsigned int _controlPointsCount;
    Color _color;
    float _width;
};

}
#endif // SPLINE_H
