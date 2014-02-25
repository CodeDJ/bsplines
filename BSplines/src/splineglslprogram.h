#ifndef SPLINEGLSLPROGRAM_H
#define SPLINEGLSLPROGRAM_H

#include "glslprogram.h"
#include "glsluniform.h"
#include "glslbuffer.h"

class SplineGlslProgram : public GlslProgram
{
public:
    SplineGlslProgram(unsigned int segments, unsigned int strips);
    virtual ~SplineGlslProgram();

public:
    GlslUniform1f& lineWidthAlphaX();
    GlslUniform1f& lineWidthAlphaY();
    GlslUniform4f& pointColor();
    GlslVertexBuffer& vertexBuffer();

protected:
    virtual void endCreate();
    virtual void endLink(bool result);

public:
    /* Hide these as they have to be in sync with the shaders source code */
    GlslUniform1i& segmentsPerSpline();
    GlslUniform1i& stripsPerSegment();

private:
    GlslUniform1f _lineWidthAlphaX;
    GlslUniform1f _lineWidthAlphaY;
    GlslUniform1i _segmentsPerSpline;
    GlslUniform1i _stripsPerSegment;
    GlslUniform4f _pointColor;
    GlslVertexBuffer _vertexBuffer;

protected:
    unsigned int _segments;
    unsigned int _strips;
};

class SplineGlslProgramGeomTess : public SplineGlslProgram
{
public:
    SplineGlslProgramGeomTess(unsigned int segments, unsigned int strips);

    GlslUniform<float, 2>& controlPoints();

private:
    virtual void endCreate();
    virtual void endLink(bool result);

protected:
    GlslUniform<float, 2> _controlPoints;
};

#endif // SPLINEGLSLPROGRAM_H
