#ifndef SHADER_H
#define SHADER_H

#include <string>

class SourceFile;
class Program;

class Shader
{
public:
    virtual ~Shader();

    enum Type {
        None, Vertex, TessControl, TessEvaluation, Geometry, Fragment
    };

    void setSource(const std::string& source);
    void setSource(SourceFile& sourceFile);

    unsigned int id() const;
    std::string source() const;
    Shader::Type type() const;

    bool create();
    bool compile();
    bool attach(const Program& program);
    bool dettach(const Program& program);

    virtual bool isSupported() const;

protected:
    Shader();
    Shader(const std::string& source);

private:
    Shader(const Shader&);
    Shader& operator=(const Shader&);

private:
    void clear();

protected:
    Type _type;

private:
    std::string _source;
    unsigned int _id;
    std::string _compileErrors;
};

#endif // SHADER_H
