#include "texture2dglslprogram.h"

#include "shaderloader.h"
#include "glslshader.h"

Texture2dGlslProgram::Texture2dGlslProgram()
    : _vertexBuffer(4),
      _alphaX("alphaX"),
      _alphaY("alphaY"),
      _finalAlpha("finalAlpha")
{
}


Texture2dGlslProgram::~Texture2dGlslProgram()
{

}

GlslVertexBuffer& Texture2dGlslProgram::vertexBuffer()
{
    return _vertexBuffer;
}

GlslTexture2D& Texture2dGlslProgram::texture2D()
{
    return _texture2D;
}

GlslUniform1f& Texture2dGlslProgram::alphaX()
{
    return _alphaX;
}

GlslUniform1f& Texture2dGlslProgram::alphaY()
{
    return _alphaY;
}

GlslUniform1f& Texture2dGlslProgram::finalAlpha()
{
    return _finalAlpha;
}

void Texture2dGlslProgram::endCreate()
{
    if (!id())
        return;

    addShader(ShaderLoader::instance().getShader(GlslShaderType::Vertex, "vertex_texture"));
    addShader(ShaderLoader::instance().getShader(GlslShaderType::Fragment, "fragment_texture"));
}

void Texture2dGlslProgram::endLink(bool result)
{
    if (!result)
        return;

    _alphaX.bind(*this);
    _alphaY.bind(*this);
    _finalAlpha.bind(*this);
}
