#include "glsluniform.h"

#include "glslprogram.h"

#ifdef Q_OS_MAC
#include <OpenGL/gl3.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif

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
