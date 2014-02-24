#include "spline.h"

#include <assert.h>

namespace oak
{

const float g_DefaultCurveAlpha = 0.5;
const float g_DefaultCurveWidth = 3.0;
const unsigned int g_DefaultCurveSegmentsCount = 4;

Spline::Spline(const std::vector<PointF>& controlPoints, Color color, float width /*= 1.0*/)
    : _controlPoints(controlPoints),
      _color(color),
      _width(width)
{
    assert(segments() * 3 + 1 == _controlPoints.size());
}

Spline::~Spline()
{
}

const std::vector<PointF>& Spline::controlPoints() const
{
    return _controlPoints;
}

unsigned int Spline::controlPointsCount() const
{
    return _controlPoints.size();
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
    return (_controlPoints.size() - 1) / 3;
}

/*static */
std::vector<oak::Spline> Spline::generate(unsigned int segments, unsigned int count)
{
    assert(count);

    std::vector<PointF> controlPoints;
    std::vector<oak::Spline> splines;
    const unsigned int controlPointsCount = segments * 3 + 1;
    controlPoints.reserve(controlPointsCount);

    for (unsigned int i=0; i<count; ++i)
    {
        controlPoints.clear();
        for (unsigned int j=0; j<controlPointsCount; ++j)
        {
            controlPoints[j] = PointF(((rand()%2001) / 1000.0) - 1.0, ((rand()%2001) / 1000.0) - 1.0);
        }

        splines.push_back(Spline(controlPoints,
                          Color((rand()%101) / 100.0, (rand()%101) / 100.0, (rand()%101) / 100.0, g_DefaultCurveAlpha),
                          g_DefaultCurveWidth));
    }

    return splines;
}

/*static */
std::vector<oak::Spline> Spline::defaultSplines(unsigned int count)
{
    assert(count);

    std::vector<PointF> staticPoints = {
        {-0.5f,  0.0f}, {-0.5f,  0.5f}, {-0.2f, 0.5f}, {-0.2f, 0.0f}, {-0.2f, -0.5f},
        { 0.2f, -0.5f}, { 0.2f,  0.0f}, { 0.2f, 0.5f}, { 0.5f, 0.5f}, { 0.5f,  0.0f},
        { 0.5f, -0.5f}, { 0.7f, -0.5f}, { 0.7f, 0.0f}
    };

    std::vector<oak::Spline> splines;
    for (unsigned int i=0; i<count; ++i)
    {
        splines.push_back(Spline(staticPoints,
                                 Color((rand()%101) / 100.0, (rand()%101) / 100.0, (rand()%101) / 100.0, g_DefaultCurveAlpha),
                                 g_DefaultCurveWidth)
                          );
    }

    return splines;
}

/*static */
void Spline::animate(std::vector<oak::Spline>& splines)
{
    for (auto spline = splines.begin(); spline != splines.end(); ++spline)
    {
        for (auto point = spline->_controlPoints.begin(); point != spline->_controlPoints.end(); ++point)
        {
            point->rx() += (rand()%100 - 50)/10000.0;
            point->ry() += (rand()%100 - 50)/10000.0;
        }
    }
}

}
