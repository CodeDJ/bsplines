#include "pointf.h"

#include <math.h>

float oak::PointF::size() const
{
    return sqrt(_x * _x + _y * _y);
}