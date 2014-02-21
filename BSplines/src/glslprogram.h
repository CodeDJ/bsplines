#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include <string>
#include <map>

#include "sharedvalue.h"

enum class GlslShaderType;
class GlslShader;
class SourceFile;

class GlslProgram
{
public:
    GlslProgram(const std::string& name = "");
    virtual ~GlslProgram();

    void removeShaders(GlslShaderType shaderType);
    void removeShader(const GlslShader& shader);
    void addShader(const GlslShader& shader);

    bool create();
    bool link();

    void bind();
    void unbind();

    unsigned int id() const;
    std::string name() const;

    //bool initUniform(const std::)

private:
    GlslProgram(const GlslProgram&);
    GlslProgram& operator=(const GlslProgram&);

private:
    virtual void beginCreate() {}
    virtual void endCreate() {}
    virtual void beginLink() {}
    virtual void endLink(bool result) { (void)result; }


private:
    bool attachShaders();
    bool detachShaders();
    bool compileShaders();

private:
    SharedValue<unsigned int> _id;
    std::string _name;
    std::multimap<GlslShaderType, GlslShader> _shaders;
    std::string _linkErrors;
};




#endif // GLSLPROGRAM_H


