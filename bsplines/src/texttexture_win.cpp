#include "texttexture.h"

#include "oak/rectf.h"
#include "oak/log.h"

#include <Windows.h>

class TextTexture::TextTexturePrivate
{
public:
    TextTexturePrivate() :
        _bitmap(nullptr),
        _data(nullptr)
    {
    }
    ~TextTexturePrivate()
    {
        DeleteObject(_bitmap);
        //_data = nullptr;
    }

    void initImage(int width, int height)
    {
        if (_bitmap)
            return;

        BITMAPINFO bmInfo;
        memset(&bmInfo.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
        bmInfo.bmiHeader.biSize      = sizeof(BITMAPINFOHEADER);
        bmInfo.bmiHeader.biWidth     = width;
        bmInfo.bmiHeader.biHeight    = height;
        bmInfo.bmiHeader.biPlanes    = 1;
        bmInfo.bmiHeader.biBitCount  = 24;
        bmInfo.bmiHeader.biSizeImage = width * height * 3;

        HDC hdc = CreateCompatibleDC(::GetDC(0));
        _bitmap = CreateDIBSection(hdc, &bmInfo, DIB_RGB_COLORS, (void**)&_data, NULL, 0x0);
        // cleanup
        DeleteDC(hdc);
    }

    friend class TextTexture;

    HBITMAP _bitmap;
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
    if (!_d->_bitmap)
    {
        _d->initImage(_width, _height);
    }
    return _d->_data;
}

void TextTexture::clear(const oak::Color& backgroundColor)
{
    if (!_d->_bitmap)
    {
        _d->initImage(_width, _height);
    }
    HDC hdc = CreateCompatibleDC(0);
    HGDIOBJ prevObj = SelectObject(hdc, _d->_bitmap);

    //
    RECT rect = { 0, 0, _width, _height };

    HBRUSH brush = CreateSolidBrush(RGB(backgroundColor._r * 255, backgroundColor._g * 255, backgroundColor._b * 255));
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);

    // restore
    SelectObject(hdc, prevObj);
    // cleanup
    DeleteDC(hdc);
}

oak::RectF TextTexture::boundingRect(const std::string& text)
{
    if (!_d->_bitmap)
    {
        _d->initImage(_width, _height);
    }
    HDC hdc = CreateCompatibleDC(0);
    HGDIOBJ prevObj = SelectObject(hdc, _d->_bitmap);

    SIZE size = { 0, 0 };
    std::wstring wtext(text.begin(), text.end());
    GetTextExtentPoint32(hdc, wtext.c_str(), wtext.size(), &size);

    // restore
    SelectObject(hdc, prevObj);
    // cleanup
    DeleteDC(hdc);

    return oak::RectF(0, 0, size.cx, size.cy);
}

void TextTexture::drawText(const std::string& text, int x /*= 0*/, int y /*= 0*/, const oak::Color& color /*= oak::Color::white()*/)
{
    if (!_d->_bitmap)
    {
        _d->initImage(_width, _height);
    }
    HDC hdc = CreateCompatibleDC(0);
    HGDIOBJ prevObj = SelectObject(hdc, _d->_bitmap);

    std::wstring wtext(text.begin(), text.end());
    SIZE size = { 0, 0 };
    GetTextExtentPoint32(hdc, wtext.c_str(), wtext.size(), &size);
    RECT rect = { x, y, x + size.cx, y + size.cy };

    SetTextColor(hdc, RGB(color._r * 255, color._g * 255, color._b * 255));
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, wtext.c_str(), -1, &rect, DT_SINGLELINE | DT_LEFT | DT_TOP);

    // restore
    SelectObject(hdc, prevObj);
    // cleanup
    DeleteDC(hdc);
}
