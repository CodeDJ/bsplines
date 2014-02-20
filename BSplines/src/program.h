#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <map>

#include "shader.h"

class SourceFile;

class Program
{
public:
    Program();
    ~Program();

    void removeShader(Shader::Type shaderType);
    void addShader(Shader* shader);

    bool create();
    bool link();

    unsigned int id() const;

private:
    Program(const Program&);
    Program& operator=(const Program&);

private:
    bool attachShaders();
    bool detachShaders();
    bool compileShaders();

private:
    unsigned int _id;
    std::map<Shader::Type, Shader*> _shaders;
    std::string _linkErrors;
};

#endif // PROGRAM_H
