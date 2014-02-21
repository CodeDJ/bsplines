#ifndef SPLINEGLSLPROGRAM_H
#define SPLINEGLSLPROGRAM_H

#include "glslprogram.h"
#include "glsluniform.h"

class SplineGlslProgram : public GlslProgram
{
public:
    SplineGlslProgram();
    virtual ~SplineGlslProgram();

private:
    virtual void endCreate();
    virtual void endLink(bool result);

private:
    GlslUniform _lineWidthAlphaX;
    GlslUniform _lineWidthAlphaY;
    GlslUniform _numStrips;
    GlslUniform _numSegments;
    GlslUniform _curbColor;
    GlslUniform _controlPoints;
};

#endif // SPLINEGLSLPROGRAM_H
