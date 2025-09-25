TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += BUILD_X2T_AS_LIBRARY_DYLIB

X2T_DIR = $$PWD/../..

include($$X2T_DIR/build/Qt/X2tConverter.pri)

HEADERS += $$X2T_DIR/src/dylib/x2t.h
SOURCES += $$X2T_DIR/src/dylib/x2t.cpp

SOURCES += main.cpp

DESTDIR = $$CORE_BUILDS_BINARY_PATH
