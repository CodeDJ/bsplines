#include "glslsplinepainter.h"

#include "glslshader.h"
#include "controlpointsglslprogram.h"
#include "splineglslprogram.h"
#include "splineglslprogram21.h"
#include "texture2dglslprogram.h"
#include "texttexture.h"

#include "oak/spline.h"
#include "oak/window.h"
#include "oak/pointf.h"
#include "oak/log.h"

#ifdef Q_OS_MAC
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif

namespace
{
    static const unsigned int g_DefaultStripsPerSegments = 30;
}


GlslSplinePainter::GlslSplinePainter(GlslSplinePainterVersion v, std::vector<oak::Spline>& splines)
    : GlslPainter<oak::Spline>(splines),
      _v(v),
      _stripsPerSegment(g_DefaultStripsPerSegments)
{
    assert(splines.size());
}

GlslSplinePainter::GlslSplinePainter(GlslSplinePainterVersion v)
    : GlslPainter<oak::Spline>(std::vector<oak::Spline>()),
      _v(v),
      _stripsPerSegment(g_DefaultStripsPerSegments)
{

}

bool GlslSplinePainter::prepare()
{
    switch(_v)
    {
    case GlslSplinePainterVersion::Basic2_1:
        return prepare2_1();
    case GlslSplinePainterVersion::Basic3_2:
        return prepare3_2(false);
    case GlslSplinePainterVersion::Tess4_0:
        return prepare3_2(true);
    default:
        assert(false);
        break;
    }

    return false;
}

bool GlslSplinePainter::prepare2_1()
{
    if (_isPrepared)
        return true;

    if (_objects.empty())
        return false;

    unsigned int controlPointsCount = _objects.at(0).controlPointsCount();
    unsigned int segmentCount = _objects.at(0).segmentCount();

    _isPrepared = addProgram<SplineGlslProgram21>(segmentCount, g_DefaultStripsPerSegments);
    if (!_isPrepared)
        return false;

    long vertexBufferSize = controlPointsCount * sizeof(oak::PointF);
    splinesProg21()->vertexBuffer().alloc(vertexBufferSize);

    return _isPrepared;
}

bool GlslSplinePainter::prepare3_2(bool withTess)
{
    if (_isPrepared)
        return true;

    if (_objects.empty())
        return false;

    unsigned int controlPointsCount = _objects.at(0).controlPointsCount();
    unsigned int segmentCount = _objects.at(0).segmentCount();

    _isPrepared = addProgram<ControlPointsGlslProgram>();
    if (!_isPrepared)
        return false;
    _isPrepared = withTess ? addProgram<SplineGlslProgram>(segmentCount, g_DefaultStripsPerSegments) :
                             addProgram<SplineGlslProgramGeomTess>(segmentCount, g_DefaultStripsPerSegments);
    if (!_isPrepared)
        return false;

    long vertexBufferSize = controlPointsCount * sizeof(oak::PointF);
    controlPointsProg()->vertexBuffer().alloc(vertexBufferSize);
    splinesProg()->vertexBuffer().alloc(vertexBufferSize);

    return _isPrepared;
}

void GlslSplinePainter::paint(oak::Window* window)
{
    switch(_v)
    {
    case GlslSplinePainterVersion::Basic2_1:
        paint2_1(window);
        break;
    case GlslSplinePainterVersion::Basic3_2:
        paint3_2(window);
        break;
    case GlslSplinePainterVersion::Tess4_0:
        paint4_0(window);
        break;
    default:
        assert(false);
        break;
    }
}

void GlslSplinePainter::paint2_1(oak::Window* window)
{
    UNUSED(window);
    if (!_isPrepared)
        return;

    splinesProg21()->bind();
    splinesProg21()->passThrough().set(1);
    for (auto iter = _objects.begin(); iter != _objects.end(); ++iter)
    {
        oak::Color color(iter->color());
        splinesProg21()->pointColor().set(color._r, color._g, color._b, color._a);

        GlslVertexBuffer& vertexBuffer = splinesProg21()->vertexBuffer();
        vertexBuffer.set(reinterpret_cast<const void*>(iter->controlPoints().data()));
        vertexBuffer.enable();

        glPointSize(5.0f);

        glDrawArrays(GL_POINTS, 0, iter->controlPointsCount());
        CHECK_OPENGL_ERROR();

        vertexBuffer.disable();
    }

    splinesProg21()->passThrough().set(0);
    for (auto iter = _objects.begin(); iter != _objects.end(); ++iter)
    {
        unsigned int controlPointsCount = iter->controlPointsCount();
        oak::Color color(iter->color());
        splinesProg21()->pointColor().set(color._r, color._g, color._b, color._a);

        const std::vector<oak::PointF>& controlPoints = iter->controlPoints();
        splinesProg21()->controlPoints().set(
                    reinterpret_cast<const float*>(controlPoints.data()),
                    controlPointsCount);

        glLineWidth(3.0);
        glBegin(GL_LINE_STRIP);
        float step = 1.0f/30.0f;
        for (int i = 0; i < iter->segmentCount(); ++i)
        {
            for (float t = 0.0f; t<=1.0f; t += step)
            {
                glVertex3f(t, float(i), 0.0f);
            }
            glVertex3f(1.0f, float(i), 0.0f);
        }
        glEnd();

    }

    /*
    GlslVertexBuffer& vertexBuffer = splinesProg21()->vertexBuffer();
    const std::vector<oak::PointF>& controlPoints = iter->controlPoints();
    float vertexPositions[4] = {
        controlPoints[0].x(), controlPoints[0].y(),
        controlPoints[controlPointsCount-1].x(), controlPoints[controlPointsCount-1].y(),
    };
    vertexBuffer.set(reinterpret_cast<void*>(vertexPositions), sizeof(vertexPositions));
    vertexBuffer.enable();

    glDrawArrays(GL_LINE_STRIP, 0, 2);
    CHECK_OPENGL_ERROR();

    vertexBuffer.disable();
    */

    splinesProg21()->unbind();
}

void GlslSplinePainter::paintPoints3_2(oak::Window* window)
{
    UNUSED(window);

    if (!_isPrepared)
        return;

    controlPointsProg()->bind();
    for (auto iter = _objects.begin(); iter != _objects.end(); ++iter)
    {
        oak::Color color(iter->color());
        controlPointsProg()->pointColor().set(color._r, color._g, color._b, color._a);

        GlslVertexBuffer& vertexBuffer = controlPointsProg()->vertexBuffer();
        vertexBuffer.set(reinterpret_cast<const void*>(iter->controlPoints().data()));
        vertexBuffer.enable();

        glPointSize(5.0f);

        glDrawArrays(GL_POINTS, 0, iter->controlPointsCount());
        CHECK_OPENGL_ERROR();

        vertexBuffer.disable();
    }
    controlPointsProg()->unbind();
}

void GlslSplinePainter::paint3_2(oak::Window* window)
{
    if (!_isPrepared)
        return;
    splinesProg()->bind();
    for (auto iter = _objects.begin(); iter != _objects.end(); ++iter)
    {
        unsigned int controlPointsCount = iter->controlPointsCount();
        oak::Color color(iter->color());
        splinesProg()->pointColor().set(color._r, color._g, color._b, color._a);
        splinesProg()->lineWidthAlphaX().set(2.0f / window->width() * iter->width());
        splinesProg()->lineWidthAlphaY().set(2.0f / window->height() * iter->width());

        GlslVertexBuffer& vertexBuffer = splinesProg()->vertexBuffer();
        const std::vector<oak::PointF>& controlPoints = iter->controlPoints();
        static_cast<SplineGlslProgramGeomTess*>(splinesProg())->controlPoints().set(
                    reinterpret_cast<const float*>(controlPoints.data()),
                    controlPointsCount);

        float vertexPositions[4] = {
            controlPoints[0].x(), controlPoints[0].y(),
            controlPoints[controlPointsCount-1].x(), controlPoints[controlPointsCount-1].y(),
        };
        vertexBuffer.set(reinterpret_cast<void*>(vertexPositions), sizeof(vertexPositions));
        vertexBuffer.enable();

        glDrawArrays(GL_LINE_STRIP, 0, 2);
        CHECK_OPENGL_ERROR();

        vertexBuffer.disable();
    }
    splinesProg()->unbind();
}

void GlslSplinePainter::paint4_0(oak::Window* window)
{
    if (!_isPrepared)
        return;
    paintPoints3_2(window);

    splinesProg()->bind();
    for (auto iter = _objects.begin(); iter != _objects.end(); ++iter)
    {
        unsigned int controlPointsCount = iter->controlPointsCount();
        oak::Color color(iter->color());
        splinesProg()->pointColor().set(color._r, color._g, color._b, color._a);
        splinesProg()->lineWidthAlphaX().set(2.0f / window->width() * iter->width());
        splinesProg()->lineWidthAlphaY().set(2.0f / window->height() * iter->width());

        GlslVertexBuffer& vertexBuffer = splinesProg()->vertexBuffer();
        vertexBuffer.set(reinterpret_cast<const void*>(iter->controlPoints().data()));
        vertexBuffer.enable();

        glPatchParameteri(GL_PATCH_VERTICES, controlPointsCount);
        glDrawArrays(GL_PATCHES, 0, controlPointsCount);
        CHECK_OPENGL_ERROR();

        vertexBuffer.disable();
    }
    splinesProg()->unbind();
}

void GlslSplinePainter::setSplines(std::vector<oak::Spline>& splines)
{
    _isPrepared = false;
    _objects = splines;
}

void GlslSplinePainter::setVersion(GlslSplinePainterVersion v)
{
    if (_v != v)
    {
        _isPrepared = false;
        _v = v;
        for (auto iter = _programs.begin(); iter != _programs.end(); ++iter)
        {
            delete *iter;
        }
        _programs.clear();
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
    assert(_isPrepared && (_v == GlslSplinePainterVersion::Basic3_2 || _v == GlslSplinePainterVersion::Tess4_0));

    return static_cast<ControlPointsGlslProgram*>(_programs[0]);
}

SplineGlslProgram* GlslSplinePainter::splinesProg() const
{
    assert(_isPrepared && (_v == GlslSplinePainterVersion::Basic3_2 || _v == GlslSplinePainterVersion::Tess4_0));

    return static_cast<SplineGlslProgram*>(_programs[1]);
}

SplineGlslProgram21* GlslSplinePainter::splinesProg21() const
{
    assert(_isPrepared && _v == GlslSplinePainterVersion::Basic2_1);

    return static_cast<SplineGlslProgram21*>(_programs[0]);
}




