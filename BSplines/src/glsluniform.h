#ifndef GLSLUNIFORM_H
#define GLSLUNIFORM_H

#include <string>

class GlslProgram;

class GlslUniform
{
public:
    GlslUniform(const std::string& name);

    bool bind(const GlslProgram& glslProgram);
    void unbind();

private:
    const std::string _name;
    unsigned int _location;
};

#endif // GLSLUNIFORM_H
