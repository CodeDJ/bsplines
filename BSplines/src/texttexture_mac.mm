#include "texttexture.h"

#include "oak/rectf.h"

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

    NSAttributedString* buildAttributtedString(const std::string& text, const oak::Color& color)
    {
        NSMutableDictionary *attrDictionary = [NSMutableDictionary dictionaryWithObjectsAndKeys: [NSFont systemFontOfSize: [NSFont systemFontSize]], (NSString *)NSFontAttributeName,
                                                                            [NSColor colorWithDeviceRed: color._r green: color._g blue: color._b alpha: color._a], (NSString *)NSForegroundColorAttributeName,
                                                                            nil];
        NSAttributedString* drawStringAttr = [[NSAttributedString alloc] initWithString: [NSString stringWithUTF8String: text.c_str()] attributes: attrDictionary];
        return drawStringAttr;
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
    delete _d;
}

unsigned char* TextTexture::data() const
{
    if (!_d->_offscreenRep)
    {
        _d->initImage(_width, _height);
    }
    return [_d->_offscreenRep bitmapData];
}

void TextTexture::clear(const oak::Color& backgroundColor)
{
    if (!_d->_offscreenRep)
    {
        _d->initImage(_width, _height);
    }
    NSGraphicsContext *ctx = [NSGraphicsContext graphicsContextWithBitmapImageRep:_d->_offscreenRep];

    [NSGraphicsContext saveGraphicsState];
    [NSGraphicsContext setCurrentContext:ctx];
    [ctx setCompositingOperation: NSCompositeCopy];

    NSRect destRect = NSMakeRect(0.0, 0.0, _width, _height);
    [[NSColor colorWithDeviceRed: backgroundColor._r green: backgroundColor._g blue: backgroundColor._b alpha: backgroundColor._a] set];
    [NSBezierPath fillRect: destRect];

    [ctx flushGraphics];
    // done drawing, so set the current context back to what it was
    [NSGraphicsContext restoreGraphicsState];
}

oak::RectF TextTexture::boundingRect(const std::string& text)
{
    NSAttributedString* drawStringAttr = _d->buildAttributtedString(text, oak::Color::black());
    NSRect destRect = [drawStringAttr boundingRectWithSize: NSMakeSize(_width, _height) options:NSStringDrawingUsesLineFragmentOrigin];
    [drawStringAttr release];
    return oak::RectF(destRect.origin.x, destRect.origin.y, destRect.size.width, destRect.size.height);
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

    // flip axis
    NSAffineTransform *transform = [NSAffineTransform transform];
    [transform translateXBy:0.0 yBy:_height];
    [transform scaleXBy:1.0 yBy:-1.0];
    [transform set];

    NSAttributedString* drawStringAttr = _d->buildAttributtedString(text, color);
    [drawStringAttr drawAtPoint: NSMakePoint(x, y)];
    [drawStringAttr release];

    [ctx flushGraphics];
    // done drawing, so set the current context back to what it was
    [NSGraphicsContext restoreGraphicsState];

}

