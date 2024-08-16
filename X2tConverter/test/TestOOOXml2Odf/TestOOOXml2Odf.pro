TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

DEFINES += BUILD_X2T_AS_LIBRARY_DYLIB

X2T_PATH = $$PWD/../..
CORE_PATH = $$PWD/../../..

include($$X2T_PATH/build/Qt/X2tConverter.pri)
include($$CORE_PATH/Common/3dParty/googletest/googletest.pri)

HEADERS += $$X2T_PATH/src/dylib/x2t.h
SOURCES += main.cpp\
    $$X2T_PATH/src/dylib/x2t.cpp

DESTDIR = $$CORE_BUILDS_BINARY_PATH
