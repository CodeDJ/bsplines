#include "spline.h"

#include <assert.h>

namespace geometry
{

const float g_DefaultCurveAlpha = 0.5;
const float g_DefaultCurveWidth = 3.0;
const unsigned int g_DefaultCurveSegmentsCount = 4;

Spline::Spline(const std::vector<Point>& controlPoints, Color color, float width /*= 1.0*/)
    : GeometricObject(),
      _color(color),
      _width(width)
{
    _controlPoints = pointsToPlain(controlPoints);

    assert(segments() * 3 + 1 == _controlPointsCount);
}

Spline::Spline(const PlainPoint* const controlPoints, unsigned int controlPointsCount, const Color& color, float width /*= 1.0*/)
    : GeometricObject(),
      _controlPointsCount(controlPointsCount),
      _color(color),
      _width(width)
{
    assert(controlPoints && controlPointsCount);

    _controlPoints = new PlainPoint[controlPointsCount];
    memcpy(_controlPoints, controlPoints, controlPointsCount * sizeof(PlainPoint));

    assert(segments() * 3 + 1 == _controlPointsCount);
}

Spline::Spline(const Spline& spline)
{
    _controlPoints = 0;
    *this = spline;
}

Spline& Spline::operator=(const Spline& spline)
{
    delete[] _controlPoints;
    _controlPointsCount = spline._controlPointsCount;
    _controlPoints = new PlainPoint[_controlPointsCount];
    memcpy(_controlPoints, spline._controlPoints, _controlPointsCount * sizeof(PlainPoint));
    _color = spline._color;
    _width = spline._width;
    return *this;
}


Spline::~Spline()
{
    delete[] _controlPoints;
}

PlainPoint* Spline::pointsToPlain(const std::vector<Point>& points)
{
    _controlPointsCount = points.size();

    assert(_controlPointsCount );

    PlainPoint* plainPoints = new PlainPoint[_controlPointsCount];
    int i = 0;
    for(auto iter = points.begin(); iter != points.end(); ++iter, ++i )
    {
        plainPoints[i].x = iter->_x;
        plainPoints[i].y = iter->_y;
    }
    return plainPoints;
}

geometry::PlainPoint* Spline::controlPoints() const
{
    return _controlPoints;
}

unsigned int Spline::controlPointsCount() const
{
    return _controlPointsCount;
}

Color Spline::color() const
{
    return _color;
}

float Spline::width() const
{
    return _width;
}

unsigned int Spline::segments() const
{
    return (_controlPointsCount - 1) / 3;
}

/*static */
std::vector<geometry::Spline> Spline::generate(unsigned int segments, unsigned int count)
{
    assert(count);

    std::vector<Point> controlPoints;
    std::vector<geometry::Spline> splines;
    const unsigned int controlPointsCount = segments * 3 + 1;
    for (unsigned int i=0; i<count; ++i)
    {
        controlPoints.clear();
        for (unsigned int j=0; j<controlPointsCount; ++j)
        {
            controlPoints.push_back(Point(((rand()%2001) / 1000.0) - 1.0, ((rand()%2001) / 1000.0) - 1.0));
        }

        splines.push_back(Spline(controlPoints,
                          Color((rand()%101) / 100.0, (rand()%101) / 100.0, (rand()%101) / 100.0, g_DefaultCurveAlpha),
                          g_DefaultCurveWidth));
    }

    return splines;
}

/*static */
std::vector<geometry::Spline> Spline::defaultSplines(unsigned int count)
{
    assert(count);

    PlainPoint staticPoints[g_DefaultCurveSegmentsCount*3 + 1] = {
        {-0.5, 0.0}, {-0.5, 0.5}, {-0.2, 0.5}, {-0.2, 0.0}, {-0.2, -0.5},
        {0.2, -0.5}, {0.2, 0.0}, {0.2, 0.5}, {0.5, 0.5}, {0.5, 0.0},
        {0.5, -0.5}, {0.7, -0.5}, {0.7, 0.0}
    };

    std::vector<geometry::Spline> splines;
    for (unsigned int i=0; i<count; ++i)
    {
        splines.push_back(Spline(staticPoints, g_DefaultCurveSegmentsCount * 3 + 1,
                                 Color((rand()%101) / 100.0, (rand()%101) / 100.0, (rand()%101) / 100.0, g_DefaultCurveAlpha),
                                 g_DefaultCurveWidth)
                          );
    }

    return splines;
}

/*static */
void Spline::animate(std::vector<geometry::Spline>& splines)
{
    for (auto iter = splines.begin(); iter != splines.end(); ++iter)
    {
        for (unsigned int i=0; i<iter->_controlPointsCount; ++i)
        {
            iter->_controlPoints[i].x += (rand()%100 - 50)/10000.0;
            iter->_controlPoints[i].y += (rand()%100 - 50)/10000.0;
        }
    }
}

}
