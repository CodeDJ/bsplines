#include "shaderloader.h"

#include "glslshader.h"
#include "util.h"
#include "global.h"


/*static */
ShaderLoader& ShaderLoader::instance()
{
    static ShaderLoader shaderLoader;
    return shaderLoader;
}

ShaderLoader::ShaderLoader()
{
}

GlslShader ShaderLoader::getShader(GlslShaderType type, const std::string& name)
{
    auto iter = _shaders.find(name);
    if (iter != _shaders.end())
        return iter->second;

    GlslShader shader(type);

    std::string src = Util::loadFile(std::string("shaders/") + name + ".glsl");
    if (!src.empty())
    {
        shader.setSource(src);
        _shaders.insert(std::pair<std::string, GlslShader>(name, shader));
    }
    return shader;
}

int ShaderLoader::loadFromDir(const std::string& dir)
{
    UNUSED(dir);
    return 0;
}

void ShaderLoader::clear()
{
    _shaders.clear();
}

