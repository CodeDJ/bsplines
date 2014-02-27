#ifndef GLSLPAINTER_H
#define GLSLPAINTER_H

#include "glslprogram.h"
#include "oak/rectf.h"
#include "oak/geometricobject.h"
#include "oak/statictext.h"
#include "oak/color.h"

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
    GlslPainter(const Object& object)
        : _isPrepared(false)
    {
            static_assert(std::is_base_of<oak::GeometricObject, Object>::value, "Object not derived from oak::GeometricObject");
            _objects .push_back(object);
    }
    GlslPainter(const std::vector<Object>& objects)
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
    template<class Program, typename ...Args>
    bool addProgram(Args... args)
    {
        static_assert(std::is_base_of<GlslProgram, Program>::value, "Program not derived from GlslProgram");

        Program* program = new Program(args...);
        program->create();
        if (!program->link())
            return false;
        _programs.push_back(program);
        return true;
    }

protected:
    std::vector<GlslProgram*> _programs;
    std::vector<Object> _objects;
    bool _isPrepared;
};

class GlslScenePainter
{
public:

private:

};

#endif // GLSLPAINTER_H
