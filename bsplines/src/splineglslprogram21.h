#ifndef SPLINEGLSLPROGRAM21_H
#define SPLINEGLSLPROGRAM21_H

#include "glslprogram.h"
#include "glsluniform.h"
#include "glslbuffer.h"

#ifdef Q_OS_MAC
#include <OpenGL/gltypes.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif

class SplineGlslProgram21 : public GlslProgram
{
public:
    SplineGlslProgram21(GLuint segments = 1, GLuint strips = 1);
    virtual ~SplineGlslProgram21();

public:
    GlslUniform4f& pointColor();
    GlslVertexBuffer& vertexBuffer();
    GlslUniform<float, 2>& controlPoints();
    GlslUniform1i& passThrough();

protected:
    virtual void endCreate();
    virtual void endLink(bool result);

private:
    GlslUniform4f _pointColor;
    GlslVertexBuffer _vertexBuffer;
    GlslUniform<float, 2> _controlPoints;
    GlslUniform1i _passThrough;
    GLuint _segments;
    GLuint _strips;
};

#endif // SPLINEGLSLPROGRAM21_H
