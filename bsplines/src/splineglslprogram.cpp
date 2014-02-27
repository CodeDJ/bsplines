#include "splineglslprogram.h"

#include "glslshader.h"
#include "shaderloader.h"

#include <sstream>

SplineGlslProgram::SplineGlslProgram(GLuint segments /*= 0*/, unsigned int strips /*= 0*/)
    : _lineWidthAlphaX("lineWidthAlphaX"),
      _lineWidthAlphaY("lineWidthAlphaY"),
      _segmentsPerSpline("segmentsPerSpline"),
      _stripsPerSegment("stripsPerSegment"),
      _pointColor("pointColor"),
      _segments(segments),
      _strips(strips)
{
}

SplineGlslProgram::~SplineGlslProgram()
{

}

void SplineGlslProgram::updateShaderSegments()
{
    std::stringstream ss;
    ss << (_segments * 3 + 1);

    auto iter = _shaders.find(GlslShaderType::TessControl);
    if (iter == _shaders.end())
    {
        GlslShader shader = ShaderLoader::instance().getShader(GlslShaderType::TessControl, "tesscontrol");
        shader.setParam("CONTROL_POINTS_COUNT", ss.str());
        addShader(shader);
    }
    else
    {
        GlslShader shader = iter->second;
        shader.setParam("CONTROL_POINTS_COUNT", ss.str());
    }
}

void SplineGlslProgram::setSegments(GLuint segments)
{
    if (_segments != segments)
    {
        _segments = segments;
        if (id())
            updateShaderSegments();
    }
}

void SplineGlslProgram::setStrips(GLuint strips)
{
    _strips = strips;
}

void SplineGlslProgram::endCreate()
{
    if (!id())
        return;

    addShader(ShaderLoader::instance().getShader(GlslShaderType::Vertex, "vertex"));
    addShader(ShaderLoader::instance().getShader(GlslShaderType::Fragment, "fragment"));

    updateShaderSegments();

    addShader(ShaderLoader::instance().getShader(GlslShaderType::TessEvaluation, "tesseval"));
    addShader(ShaderLoader::instance().getShader(GlslShaderType::Geometry, "geometry"));
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


    _segmentsPerSpline.setIn(_segments);
    _stripsPerSegment.setIn(_strips);
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

SplineGlslProgramGeomTess::SplineGlslProgramGeomTess(GLuint segments /*= 1*/, GLuint strips /*= 1*/)
    : SplineGlslProgram(segments, strips),
      _controlPoints("controlPoints")
{
}

void SplineGlslProgramGeomTess::endCreate()
{
    if (!id())
        return;

    addShader(ShaderLoader::instance().getShader(GlslShaderType::Vertex, "vertex"));
    addShader(ShaderLoader::instance().getShader(GlslShaderType::Fragment, "fragment"));

    GlslShader shader = ShaderLoader::instance().getShader(GlslShaderType::Geometry, "geometry_tess");
    std::stringstream ss;
    ss << (_segments * 3 + 1);
    shader.setParam("CONTROL_POINTS_COUNT", ss.str());
    ss.str(std::string());
    ss << ((4 + 2 * _strips) * _segments);
    shader.setParam("MAX_VERTICES", ss.str());
    addShader(shader);
}

void SplineGlslProgramGeomTess::endLink(bool result)
{
    if (!result)
        return;

    SplineGlslProgram::endLink(result);
    _controlPoints.bind(*this);
}

GlslUniform<float, 2>& SplineGlslProgramGeomTess::controlPoints()
{
    return _controlPoints;
}

