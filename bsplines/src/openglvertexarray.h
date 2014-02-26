#ifndef OPENGLVERTEXARRAY_H
#define OPENGLVERTEXARRAY_H

#include "sharedvalue.h"

class OpenglVertexArray
{
public:
    OpenglVertexArray();
    ~OpenglVertexArray();

    bool bind();
    void unbind();

private:
    SharedValue<unsigned int> _vao;
};

#endif // OPENGLVERTEXARRAY_H
