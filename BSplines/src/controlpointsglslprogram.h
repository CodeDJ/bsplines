#ifndef CONTROLPOINTSGLSLPROGRAM_H
#define CONTROLPOINTSGLSLPROGRAM_H

#include "glslprogram.h"
#include "glsluniform.h"

class ControlPointsGlslProgram : public GlslProgram
{
public:
    ControlPointsGlslProgram();
    virtual ~ControlPointsGlslProgram();

private:
    virtual void endCreate();
    virtual void endLink(bool result);

private:
    GlslUniform _curbColor;
};

#endif // CONTROLPOINTSGLSLPROGRAM_H
