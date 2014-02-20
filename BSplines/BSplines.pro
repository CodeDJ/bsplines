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
    main.cpp \
    src/program.cpp \
    src/shader.cpp \
    src/sourcefile.cpp \
    src/util.cpp \
    src/vertexshader.cpp \

HEADERS += \
    src/program.h \
    src/shader.h \
    src/sourcefile.h \
    src/util.h \
    src/vertexshader.h \
