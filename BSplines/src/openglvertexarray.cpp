#include "openglvertexarray.h"

#ifdef Q_OS_MAC
#include <OpenGL/gl3.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif


OpenglVertexArray::OpenglVertexArray()
    : _vao(0)
{
}


OpenglVertexArray::~OpenglVertexArray()
{
    if (_vao)
        glDeleteVertexArrays(1, &_vao);
}

bool OpenglVertexArray::bind()
{
    if (!_vao)
    {
        glGenVertexArrays(1, &_vao);
        if (!_vao)
            return false;
    }
    glBindVertexArray(_vao);
    return true;
}

void OpenglVertexArray::unbind()
{
    glBindVertexArray(0);
}
