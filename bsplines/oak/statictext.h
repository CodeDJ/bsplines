#ifndef STATICTEXT_H
#define STATICTEXT_H

#include "geometricobject.h"
#include "rectf.h"

#include <vector>
#include <string>

namespace oak
{

class StaticText : public oak::GeometricObject
{
public:
    StaticText(const oak::RectF& rect, const std::vector<std::string>& lines)
        : _rect(rect),
          _lines(lines)
    { }

    oak::RectF rect() const { return _rect; }
    std::vector<std::string> lines() const { return _lines; }

private:
    oak::RectF _rect;
    std::vector<std::string> _lines;
};
}


#endif // STATICTEXT_H
