#ifndef CONTROLPOINTSGLSLPROGRAM_H
#define CONTROLPOINTSGLSLPROGRAM_H

#include "glslprogram.h"
#include "glsluniform.h"
#include "glslbuffer.h"

class ControlPointsGlslProgram : public GlslProgram
{
public:
    ControlPointsGlslProgram();
    virtual ~ControlPointsGlslProgram();

public:
    GlslUniform4f& pointColor();
    GlslVertexBuffer& vertexBuffer();

private:
    virtual void endCreate();
    virtual void endLink(bool result);

private:
    GlslUniform4f _pointColor;
    GlslVertexBuffer _vertexBuffer;
};

#endif // CONTROLPOINTSGLSLPROGRAM_H
