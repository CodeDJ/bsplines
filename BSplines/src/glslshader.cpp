#include "glslshader.h"

#include "sourcefile.h"
#include "glslprogram.h"

#ifdef Q_OS_MAC
#include <OpenGL/gl3.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif

#include <assert.h>

namespace {

static GLenum shaderTypeToOpenGlType(GlslShaderType type)
{
    switch(type)
    {
    case GlslShaderType::Vertex:
        return GL_VERTEX_SHADER;
    case GlslShaderType::TessControl:
        return GL_TESS_CONTROL_SHADER;
    case GlslShaderType::TessEvaluation:
        return GL_TESS_EVALUATION_SHADER;
    case GlslShaderType::Geometry:
        return GL_GEOMETRY_SHADER;
    case GlslShaderType::Fragment:
        return GL_FRAGMENT_SHADER;
    default:
        assert(0);
        break;
    }
}

static std::string shaderTypeToStr(GlslShaderType type)
{
    switch(type)
    {
    //case GL_COMPUTE_SHADER:
    //    return "compute";
    case GlslShaderType::Vertex:
        return "vertex";
    case GlslShaderType::TessControl:
        return "tessellation control";
    case GlslShaderType::TessEvaluation:
        return "tessellation evaluation";
    case GlslShaderType::Geometry:
        return "geometry";
    case GlslShaderType::Fragment:
        return "fragment";
    default:
        return std::string();
    }

    return std::string();
}

}

GlslShader::GlslShader(GlslShaderType type)
    : _type(type),
      _id(0, [](const unsigned int& id) { if (id) glDeleteShader(id);})
{

}

GlslShader::GlslShader(GlslShaderType type, const std::string& source)
    : _type(type),
      _id(0, [](const unsigned int& id) { if (id) glDeleteShader(id);}),
      _source(source)
{

}

GlslShader::~GlslShader()
{
}

void GlslShader::setSource(const std::string& source)
{
    _id.reset(0);
    _source = source;
}

void GlslShader::setSource(SourceFile& sourceFile)
{
    _id.reset(0);
    _source = sourceFile.getSource();
}

bool GlslShader::hasSource() const
{
    return !_source.empty();
}

unsigned int GlslShader::id() const
{
    return _id;
}

std::string GlslShader::source() const
{
    return _source;
}

GlslShaderType GlslShader::type() const
{
    return _type;
}

std::string GlslShader::typeAsStr() const
{
    return shaderTypeToStr(_type);
}

std::string GlslShader::compileErrors() const
{
    return _compileErrors;
}

bool GlslShader::create()
{
    if (_id)
        return true;

    if (_source.empty())
        return false;

    _id = glCreateShader(shaderTypeToOpenGlType(_type));

    return _id != 0;
}

bool GlslShader::compile()
{
    if (_source.empty())
    {
        _compileErrors.append(std::string("Shader type: ") + shaderTypeToStr(_type) +std::string(" source empty!"));
        return false;
    }

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

bool GlslShader::attach(const GlslProgram& program)
{
    if (!create())
        return false;

    glAttachShader(program.id(), _id);

    return true;
}

bool GlslShader::dettach(const GlslProgram& program)
{
    if (!_id)
        return true;

    glDetachShader(program.id(), _id);

    return true;
}

bool GlslShader::isSupported() const
{
    return true;
}
