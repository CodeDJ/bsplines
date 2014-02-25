#include "texture2dglslprogram.h"

#include "shaderloader.h"
#include "glslshader.h"

Texture2dGlslProgram::Texture2dGlslProgram()
    : _vertexBuffer(4)
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

void Texture2dGlslProgram::endCreate()
{
    if (!id())
        return;

    addShader(ShaderLoader::instance().getShader(GlslShaderType::Vertex, "vertex_texture"));
    addShader(ShaderLoader::instance().getShader(GlslShaderType::Fragment, "fragment_texture"));
}

void Texture2dGlslProgram::endLink(bool result)
{
    printf("===========================================\n");
    if (!result)
        return;
}
