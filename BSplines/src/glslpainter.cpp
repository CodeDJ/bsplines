#include "glslpainter.h"

#include "glslshader.h"
#include "controlpointsglslprogram.h"
#include "splineglslprogram.h"

#include "oak/spline.h"
#include "oak/window.h"

#ifdef Q_OS_MAC
#include <OpenGL/gl3.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif

namespace
{
    const unsigned int g_DefaultStripsPerSegments = 30;
}


bool GlslSplinePainter::prepare()
{
    if (_isPrepared)
        return true;

    _isPrepared = addProgram<ControlPointsGlslProgram>() &&
                  (_useTessellation ? addProgram<SplineGlslProgram>() :
                                      addProgram<SplineGlslProgramGeomTess>()
                                      );

    long vertexBufferSize = _objects.at(0).controlPointsCount() * sizeof(oak::PointF);
    controlPointsProg()->vertexBuffer().alloc(vertexBufferSize);
    splinesProg()->vertexBuffer().alloc(vertexBufferSize);

    return _isPrepared;
}

GlslSplinePainter::GlslSplinePainter(std::vector<oak::Spline>& splines, bool useTessellation /*= true*/)
    : GlslPainter<oak::Spline>(splines),
      _useTessellation(useTessellation),
      _stripsPerSegment(g_DefaultStripsPerSegments)
{
    assert(splines.size());
}

void GlslSplinePainter::paint(oak::Window* window)
{
    if (!_isPrepared)
        return;

    controlPointsProg()->bind();
    for (auto iter = _objects.begin(); iter != _objects.end(); ++iter)
    {
        Color color(iter->color());
        controlPointsProg()->pointColor().set(color._r, color._g, color._b, color._a);

        GlslVertexBuffer& vertexBuffer = controlPointsProg()->vertexBuffer();
        vertexBuffer.set(reinterpret_cast<const void*>(iter->controlPoints().data()));
        vertexBuffer.enable();

        glPointSize(5.0f);

        glDrawArrays(GL_POINTS, 0, iter->controlPointsCount());

        vertexBuffer.disable();
    }
    controlPointsProg()->unbind();

    int i = 0;
    splinesProg()->bind();
    for (auto iter = _objects.begin(); iter != _objects.end(); ++iter, ++i)
    {
        unsigned int controlPointsCount = iter->controlPointsCount();
        Color color(iter->color());
        splinesProg()->pointColor().set(color._r, color._g, color._b, color._a);
        splinesProg()->stripsPerSegment().set(_stripsPerSegment);
        splinesProg()->segmentsPerSpline().set(iter->segments());
        splinesProg()->lineWidthAlphaX().set(2.0 / window->width() * iter->width());
        splinesProg()->lineWidthAlphaY().set(2.0 / window->height() * iter->width());

        GlslVertexBuffer& vertexBuffer = splinesProg()->vertexBuffer();
        if (_useTessellation)
        {
            vertexBuffer.set(reinterpret_cast<const void*>(iter->controlPoints().data()));
        }
        else
        {
            const std::vector<oak::PointF>& controlPoints = iter->controlPoints();
            static_cast<SplineGlslProgramGeomTess*>(splinesProg())->controlPoints().set(
                        reinterpret_cast<const float*>(controlPoints.data()),
                        controlPointsCount);

            float vertexPositions[4] = {
                controlPoints[0].x(), controlPoints[0].y(),
                controlPoints[controlPointsCount-1].x(), controlPoints[controlPointsCount-1].y(),
            };
            vertexBuffer.set(reinterpret_cast<void*>(vertexPositions), sizeof(vertexPositions));
        }
        vertexBuffer.enable();

        if (_useTessellation)
        {
            glPatchParameteri(GL_PATCH_VERTICES, controlPointsCount);
            glDrawArrays(GL_PATCHES, 0, controlPointsCount);
        }
        else
        {
            glDrawArrays(GL_LINE_STRIP, 0, 2);
        }

        vertexBuffer.disable();
    }
    splinesProg()->unbind();
}

void GlslSplinePainter::setUseTessellation(bool use)
{
    if (_useTessellation != use)
    {
        _isPrepared = false;
        _useTessellation = use;
    }
}

void GlslSplinePainter::setStripsPerSegment(unsigned int value)
{
    if (_stripsPerSegment != value)
    {
        _isPrepared = false;
        _stripsPerSegment = value;
    }
}

ControlPointsGlslProgram* GlslSplinePainter::controlPointsProg() const
{
    assert(_isPrepared);

    return static_cast<ControlPointsGlslProgram*>(_programs[0]);
}

SplineGlslProgram* GlslSplinePainter::splinesProg() const
{
    assert(_isPrepared);

    return static_cast<SplineGlslProgram*>(_programs[1]);
}

