#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include <map>
#include <string>

class GlslShader;
enum class GlslShaderType;

class ShaderLoader
{
public:
    static ShaderLoader& instance();

    ShaderLoader(const ShaderLoader&) = delete;
    ShaderLoader& operator=(const ShaderLoader&) = delete;

    int loadFromDir(const std::string& dir);
    void clear();

    GlslShader getShader(GlslShaderType type, const std::string& name);

private:
    ShaderLoader();


private:
    std::map<std::string, GlslShader> _shaders;
};

#endif // SHADERLOADER_H
