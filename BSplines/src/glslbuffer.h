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
    virtual void enable();
    virtual void disable();

};

#endif // GLSLBUFFER_H
