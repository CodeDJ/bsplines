#include "glslpainter.h"

#include "glslshader.h"
#include "controlpointsglslprogram.h"
#include "splineglslprogram.h"
#include "texture2dglslprogram.h"
#include "texttexture.h"

#include "oak/spline.h"
#include "oak/window.h"
#include "oak/pointf.h"

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

    unsigned int controlPointsCount = _objects.at(0).controlPointsCount();
    unsigned int segmentCount = _objects.at(0).segmentCount();

    _isPrepared = addProgram<ControlPointsGlslProgram>();
    if (!_isPrepared)
        return false;

    GlslProgram* program = _useTessellation ? new SplineGlslProgram(segmentCount, g_DefaultStripsPerSegments) :
                                              new SplineGlslProgramGeomTess(segmentCount, g_DefaultStripsPerSegments);
    program->create();
    if (!program->link())
        return false;
    _programs.push_back(program);

    long vertexBufferSize = controlPointsCount * sizeof(oak::PointF);
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
        oak::Color color(iter->color());
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
        oak::Color color(iter->color());
        splinesProg()->pointColor().set(color._r, color._g, color._b, color._a);
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

GlslMenuPainter::GlslMenuPainter(const oak::Menu& menu)
    : GlslPainter<oak::Menu>(menu),
      _textTexture(0)
{
}

GlslMenuPainter::~GlslMenuPainter()
{
    delete _textTexture;
}

bool GlslMenuPainter::prepare()
{
    if (_isPrepared)
        return true;

    _isPrepared = addProgram<Texture2dGlslProgram>();
    if (!_isPrepared)
        return false;

    if (_isPrepared)
    {
        long vertexBufferSize = 4 * sizeof(oak::PointF) * 2;
        textureProg()->vertexBuffer().alloc(vertexBufferSize);
        textureProg()->texture2D().alloc();

        oak::RectF rect = _objects[0].rect();
        _textTexture = new TextTexture(rect.width(), rect.height());
    }

    _isPrepared = addProgram<ControlPointsGlslProgram>();
    if (!_isPrepared)
        return false;

    controlPointsProg()->vertexBuffer().alloc(4 * sizeof(oak::PointF));

    return _isPrepared;
}

void GlslMenuPainter::paint(oak::Window* window)
{
    if (!_isPrepared)
        return;

    float xPixel = 2.0f / window->width();
    float yPixel = 2.0f / window->height();
    oak::RectF rect = _objects[0].rect();

    {
        controlPointsProg()->bind();
        GlslVertexBuffer& vertexBuffer = controlPointsProg()->vertexBuffer();

        controlPointsProg()->pointColor().set(1.0, 0, 0, 1.0);



        oak::PointF rectVertices[4] = { { rect.x()                      , rect.y()                       },
                                        { rect.x() + rect.width()*xPixel, rect.y()                       },
                                        { rect.x()                      , rect.y() - rect.height()*yPixel},
                                        { rect.x() + rect.width()*xPixel, rect.y() - rect.height()*yPixel},
                                  };
        vertexBuffer.set(reinterpret_cast<const void*>(rectVertices));
        vertexBuffer.enable();

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        vertexBuffer.disable();
        controlPointsProg()->unbind();
    }


    {
        // Draw text
        textureProg()->bind();
        GlslVertexBuffer& vertexBuffer = textureProg()->vertexBuffer();

        oak::PointF vertices[8] = { { rect.x() * xPixel                      , rect.y() * yPixel                       }, {0           , 0},
                                    { rect.x() * xPixel + rect.width()*xPixel, rect.y() * yPixel                       }, {rect.width(), 0},
                                    { rect.x() * xPixel                      , rect.y() * yPixel - rect.height()*yPixel}, {0           , rect.height()},
                                    { rect.x() * xPixel + rect.width()*xPixel, rect.y() * yPixel - rect.height()*yPixel}, {rect.width(), rect.height()}
                                  };

        vertexBuffer.set(reinterpret_cast<const void*>(vertices));
        vertexBuffer.enable();


        _textTexture->drawText("xxx", 50, 50, oak::Color(0.0, 1.0, 0.0, 1.0));
        _textTexture->drawText("abc", 50, 60, oak::Color(0.0, 1.0, 0.0, 1.0));
        _textTexture->drawText("123", 50, 70, oak::Color(0.0, 1.0, 0.0, 1.0));

        textureProg()->texture2D().bind();
        textureProg()->texture2D().set(_textTexture->data(), _textTexture->width(), _textTexture->height());


        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        vertexBuffer.disable();

        textureProg()->unbind();
    }
}

Texture2dGlslProgram* GlslMenuPainter::textureProg() const
{
    return static_cast<Texture2dGlslProgram*>(_programs[0]);
}

ControlPointsGlslProgram* GlslMenuPainter::controlPointsProg() const
{
    return static_cast<ControlPointsGlslProgram*>(_programs[1]);
}
