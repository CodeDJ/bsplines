#ifndef GLSLSHADER_H
#define GLSLSHADER_H

#include <string>
#include <unordered_map>

#include "sharedvalue.h"


class SourceFile;
class GlslProgram;

enum class GlslShaderType {
    Vertex, TessControl, TessEvaluation, Geometry, Fragment
};



class GlslShader
{
public:
    GlslShader(GlslShaderType type);
    GlslShader(GlslShaderType type, const std::string& source);
    ~GlslShader();

    void setSource(const std::string& source);
    void setSource(SourceFile& sourceFile);

    bool hasSource() const;

    unsigned int id() const;
    std::string getSource();
    GlslShaderType type() const;
    std::string typeAsStr() const;
    std::string compileErrors() const;

    bool create();
    bool compile();
    bool attach(const GlslProgram& program);
    bool dettach(const GlslProgram& program);

    bool isSupported() const;

    void setParam(const std::string& name, const std::string& value);

private:

protected:
    GlslShaderType _type;

private:
    SharedValue<unsigned int> _id;
    std::string _origSource;
    std::string _compileErrors;
    std::unordered_map<std::string, std::string> _params;
    bool _dirtySource;
    std::string _source;
};

#endif // GLSLSHADER_H
