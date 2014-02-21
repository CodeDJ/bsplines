#include "splineglslprogram.h"

#include "glslshader.h"
#include "shaderloader.h"

SplineGlslProgram::SplineGlslProgram()
    : _lineWidthAlphaX("lineWidthAlphaX"),
      _lineWidthAlphaY("lineWidthAlphaY"),
      _numStrips("numStrips"),
      _numSegments("numSegments"),
      _curbColor("curbColor"),
      _controlPoints("ControlPoints")
{
}

SplineGlslProgram::~SplineGlslProgram()
{

}

void SplineGlslProgram::endCreate()
{
    if (!id())
        return;

    addShader(ShaderLoader::instance().getShader(GlslShaderType::Vertex, "vertexshader"));
    addShader(ShaderLoader::instance().getShader(GlslShaderType::Fragment, "fragmentshader"));

#ifdef WITH_TESS
    addShader(ShaderLoader::instance().getShader(GlslShaderType::TessControl, "tcs_shader"));
    addShader(ShaderLoader::instance().getShader(GlslShaderType::TessEvaluation, "tes_shader"));
    addShader(ShaderLoader::instance().getShader(GlslShaderType::Geometry, "geometry_shader"));
#else
    addShader(ShaderLoader::instance().getShader(GlslShaderType::Geometry, "geometry_shader1"));
#endif
}

void SplineGlslProgram::endLink(bool result)
{
    if (!result)
        return;

    _lineWidthAlphaX.bind(*this);
    _lineWidthAlphaY.bind(*this);
    _numStrips.bind(*this);
    _numSegments.bind(*this);
    _curbColor.bind(*this);

#ifndef WITH_TESS
    _controlPoints.bind(*this);
#endif
}

