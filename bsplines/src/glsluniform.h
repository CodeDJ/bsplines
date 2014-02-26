#ifndef GLSLUNIFORM_H
#define GLSLUNIFORM_H

#include <string>
#include <assert.h>

#include "glslprogram.h"


int glslbind_getuniformlocation(unsigned int program, const char *name);

void glsluniform_set(unsigned int location, const float* data, unsigned int values, unsigned int length);
void glsluniform_set(unsigned int location, const int* data, unsigned int values, unsigned int length);
void glslprogramuniform_set(unsigned int program, unsigned int location, float* data, unsigned int values, unsigned int length);
void glslprogramuniform_set(unsigned int program, unsigned int location, int* data, unsigned int values, unsigned int length);

template<typename T, unsigned int paramValues=1, unsigned int paramLength=1>
class GlslUniform
{
public:
    GlslUniform(const std::string& name)
    : _name(name),
      _location(0),
      _program(0)
    {
        static_assert(paramValues, "A uniform requires at least 1 value");
    }

    virtual ~GlslUniform()
    { }

    std::string name() const
    {
        return _name;
    }

    bool bind(const GlslProgram& glslProgram)
    {
        _program = glslProgram.id();
        assert(_program);
        return (_location = glslbind_getuniformlocation(glslProgram.id(), _name.c_str())) != 0;
    }

    void unbind()
    {
        _program = 0;
        _location = 0;
    }

    void link(const std::shared_ptr<T>& data)
    {
        _data = data;
    }

    void unlink()
    {
        _data.reset();
    }

    std::shared_ptr<T> data() const
    {
        return _data.lock();
    }

    void set()
    {
        std::shared_ptr<T> tmpData = _data.lock();
        if (tmpData)
        {
            glsuniform_set(_location, tmpData.get(), paramValues, paramLength);
        }
    }

    void set(const T* data)
    {
        assert(data);
        glsluniform_set(_location, data, paramValues, paramLength);
    }

    void set(const T* data, unsigned int length)
    {
        glsluniform_set(_location, data, paramValues, length);
    }

    void setIn()
    {
        std::shared_ptr<T> tmpData = _data.lock();
        if (tmpData)
        {
            glsprogramuniform_set(_program, _location, tmpData.get(), paramValues, paramLength);
        }
    }

    void setIn(const T* data)
    {
        assert(data);
        glslprogramuniform_set(_program, _location, data, paramValues, paramLength);
    }
    void setIn(const T* data, unsigned int length)
    {
        glslprogramuniform_set(_program, _location, data, length);
    }

protected:
    const std::string _name;
    unsigned int _location;
    unsigned int _program;
    std::weak_ptr<T> _data;
};


class GlslUniform4f : public GlslUniform<float, 4>
{
public:
    GlslUniform4f(const std::string& name);

    void set(float v1, float v2, float v3, float v4);
    void setIn(float v1, float v2, float v3, float v4);
};

class GlslUniform1f : public GlslUniform<float, 1>
{
public:
    GlslUniform1f(const std::string& name);

    void set(float v1);
    void setIn(float v1);
};

class GlslUniform1i : public GlslUniform<int, 1>
{
public:
    GlslUniform1i(const std::string& name);

    void set(int v1);
    void setIn(int v1);
};


#endif // GLSLUNIFORM_H
