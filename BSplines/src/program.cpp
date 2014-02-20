#include "program.h"

#include "shader.h"
#include "sourcefile.h"

#include <assert.h>

#ifdef Q_OS_MAC
#include <OpenGL/gl3.h>
#endif

#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif

Program::Program()
    : _id(0)
{
}

Program::~Program()
{
    for (std::map<Shader::Type, Shader*>::iterator iter = _shaders.begin(); iter != _shaders.end(); )
    {
        delete iter->second;
    }

    if (_id)
    {
        glDeleteProgram(_id);
        _id = 0;
    }
}


void Program::removeShader(Shader::Type shaderType)
{
    std::map<Shader::Type, Shader*>::iterator iter = _shaders.find(shaderType);
    if (iter != _shaders.end())
    {
        delete iter->second;
        _shaders.erase(iter);
    }
}

void Program::addShader(Shader* shader)
{
    assert(shader && shader->type() != Shader::None);

    removeShader(shader->type());
    _shaders[shader->type()] = shader;
}


bool Program::create()
{
    if (_id)
        return true;

    _id = glCreateProgram();
    return _id != 0;
}

bool Program::link()
{
    _linkErrors.clear();
    if (!create())
        return false;

    if (!compileShaders())
        return false;

    attachShaders();

    glLinkProgram(_id);

    GLint status;
    glGetProgramiv(_id, GL_LINK_STATUS, &status);
    if (!status)
    {
        GLint infoLogLen = 0;
        glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &infoLogLen);
        GLchar* infoLog = new GLchar[infoLogLen + 1];
        glGetProgramInfoLog(_id, infoLogLen, 0, infoLog);
        _linkErrors = infoLog;
        //printf("[ERR] Link failure:\n%s\n", infoLog);
        delete[] infoLog;
    }

    detachShaders();

    return _id != 0;
}

unsigned int Program::id() const
{
    return _id;
}

bool Program::attachShaders()
{
    if (!create())
        return false;

    for (std::map<Shader::Type, Shader*>::const_iterator iter = _shaders.begin(); iter != _shaders.end(); iter++)
    {
        iter->second->attach(*this);
    }

    return true;
}

bool Program::detachShaders()
{
    if (!_id)
        return true;

    for (std::map<Shader::Type, Shader*>::const_iterator iter = _shaders.begin(); iter != _shaders.end(); iter++)
    {
        iter->second->dettach(*this);
    }
    return true;
}

bool Program::compileShaders()
{
    for (std::map<Shader::Type, Shader*>::const_iterator iter = _shaders.begin(); iter != _shaders.end(); iter++)
    {
        if (!iter->second->compile())
            return false;
    }
    return true;
}
