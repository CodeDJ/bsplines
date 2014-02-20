TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -framework OpenGL -framework GLUT

SOURCES += main.cpp

#macx:QMAKE_MAC_SDK = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk
macx:QMAKE_MAC_SDK = macosx
