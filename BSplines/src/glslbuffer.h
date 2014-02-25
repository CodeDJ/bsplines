#ifndef GLSLBUFFER_H
#define GLSLBUFFER_H

class GlslBuffer
{
public:
    GlslBuffer();
    virtual ~GlslBuffer();

    unsigned int size() const;

    bool alloc(long size);
    void free();
    void set(const void* data, long size = -1);

    virtual void enable() =0;
    virtual void disable() =0;

private:
    long _size;
    unsigned int _object;
};

class GlslVertexBuffer : public GlslBuffer
{
public:
    GlslVertexBuffer(unsigned int components = 2);

    virtual void enable();
    virtual void disable();

private:
    unsigned int _components;

};

#endif // GLSLBUFFER_H
