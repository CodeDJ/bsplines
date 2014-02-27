#include "texture2dglslprogram.h"

#include "shaderloader.h"
#include "glslshader.h"

Texture2dGlslProgram::Texture2dGlslProgram(const std::string& version)
    : _vertexBuffer(4),
      _alphaX("alphaX"),
      _alphaY("alphaY"),
      _finalAlpha("finalAlpha"),
      _version(version)
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

    addShader(ShaderLoader::instance().getShader(GlslShaderType::Vertex, std::string("vertex_texture") + "_" + _version));
    addShader(ShaderLoader::instance().getShader(GlslShaderType::Fragment, std::string("fragment_texture") + "_" + _version));
}

void Texture2dGlslProgram::endLink(bool result)
{
    if (!result)
        return;

    _alphaX.bind(*this);
    _alphaY.bind(*this);
    _finalAlpha.bind(*this);
}
