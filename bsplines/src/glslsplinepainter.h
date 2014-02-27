#ifndef GLSLSPLINEPAINTER_H
#define GLSLSPLINEPAINTER_H

#include "glslpainter.h"


class ControlPointsGlslProgram;
class SplineGlslProgram;
class SplineGlslProgram21;
class Texture2dGlslProgram;

enum class GlslSplinePainterVersion { Basic2_1, Basic3_2, Tess4_0 };

class GlslSplinePainter : public GlslPainter<oak::Spline>
{
public:
    GlslSplinePainter(GlslSplinePainterVersion v, std::vector<oak::Spline>& splines);
    GlslSplinePainter(GlslSplinePainterVersion v);

    virtual bool prepare();
    virtual void paint(oak::Window* window);

    void setSplines(std::vector<oak::Spline>& splines);
    void setVersion(GlslSplinePainterVersion v);
    void setStripsPerSegment(unsigned int value);

private:
    ControlPointsGlslProgram* controlPointsProg() const;
    SplineGlslProgram* splinesProg() const;
    SplineGlslProgram21* splinesProg21() const;

private:
    bool prepare2_1();
    bool prepare3_2(bool withTess);
    void paint2_1(oak::Window* window);
    void paint3_2(oak::Window* window);
    void paint4_0(oak::Window* window);
    void paintPoints3_2(oak::Window* window);


private:
    GlslSplinePainterVersion _v;
    unsigned int _stripsPerSegment;

};

#endif // GLSLSPLINEPAINTER_H
