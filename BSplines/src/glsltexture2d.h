#ifndef GLSLTEXTURE2D_H
#define GLSLTEXTURE2D_H

class GlslTexture2D
{
public:
    GlslTexture2D();

    void alloc();
    void bind();
    void unbind();
    void set(void* data, int width, int height);

private:
    unsigned int _id;
};

#endif // GLSLTEXTURE2D_H
