#include "splineglslprogram.h"

#include "glslshader.h"
#include "shaderloader.h"

SplineGlslProgram::SplineGlslProgram()
    : _lineWidthAlphaX("lineWidthAlphaX"),
      _lineWidthAlphaY("lineWidthAlphaY"),
      _segmentsPerSpline("segmentsPerSpline"),
      _stripsPerSegment("stripsPerSegment"),
      _pointColor("pointColor")
{
}

SplineGlslProgram::~SplineGlslProgram()
{

}

void SplineGlslProgram::endCreate()
{
    if (!id())
        return;

    addShader(ShaderLoader::instance().getShader(GlslShaderType::Vertex, "vertex"));
    addShader(ShaderLoader::instance().getShader(GlslShaderType::Fragment, "fragment"));
    addShader(ShaderLoader::instance().getShader(GlslShaderType::Geometry, "geometry_tess"));
}

void SplineGlslProgram::endLink(bool result)
{
    if (!result)
        return;

    _lineWidthAlphaX.bind(*this);
    _lineWidthAlphaY.bind(*this);
    _segmentsPerSpline.bind(*this);
    _stripsPerSegment.bind(*this);
    _pointColor.bind(*this);
}

GlslUniform1f& SplineGlslProgram::lineWidthAlphaX()
{
    return _lineWidthAlphaX;
}

GlslUniform1f& SplineGlslProgram::lineWidthAlphaY()
{
    return _lineWidthAlphaY;
}

GlslUniform1i& SplineGlslProgram::segmentsPerSpline()
{
    return _segmentsPerSpline;
}

GlslUniform1i& SplineGlslProgram::stripsPerSegment()
{
    return _stripsPerSegment;
}

GlslUniform4f& SplineGlslProgram::pointColor()
{
    return _pointColor;
}

GlslVertexBuffer& SplineGlslProgram::vertexBuffer()
{
    return _vertexBuffer;
}

SplineGlslProgramTess::SplineGlslProgramTess()
    : _controlPoints("ControlPoints")
{
}

void SplineGlslProgramTess::endCreate()
{
    addShader(ShaderLoader::instance().getShader(GlslShaderType::Vertex, "vertex"));
    addShader(ShaderLoader::instance().getShader(GlslShaderType::Fragment, "fragment"));
    addShader(ShaderLoader::instance().getShader(GlslShaderType::TessControl, "tesscontrol"));
    addShader(ShaderLoader::instance().getShader(GlslShaderType::TessEvaluation, "tesseval"));
    addShader(ShaderLoader::instance().getShader(GlslShaderType::Geometry, "geometry"));
}

void SplineGlslProgramTess::endLink(bool result)
{
    if (!result)
        return;

    SplineGlslProgram::endLink(result);
    _controlPoints.bind(*this);
}

