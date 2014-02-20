TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -framework OpenGL -framework GLUT

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





