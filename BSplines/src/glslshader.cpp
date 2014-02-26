#include "glslshader.h"

#include "sourcefile.h"
#include "glslprogram.h"
#include "oak/log.h"

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

GlslShader::GlslShader(const std::string& name, GlslShaderType type)
    : _name(name),
      _type(type),
      _id(0, [](unsigned int& id) { if (id) glDeleteShader(id);}),
      _dirtySource(true)
{

}

GlslShader::GlslShader(const std::string& name, GlslShaderType type, const std::string& source)
    : _name(name),
      _type(type),
      _id(0, [](unsigned int& id) { if (id) glDeleteShader(id);}),
      _origSource(source),
      _dirtySource(true)
{

}

GlslShader::~GlslShader()
{
}

void GlslShader::setSource(const std::string& source)
{
    _id.reset(0);
    _origSource = source;
    _dirtySource = true;
}

void GlslShader::setSource(SourceFile& sourceFile)
{
    _id.reset(0);
    _origSource = sourceFile.getSource();
    _dirtySource = true;
}

bool GlslShader::hasSource() const
{
    return !_origSource.empty();
}

unsigned int GlslShader::id() const
{
    return _id;
}

std::string GlslShader::getSource()
{
    if (_dirtySource)
    {
        static const std::string paramMarker("%");
        _source = _origSource;
        for(auto param = _params.begin(); param != _params.end(); ++param)
        {
            std::string name = paramMarker + param->first + paramMarker;
            int replacements = 0;
            auto startPos = _source.find(name);
            while(startPos != std::string::npos)
            {
                assert(startPos + name.size() <= _source.size());
                _source.replace(startPos, name.size(), param->second);
                ++replacements;
                startPos = _source.find(name, startPos + param->second.size());
            }
            assert(!_params.size() || replacements);
        }
        _dirtySource = false;
    }

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

    if (getSource().empty())
        return false;

    _id = glCreateShader(shaderTypeToOpenGlType(_type));

    return _id != 0;
}

bool GlslShader::compile()
{
    if (getSource().empty())
    {
        _compileErrors.append(std::string("Shader type: ") + shaderTypeToStr(_type) +std::string(" source empty!"));
        return false;
    }

    if (!create())
        return false;

    _compileErrors.clear();

    std::string src = getSource();
    const char* sourceStr = src.c_str();
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

void GlslShader::setParam(const std::string& name, const std::string& value)
{
    _params[name] = value;
    _dirtySource = true;
}


