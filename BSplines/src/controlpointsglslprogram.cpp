#include "controlpointsglslprogram.h"

#include "glsluniform.h"
#include "glslshader.h"
#include "shaderloader.h"

ControlPointsGlslProgram::ControlPointsGlslProgram()
    : GlslProgram("ControlPointsProgram"),
      _pointColor("pointColor")
{
}

ControlPointsGlslProgram::~ControlPointsGlslProgram()
{

}

void ControlPointsGlslProgram::endCreate()
{
    if (!id())
        return;

    addShader(ShaderLoader::instance().getShader(GlslShaderType::Vertex, "vertex"));
    addShader(ShaderLoader::instance().getShader(GlslShaderType::Fragment, "fragment"));
}

void ControlPointsGlslProgram::endLink(bool result)
{
    if (!result)
        return;

    _pointColor.bind(*this);
}

GlslUniform4f& ControlPointsGlslProgram::pointColor()
{
    return _pointColor;
}
