#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

#include "shader.h"

class VertexShader : public Shader
{
public:
    VertexShader();
    VertexShader(const std::string& source);
};

#endif // VERTEXSHADER_H
