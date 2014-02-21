
#include "oak/timer.h"
#include "oak/clock.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef Q_OS_MAC
//#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <OpenCL/cl.h>
#include <OpenGL/OpenGL.h>
#endif

#ifdef Q_OS_WIN
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/freeglut.h>
#endif

#include <string>
#include <vector>
#include <iostream>

//#include "Angel.h"

//#define OPENGL_2
//#define WITH_TESS

#define ANIMATE 1
#define MAX_CURVES 100
#define FULLSCREEN 0
#define RANDOM_POINTS 1
#define TIMER_MS 16

#define DEFAULT_SCREEN_WIDTH    640
#define DEFAULT_SCREEN_HEIGHT   480

int sScreenWidth = DEFAULT_SCREEN_WIDTH;
int sScreenHeight = DEFAULT_SCREEN_HEIGHT;

GLfloat gMinX = -1.0f;
GLfloat gMinY = -1.0f;
GLfloat gMaxX = 1.0f;
GLfloat gMaxY = 1.0f;

int window;
int shader_id;
GLuint gPositionBufferObject = 0;

#define CURVE_STRIPS 4
#define CURVE_STRIP_SEGMENTS 30

// THIS IS ALSO HARDCODED IN TCS
#define CURVE_CONTROL_POINTS (CURVE_STRIPS*3 + 1)

#define DEFAULT_CURVE_ALPHA 1.0
#define DEFAULT_CURVE_WIDTH 3

typedef struct
{
    float x;
    float y;
} Point;

typedef struct
{
    float r;
    float g;
    float b;
    float a;
} Color;

typedef struct {
    Point points[CURVE_CONTROL_POINTS];
    Color color;
    int width;
} CurveData;

CurveData *curves;

void printOpenGLError(int error, const char* message)
{
    printf("[ERR]: %s - OpenGL_Error: %s\n", message ? message : "", gluErrorString(static_cast<GLenum>(error)));
}

void ifOpenGLErrorCloseAndExit(void (*close)(), const char* message /*= 0*/)
{
    GLenum error = glGetError();
    if (GL_NO_ERROR == error)
        return;
    printOpenGLError(error, message);
    close();
    exit(0);
}

void randomize_curves (CurveData **curves)
{
    *curves = (CurveData *)malloc (sizeof (CurveData) * MAX_CURVES);
    int i;

    Point staticPoints[CURVE_CONTROL_POINTS] = {
        {-0.5, 0.0}, {-0.5, 0.5}, {-0.2, 0.5}, {-0.2, 0.0}, {-0.2, -0.5},
        {0.2, -0.5}, {0.2, 0.0}, {0.2, 0.5}, {0.5, 0.5}, {0.5, 0.0},
        {0.5, -0.5},  {0.7, -0.5}, {0.7, 0.0}
    };
    for (i = 0; i < MAX_CURVES; i++) {
        for (int j=0; j<CURVE_CONTROL_POINTS; ++j)
        {
            if (RANDOM_POINTS)
            {
                (*curves) [i].points[j].x = ((rand()%2001) / 1000.0) - 1.0;
                (*curves) [i].points[j].y = ((rand()%2001) / 1000.0) - 1.0;
            }
            else
            {
                (*curves) [i].points[j] = staticPoints[j];
            }
        }

        (*curves) [i].color.r = (rand()%101) / 100.0;
        (*curves) [i].color.g = (rand()%101) / 100.0;
        (*curves) [i].color.b = (rand()%101) / 100.0;
        (*curves) [i].color.a = DEFAULT_CURVE_ALPHA;

        (*curves) [i].width = DEFAULT_CURVE_WIDTH;
    }
}


GLuint render_vao;
void initVertexBuffer()
{
    glGenVertexArrays(1, &render_vao);
    glBindVertexArray(render_vao);

    glGenBuffers(1, &gPositionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gPositionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * 2 * CURVE_CONTROL_POINTS, 0, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


GLuint createShader(GLenum shaderType, const std::string& shaderCode)
{
    GLuint shader = glCreateShader(shaderType);
    const char* data = shaderCode.c_str();
    glShaderSource(shader, 1, &data, 0);

    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        GLchar* infoLog = new GLchar[infoLogLen + 1];
        glGetShaderInfoLog(shader, infoLogLen, 0, infoLog);
        const char* strShaderType = (shaderType == GL_VERTEX_SHADER ? "vertex" : (shaderType == GL_FRAGMENT_SHADER ? "fragment" : "geometry"));
        printf("[ERR] Compiler failure in %s shader:\n%s\n", strShaderType, infoLog);
        delete[] infoLog;
    }
    return shader;
}

GLuint createProgram(const std::vector<GLuint>& shaderList)
{
    GLuint program = glCreateProgram();
    for (auto shader : shaderList)
    {
        glAttachShader(program, shader);
    }

    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status)
    {
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        GLchar* infoLog = new GLchar[infoLogLen + 1];
        glGetProgramInfoLog(program, infoLogLen, 0, infoLog);
        printf("[ERR] Link failure:\n%s\n", infoLog);
        delete[] infoLog;
    }
    for (auto shader : shaderList)
    {
        glDetachShader(program, shader);
    }
    return program;
}


#ifdef OPENGL_2
static const std::string vertexShaderCode =   "#version 110\n"
                                              ""
                                              "void main()\n"
                                              "{\n"
                                              "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
                                              "}";


static const std::string fragmentShaderCode = "#version 110\n"
                                              ""
                                              "void main()\n"
                                              "{\n"
                                              "  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); \n"
                                              "}";
#else

static const std::string vertexShaderCode =   "#version 330\n"
                                              "layout (location = 0 ) in vec2 VertexPosition;\n"
        "out vec3 SegmentColor;\n"
                                              "void main()\n"
                                              "{\n"
                                              "gl_Position = vec4(VertexPosition, 0.0, 1.0);\n"
        "SegmentColor = vec3(1.0, 1.0, 1.0);"
                                              "}";


static const std::string fragmentShaderCode = "#version 330\n"
                                              "in vec3 SegmentColor;\n"
                                              "uniform vec4 curbColor;\n"
                                              "layout ( location = 0 ) out vec4 FragColor;\n"
                                              "void main()\n"
                                              "{\n"
                                              "  //FragColor = vec4(SegmentColor, 0.5); \n"
                                              "  FragColor = curbColor; \n"
                                              "}";

static const std::string TCShaderCode = ""
                                        "#version 150\n"
                                        "layout( vertices=13 ) out;\n"
                                        "uniform int numSegments = 30;\n"
                                        "uniform int numStrips = 3;\n"
                                        "void main() {\n"
                                                   "// Pass along the vertex position unmodified\n"
                                                   "gl_out[gl_InvocationID].gl_Position =\n"
                                                             "gl_in[gl_InvocationID].gl_Position;\n"
                                                   "// Define the tessellation levels (this works on\n"
                                                   "// ATI Catalyst drivers as of this writing, you may\n"
                                                   "// need to swap these)\n"
                                                   "gl_TessLevelOuter[0] = float(numStrips);\n"
                                                   "gl_TessLevelOuter[1] = float(numSegments);\n"
                                        "}\n";

/*
static const std::string TEShaderCode = ""
                                        "#version 400\n"
                                           "layout( isolines ) in;\n"
                                           "//uniform mat4 MVP;  // projection * view * model\n"
                                        "void main() {\n"
                                               "// The tessellation u coordinate\n"
                                               "float u = gl_TessCoord.x;\n"
                                               "// The patch vertices (control points)\n"
                                               "vec3 p0 = gl_in[0].gl_Position.xyz;\n"
                                               "vec3 p1 = gl_in[1].gl_Position.xyz;\n"
                                               "vec3 p2 = gl_in[2].gl_Position.xyz;\n"
                                               "vec3 p3 = gl_in[3].gl_Position.xyz;\n"
                                               "float u1 = (1.0 - u);\n"
                                               "float u2 = u * u;\n"
                                               "// Bernstein polynomials evaluated at u\n"
                                               "float b3 = u2 * u;\n"
                                               "float b2 = 3.0 * u2 * u1;\n"
                                               "float b1 = 3.0 * u * u1 * u1;\n"
                                               "float b0 = u1 * u1 * u1;\n"
                                               "// Cubic Bezier interpolation\n"
                                               "vec3 p = p0 * b0 + p1 * b1 + p2 * b2 + p3 * b3;\n"
                                               "gl_Position = vec4(p, 1.0);//MVP * \n"
                                           "}\n";
*/
static const std::string TEShaderCode = ""
                                        "#version 150\n"
                                           "layout( isolines ) in;\n"
                                           "uniform int numStrips = 3;\n"
                                            "out vec3 tesSegmentColor;\n"
        "vec3 quadratic_bezier(vec3 p0, vec3 p1, vec3 p2, float u)\n"
        "{\n"
        "vec3 p01 = mix(p0, p1, u);\n"
        "vec3 p12 = mix(p1, p2, u);\n"
        "vec3 p = mix(p01, p12, u);\n"
        "return p;\n"
        "}\n"
        "vec3 cubic_bezier(vec3 p0, vec3 p1, vec3 p2, vec3 p3, float u)\n"
        "{\n"
        "vec3 p01 = mix(p0, p1, u);\n"
        "vec3 p12 = mix(p1, p2, u);\n"
        "vec3 p23 = mix(p2, p3, u);\n"
        "return quadratic_bezier(p01, p12, p23, u);\n"
        "}\n"
        "vec3 cubic_bspline(vec3 p[4], float u)\n"
        "{\n"
        "if (u <= 0.0) return p[0];\n"
        "if (u >= 1.0) return p[3];\n"
        "return cubic_bezier(p[0], p[1], p[2], p[3], u);\n"
        "}\n"
        "vec3 evaluateBezierPosition( vec3 v[4], float t )"
        "{"
        "   float OneMinusT = 1.0 - t;"
        "   float b0 = OneMinusT*OneMinusT*OneMinusT;"
        "   float b1 = 3.0*t*OneMinusT*OneMinusT;"
        "   float b2 = 3.0*t*t*OneMinusT;"
        "   float b3 = t*t*t;"
        "   return b0*v[0] + b1*v[1] + b2*v[2] + b3*v[3];"
        "}\n"
                                        "void main() {\n"
                                                "// The tessellation u coordinate\n"
                                                "float u = gl_TessCoord.x;\n"
                                                "int step = int(floor(100.0/numStrips));\n"
                                                "int v = int(floor(gl_TessCoord.y*100.0))/step;\n"
                                                "//// The patch vertices (control points)\n"
                                                "vec3 p[4]; \n"
                                                "for (int i=0; i<4; ++i) p[i] = gl_in[v*3+i].gl_Position.xyz;\n"
                                                "// Cubic Bezier interpolation\n"
                                                "vec3 pf = cubic_bspline(p, u);\n"
                                                "//vec3 pf = evaluateBezierPosition(p, u);\n"
                                                "gl_Position = vec4(pf, 1.0); \n"
                                                "if (v==0) tesSegmentColor = vec3(1.0, 0.0, 0.0);\n"
                                                "else if (v==1) tesSegmentColor = vec3(0.0, 1.0, 0.0);\n"
                                                "else if (v==2) tesSegmentColor = vec3(0.0, 0.0, 1.0);\n"
                                                "else if (v==3) tesSegmentColor = vec3(1.0, 1.0, 0.0);\n"
                                                "else tesSegmentColor = vec3(1.0, 1.0, 1.0);\n"
                                           "}\n";

/*
"vec3 p[3*];\n"
"p[0] = gl_in[0].gl_Position.xyz;\n"
"p[1] = gl_in[1].gl_Position.xyz;\n"
"p[2] = gl_in[2].gl_Position.xyz;\n"
"p[3] = gl_in[3].gl_Position.xyz;\n"
"p[4] = gl_in[4].gl_Position.xyz;\n"
"p[5] = gl_in[5].gl_Position.xyz;\n"
"p[6] = gl_in[6].gl_Position.xyz;\n"
"p[7] = gl_in[7].gl_Position.xyz;\n"
"p[8] = gl_in[8].gl_Position.xyz;\n"
"p[9] = gl_in[9].gl_Position.xyz;\n"
"//p[10] = gl_in[10].gl_Position.xyz;\n"
"//p[11] = gl_in[11].gl_Position.xyz;\n"
"//p[12] = gl_in[12].gl_Position.xyz;\n"
*/
/*
static const std::string TEShaderCode = ""
                                        "#version 400\n"
                                           "layout( isolines ) in;\n"
                                           "//uniform mat4 MVP;  // projection * view * model\n"
                                        "void main() {\n"
                                                "float u = gl_TessCoord.x;\n"
                                                "vec3 p0 = gl_in[0].gl_Position.xyz;\n"
                                               "gl_Position = vec4(p0*u, 1.0);//MVP * \n"
                                           "}\n";
*/


static const std::string geomShaderCode =
                                            "#version 400\n"
                                            "layout (lines) in;\n"
                                            "layout (triangle_strip, max_vertices = 4) out;\n"
                                            "in vec3 tesSegmentColor[];\n"
                                            "out vec3 SegmentColor;\n"
                                            "uniform float lineWidthAlphaX = 0.003;"
                                            "uniform float lineWidthAlphaY = 0.003;"
                                            "void main()\n"
                                            "{\n"
                                            "SegmentColor = tesSegmentColor[0];\n"
                                            "float pixels = 0.003; \n"
                                            "vec3 v = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;\n"
                                            "vec4 n1 = vec4(normalize(vec3(-v.y, v.x, 0.0)), 0.0);\n"
                                            "vec4 n2 = vec4(normalize(vec3(v.y, -v.x, 0.0)), 0.0);\n"
                                            "n1.x = n1.x * lineWidthAlphaX / 2; \n"
                                            "n1.y = n1.y * lineWidthAlphaY / 2; \n"
                                            "n2.x = n2.x * lineWidthAlphaX / 2; \n"
                                            "n2.y = n2.y * lineWidthAlphaY / 2; \n"
                                            "gl_Position = gl_in[0].gl_Position + n2;//lineWidthAlphaX*n2;\n"
                                            "EmitVertex();\n"
                                            "gl_Position = gl_in[0].gl_Position + n1;//lineWidthAlpha*n1;\n"
                                            "EmitVertex();\n"
                                            "gl_Position= gl_in[1].gl_Position + n2;//lineWidthAlpha*n2;\n"
                                            "EmitVertex();\n"
                                            "gl_Position = gl_in[1].gl_Position + n1;//lineWidthAlpha*n1;\n"
                                            "EmitVertex();\n"
                                            "}";

/*
static const std::string geomShaderCode =
                                            "#version 150\n"
                                            "layout (lines) in;\n"
                                            "layout (line_strip, max_vertices = 2) out;\n"
                                            "void main()\n"
                                            "{\n"
                                                "for(int i=0; i<gl_in.length(); ++i) { gl_Position = gl_in[i].gl_Position; EmitVertex(); }\n"
                                                "EndPrimitive();\n"
                                            "}";
*/


static const std::string geomShaderCode1 =
                                            "#version 150\n"
                                            "layout (lines) in;\n"
                                            "layout (triangle_strip) out;\n"
                                            "uniform vec2 ControlPoints[13];"
        "uniform float lineWidthAlphaX = 0.003;"
        "uniform float lineWidthAlphaY = 0.003;"
                                            "layout(max_vertices = 1024) out;\n"
        "vec3 quadratic_bezier(vec3 p0, vec3 p1, vec3 p2, float u)\n"
        "{\n"
        "   vec3 p01 = mix(p0, p1, u);\n"
        "   vec3 p12 = mix(p1, p2, u);\n"
        "   vec3 p = mix(p01, p12, u);\n"
        "   return p;\n"
        "}\n"
        "vec3 cubic_bezier(vec3 p0, vec3 p1, vec3 p2, vec3 p3, float u)\n"
        "{\n"
        "   vec3 p01 = mix(p0, p1, u);\n"
        "   vec3 p12 = mix(p1, p2, u);\n"
        "   vec3 p23 = mix(p2, p3, u);\n"
        "   return quadratic_bezier(p01, p12, p23, u);\n"
        "}\n"
        "vec3 cubic_bspline(vec3 p[4], float u)\n"
        "{\n"
        "   if (u <= 0.0) return p[0];\n"
        "   if (u >= 1.0) return p[3];\n"
        "   return cubic_bezier(p[0], p[1], p[2], p[3], u);\n"
        "}\n"

        "vec3 evaluateBezierPosition( vec3 v[4], float t )"
        "{"
        "   float OneMinusT = 1.0 - t;"
        "   float b0 = OneMinusT*OneMinusT*OneMinusT;"
        "   float b1 = 3.0*t*OneMinusT*OneMinusT;"
        "   float b2 = 3.0*t*t*OneMinusT;"
        "   float b3 = t*t*t;"
        "   return b0*v[0] + b1*v[1] + b2*v[2] + b3*v[3];"
        "}\n"

        "void process(vec3 p0, vec3 p1, bool all) {\n"
        "vec3 v = p1 - p0;\n"
        "vec4 n1 = vec4(normalize(vec3(-v.y, v.x, 0.0)), 0.0);\n"
        "vec4 n2 = vec4(normalize(vec3(v.y, -v.x, 0.0)), 0.0);\n"
        "n1.x = n1.x * lineWidthAlphaX / 2; \n"
        "n1.y = n1.y * lineWidthAlphaY / 2; \n"
        "n2.x = n2.x * lineWidthAlphaX / 2; \n"
        "n2.y = n2.y * lineWidthAlphaY / 2; \n"
        "gl_Position = vec4(p1, 1.0) + n2;//lineWidthAlpha*n2;\n"
        "EmitVertex();\n"
        "gl_Position = vec4(p1, 1.0) + n1;//lineWidthAlpha*n1;\n"
        "EmitVertex();\n"
        "}\n"

        "void process_all(vec3 p0, vec3 p1, bool all) {\n"
        "vec3 v = p1 - p0;\n"
        "vec4 n1 = vec4(normalize(vec3(-v.y, v.x, 0.0)), 0.0);\n"
        "vec4 n2 = vec4(normalize(vec3(v.y, -v.x, 0.0)), 0.0);\n"
        "n1.x = n1.x * lineWidthAlphaX / 2; \n"
        "n1.y = n1.y * lineWidthAlphaY / 2; \n"
        "n2.x = n2.x * lineWidthAlphaX / 2; \n"
        "n2.y = n2.y * lineWidthAlphaY / 2; \n"
        "gl_Position = vec4(p0, 1.0) + n2;//lineWidthAlphaX*n2;\n"
        "EmitVertex();\n"
        "gl_Position = vec4(p0, 1.0) + n1;//lineWidthAlpha*n1;\n"
        "EmitVertex();\n"
        "gl_Position = vec4(p1, 1.0) + n2;//lineWidthAlpha*n2;\n"
        "EmitVertex();\n"
        "gl_Position = vec4(p1, 1.0) + n1;//lineWidthAlpha*n1;\n"
        "EmitVertex();\n"
        "}\n"


        "void main()\n"
        "{\n"
        "vec3 p[4];\n"
        "vec3 prev, crt;\n"
        "float step = 1.0/30.0;\n"
        "for (int v=0; v<4; ++v) {"
        "   p[0] = vec3(ControlPoints[3*v], 0.0);"
        "   p[1] = vec3(ControlPoints[3*v + 1], 0.0);"
        "   p[2] = vec3(ControlPoints[3*v + 2], 0.0);"
        "   p[3] = vec3(ControlPoints[3*v + 3], 0.0);"
        "   float u = 0.0;"
        "   //prev = cubic_bspline(p, u);\n"
        "   prev = p[0];\n"
        "   u += step;\n"
        "   //crt = cubic_bspline(p, u);\n"
        "   crt = evaluateBezierPosition(p, u);\n"
        "   process_all(prev, crt, true);\n"
        "   prev = crt;\n"
        "   for(int j=2; j<30; ++j) { "
        "       u += step;\n"
        "       //crt = cubic_bspline(p, u);\n"
        "       crt = evaluateBezierPosition(p, u);\n"
        "       process(prev, crt, false);"
        "       prev = crt;"
        "   }\n"
        "   crt = p[3];\n"
        "   process(prev, crt, false);"
        "}\n"
        "EndPrimitive();\n"
                                            "}";
///
/*
"//process(prev, vec3(ControlPoints[12], 0.0), false);\n"
*/

#endif

GLuint gProgram = 0;
GLuint gSimpleProgram = 0;

GLint gLineWidthAlphaX = -1;
GLint gLineWidthAlphaY = -1;
GLint gNumStrips = -1;
GLint gNumSegments = -1;
GLint gCurbColor = -1;
GLint gSimpleCurbColor = -1;

GLint gControlPoints = -1;

void initProgram()
{
    std::vector<GLuint> shaderList;
    shaderList.push_back(createShader(GL_VERTEX_SHADER, vertexShaderCode));
    shaderList.push_back(createShader(GL_FRAGMENT_SHADER, fragmentShaderCode));

#ifdef WITH_TESS
    shaderList.push_back(createShader(GL_TESS_EVALUATION_SHADER, TEShaderCode));
    shaderList.push_back(createShader(GL_TESS_CONTROL_SHADER, TCShaderCode));

    shaderList.push_back(createShader(GL_GEOMETRY_SHADER, geomShaderCode));

#else
    shaderList.push_back(createShader(GL_GEOMETRY_SHADER, geomShaderCode1));
#endif

    std::vector<GLuint> shaderList1;
    shaderList1.push_back(createShader(GL_VERTEX_SHADER, vertexShaderCode));
    shaderList1.push_back(createShader(GL_FRAGMENT_SHADER, fragmentShaderCode));



    gProgram = createProgram(shaderList);
    gSimpleProgram = createProgram(shaderList1);

    for(auto shader : shaderList)
    {
        glDeleteShader(shader);
    }

    gLineWidthAlphaX = glGetUniformLocation(gProgram, "lineWidthAlphaX");
    gLineWidthAlphaY = glGetUniformLocation(gProgram, "lineWidthAlphaY");

    gNumStrips = glGetUniformLocation(gProgram, "numStrips");
    gNumSegments = glGetUniformLocation(gProgram, "numSegments");
    gCurbColor = glGetUniformLocation(gProgram, "curbColor");
    gSimpleCurbColor = glGetUniformLocation(gSimpleProgram, "curbColor");

#ifndef WITH_TESS
    gControlPoints = glGetUniformLocation(gProgram, "ControlPoints");
#endif
}

void keyboardFunc(unsigned char /*key*/, int /*x*/, int /*y*/)
{
    glutDestroyWindow (window);
    exit (0);
}

void draw_curve_geom1(const CurveData& curveData)
{
    glBindBuffer(GL_ARRAY_BUFFER, gPositionBufferObject);
    float vertexPositions[4] = {
        curveData.points[0].x, curveData.points[0].y,
        curveData.points[12].x, curveData.points[12].y,
    };

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPositions), vertexPositions);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glPatchParameteri(GL_PATCH_VERTICES, CURVE_CONTROL_POINTS);
    glProgramUniform1f(gProgram, gLineWidthAlphaX, 2.0/sScreenWidth * curveData.width);
    glProgramUniform1f(gProgram, gLineWidthAlphaY, 2.0/sScreenHeight * curveData.width);
    glProgramUniform1i(gProgram, gNumStrips, CURVE_STRIPS);
    glProgramUniform1i(gProgram, gNumSegments, CURVE_STRIP_SEGMENTS);

    glProgramUniform4f(gProgram, gCurbColor, curveData.color.r, curveData.color.g, curveData.color.b, curveData.color.a);

    float vertexPositions1[26];
    for (int i=0; i<13; ++i)
    {
        vertexPositions1[i*2] = curveData.points[i].x;
        vertexPositions1[i*2+1] = curveData.points[i].y;
    }

    glUniform2fv(gControlPoints, 13, vertexPositions1);

    //clock_t tBegin = clock();
    //clock_t tEnd, t;

    glDrawArrays(GL_LINE_STRIP, 0, 2);

    //tEnd = clock();
    //t = tEnd - tBegin;
    //if (((float)t)/CLOCKS_PER_SEC*1000.0 > 5.0)
    //    printf ("draw_curve_geom1 - %d clocks - %fms\n",t,((float)t)/CLOCKS_PER_SEC*1000.0);

    glDisableVertexAttribArray(0);
}


void draw_points_geom(const CurveData& curveData)
{
    glBindBuffer(GL_ARRAY_BUFFER, gPositionBufferObject);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(curveData.points), curveData.points);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glPointSize(5.0f);
    glProgramUniform4f(gSimpleProgram, gSimpleCurbColor, curveData.color.r, curveData.color.g, curveData.color.b, 1.0);

    glDrawArrays(GL_POINTS, 0, CURVE_CONTROL_POINTS);
    glDisableVertexAttribArray(0);
}


void draw_curve_geom(const CurveData& curveData)
{
    glBindBuffer(GL_ARRAY_BUFFER, gPositionBufferObject);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(curveData.points), curveData.points);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);


    glProgramUniform1f(gProgram, gLineWidthAlphaX, 2.0/sScreenWidth * curveData.width);
    glProgramUniform1f(gProgram, gLineWidthAlphaY, 2.0/sScreenHeight * curveData.width);
    glProgramUniform1i(gProgram, gNumStrips, CURVE_STRIPS);
    glProgramUniform1i(gProgram, gNumSegments, CURVE_STRIP_SEGMENTS);
    glProgramUniform4f(gProgram, gCurbColor, curveData.color.r, curveData.color.g, curveData.color.b, curveData.color.a);

    glPatchParameteri(GL_PATCH_VERTICES, CURVE_CONTROL_POINTS);

    glDrawArrays(GL_PATCHES, 0, CURVE_CONTROL_POINTS);
    glDisableVertexAttribArray(0);
}

void updateCurves()
{
    for (int i=0; i<MAX_CURVES; ++i)
    {
        for (int j=0; j<CURVE_CONTROL_POINTS; ++j )
        {
            //if (j%3)
            {
                curves[i].points[j].x += (rand()%100 - 50)/10000.0;
                curves[i].points[j].y += (rand()%100 - 50)/10000.0;
            }

        }
    }
}

void displayFunc(void)
{
    oak::Clock clck("displayFunc");
    std::cout << CGLGetCurrentContext() << std::endl;

    glClear (GL_COLOR_BUFFER_BIT);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(3.0);
    //GLint error = glGetError(); printOpenGLError(error, 0);
    //GLint range[3];
    //glGetIntegerv(GL_LINE_WIDTH_RANGE, range);
    //glGetIntegerv(GL_ALIASED_LINE_WIDTH_RANGE, range);
    //glGetIntegerv(GL_SMOOTH_LINE_WIDTH_RANGE, range);
    //printf("------ %d %d %d\n", range[0], range[1], range[2]);

    glUseProgram(gProgram);
    for (int i = 0; i < MAX_CURVES; ++i)
    {
#ifdef WITH_TESS
        draw_curve_geom(curves[i]);
#else
        draw_curve_geom1(curves[i]);
#endif
    }

    glUseProgram(0);
    glUseProgram(gSimpleProgram);
    for (int i = 0; i < MAX_CURVES; ++i)
    {
        draw_points_geom(curves[i]);
    }

    glUseProgram(0);

    //glFlush();
    //std::cout << "1 - " << clck << std::endl;
    //glFinish();
    //std::cout << "2 - " << clck << std::endl;
    //Sleep(23);
    glutSwapBuffers();

    std::cout << "3 - " << clck << std::endl;
    //usleep(10000);
    //std::cout << "4 - " << clck << std::endl << std::endl;
}


void resize(int width, int height)
{
    sScreenWidth = width;
    sScreenHeight = height;

    glViewport(0, 0, sScreenWidth, sScreenHeight);

    GLfloat aspect = (GLfloat)width / height;
#ifdef OPENGL_2
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#endif

    if (width >= height)
    {
        gMinX = 0.0f;
        gMaxX = 1.0f * aspect;
        gMinY = 0.0f;
        gMaxY = 1.0f;
        /*
        gMinX = -1.0f * aspect;
        gMaxX = 1.0f * aspect;
        gMinY = -1.0f;
        gMaxY = 1.0f;
        */
    }
    else
    {
        gMinX = 0.0f;
        gMaxX = 1.0f;
        gMinY = 0.0f;
        gMaxY = 1.0f / aspect;
        /*
        gMinX = -1.0f;
        gMaxX = 1.0f;
        gMinY = -1.0f / aspect;
        gMaxY = 1.0f / aspect;
        */
    }

    //gMinX = 0.0f; gMaxX = width;
    //gMinY = 0.0f; gMaxY = height;
#ifdef OPENGL_2
    gluOrtho2D(gMinX, gMaxX, gMinY, gMaxY);
#endif
    printf("gMin/Max: (%5.2f,%5.2f) (%5.2f,%5.2f) \n", gMinX, gMaxX, gMinY, gMaxY);
    //gluOrtho2D(0, width, 0, height);
    glDisable(GL_DEPTH_TEST);

}

void initOpenGL(int argc, char* argv[])
{
    glutInit (&argc, argv);
#ifdef OPENGL_2
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
#else
#ifdef Q_OS_MAC
    glutInitDisplayMode (GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE);
#endif
#ifdef Q_OS_WIN
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
    glutInitContextVersion(3, 2);
#endif
#endif
    glutInitWindowSize (DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
    glutInitWindowPosition(300, 300);

    window = glutCreateWindow(argv[0]);
#ifdef Q_OS_WIN
    glewExperimental = GL_TRUE;
    glewInit();

    //wglSwapIntervalEXT(0);
#endif

#ifdef Q_OS_MAC
    //GLint vsyncOff = 0;
    //std::cout << CGLGetCurrentContext() << std::endl;
    //CGLGetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &vsyncOff);
    //std::cout << vsyncOff << std::endl;
    //vsyncOff = 0;
    //CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &vsyncOff);
    //CGLGetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &vsyncOff);
    //std::cout << vsyncOff << std::endl;
#endif

    if (FULLSCREEN) glutFullScreen();


    printf("%s\n%s\n",
            glGetString(GL_RENDERER),  // e.g. Intel HD Graphics 3000 OpenGL Engine
            glGetString(GL_VERSION)    // e.g. 3.2 INTEL-8.0.61
            );


    glutDisplayFunc (displayFunc);
    glutKeyboardFunc (keyboardFunc);
    glutReshapeFunc(resize);
    if (ANIMATE)
    {
        oak::Timer* timer = new oak::Timer(TIMER_MS, true,
            [] (oak::Timer*)
            {
                updateCurves();
                glutPostRedisplay();
            });
        timer->start();
    }

    /* GL settings */
    glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
    resize(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);

    srand (time(0));
    randomize_curves (&curves);

/*
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    */

}

int main(int argc, char* argv[])
{
    initOpenGL(argc, argv);
    initProgram();
    initVertexBuffer();

    //config_programs();

    glutMainLoop();
    return 0;
}
