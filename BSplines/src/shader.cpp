#include "shader.h"

#include "sourcefile.h"
#include "program.h"

#include <OpenGL/gl3.h>

#include <assert.h>

namespace {

static GLenum shaderTypeToOpenGlType(Shader::Type type)
{
    switch(type)
    {
    case Shader::Vertex:
        return GL_VERTEX_SHADER;
    case Shader::TessControl:
        return GL_TESS_CONTROL_SHADER;
    case Shader::TessEvaluation:
        return GL_TESS_EVALUATION_SHADER;
    case Shader::Geometry:
        return GL_GEOMETRY_SHADER;
    case Shader::Fragment:
        return GL_FRAGMENT_SHADER;
    case Shader::None:
    default:
        assert(0);
        break;
    }
}

#if SHOW_UNUSED
std::string shaderTypeToStr(GLenum type)
{
    switch(type)
    {
    //case GL_COMPUTE_SHADER:
    //    return "compute";
    case GL_VERTEX_SHADER:
        return "vertex";
    case GL_TESS_CONTROL_SHADER:
        return "tessellation control";
    case GL_TESS_EVALUATION_SHADER:
        return "tessellation evaluation";
    case GL_GEOMETRY_SHADER:
        return "geometry";
    case GL_FRAGMENT_SHADER:
        return "fragment";
    default:
        return std::string();
    }

    return std::string();
}
#endif

}

Shader::Shader()
    : _type(None),
      _id(0)
{

}

Shader::Shader(const std::string& source)
    : _type(None),
      _source(source),
      _id(0)
{

}


Shader::~Shader()
{
    clear();
}

void Shader::clear()
{
    if (_id)
    {
        glDeleteShader(_id);
        _id = 0;
    }
    _source.clear();
}

void Shader::setSource(const std::string& source)
{
    clear();
    _source = source;
}

void Shader::setSource(SourceFile& sourceFile)
{
    clear();
    _source = sourceFile.getSource();
}

unsigned int Shader::id() const
{
    return _id;
}

std::string Shader::source() const
{
    return _source;
}

Shader::Type Shader::type() const
{
    return _type;
}


//        printf("[ERR] Compiler failure in %s shader:\n%s\n", strShaderType, infoLog);

bool Shader::create()
{
    if (_id)
        return true;

    if (_source.empty())
        return false;

    _id = glCreateShader(shaderTypeToOpenGlType(_type));

    return _id != 0;
}

bool Shader::compile()
{
    if (!create())
        return false;

    _compileErrors.clear();

    const char* sourceStr = _source.c_str();
    glShaderSource(_id, 1, &sourceStr, 0);

    glCompileShader(_id);

    GLint status;
    glGetShaderiv(_id, GL_COMPILE_STATUS, &status);
    if (status)
        return true;

    GLint infoLogLen = 0;
    glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &infoLogLen);
    GLchar* infoLog = new GLchar[infoLogLen + 1];
    glGetShaderInfoLog(_id, infoLogLen, 0, infoLog);
    _compileErrors = infoLog;
    delete[] infoLog;

    return false;
}

bool Shader::attach(const Program& program)
{
    if (!create())
        return false;
    glAttachShader(program.id(), _id);
    return true;
}

bool Shader::dettach(const Program& program)
{
    if (!_id)
        return true;
    glDetachShader(program.id(), _id);
    return true;
}

bool Shader::isSupported() const
{
    return false;
}
