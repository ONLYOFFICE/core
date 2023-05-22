#-------------------------------------------------
#
# Project created by QtCreator 2015-07-21T18:28:42
#
#-------------------------------------------------

QT       -= core

QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

CONFIG += c++11
TEMPLATE = app

CORE_ROOT_DIR = $$PWD/../../../..
PWD_ROOT_DIR = $$PWD
include($$CORE_ROOT_DIR/Common/base.pri)
include($$CORE_ROOT_DIR/3dParty/icu/icu.pri)

ADD_DEPENDENCY(kernel, graphics, UnicodeConverter)
win32 {
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

linux-g++ | linux-g++-64 | linux-g++-32 {
    LIBS += -lz
}

SOURCES += main.cpp
SOURCES += ../../../fontengine/ApplicationFontsWorker.cpp

DESTDIR = $$PWD_ROOT_DIR/build/$$CORE_BUILDS_PLATFORM_PREFIX/$$CORE_BUILDS_CONFIGURATION_PREFIX
