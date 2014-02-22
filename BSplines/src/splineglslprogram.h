#ifndef SPLINEGLSLPROGRAM_H
#define SPLINEGLSLPROGRAM_H

#include "glslprogram.h"
#include "glsluniform.h"

class SplineGlslProgram : public GlslProgram
{
public:
    SplineGlslProgram();
    virtual ~SplineGlslProgram();

public:
    GlslUniform1f& lineWidthAlphaX();
    GlslUniform1f& lineWidthAlphaY();
    GlslUniform1i& numStrips();
    GlslUniform1i& numSegments();
    GlslUniform4f& pointColor();
    //GlslUniform<float> _controlPoints();

protected:
    virtual void endCreate();
    virtual void endLink(bool result);

public:
    GlslUniform1f _lineWidthAlphaX;
    GlslUniform1f _lineWidthAlphaY;
    GlslUniform1i _numStrips;
    GlslUniform1i _numSegments;
    GlslUniform4f _pointColor;
};

class SplineGlslProgramTess : public SplineGlslProgram
{
public:
    SplineGlslProgramTess();

private:
    virtual void endCreate();
    virtual void endLink(bool result);

protected:
    GlslUniform<float> _controlPoints;
};

#endif // SPLINEGLSLPROGRAM_H
