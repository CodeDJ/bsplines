#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include <string>
#include <map>

#include "sharedvalue.h"
#include "global.h"

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
    bool compile();
    bool link();
    std::string linkErrors() const;
    std::string compileErrors() const;

    void bind();
    void unbind();

    unsigned int id() const;
    std::string name() const;

protected:
    virtual void beginCreate() {}
    virtual void endCreate() {}
    virtual void beginCompile() {}
    virtual void endCompile(bool result) { UNUSED(result); }
    virtual void beginLink() {}
    virtual void endLink(bool result) { UNUSED(result); }

private:
    bool attachShaders();
    bool detachShaders();
    bool compileShaders();

private:
    SharedValue<unsigned int> _id;
    std::string _name;
    std::multimap<GlslShaderType, GlslShader> _shaders;
    std::string _linkErrors;
    std::string _compileErrors;
};




#endif // GLSLPROGRAM_H


