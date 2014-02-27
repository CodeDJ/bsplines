#ifndef SPLINEGLSLPROGRAM_H
#define SPLINEGLSLPROGRAM_H

#include "glslprogram.h"
#include "glsluniform.h"
#include "glslbuffer.h"

#ifdef Q_OS_MAC
#include <OpenGL/gltypes.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif

class SplineGlslProgram : public GlslProgram
{
public:
    SplineGlslProgram(GLuint segments = 1, GLuint strips = 1);
    virtual ~SplineGlslProgram();

    void setSegments(GLuint segments);
    void setStrips(GLuint strips);

public:
    GlslUniform1f& lineWidthAlphaX();
    GlslUniform1f& lineWidthAlphaY();
    GlslUniform4f& pointColor();
    GlslVertexBuffer& vertexBuffer();

protected:
    virtual void endCreate();
    virtual void endLink(bool result);

private:
    /* Hide these as they have to be in sync with the shaders source code */
    GlslUniform1i& segmentsPerSpline();
    GlslUniform1i& stripsPerSegment();

    void updateShaderSegments();

private:
    GlslUniform1f _lineWidthAlphaX;
    GlslUniform1f _lineWidthAlphaY;
    GlslUniform1i _segmentsPerSpline;
    GlslUniform1i _stripsPerSegment;
    GlslUniform4f _pointColor;
    GlslVertexBuffer _vertexBuffer;

protected:
    GLuint _segments;
    GLuint _strips;
};

class SplineGlslProgramGeomTess : public SplineGlslProgram
{
public:
    SplineGlslProgramGeomTess(GLuint segments = 1, GLuint strips = 1);

    GlslUniform<float, 2>& controlPoints();

private:
    virtual void endCreate();
    virtual void endLink(bool result);

protected:
    GlslUniform<float, 2> _controlPoints;
};

#endif // SPLINEGLSLPROGRAM_H
