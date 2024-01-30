TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += BUILD_X2T_AS_LIBRARY_DYLIB

X2T_DIR = $$PWD/../../X2tConverter

include($$X2T_DIR/build/Qt/X2tConverter.pri)
include($$X2T_DIR/../Common/3dParty/googletest/googletest.pri)

HEADERS += $$X2T_DIR/src/dylib/x2t.h
SOURCES += $$X2T_DIR/src/dylib/x2t.cpp

SOURCES += main.cpp\
           common.cpp\
           xlsb2xlsx/conversion.cpp

HEADERS += common.h

DESTDIR = $$CORE_BUILDS_BINARY_PATH


SOURCES -= $$CORE_GTEST_PATH/src/gtest_main.cc
