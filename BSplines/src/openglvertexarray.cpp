#include "openglvertexarray.h"

#include "oak/log.h"

#ifdef Q_OS_MAC
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#endif
#ifdef Q_OS_WIN
#include <GL/glew.h>
#endif

#include <string>

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
        CHECK_OPENGL_ERROR();
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
