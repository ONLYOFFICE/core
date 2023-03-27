CONFIG -= qt
TARGET = openjpeg
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

CORE_ROOT_DIR = $$PWD/../../../../../core
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)

include(openjpeg.pri)

SOURCES += ./main.cpp

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter)

DESTDIR = $$PWD/build
