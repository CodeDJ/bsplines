#include "glslshader.h"

#include "sourcefile.h"
#include "glslprogram.h"

#include <OpenGL/gl3.h>

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
