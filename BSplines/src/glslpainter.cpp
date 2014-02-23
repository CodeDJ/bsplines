#include "glslpainter.h"

#include "glslshader.h"
#include "controlpointsglslprogram.h"
#include "splineglslprogram.h"
#include "applicationcontroller.h"

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
                  (_useTessellation ? addProgram<SplineGlslProgramTess>() :
                                      addProgram<SplineGlslProgram>()
                                      );

    long vertexBufferSize = _objects.at(0).controlPointsCount() * sizeof(geometry::PlainPoint);
    controlPointsProg()->vertexBuffer().alloc(vertexBufferSize);
    splinesProg()->vertexBuffer().alloc(vertexBufferSize);

    return _isPrepared;
}

GlslSplinePainter::GlslSplinePainter(ApplicationController* applicationController, std::vector<geometry::Spline>& splines, bool useTessellation /*= true*/)
    : GlslPainter<geometry::Spline>(splines),
      _useTessellation(useTessellation),
      _stripsPerSegment(g_DefaultStripsPerSegments),
      _applicationController(applicationController)
{
    assert(splines.size());
    assert(_applicationController);
}

#include <GLUT/glut.h>

void GlslSplinePainter::paint()
{
    if (!_isPrepared)
        return;

    controlPointsProg()->bind();
    for (auto iter = _objects.begin(); iter != _objects.end(); ++iter)
    {
        Color color(iter->color());
        controlPointsProg()->pointColor().set(color._r, color._g, color._b, color._a);

        GlslVertexBuffer& vertexBuffer = controlPointsProg()->vertexBuffer();
        vertexBuffer.set(reinterpret_cast<void*>(iter->controlPoints()));
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
        Color color(iter->color());
        splinesProg()->pointColor().set(color._r, color._g, color._b, color._a);
        splinesProg()->stripsPerSegment().set(_stripsPerSegment);
        splinesProg()->segmentsPerSpline().set(iter->segments());
        splinesProg()->lineWidthAlphaX().set(_applicationController->xRatio() * iter->width());
        splinesProg()->lineWidthAlphaY().set(_applicationController->yRatio() * iter->width());

        GlslVertexBuffer& vertexBuffer = splinesProg()->vertexBuffer();
        vertexBuffer.set(reinterpret_cast<void*>(iter->controlPoints()));
        vertexBuffer.enable();

        glPatchParameteri(GL_PATCH_VERTICES, iter->controlPointsCount());
        glDrawArrays(GL_PATCHES, 0, iter->controlPointsCount());

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

