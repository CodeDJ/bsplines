#ifndef GLSLPAINTER_H
#define GLSLPAINTER_H

#include "glslprogram.h"

#include <vector>

namespace oak
{
    class GeometricObject;
    class Spline;
    class Window;
}

template<class Object>
class GlslPainter
{
public:
    GlslPainter(std::vector<Object>& objects)
        : _objects(objects),
          _isPrepared(false)
    {
            static_assert(std::is_base_of<oak::GeometricObject, Object>::value, "Object not derived from oak::GeometricObject");
    }

    virtual ~GlslPainter()
    {
        for(auto iter = _programs.begin(); iter != _programs.end(); ++iter)
        {
            delete *iter;
        }
    }

    std::vector<Object>& objects()
    {
        return _objects;
    }

    const std::vector<Object>&  objects() const
    {
        return _objects;
    }

    virtual bool prepare() =0;
    virtual void paint(oak::Window* window) =0;

protected:
    template<class Program>
    bool addProgram()
    {
        static_assert(std::is_base_of<GlslProgram, Program>::value, "Program not derived from GlslProgram");

        Program* program = new Program;
        program->create();
        if (!program->link())
        {
            //printf("Compile errors:%s\n", controlPointsProgram->compileErrors().c_str());
            //printf("Link errors: %s\n", controlPointsProgram->linkErrors().c_str());
            return false;
        }
        _programs.push_back(program);
        return true;
    }

protected:
    std::vector<GlslProgram*> _programs;
    std::vector<Object> _objects;
    bool _isPrepared;
};

class ControlPointsGlslProgram;
class SplineGlslProgram;
class ApplicationController;

class GlslSplinePainter : public GlslPainter<oak::Spline>
{
public:
    GlslSplinePainter(std::vector<oak::Spline>& splines, bool useTessellation = true);

    virtual bool prepare();
    virtual void paint(oak::Window* window);

    void setUseTessellation(bool use);
    void setStripsPerSegment(unsigned int value);

    ControlPointsGlslProgram* controlPointsProg() const;
    SplineGlslProgram* splinesProg() const;

private:
    bool _useTessellation;
    unsigned int _stripsPerSegment;

};

class GlslScenePainter
{
public:

private:

};

#endif // GLSLPAINTER_H
