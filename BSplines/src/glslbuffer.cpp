#include "glslbuffer.h"

#ifdef Q_OS_MAC
#include <OpenGL/gl3.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif


GlslBuffer::GlslBuffer()
    : _size(0),
      _object(0) // TODO: check if 0 is an invalid value
{
}

GlslBuffer::~GlslBuffer()
{
    free();
}


unsigned int GlslBuffer::size() const
{
    return _size;
}

bool GlslBuffer::alloc(unsigned int size)
{
    if (_object)
    {
        if (_size == size)
            return true;
        free();
    }
    glGenBuffers(1, &_object);

    glBindBuffer(GL_ARRAY_BUFFER, _object);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    _size = size;
    return true;
}

void GlslBuffer::free()
{
    glDeleteBuffers(1, &_object);
    _object = 0;
    _size = 0;
}

void GlslBuffer::set(void* data, long size)
{
    glBindBuffer(GL_ARRAY_BUFFER, _object);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void GlslVertexBuffer::enable()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void GlslVertexBuffer::disable()
{
    glDisableVertexAttribArray(0);
}

