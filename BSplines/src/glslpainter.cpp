#include "glslpainter.h"

#include "glslshader.h"
#include "controlpointsglslprogram.h"
#include "splineglslprogram.h"
#include "texture2dglslprogram.h"
#include "texttexture.h"

#include "oak/spline.h"
#include "oak/window.h"
#include "oak/pointf.h"
#include "oak/log.h"

#ifdef Q_OS_MAC
#include <OpenGL/gl3.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif

namespace
{
    static const unsigned int g_DefaultStripsPerSegments = 30;
    static const int g_DefaultLineSpacing = 10;
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
    if (!(_isPrepared = program->link()))
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
        CHECK_OPENGL_ERROR();

        vertexBuffer.disable();
    }
    controlPointsProg()->unbind();

    splinesProg()->bind();
    for (auto iter = _objects.begin(); iter != _objects.end(); ++iter)
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
            CHECK_OPENGL_ERROR();
        }
        else
        {
            glDrawArrays(GL_LINE_STRIP, 0, 2);
            CHECK_OPENGL_ERROR();
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

GlslStaticTextPainter::GlslStaticTextPainter(const oak::StaticText& staticText, const oak::Color& backgroundColor)
    : GlslPainter<oak::StaticText>(staticText),
      _staticText(staticText),
      _backgroundColor(backgroundColor),
      _textTexture(0)
{
}

GlslStaticTextPainter::~GlslStaticTextPainter()
{
    delete _textTexture;
}

bool GlslStaticTextPainter::prepare()
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

    return _isPrepared;
}

void GlslStaticTextPainter::paint(oak::Window* window)
{
    if (!_isPrepared)
        return;

    float xPixel = 2.0f / window->width();
    float yPixel = 2.0f / window->height();
    oak::RectF rect = _objects[0].rect();

    // Draw text
    textureProg()->bind();
    GlslVertexBuffer& vertexBuffer = textureProg()->vertexBuffer();

    oak::PointF vertices[8] = { { rect.x()               , rect.y()                }, {0           , 0},
                                { rect.x() + rect.width(), rect.y()                }, {rect.width(), 0},
                                { rect.x()               , rect.y() + rect.height()}, {0           , rect.height()},
                                { rect.x() + rect.width(), rect.y() + rect.height()}, {rect.width(), rect.height()}
                              };

    vertexBuffer.set(reinterpret_cast<const void*>(vertices));
    vertexBuffer.enable();

    _textTexture->clear(_backgroundColor);
    std::vector<std::string> lines = _staticText.lines();
    int y = rect.height();
    for(auto iter = lines.begin(); iter != lines.end(); ++iter)
    {
        oak::RectF boundingRect = _textTexture->boundingRect(*iter);
        y -= boundingRect.height();
        _textTexture->drawText(*iter, 0, y, oak::Color(0.0, 1.0, 0.0, 1.0));
        y -= g_DefaultLineSpacing;
    }

    textureProg()->texture2D().bind();
    textureProg()->texture2D().set(_textTexture->data(), _textTexture->width(), _textTexture->height());

    textureProg()->alphaX().set(xPixel);
    textureProg()->alphaY().set(yPixel);
    textureProg()->finalAlpha().set(-0.7);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    CHECK_OPENGL_ERROR();

    vertexBuffer.disable();

    textureProg()->unbind();
}

Texture2dGlslProgram* GlslStaticTextPainter::textureProg() const
{
    return static_cast<Texture2dGlslProgram*>(_programs[0]);
}

