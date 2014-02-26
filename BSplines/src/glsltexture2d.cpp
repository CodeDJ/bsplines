#include "glsltexture2d.h"

#include "oak/log.h"

#ifdef Q_OS_MAC
#include <OpenGL/gl3.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif

#include <stdio.h>


GlslTexture2D::GlslTexture2D()
    : _id(0)
{
}


void GlslTexture2D::alloc()
{
    if (_id)
        return;

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &_id);
    CHECK_OPENGL_ERROR();
}

void GlslTexture2D::bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    CHECK_OPENGL_ERROR();
}

void GlslTexture2D::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GlslTexture2D::set(void* data, int width, int height)
{
    if (!_id)
        return;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    CHECK_OPENGL_ERROR();
}
