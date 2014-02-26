#include "texttexture.h"

#include "oak/rectf.h"

#include <Windows.h>

class TextTexture::TextTexturePrivate
{
public:
    TextTexturePrivate() :
        _offScreenRep(nullptr),
        _data(nullptr)
    {
    }
    ~TextTexturePrivate()
    {
        DeleteObject(_offScreenRep);
        //_data = nullptr;
    }

    void initImage(int width, int height)
    {
        if (_offScreenRep)
            return;

        BITMAPINFO bmInfo;
        memset(&bmInfo.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
        bmInfo.bmiHeader.biSize      = sizeof(BITMAPINFOHEADER);
        bmInfo.bmiHeader.biWidth     = width;
        bmInfo.bmiHeader.biHeight    = height;
        bmInfo.bmiHeader.biPlanes    = 1;
        bmInfo.bmiHeader.biBitCount  = 24;
        bmInfo.bmiHeader.biSizeImage = width * height * 3;

        HDC pDC = ::GetDC(0);
        HDC tmpDC = CreateCompatibleDC(pDC);
        _offScreenRep = CreateDIBSection(tmpDC, &bmInfo, DIB_RGB_COLORS, (void**)&_data, NULL, 0x0);
        HGDIOBJ prevObj = SelectObject(tmpDC, _offScreenRep);

        memset(_data, 0xff, bmInfo.bmiHeader.biSizeImage);
        //
        RECT rect = { 0, 0, width, height };

        HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
        FillRect(tmpDC, &rect, brush);
        DeleteObject(brush);

        DrawText(tmpDC, L"BSplines", -1, &rect, DT_SINGLELINE | DT_LEFT | DT_TOP);

        // restore
        SelectObject(tmpDC, prevObj);
        // cleanup
        DeleteDC(tmpDC);
#if 0
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
#endif
    }

#if 0
    NSAttributedString* buildAttributtedString(const std::string& text, const oak::Color& color)
    {
        NSMutableDictionary *attrDictionary = [NSMutableDictionary dictionaryWithObjectsAndKeys: [NSFont systemFontOfSize: [NSFont systemFontSize]], (NSString *)NSFontAttributeName,
                                                                            [NSColor colorWithDeviceRed: color._r green: color._g blue: color._b alpha: color._a], (NSString *)NSForegroundColorAttributeName,
                                                                            nil];
        NSAttributedString* drawStringAttr = [[NSAttributedString alloc] initWithString: [NSString stringWithUTF8String: text.c_str()] attributes: attrDictionary];
        return drawStringAttr;
    }
#endif

    friend class TextTexture;

    HBITMAP _offScreenRep;
    unsigned char* _data;
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
    if (!_d->_offScreenRep)
    {
        _d->initImage(_width, _height);
    }
    return _d->_data;
}

void TextTexture::clear(const oak::Color& backgroundColor)
{
#if 0
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
#endif
}

oak::RectF TextTexture::boundingRect(const std::string& text)
{
    return oak::RectF(0, 0, _width, _height);
#if 0
    NSAttributedString* drawStringAttr = _d->buildAttributtedString(text, oak::Color::black());
    NSRect destRect = [drawStringAttr boundingRectWithSize: NSMakeSize(_width, _height) options:NSStringDrawingUsesLineFragmentOrigin];
    [drawStringAttr release];
    return oak::RectF(destRect.origin.x, destRect.origin.y, destRect.size.width, destRect.size.height);
#endif
}

void TextTexture::drawText(const std::string& text, int x /*= 0*/, int y /*= 0*/, const oak::Color& color /*= oak::Color::white()*/)
{
#if 0
    if (!_d->_offscreenRep)
    {
        _d->initImage(_width, _height);
    }


    // set offscreen context
    NSGraphicsContext *ctx = [NSGraphicsContext graphicsContextWithBitmapImageRep:_d->_offscreenRep];

    [NSGraphicsContext saveGraphicsState];
    [NSGraphicsContext setCurrentContext:ctx];

    NSAttributedString* drawStringAttr = _d->buildAttributtedString(text, color);
    [drawStringAttr drawAtPoint: NSMakePoint(x, y)];
    [drawStringAttr release];

    [ctx flushGraphics];
    // done drawing, so set the current context back to what it was
    [NSGraphicsContext restoreGraphicsState];
#endif
}
