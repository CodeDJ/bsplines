#include "glsluniform.h"

#include "glslprogram.h"

#ifdef Q_OS_MAC
#include <OpenGL/gl3.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif


void glsuniform_set(unsigned int location, const float* data, unsigned int values, unsigned int length)
{
    if (length > 0)
    {
        switch(values)
        {
        case 1:
            glUniform1fv(location, length, data);
            break;
        case 2:
            glUniform2fv(location, length, data);
            break;
        case 3:
            glUniform3fv(location, length, data);
            break;
        case 4:
            glUniform4fv(location, length, data);
            break;
            assert(false);
        }
    }
    else
    {
        switch(values)
        {
        case 1:
            glUniform1f(location, data[0]);
            break;
        case 2:
            glUniform2f(location, data[0], data[1]);
            break;
        case 3:
            glUniform3f(location, data[0], data[1], data[2]);
            break;
        case 4:
            glUniform4f(location, data[0], data[1], data[2], data[3]);
            break;
        default:
            assert(false);
            break;
        }
    }
}

void glsuniform_set(unsigned int location, const int* data, unsigned int values, unsigned int length)
{
    if (length > 0)
    {
        switch(values)
        {
        case 1:
            glUniform1iv(location, length, data);
            break;
        case 2:
            glUniform2iv(location, length, data);
            break;
        case 3:
            glUniform3iv(location, length, data);
            break;
        case 4:
            glUniform4iv(location, length, data);
            break;
            assert(false);
        }
    }
    else
    {
        switch(values)
        {
        case 1:
            glUniform1i(location, data[0]);
            break;
        case 2:
            glUniform2i(location, data[0], data[1]);
            break;
        case 3:
            glUniform3i(location, data[0], data[1], data[2]);
            break;
        case 4:
            glUniform4i(location, data[0], data[1], data[2], data[3]);
            break;
        default:
            assert(false);
            break;
        }
    }
}


void glsprogramuniform_set(unsigned int program, unsigned int location, float* data, unsigned int values, unsigned int length)
{
    if (length > 0)
    {
        switch(values)
        {
        case 1:
            glProgramUniform1fv(program, location, length, data);
            break;
        case 2:
            glProgramUniform2fv(program, location, length, data);
            break;
        case 3:
            glProgramUniform3fv(program, location, length, data);
            break;
        case 4:
            glProgramUniform4fv(program, location, length, data);
            break;
            assert(false);
        }
    }
    else
    {
        switch(values)
        {
        case 1:
            glProgramUniform1f(program, location, data[0]);
            break;
        case 2:
            glProgramUniform2f(program, location, data[0], data[1]);
            break;
        case 3:
            glProgramUniform3f(program, location, data[0], data[1], data[2]);
            break;
        case 4:
            glProgramUniform4f(program, location, data[0], data[1], data[2], data[3]);
            break;
        default:
            assert(false);
            break;
        }
    }
}

void glsprogramuniform_set(unsigned int program, unsigned int location, int* data, unsigned int values, unsigned int length)
{
    if (length > 0)
    {
        switch(values)
        {
        case 1:
            glProgramUniform1iv(program, location, length, data);
            break;
        case 2:
            glProgramUniform2iv(program, location, length, data);
            break;
        case 3:
            glProgramUniform3iv(program, location, length, data);
            break;
        case 4:
            glProgramUniform4iv(program, location, length, data);
            break;
            assert(false);
        }
    }
    else
    {
        switch(values)
        {
        case 1:
            glProgramUniform1i(program, location, data[0]);
            break;
        case 2:
            glProgramUniform2i(program, location, data[0], data[1]);
            break;
        case 3:
            glProgramUniform3i(program, location, data[0], data[1], data[2]);
            break;
        case 4:
            glProgramUniform4i(program, location, data[0], data[1], data[2], data[3]);
            break;
        default:
            assert(false);
            break;
        }
    }
}

GlslUniform4f::GlslUniform4f(const std::string& name)
    : GlslUniform<float, 4>(name)
{

}

void GlslUniform4f::set(float v1, float v2, float v3, float v4)
{
    glUniform4f(_location, v1, v2, v3, v4);
}

void GlslUniform4f::setIn(float v1, float v2, float v3, float v4)
{
    glProgramUniform4f(_program, _location, v1, v2, v3, v4);
}

GlslUniform1f::GlslUniform1f(const std::string& name)
    : GlslUniform<float, 1>(name)
{

}

void GlslUniform1f::set(float v1)
{
    glUniform1f(_location, v1);
}

void GlslUniform1f::setIn(float v1)
{
    glProgramUniform1f(_program, _location, v1);
}

GlslUniform1i::GlslUniform1i(const std::string& name)
    : GlslUniform<int, 1>(name)
{

}

void GlslUniform1i::set(float v1)
{
    glUniform1i(_location, v1);
}

void GlslUniform1i::setIn(float v1)
{
    glProgramUniform1i(_program, _location, v1);
}

