#ifndef GLSLSTATICTEXTPAINTER_H
#define GLSLSTATICTEXTPAINTER_H

#include "glslpainter.h"

class TextTexture;
class Texture2dGlslProgram;

class GlslStaticTextPainter : public GlslPainter<oak::StaticText>
{
public:
    GlslStaticTextPainter(const oak::StaticText& staticText, bool use21 = false);
    ~GlslStaticTextPainter();

    virtual bool prepare();
    virtual void paint(oak::Window* window);

    oak::StaticText& staticText() { return _staticText; }

    Texture2dGlslProgram* textureProg() const;

private:
    oak::StaticText _staticText;
    TextTexture* _textTexture;
    bool _use21;
};

#endif // GLSLSTATICTEXTPAINTER_H
