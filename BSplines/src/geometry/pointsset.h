#ifndef POINTSSET_H
#define POINTSSET_H

#include "geometricobject.h"

namespace geometry
{

class PointsSet : public GeometricObject
{
public:
    PointsSet(int size);

private:
    //std::shared_ptr<float> _points;
};

}

#endif // POINTSSET_H
