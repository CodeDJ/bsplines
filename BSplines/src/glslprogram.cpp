#include "glslprogram.h"

#include "glslshader.h"

#include <assert.h>

#ifdef Q_OS_MAC
#include <OpenGL/gl3.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif

GlslProgram::GlslProgram(const std::string& name /*= ""*/)
    : _id(0, [](unsigned int& id) { if (id) glDeleteProgram(id);}),
      _name(name)
{
}

GlslProgram::~GlslProgram()
{
}


void GlslProgram::removeShaders(GlslShaderType shaderType)
{
    _shaders.erase(shaderType);
}

void GlslProgram::removeShader(const GlslShader& shader)
{
    auto range = _shaders.equal_range(shader.type());
    _shaders.erase(range.first, range.second);
}

void GlslProgram::addShader(const GlslShader& shader)
{
    _shaders.insert(std::pair<GlslShaderType, GlslShader>(shader.type(), shader));
}


bool GlslProgram::create()
{
    if (_id)
        return true;

    beginCreate();
    _id = glCreateProgram();
    endCreate();

    return _id != 0;
}

bool GlslProgram::compile()
{
    beginCompile();

    bool result = false;
    _compileErrors.clear();

    if (create())
    {
        result = true;
        for (auto iter = _shaders.begin(); iter != _shaders.end(); iter++)
        {
            if (!iter->second.compile())
            {
                _compileErrors = "Shader " + iter->second.typeAsStr() + " - " + iter->second.compileErrors();
                result = false;
                break;
            }
        }
    }

    endCompile(result);

    return result;
}

bool GlslProgram::link()
{
    beginLink();

    bool result = false;
    _linkErrors.clear();

    if (create() && compile())
    {
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
            delete[] infoLog;
        }

        detachShaders();
        result = _id != 0;
    }

    endLink(result);

    return result;
}

std::string GlslProgram::linkErrors() const
{
    return _linkErrors;
}

std::string GlslProgram::compileErrors() const
{
    return _compileErrors;
}

void GlslProgram::bind()
{
    glUseProgram(_id);
}

void GlslProgram::unbind()
{
    glUseProgram(0);
}

unsigned int GlslProgram::id() const
{
    return _id;
}

std::string GlslProgram::name() const
{
    return _name;
}

bool GlslProgram::attachShaders()
{
    if (!create())
        return false;

    for (auto iter = _shaders.begin(); iter != _shaders.end(); iter++)
    {
        iter->second.attach(*this);
    }

    return true;
}

bool GlslProgram::detachShaders()
{
    if (!_id)
        return true;

    for (auto iter = _shaders.begin(); iter != _shaders.end(); iter++)
    {
        iter->second.dettach(*this);
    }
    return true;
}




