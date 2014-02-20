#include "vertexshader.h"


VertexShader::VertexShader()
{
    _type = Vertex;
}

VertexShader::VertexShader(const std::string& source)
    : Shader(source)
{
    _type = Vertex;
}

