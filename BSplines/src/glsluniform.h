#ifndef GLSLUNIFORM_H
#define GLSLUNIFORM_H

#include <string>

#include "glslprogram.h"

extern unsigned int glsluniform_bind(unsigned int program, const char* name);
extern void glsluniform_set4f(unsigned int program, unsigned int location, float v1, float v2, float v3, float v4);
extern void glsluniform_set1f(unsigned int program, unsigned int location, float v);
extern void glsluniform_set1i(unsigned int program, unsigned int location, int v);

template<typename T, int len=1>
class GlslUniform
{
public:
    GlslUniform(const std::string& name)
    : _name(name)
    { }

    virtual ~GlslUniform()
    { }

    bool bind(const GlslProgram& glslProgram)
    {
        _programId = glslProgram.id();
        return (_location = glsluniform_bind(_programId, _name.c_str())) != 0;
    }

    void unbind()
    {
        _location = 0;
    }

protected:
    const std::string _name;
    unsigned int _location;
    unsigned int _programId;
};

class GlslUniform4f : public GlslUniform<float, 4>
{
public:
    GlslUniform4f(const std::string& name)
        : GlslUniform(name)
    { }

    void set(float v1, float v2, float v3, float v4)
    {
        glsluniform_set4f(_programId, _location, v1, v2, v3, v4);
    }
};

class GlslUniform1f : public GlslUniform<float, 1>
{
public:
    GlslUniform1f(const std::string& name)
        : GlslUniform(name)
    { }

    void set(float v)
    {
        glsluniform_set1f(_programId, _location, v);
    }
};

class GlslUniform1i : public GlslUniform<int, 1>
{
public:
    GlslUniform1i(const std::string& name)
        : GlslUniform(name)
    { }

    void set(int v)
    {
        glsluniform_set1i(_programId, _location, v);
    }
};
/*
class GlslUniform2fv : public GlslUniform<float*, 2>
{
public:
    GlslUniform1i(const std::string& name)
        : GlslUniform(name)
    { }

    void set(int v)
    {
        glsluniform_set1i(_programId, _location, v);
    }
};
*/


#endif // GLSLUNIFORM_H
