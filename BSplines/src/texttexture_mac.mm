#include "texttexture.h"

#include <AppKit/AppKit.h>

class TextTexture::TextTexturePrivate
{
public:
    TextTexturePrivate()
        : _offscreenRep(nil)
    {
    }
    ~TextTexturePrivate()
    {
        [_offscreenRep release];
    }

    void initImage(int width, int height)
    {
        if (_offscreenRep)
            return;

        NSRect imgRect = NSMakeRect(0.0, 0.0, width, height);
        NSSize imgSize = imgRect.size;

        NSBitmapImageRep* offscreenRep = [[NSBitmapImageRep alloc]
           initWithBitmapDataPlanes:NULL
           pixelsWide:imgSize.width
           pixelsHigh:imgSize.height
           bitsPerSample:8
           samplesPerPixel:4
           hasAlpha:YES
           isPlanar:NO
           colorSpaceName:NSDeviceRGBColorSpace
           bitmapFormat:0
           bytesPerRow:width*4
           bitsPerPixel:8*4];

        _offscreenRep = offscreenRep;
    }

    friend class TextTexture;

    NSBitmapImageRep* _offscreenRep;
};

TextTexture::TextTexture(int width, int height)
    : _width(width),
      _height(height),
      _d(new TextTexturePrivate)
{
}

TextTexture::~TextTexture()
{
}

unsigned char* TextTexture::data() const
{
    if (!_d->_offscreenRep)
    {
        _d->initImage(_width, _height);
    }
    return [_d->_offscreenRep bitmapData];
}

void TextTexture::drawText(const std::string& text, int x /*= 0*/, int y /*= 0*/, const oak::Color& color /*= oak::Color::white()*/)
{
    if (!_d->_offscreenRep)
    {
        _d->initImage(_width, _height);
    }


    // set offscreen context
    NSGraphicsContext *ctx = [NSGraphicsContext graphicsContextWithBitmapImageRep:_d->_offscreenRep];

    [NSGraphicsContext saveGraphicsState];
    [NSGraphicsContext setCurrentContext:ctx];


    NSRect destRect = NSMakeRect(0.0, 0.0, _width, _height);

    NSMutableDictionary *attrDictionary = [NSMutableDictionary dictionaryWithObjectsAndKeys: [NSFont systemFontOfSize: [NSFont systemFontSize]], (NSString *)NSFontAttributeName,
                                                                        [NSColor colorWithDeviceRed: color._r green: color._g blue: color._b alpha: color._a], (NSString *)NSForegroundColorAttributeName,
                                                                        nil];
    NSAttributedString* drawStringAttr = [[NSAttributedString alloc] initWithString: [NSString stringWithCString: text.c_str()] attributes: attrDictionary];
    [[NSColor blueColor] set];
    [drawStringAttr drawAtPoint: NSMakePoint(x, y)];

    // done drawing, so set the current context back to what it was
    [NSGraphicsContext restoreGraphicsState];

}

