#include "splineglslprogram21.h"

#include "shaderloader.h"
#include "glslshader.h"

#include <sstream>

SplineGlslProgram21::SplineGlslProgram21(GLuint segments /*= 1*/, GLuint strips /*= 1*/)
    : _pointColor("pointColor"),
      _controlPoints("controlPoints"),
      _passThrough("passThrough"),
      _segments(segments),
      _strips(strips)

{
}

SplineGlslProgram21::~SplineGlslProgram21()
{
}

void SplineGlslProgram21::endCreate()
{
    if (!id())
        return;

    GlslShader shader = ShaderLoader::instance().getShader(GlslShaderType::Vertex, "vertex_21");
    std::stringstream ss;
    ss << (_segments * 3 + 1);
    shader.setParam("CONTROL_POINTS_COUNT", ss.str());
    addShader(shader);

    addShader(ShaderLoader::instance().getShader(GlslShaderType::Fragment, "fragment_21"));
}

void SplineGlslProgram21::endLink(bool result)
{
    if (!result)
        return;

    _pointColor.bind(*this);
    _controlPoints.bind(*this);
    _passThrough.bind(*this);
}

GlslUniform4f& SplineGlslProgram21::pointColor()
{
    return _pointColor;
}

GlslVertexBuffer& SplineGlslProgram21::vertexBuffer()
{
    return _vertexBuffer;
}

GlslUniform<float, 2>& SplineGlslProgram21::controlPoints()
{
    return _controlPoints;
}

GlslUniform1i& SplineGlslProgram21::passThrough()
{
    return _passThrough;
}




