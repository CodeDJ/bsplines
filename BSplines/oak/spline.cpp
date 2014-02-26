#include "spline.h"

#include <assert.h>
#include <math.h>

#define SPLINE_CONSTRAINT ensureContinous1stDeriv
//#define SPLINE_CONSTRAINT ensureContinous2ndDeriv

namespace oak
{

static const float g_DefaultCurveAlpha = 0.5;
static const float g_DefaultCurveWidth = 3.0;
static const size_t g_DefaultCurveSegmentsCount = 4;

Spline::Spline(const std::vector<PointF>& controlPoints, Color color, float width /*= 1.0*/)
    : _controlPoints(controlPoints),
      _color(color),
      _width(width)
{
    assert(neededControlPoints(_controlPoints.size()) == _controlPoints.size());
}

Spline::~Spline()
{
}

const std::vector<PointF>& Spline::controlPoints() const
{
    return _controlPoints;
}

size_t Spline::controlPointsCount() const
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

size_t Spline::segmentCount() const
{
    return segmentsForControlPoints(_controlPoints.size());
}

size_t Spline::segmentsForControlPoints(size_t controlPointsCount)
{
    return controlPointsCount ? (controlPointsCount - 1) / 3 : 0;
}

size_t Spline::controlPointsForSegments(size_t segmentCount)
{
    return segmentCount * 3 + 1;
}

size_t Spline::neededControlPoints(size_t availableControlPoints)
{
    return controlPointsForSegments(segmentsForControlPoints(availableControlPoints));
}

std::vector<PointF>& Spline::ensureContinous1stDeriv(std::vector<PointF>& controlPoints)
{
    const size_t segments = segmentsForControlPoints(controlPoints.size());
    for (size_t i = 1; i < segments; i++)
    {
        const PointF& left = controlPoints[i*3-1];
        const PointF& center = controlPoints[i*3];
        PointF& right = controlPoints[i*3+1];

        PointF deltaLeft = center - left;
        float leftSize = deltaLeft.size();
        if (leftSize)
        {
            float rightSize = (right - center).size();
            float ratio = rightSize / leftSize;
            right = center + deltaLeft * ratio;
        }
        else
        {
            right = center;
        }
    }
    return controlPoints;
}

std::vector<PointF>& Spline::ensureContinous2ndDeriv(std::vector<PointF>& controlPoints)
{
    const size_t segments = segmentsForControlPoints(controlPoints.size());
    for (size_t i = 1; i < segments; i++)
    {
        const PointF& left = controlPoints[i*3-1];
        const PointF& center = controlPoints[i*3];
        PointF& right = controlPoints[i*3+1];
        right = center * 2 - left;
    }
    return controlPoints;
}

/*static */
std::vector<oak::Spline> Spline::generate(size_t segments, size_t count)
{
    assert(count);

    std::vector<PointF> controlPoints;
    std::vector<oak::Spline> splines;
    const size_t controlPointsCount = controlPointsForSegments(segments);
    controlPoints.reserve(controlPointsCount);

    for (size_t i=0; i < count; ++i)
    {
        controlPoints.clear();
        for (size_t j=0; j < controlPointsCount; ++j)
        {
            controlPoints.push_back(PointF(rand() * 2.0f / RAND_MAX - 1.0f, rand() * 2.0f / RAND_MAX - 1.0f));
        }
#ifdef SPLINE_CONSTRAINT
        SPLINE_CONSTRAINT(controlPoints);
#endif

        splines.push_back(Spline(controlPoints,
                          Color(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, g_DefaultCurveAlpha),
                          g_DefaultCurveWidth));
    }

    return splines;
}

/*static */
std::vector<oak::Spline> Spline::defaultSplines(size_t count)
{
    assert(count);

    std::vector<PointF> staticPoints = {
        {-0.5f,  0.0f}, {-0.5f,  0.5f}, {-0.2f, 0.5f}, {-0.2f, 0.0f}, {-0.2f, -0.5f},
        { 0.2f, -0.5f}, { 0.2f,  0.0f}, { 0.2f, 0.5f}, { 0.5f, 0.5f}, { 0.5f,  0.0f},
        { 0.5f, -0.5f}, { 0.7f, -0.5f}, { 0.7f, 0.0f}
    };
#ifdef SPLINE_CONSTRAINT
        SPLINE_CONSTRAINT(staticPoints);
#endif

    std::vector<oak::Spline> splines;
    for (unsigned int i=0; i<count; ++i)
    {
        splines.push_back(Spline(staticPoints,
                                 Color(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, g_DefaultCurveAlpha),
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
            float dx = (rand() * 100.0f / RAND_MAX - 50)/10000.0f;
            float newX = point->x() + dx;
            if ((newX > 1) || (newX < -1))
            {
                dx = -dx;
            }
            float dy = (rand() * 100.0f / RAND_MAX - 50)/10000.0f;
            float newY = point->y() + dy;
            if ((newY > 1) || (newY < -1))
            {
                dy = -dy;
            }
            point->rx() += dx;
            point->ry() += dy;
        }
#ifdef SPLINE_CONSTRAINT
        SPLINE_CONSTRAINT(spline->_controlPoints);
#endif

    }
}

}
