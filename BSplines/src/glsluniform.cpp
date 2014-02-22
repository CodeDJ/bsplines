#include "glsluniform.h"

#include "glslprogram.h"

#ifdef Q_OS_MAC
#include <OpenGL/gl3.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif

unsigned int glsluniform_bind(unsigned int program, const char* name)
{
    return glGetUniformLocation(program, name);
}

void glsluniform_set4f(unsigned int program, unsigned int location, float v1, float v2, float v3, float v4)
{
    glProgramUniform4f(program, location, v1, v2, v3, v4);
}

void glsluniform_set1f(unsigned int program, unsigned int location, float v)
{
    glProgramUniform1f(program, location, v);
}

void glsluniform_set1i(unsigned int program, unsigned int location, int v)
{
    glProgramUniform1i(program, location, v);
}
