#ifndef OPENGLVERTEXARRAY_H
#define OPENGLVERTEXARRAY_H

class OpenglVertexArray
{
public:
    OpenglVertexArray();
    ~OpenglVertexArray();

    bool bind();
    void unbind();

private:
    unsigned int _vao;
};

#endif // OPENGLVERTEXARRAY_H
