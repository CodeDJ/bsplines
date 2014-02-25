#ifndef TEXTURE2DGLSLPROGRAM_H
#define TEXTURE2DGLSLPROGRAM_H


#include "glslprogram.h"
#include "glsluniform.h"
#include "glslbuffer.h"
#include "glsltexture2d.h"

class Texture2dGlslProgram : public GlslProgram
{
public:
    Texture2dGlslProgram();
    virtual ~Texture2dGlslProgram();

public:
    GlslVertexBuffer& vertexBuffer();
    GlslTexture2D& texture2D();

private:
    virtual void endCreate();
    virtual void endLink(bool result);

private:
    GlslVertexBuffer _vertexBuffer;
    GlslTexture2D _texture2D;
};

#endif // TEXTURE2DGLSLPROGRAM_H
