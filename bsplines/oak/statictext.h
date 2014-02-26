#ifndef STATICTEXT_H
#define STATICTEXT_H

#include "geometricobject.h"
#include "rectf.h"
#include "color.h"

#include <vector>
#include <string>

namespace oak
{

class StaticText : public oak::GeometricObject
{
public:
    StaticText(const oak::RectF& rect, const std::vector<std::string>& lines, const oak::Color& color, const oak::Color& bckColor)
        : _rect(rect),
          _lines(lines),
          _color(color),
          _backgroundColor(bckColor)
    { }

    float x() const { return _rect.x(); }
    float y() const { return _rect.y(); }
    oak::RectF rect() const { return _rect; }
    std::vector<std::string>& lines() { return _lines; }
    oak::Color color() const { return _color;}
    oak::Color backgroundColor() const { return _backgroundColor;}

    void setPos(float x, float y) { _rect.setX(x); _rect.setY(y); }
    void setRect(const RectF& rect);
    void setColor(const oak::Color& color) { _color = color; }
    void setBackgroundColor(const oak::Color& backgroundColor) { _backgroundColor = backgroundColor; }

private:
    oak::RectF _rect;
    std::vector<std::string> _lines;
    oak::Color _color;
    oak::Color _backgroundColor;
};
}


#endif // STATICTEXT_H
