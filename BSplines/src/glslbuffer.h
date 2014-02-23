#ifndef GLSLBUFFER_H
#define GLSLBUFFER_H

class GlslBuffer
{
public:
    GlslBuffer();
    virtual ~GlslBuffer();

    unsigned int size() const;

    bool alloc(unsigned int size);
    void free();
    void set(void* data, long size);

    virtual void enable() =0;
    virtual void disable() =0;

private:
    unsigned int _size;
    unsigned int _object;
};

class GlslVertexBuffer : public GlslBuffer
{
public:
    virtual void enable();
    virtual void disable();

};

#endif // GLSLBUFFER_H
