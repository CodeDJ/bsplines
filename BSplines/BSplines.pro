TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

win32:DEFINES += Q_OS_WIN
win32:INCLUDEPATH += ../3rdparty/glew-1.10.0/include ../3rdparty/freeglut/include
win32:LIBS += -L../3rdparty/glew-1.10.0/lib/Release/Win32 -lglew32 -L../3rdparty/freeglut/lib

macx:DEFINES += Q_OS_MAC
macx:LIBS += -framework OpenGL -framework GLUT
#macx:QMAKE_MAC_SDK = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk
macx:QMAKE_MAC_SDK = macosx

SOURCES += \
    oak/timer.cpp \

HEADERS += \
    oak/timer.h \


SOURCES += \
    main.cpp \
    src/glslprogram.cpp \
    src/glslshader.cpp \
    src/sourcefile.cpp \
    src/util.cpp \
    src/glsluniform.cpp \
    src/controlpointsglslprogram.cpp \
    src/splineglslprogram.cpp \
    src/shaderloader.cpp

HEADERS += \
    src/glslprogram.h \
    src/glslshader.h \
    src/sourcefile.h \
    src/util.h \
    src/sharedvalue.h \
    src/glsluniform.h \
    src/controlpointsglslprogram.h \
    src/splineglslprogram.h \
    src/shaderloader.h

