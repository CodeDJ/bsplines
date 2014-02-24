TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

CONFIG (debug, debug|release) {
    BUILD_TYPE = "debug"
} else {
    BUILD_TYPE = "release"
}

win32:PLATFORM_DIR = "win"
macx:PLATFORM_DIR = "mac"

OBJECTS_DIR = .build/$$BUILD_TYPE/$$PLATFORM_DIR

INCLUDEPATH += src .

win32:DEFINES += Q_OS_WIN
win32:INCLUDEPATH += ../3rdparty/glew-1.10.0/include ../3rdparty/freeglut/include
win32:LIBS += -L../3rdparty/glew-1.10.0/lib/Release/Win32 -lglew32 -L../3rdparty/freeglut/lib

macx:DEFINES += Q_OS_MAC
macx:LIBS += -framework OpenGL -framework GLUT
macx:QMAKE_MAC_SDK = macosx

SOURCES += \
    oak/application.cpp \
    oak/clock.cpp \
    oak/color.cpp \
    oak/geometricobject.cpp \
    oak/point.cpp \
    oak/spline.cpp \
    oak/timer.cpp \
    oak/window.cpp \

HEADERS += \
    oak/application.h \
    oak/clock.h \
    oak/color.h \
    oak/config.h \
    oak/geometricobject.h \
    oak/point.h \
    oak/spline.h \
    oak/timer.h \
    oak/window.h \

SOURCES += \
    main.cpp \
    src/applicationcontroller.cpp \
    src/glslbuffer.cpp \
    src/glslpainter.cpp \
    src/glslprogram.cpp \
    src/glslshader.cpp \
    src/sourcefile.cpp \
    src/util.cpp \
    src/glsluniform.cpp \
    src/controlpointsglslprogram.cpp \
    src/splineglslprogram.cpp \
    src/shaderloader.cpp \
    src/openglvertexarray.cpp \

HEADERS += \
    src/applicationcontroller.h \
    src/glslbuffer.h \
    src/glslpainter.h \
    src/glslprogram.h \
    src/glslshader.h \
    src/global.h \
    src/sourcefile.h \
    src/util.h \
    src/sharedvalue.h \
    src/glsluniform.h \
    src/controlpointsglslprogram.h \
    src/splineglslprogram.h \
    src/shaderloader.h \
    src/openglvertexarray.h \

### Shaders ###

OTHER_FILES += \
    shaders/fragment.glsl \
    shaders/geometry.glsl \
    shaders/geometry_tess.glsl \
    shaders/tesscontrol.glsl \
    shaders/tesseval.glsl \
    shaders/vertex.glsl \
