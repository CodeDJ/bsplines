#include "openglvertexarray.h"

#ifdef Q_OS_MAC
#include <OpenGL/gl3.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif


OpenglVertexArray::OpenglVertexArray() :
    _vao(0, [](unsigned int& id) { if (id) glDeleteVertexArrays(1, &id); })
{
}


OpenglVertexArray::~OpenglVertexArray()
{
}

bool OpenglVertexArray::bind()
{
    if (!_vao)
    {
        unsigned int vao = 0;
        glGenVertexArrays(1, &vao);
        if (!vao)
            return false;
        _vao = vao;
    }
    glBindVertexArray(_vao);
    return true;
}

void OpenglVertexArray::unbind()
{
    glBindVertexArray(0);
}
