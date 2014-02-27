TEMPLATE = app
TARGET = bsplines

CONFIG -= qt
CONFIG += c++11

CONFIG (debug, debug|release) {
    BUILD_TYPE = "debug"
    DEFINES += DEBUG
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
win32:QMAKE_LFLAGS += /ENTRY:mainCRTStartup

macx:DEFINES += Q_OS_MAC
macx:LIBS += -framework OpenGL -framework GLUT -framework AppKit
macx:QMAKE_MAC_SDK = macosx

macx:QMAKE_POST_LINK = mkdir -p $${TARGET}.app/Contents/Resources/ && rm -rf $${TARGET}.app/Contents/Resources/shaders && ditto shaders $${TARGET}.app/Contents/Resources/shaders

SOURCES += \
    oak/application.cpp \
    oak/clock.cpp \
    oak/color.cpp \
    oak/geometricobject.cpp \
    oak/pointf.cpp \
    oak/rectf.cpp \
    oak/spline.cpp \
    oak/statictext.cpp \
    oak/timer.cpp \
    oak/window.cpp \
    oak/log.cpp \

HEADERS += \
    oak/application.h \
    oak/clock.h \
    oak/color.h \
    oak/config.h \
    oak/geometricobject.h \
    oak/pointf.h \
    oak/rectf.h \
    oak/spline.h \
    oak/statictext.h \
    oak/timer.h \
    oak/window.h \
    oak/log.h \

SOURCES += \
    main.cpp \
    src/applicationcontroller.cpp \
    src/glslbuffer.cpp \
    src/glslpainter.cpp \
    src/glslprogram.cpp \
    src/glslshader.cpp \
    src/glsltexture2d.cpp \
    src/sourcefile.cpp \
    src/util.cpp \
    src/glsluniform.cpp \
    src/controlpointsglslprogram.cpp \
    src/splineglslprogram.cpp \
    src/shaderloader.cpp \
    src/openglvertexarray.cpp \
    src/texture2dglslprogram.cpp \
    src/texttexture.cpp \

HEADERS += \
    src/applicationcontroller.h \
    src/glslbuffer.h \
    src/glslpainter.h \
    src/glslprogram.h \
    src/glslshader.h \
    src/glsltexture2d.h \
    src/global.h \
    src/sourcefile.h \
    src/util.h \
    src/sharedvalue.h \
    src/glsluniform.h \
    src/controlpointsglslprogram.h \
    src/splineglslprogram.h \
    src/shaderloader.h \
    src/openglvertexarray.h \
    src/texture2dglslprogram.h \
    src/texttexture.h \

macx:OBJECTIVE_SOURCES += \
    src/texttexture_mac.mm \
    oak/application_mac.mm \

win32:SOURCES += \
    src/texttexture_win.cpp \

### Shaders ###

OTHER_FILES += \
    shaders/fragment.glsl \
    shaders/fragment_texture.glsl \
    shaders/geometry.glsl \
    shaders/geometry_tess.glsl \
    shaders/tesscontrol.glsl \
    shaders/tesseval.glsl \
    shaders/vertex.glsl \
    shaders/vertex_texture.glsl \



