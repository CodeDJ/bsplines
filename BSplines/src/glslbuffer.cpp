#include "glslbuffer.h"

#include "oak/log.h"

#include <assert.h>

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

bool GlslBuffer::alloc(long size)
{
    assert(size > 0);

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
    CHECK_OPENGL_ERROR();

    _size = size;
    return true;
}

void GlslBuffer::free()
{
    glDeleteBuffers(1, &_object);
    _object = 0;
    _size = 0;
}

void GlslBuffer::set(const void* data, long size /*= -1*/)
{
    if (size < 0 )
        size = _size;

    glBindBuffer(GL_ARRAY_BUFFER, _object);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    CHECK_OPENGL_ERROR();
}

GlslVertexBuffer::GlslVertexBuffer(unsigned int components /*= 2*/)
    : _components(components)
{
    assert(_components >= 1 && _components <= 4);
}

void GlslVertexBuffer::enable()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, _components, GL_FLOAT, GL_FALSE, 0, 0);
    CHECK_OPENGL_ERROR();
}

void GlslVertexBuffer::disable()
{
    glDisableVertexAttribArray(0);
}

