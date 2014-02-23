#include "glslpainter.h"

#include "glslshader.h"
#include "controlpointsglslprogram.h"
#include "splineglslprogram.h"

#ifdef Q_OS_MAC
#include <OpenGL/gl3.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif



bool GlslSplinePainter::prepare()
{
    if (_isPrepared)
        return true;

    _isPrepared = addProgram<ControlPointsGlslProgram>() &&
                  (_useTessellation ? addProgram<SplineGlslProgramTess>() :
                                      addProgram<SplineGlslProgram>()
                                      );
    //controlPointsProg()->vertexBuffer().alloc(_objects.at(0).controlPoints().size());
    controlPointsProg()->vertexBuffer().alloc(100); // TODO: remove it and uncomment the previous line

    return _isPrepared;
}

GlslSplinePainter::GlslSplinePainter(std::vector<geometry::Spline>& splines)
    : GlslPainter<geometry::Spline>(splines),
      _useTessellation(true)
{
    assert(splines.size());
}

void GlslSplinePainter::paint()
{
    if (!_isPrepared)
        return;

    controlPointsProg()->bind();

    controlPointsProg()->pointColor().set(1.0, 0.0, 0.0, 1.0);
    GlslVertexBuffer& vertexBuffer = controlPointsProg()->vertexBuffer();

    float a[4] = {0.5, 0.5, 0.7, 0.7};

    vertexBuffer.set(a, sizeof(a));
    vertexBuffer.enable();
    glPointSize(10.0f);
    glDrawArrays(GL_POINTS, 0, 2);
    vertexBuffer.disable();

    controlPointsProg()->unbind();
}

void GlslSplinePainter::setUseTessellation(bool use)
{
    if (_useTessellation != use)
    {
        _isPrepared = false;
        _useTessellation = use;
    }
}

ControlPointsGlslProgram* GlslSplinePainter::controlPointsProg() const
{
    assert(_isPrepared);

    return static_cast<ControlPointsGlslProgram*>(_programs[0]);
}

