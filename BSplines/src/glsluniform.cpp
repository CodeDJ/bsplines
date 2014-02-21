#include "glsluniform.h"

#include "glslprogram.h"

#include <OpenGL/gl3.h>

GlslUniform::GlslUniform(const std::string& name)
    : _name(name)
{
}

bool GlslUniform::bind(const GlslProgram& glslProgram)
{
    _location = glGetUniformLocation(glslProgram.id(), _name.c_str());
    return _location != 0;
}

void GlslUniform::unbind()
{
    _location = 0;
}
