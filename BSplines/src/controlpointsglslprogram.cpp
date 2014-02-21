#include "controlpointsglslprogram.h"

#include "glsluniform.h"
#include "glslshader.h"
#include "shaderloader.h"

ControlPointsGlslProgram::ControlPointsGlslProgram()
    : GlslProgram("ControlPointsProgram"),
      _curbColor("curbColor")
{
}

ControlPointsGlslProgram::~ControlPointsGlslProgram()
{

}

GlslUniform* _lineWidthAlphaX;
GlslUniform* _lineWidthAlphaY;
GlslUniform* _numStrips;
GlslUniform* _numSegments;
GlslUniform* _controlPoints;


void ControlPointsGlslProgram::endCreate()
{
    if (!id())
        return;

    addShader(ShaderLoader::instance().getShader(GlslShaderType::Vertex, "vertexshader"));
    addShader(ShaderLoader::instance().getShader(GlslShaderType::Fragment, "fragmentshader"));
}

void ControlPointsGlslProgram::endLink(bool result)
{
    if (!result)
        return;

    _curbColor.bind(*this);
}
