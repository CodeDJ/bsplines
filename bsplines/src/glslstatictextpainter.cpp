#include "glslstatictextpainter.h"

#include "texttexture.h"
#include "texture2dglslprogram.h"

#include "oak/pointf.h"
#include "oak/window.h"
#include "oak/log.h"

#ifdef Q_OS_MAC
#include <OpenGL/gl3.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif


namespace
{
    static const int g_DefaultLineSpacing = 0;
}


GlslStaticTextPainter::GlslStaticTextPainter(const oak::StaticText& staticText, bool use21 /*= false*/)
    : GlslPainter<oak::StaticText>(staticText),
      _staticText(staticText),
      _textTexture(0),
      _use21(use21)
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

    _isPrepared = addProgram<Texture2dGlslProgram>(_use21 ? std::string("21"): std::string());
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

    oak::RectF destRect(rect.x() * xPixel - 1.0f, 1.0f - rect.y() * yPixel - rect.height() * yPixel, rect.width() * xPixel, rect.height() * yPixel);
    if (!_use21)
    {
        oak::PointF vertices[8] = { { destRect.x()                   , destRect.y()                    }, {0           , 0},
                                    { destRect.x() + destRect.width(), destRect.y()                    }, {rect.width(), 0},
                                    { destRect.x()                   , destRect.y() + destRect.height()}, {0           , rect.height()},
                                    { destRect.x() + destRect.width(), destRect.y() + destRect.height()}, {rect.width(), rect.height()}
                                  };
        vertexBuffer.set(reinterpret_cast<const void*>(vertices));
    }
    else
    {
        oak::PointF vertices[8] = { { destRect.x()                   , destRect.y()                    }, {0.0f           , 0.0f},
                                    { destRect.x() + destRect.width(), destRect.y()                    }, {1.0f, 0.0f},
                                    { destRect.x()                   , destRect.y() + destRect.height()}, {0           , 1.0f},
                                    { destRect.x() + destRect.width(), destRect.y() + destRect.height()}, {1.0f, 1.0f}
                                  };
        vertexBuffer.set(reinterpret_cast<const void*>(vertices));
    }

    vertexBuffer.enable();

    _textTexture->clear(_staticText.backgroundColor());
    std::vector<std::string> lines = _staticText.lines();
    int yLine = 0;
    for(auto iter = lines.begin(); iter != lines.end(); ++iter)
    {
        _textTexture->drawText(*iter, 0, yLine, _staticText.color());
        oak::RectF boundingRect = _textTexture->boundingRect(*iter);
        yLine += boundingRect.height() + g_DefaultLineSpacing;
    }

    textureProg()->texture2D().bind();
    textureProg()->texture2D().set(_textTexture->data(), _textTexture->width(), _textTexture->height());

    //textureProg()->alphaX().set(xPixel);
    //textureProg()->alphaY().set(yPixel);
    textureProg()->alphaX().set(1.0f);
    textureProg()->alphaY().set(1.0f);
    textureProg()->finalAlpha().set(_staticText.alpha());

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    CHECK_OPENGL_ERROR();

    vertexBuffer.disable();

    textureProg()->unbind();
}

Texture2dGlslProgram* GlslStaticTextPainter::textureProg() const
{
    return static_cast<Texture2dGlslProgram*>(_programs[0]);
}
