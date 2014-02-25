#ifndef TEXTTEXTURE_H
#define TEXTTEXTURE_H

#include "oak/color.h"

#include <string>

class TextTexture
{
public:
    TextTexture(int width, int height);
    ~TextTexture();

    int width() const { return _width; }
    int height() const { return _height; }

    void drawText(const std::string& text, int x = 0, int y = 0, const oak::Color& color = oak::Color::white());
    unsigned char* data() const;

private:
    int _width;
    int _height;

    class TextTexturePrivate;

    TextTexturePrivate* _d;
};

#endif // TEXTTEXTURE_H
